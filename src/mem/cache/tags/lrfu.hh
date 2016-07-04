/**
 * @file
 * Declaration of a LRFU tag store.
 * The LRFU tags guarantee that the smallest F way in
 * a set will always be evicted.
 */

#ifndef __MEM_CACHE_TAGS_LRFU_HH__
#define __MEM_CACHE_TAGS_LRFU_HH__

#include "mem/cache/tags/base_set_assoc.hh"
#include "params/LRFU.hh"

class LRFU : public BaseSetAssoc
{
  public:
    /** Convenience typedef. */
    typedef LRFUParams Params;

    /**
     * Construct and initialize this tag store.
     */
    LRFU(const Params *p);

    /**
     * Destructor
     */
    ~LRFU() {}

    CacheBlk* accessBlock(Addr addr, bool is_secure, Cycles &lat,
                         int context_src);
    CacheBlk* findVictim(Addr addr);
    void insertBlock(PacketPtr pkt, BlkType *blk);
    void invalidate(CacheBlk *blk);
};

#endif // __MEM_CACHE_TAGS_LRFU_HH__
