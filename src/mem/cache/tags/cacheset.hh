/*
 * Copyright (c) 2013 ARM Limited
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2009 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Lisa Hsu
 */

/**
 * @file
 * Declaration of an associative set
 */

#ifndef __CACHESET_HH__
#define __CACHESET_HH__

#include <cassert>
#include <cmath>
#include <iostream>

#include "mem/cache/blk.hh" // base class

/**
 * An associative set of cache blocks.
 */
template <class Blktype>
class CacheSet
{
  public:
    /** The associativity of this set. */
    int assoc;

    /** Cache blocks in this set, maintained in LRU order 0 = MRU. Or heap in LRFU.*/
    Blktype **blks;
    double *CRFList;
    uint64_t *lastRefList;

    /** Counter used in LRFC*/
    uint64_t tc;

    /**
     * Find a block matching the tag in this set.
     * @param way_id The id of the way that matches the tag.
     * @param tag The Tag to find.
     * @param is_secure True if the target memory space is secure.
     * @return Pointer to the block if found. Set way_id to assoc if none found
     */
    Blktype* findBlk(Addr tag, bool is_secure, int& way_id) const ;
    Blktype* findBlk(Addr tag, bool is_secure) const ;

    /**
     * Move the given block to the head of the list.
     * @param blk The block to move.
     */
    void moveToHead(Blktype *blk);

    /**
     * Move the given block to the tail of the list.
     * @param blk The block to move
     */
    void moveToTail(Blktype *blk);

    void tcUp();

    void resetBlockInfo(Blktype *blk);

    void replaceRoot(Blktype *blk);

  protected:
    void Restore(int idx);
    double F_LRFU(uint64_t delta);
};

template <class Blktype>
Blktype*
CacheSet<Blktype>::findBlk(Addr tag, bool is_secure, int& way_id) const
{
    /**
     * Way_id returns the id of the way that matches the block
     * If no block is found way_id is set to assoc.
     */
    way_id = assoc;
    for (int i = 0; i < assoc; ++i) {
        if (blks[i]->tag == tag && blks[i]->isValid() &&
            blks[i]->isSecure() == is_secure) {
            way_id = i;
            return blks[i];
        }
    }
    return NULL;
}

template <class Blktype>
Blktype*
CacheSet<Blktype>::findBlk(Addr tag, bool is_secure) const
{
    int ignored_way_id;
    return findBlk(tag, is_secure, ignored_way_id);
}

template <class Blktype>
void
CacheSet<Blktype>::moveToHead(Blktype *blk)
{
    // nothing to do if blk is already head
    if (blks[0] == blk)
        return;

    // write 'next' block into blks[i], moving up from MRU toward LRU
    // until we overwrite the block we moved to head.

    // start by setting up to write 'blk' into blks[0]
    int i = 0;
    Blktype *next = blk;

    do {
        assert(i < assoc);
        // swap blks[i] and next
        Blktype *tmp = blks[i];
        blks[i] = next;
        next = tmp;
        ++i;
    } while (next != blk);
}

template <class Blktype>
void
CacheSet<Blktype>::moveToTail(Blktype *blk)
{
    // nothing to do if blk is already tail
    if (blks[assoc - 1] == blk)
        return;

    // write 'next' block into blks[i], moving from LRU to MRU
    // until we overwrite the block we moved to tail.

    // start by setting up to write 'blk' into tail
    int i = assoc - 1;
    Blktype *next = blk;

    do {
        assert(i >= 0);
        // swap blks[i] and next
        Blktype *tmp = blks[i];
        blks[i] = next;
        next = tmp;
        --i;
    } while (next != blk);
}

template <class Blktype>
void
CacheSet<Blktype>::tcUp()
{
    tc++;
}

template <class Blktype>
void
CacheSet<Blktype>::resetBlockInfo(Blktype *blk)
{
    int blkIdx = 0;
    while (blks[blkIdx] != blk) {
        blkIdx++;
    }

    CRFList[blkIdx] = F_LRFU(0) + F_LRFU(tc - lastRefList[blkIdx]) * CRFList[blkIdx];
    lastRefList[blkIdx] = tc;
    Restore(blkIdx);

}

template <class Blktype>
void
CacheSet<Blktype>::replaceRoot(Blktype *blk)
{
    //std::cout<<"replaceRoot1"<<std::endl;
    blks[0] = blk;
    //std::cout<<"replaceRoot1.1"<<std::endl;
    CRFList[0] = F_LRFU(0);
    //std::cout<<"replaceRoot1.2"<<std::endl;
    lastRefList[0] = tc;
    //std::cout<<"replaceRoot1.5"<<std::endl;
    Restore(0);
    //std::cout<<"replaceRoot2"<<std::endl;
}

template <class Blktype>
void
CacheSet<Blktype>::Restore(int idx)
{
    if (idx * 2 + 1 >= assoc) {
        return;
    }

    int smaller;

    if (idx * 2 + 1 == assoc - 1) {
        smaller = assoc - 1;
    }
    else {
        smaller = F_LRFU(tc - lastRefList[idx * 2 + 1]) * CRFList[idx * 2 + 1] >
                    F_LRFU(tc - lastRefList[idx * 2 + 2]) * CRFList[idx * 2 + 2] ?
                    idx * 2 + 2 : idx * 2 + 1;
    }
    if (F_LRFU(tc - lastRefList[idx]) * CRFList[idx] >
        F_LRFU(tc - lastRefList[smaller]) * CRFList[smaller]) {
        uint64_t tmp1;
        double tmp2;
        Blktype* tmp3;
        //swap
        tmp3 = blks[idx]; blks[idx] = blks[smaller]; blks[smaller] = tmp3;
        tmp1 = lastRefList[idx]; lastRefList[idx] = lastRefList[smaller]; lastRefList[smaller] = tmp1;
        tmp2 = CRFList[idx]; CRFList[idx] = CRFList[smaller]; CRFList[smaller] = tmp2;
        Restore(smaller);
    }
}

template <class Blktype>
double CacheSet<Blktype>::F_LRFU(uint64_t delta)
{
	//std::cout<<"F!"<<std::endl;	
	double lambda = 1;
        //std::cout<<"F 2!"<<std::endl;
	double rlt = pow(0.5, lambda * delta);
        //std::cout<<"F 3!"<<std::endl;
	return rlt;
}

#endif
