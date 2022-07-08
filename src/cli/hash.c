#include "hash.h"

uint32_t hash(uint64_t x, uint64_t y, uint64_t z) {
    x = x * 1610612741 + y * 805306457 + z * 402653189;
    x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31);
    return x;
}

void vertex_hash_init(VextexHash* vh, uint32_t len) {
    vh->count_bueckets = 0;
    vh->len_buckets = len;
    vh->buckets = (HashBucket*)malloc(sizeof(HashBucket) * (len+1));

    assert(vh->buckets != NULL);
    size_t bsize = _msize(vh->buckets);
    for (size_t i = 0; i < len; i++)
    {
        assert(sizeof(HashBucket) * i < bsize);
        vh->buckets[i] = (HashBucket){0};
        vh->buckets[i].used = false;
    }
}

bool cmp_key(VertexIndex a, VertexIndex b) {
    return a.normal_index == b.normal_index && a.position_index == b.position_index && a.texture_index == b.texture_index;
}

uint32_t vertex_hash_add(VextexHash* vh, VertexIndex key, uint32_t value) {
    if (vh->count_bueckets == vh->len_buckets) {
        uint32_t nlen = vh->len_buckets * 2;
        HashBucket* nptr = (HashBucket*)realloc(vh->buckets, sizeof(HashBucket) * nlen);
        if (nptr) {
            vh->len_buckets = nlen;
            vh->buckets = nptr;
        }
        else
        {
            printf("Failed to realloc hash buckets\n");
            exit(-1);
        }
    }

    uint32_t index = hash(key.normal_index, key.position_index, key.texture_index) % vh->len_buckets;
    
    assert(vh->buckets != NULL);
    assert(index < vh->len_buckets);

    while (vh->buckets[index].used)
    {
        if (cmp_key(vh->buckets[index].key, key)) return index;
        index++;
        index %= vh->len_buckets;
        assert(index < vh->len_buckets);
    }

    vh->buckets[index].value = value;
    vh->buckets[index].key = key;
    vh->buckets[index].used = true;
    return index;
}

bool vertex_hash_get(VextexHash* vh, VertexIndex key, uint32_t* value) {
    uint32_t index =( hash(key.normal_index, key.position_index, key.texture_index) % vh->len_buckets);
    assert(index < vh->len_buckets);
    while (vh->buckets[index].used)
    {
        if (cmp_key(vh->buckets[index].key, key)) {
            *value = vh->buckets[index].value;
            return true;
        }
        index++;
        index %= vh->len_buckets;
        assert(index < vh->len_buckets);
    }
    return false;
}

void vertex_hash_free(VextexHash* vh) {
    if (vh == NULL) return;
    if (vh->buckets != NULL) free(vh->buckets);
}