#ifndef _CAFFEINE_HASH_H_
#define _CAFFEINE_HASH_H_

#include "common.h"

typedef struct {
    uint32_t value;
    VertexIndex key;
    bool used;
}HashBucket;

typedef struct {
    HashBucket* buckets;
    uint32_t len_buckets, count_bueckets;
}VextexHash;


uint32_t hash(uint64_t x, uint64_t y, uint64_t z);

void vertex_hash_init(VextexHash* vh, uint32_t len);

uint32_t vertex_hash_add(VextexHash* vh, VertexIndex key, uint32_t value);

bool vertex_hash_get(VextexHash* vh, VertexIndex key, uint32_t* value);

void vertex_hash_free(VextexHash* vh);

#endif // !_CAFFEINE_HASH_H_