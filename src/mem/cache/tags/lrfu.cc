/**
 * @file
 * Definitions of a LRFU tag store.
 */

#include "debug/CacheRepl.hh"
#include "mem/cache/tags/lrfu.hh"
#include "mem/cache/base.hh"
#include <iostream>

LRFU::LRFU(const Params *p)
    : BaseSetAssoc(p)
{
    //std::cout<<"init lrfu! "<<std::endl;
}

CacheBlk* 
LRFU::accessBlock(Addr addr, bool is_secure, Cycles &lat, int master_id)
{
    //std::cout<<"access BLOCK lrfu! "<<std::endl;

    CacheBlk *blk = BaseSetAssoc::accessBlock(addr, is_secure, lat, master_id);

    //std::cout<<"SET= "<<std::endl;

    if (blk != NULL) {
        //std::cout<<"Mark1!"<<std::endl;
        // reset block's crf and last ref time.
        sets[blk->set].tcUp();
        sets[blk->set].resetBlockInfo(blk);
        DPRINTF(CacheRepl, "set %x: moving blk %x (%s) to MRU\n",
                blk->set, regenerateBlkAddr(blk->tag, blk->set),
                is_secure ? "s" : "ns");
        //std::cout<<"Mark2!"<<std::endl;
    }
    else {
    	int set = extractSet(addr);
    	sets[set].tcUp();
    }

    //std::cout<<"access end!"<<std::endl;
    return blk;
}

CacheBlk* 
LRFU::findVictim(Addr addr)
{
    //std::cout<<"Find Victim!"<<std::endl;
    int set = extractSet(addr);
    //std::cout<<"set = "<<set<<std::endl;
    // grab a replacement candidate
    BlkType *blk = sets[set].blks[0];

    if (blk->isValid()) {
        DPRINTF(CacheRepl, "set %x: selecting blk %x for replacement\n",
                set, regenerateBlkAddr(blk->tag, set));
    }
    //std::cout<<"Find Victim end!"<<std::endl;
    return blk;
}

void
LRFU::insertBlock(PacketPtr pkt, BlkType *blk)
{
    //std::cout<<"insert!"<<std::endl;

    BaseSetAssoc::insertBlock(pkt, blk);

    int set = extractSet(pkt->getAddr());

    //std::cout<<"set= "<<set<<std::endl;
    sets[set].replaceRoot(blk);
    //std::cout<<"insertEnd"<<std::endl;
}

void
LRFU::invalidate(CacheBlk *blk)
{
    //std::cout<<"invalidate!"<<std::endl;
    BaseSetAssoc::invalidate(blk);

    // should be evicted before valid blocks
    //int set = blk->set;
    //sets[set].moveToTail(blk);
}

LRFU*
LRFUParams::create()
{
    return new LRFU(this);
}
