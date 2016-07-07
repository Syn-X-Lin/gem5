/**
 * @file
 * Definitions of a LRU tag store.
 */

#include "debug/CacheRepl.hh"
#include "mem/cache/tags/double_q.hh"
#include "mem/cache/base.hh"
#include <iostream>

using std::cout;
using std::endl;

DoubleQ::DoubleQ(const Params *p)
    : BaseSetAssoc(p)
{
}

CacheBlk*
DoubleQ::accessBlock(Addr addr, bool is_secure, Cycles &lat, int master_id)
{
    //cout<<"accessBlock, 2Q"<<endl;
    CacheBlk *blk = BaseSetAssoc::accessBlock(addr, is_secure, lat, master_id);
    int set = extractSet(addr);

    if (blk != NULL) {
        if (sets[set].isInAm(blk)) {
            sets[set].moveToHeadAm(blk);
        }
    }
    
    return blk;
}

CacheBlk*
DoubleQ::findVictim(Addr addr)
{
    //cout<<"findVictim, 2Q, assoc="<<sets[0].assoc<<endl;
    int set = extractSet(addr);
    // grab a replacement candidate

    CacheBlk *blk = sets[set].findVictim2Q();
    //cout<<"finV end, 2Q"<<endl;
    return blk;
}

void
DoubleQ::insertBlock(PacketPtr pkt, BlkType *blk)
{
    //cout<<"insertBlock, 2Q"<<endl;
    BaseSetAssoc::insertBlock(pkt, blk);

    int set = extractSet(pkt->getAddr());

    sets[set].blks[sets[set].replaceIdx] = blk;
    sets[set].blkAddr[sets[set].replaceIdx] = pkt->getAddr();

    if (sets[set].isInA1out(pkt->getAddr())) {
        sets[set].addToHeadAm();
    } else {
        sets[set].addToHeadA1in();
    }
}

void
DoubleQ::invalidate(CacheBlk *blk)
{
    //cout<<"invalidate, 2Q"<<endl;
    BaseSetAssoc::invalidate(blk);
}

DoubleQ*
DoubleQParams::create()
{
    return new DoubleQ(this);
}
