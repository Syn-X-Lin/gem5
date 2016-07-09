/**
 * @file
 * Declaration of a MADDR tag store.
 * The MADDR tags guarantee that the farthest addr to the most recently used block in
 * a set will always be evicted.
 */

#ifndef __MEM_CACHE_TAGS_MADDR_HH__
#define __MEM_CACHE_TAGS_MADDR_HH__

#include "mem/cache/tags/base_set_assoc.hh"
#include "params/MADDR.hh"

class MADDR : public BaseSetAssoc
{
  public:
    /** Convenience typedef. */
    typedef MADDRParams Params;

    /**
     * Construct and initialize this tag store.
     */
    MADDR(const Params *p);

    /**
     * Destructor
     */
    ~MADDR() {}

    CacheBlk* accessBlock(Addr addr, bool is_secure, Cycles &lat,
                         int context_src);
    CacheBlk* findVictim(Addr addr);
    void insertBlock(PacketPtr pkt, BlkType *blk);
    void invalidate(CacheBlk *blk);
};

#endif // __MEM_CACHE_TAGS_MADDR_HH__
