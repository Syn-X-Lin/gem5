/**
 * @file
 * Declaration of a 2Q tag store.
 * The 2Q tags use A1in, A1out, and Am queue
 */

#ifndef __MEM_CACHE_TAGS_2Q_HH__
#define __MEM_CACHE_TAGS_2Q_HH__

#include "mem/cache/tags/base_set_assoc.hh"
#include "params/DoubleQ.hh"

class DoubleQ : public BaseSetAssoc
{
  public:
    /** Convenience typedef. */
    typedef DoubleQParams Params;

    /**
     * Construct and initialize this tag store.
     */
    DoubleQ(const Params *p);

    /**
     * Destructor
     */
    ~DoubleQ() {}

    CacheBlk* accessBlock(Addr addr, bool is_secure, Cycles &lat,
                         int context_src);
    CacheBlk* findVictim(Addr addr);
    void insertBlock(PacketPtr pkt, BlkType *blk);
    void invalidate(CacheBlk *blk);
};

#endif // __MEM_CACHE_TAGS_2Q_HH__
