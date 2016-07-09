#include "debug/CacheRepl.hh"
#include "mem/cache/tags/maddr.hh"
#include "mem/cache/base.hh"

MADDR::MADDR(const Params *p)
    : BaseSetAssoc(p)
{
}

CacheBlk*
MADDR::accessBlock(Addr addr, bool is_secure, Cycles &lat, int master_id)
{
    CacheBlk *blk = BaseSetAssoc::accessBlock(addr, is_secure, lat, master_id);

    if (blk != NULL) {
        // move this block to head of the MRU list
        sets[blk->set].moveToHead(blk);
        DPRINTF(CacheRepl, "set %x: moving blk %x (%s) to MRU\n",
                blk->set, regenerateBlkAddr(blk->tag, blk->set),
                is_secure ? "s" : "ns");
    }

    return blk;
}

CacheBlk*
MADDR::findVictim(Addr addr)
{
    int set = extractSet(addr);
    // grab a replacement candidate
    //BlkType *blk = sets[set].blks[assoc - 1];

    BlkType *blk = sets[set].findFarthestAddrBlk();

    if (blk->isValid()) {
        DPRINTF(CacheRepl, "set %x: selecting blk %x for replacement\n",
                set, regenerateBlkAddr(blk->tag, set));
    }

    return blk;
}

void
MADDR::insertBlock(PacketPtr pkt, BlkType *blk)
{
    BaseSetAssoc::insertBlock(pkt, blk);

    int set = extractSet(pkt->getAddr());
    sets[set].moveToHeadMaddr(blk, pkt->getAddr());

}

void
MADDR::invalidate(CacheBlk *blk)
{
    BaseSetAssoc::invalidate(blk);

    // should be evicted before valid blocks
    int set = blk->set;
    sets[set].moveToTailMaddr(blk);
}

MADDR*
MADDRParams::create()
{
    return new MADDR(this);
}
