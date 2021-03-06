// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_OS_BLUESTORE_BITMAPALLOCATOR_H
#define CEPH_OS_BLUESTORE_BITMAPALLOCATOR_H

#include <mutex>

#include "Allocator.h"
#include "BitAllocator.h"
#include "include/btree_interval_set.h"

class BitMapAllocator : public Allocator {
  std::mutex m_lock;

  int64_t m_block_size;
  int64_t m_num_reserved;

  BitAllocator *m_bit_alloc; // Bit allocator instance

  void insert_free(uint64_t offset, uint64_t len);

  int allocate_dis(uint64_t want_size, uint64_t alloc_unit, uint64_t max_alloc_size,
                        int64_t hint, mempool::bluestore_alloc::vector<AllocExtent> *extents, int *count, uint64_t *ret_len);

public:
  BitMapAllocator();
  BitMapAllocator(int64_t device_size, int64_t block_size);
  ~BitMapAllocator();

  int reserve(uint64_t need);
  void unreserve(uint64_t unused);

  int allocate(
    uint64_t want_size, uint64_t alloc_unit, uint64_t max_alloc_size,
    int64_t hint, mempool::bluestore_alloc::vector<AllocExtent> *extents, int *count, uint64_t *ret_len);

  int release(
    uint64_t offset, uint64_t length);

  uint64_t get_free();

  void dump() override;

  void init_add_free(uint64_t offset, uint64_t length);
  void init_rm_free(uint64_t offset, uint64_t length);

  void shutdown();
};

#endif
