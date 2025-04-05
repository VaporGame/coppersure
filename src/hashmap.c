#include "hashmap.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#define INITIAL_CACHE_CAPACITY 13
#define LOAD_FACTOR 7

struct Cache {
    CacheEntry* entries;
    size_t capacity;
    size_t length;
};

static uint64_t prime = 0x00000100000001b3, basis = 0xcbf29ce484222325;

uint64_t FNV1A(const void* data, size_t size) {
    uint64_t hash = basis;
    const unsigned char *bytes = (const unsigned char *)data;

    for(size_t i = 0; i < size; i++) {
        hash ^= bytes[i];
        hash *= prime;
    }

    return hash;
}

Cache* createCache() {
    Cache* cache = malloc(sizeof(Cache));
    if (cache == NULL) {
        return NULL;
    }

    cache->length = 0;
    cache->capacity = INITIAL_CACHE_CAPACITY;

    cache->entries = calloc(cache->capacity, sizeof(CacheEntry));
    if (cache->entries == NULL) {
        free(cache);
        return NULL;
    }
    return cache;
}

void freeCache(Cache* cache) {
    for(size_t i = 0; i < cache->capacity; i++) {
        free((void*)cache->entries[i].key);
    }

    free(cache->entries);
    free(cache);
}

void freeTextureCache(Cache* cache) {
    for(size_t i = 0; i < cache->capacity; i++) {
        if (cache->entries[i].value != NULL) {
            TextureCacheEntry* test = (TextureCacheEntry*)(cache->entries[i].value);
            if(test->texture != NULL) {
                SDL_DestroyTexture(test->texture);

            }
            free(test);
        }
        free((void*)cache->entries[i].key);
    }

    free(cache->entries);
    free(cache);
}

void* cacheGet(Cache* cache, const char* key) {
    uint64_t hash = FNV1A(key, strlen(key));
    size_t index = (size_t)(hash & (uint64_t)(cache->capacity - 1));

    while(cache->entries[index].key != NULL) {
        if (strcmp(key, cache->entries[index].key) == 0) {
            return cache->entries[index].value;
        }

        index++;
        if (index >= cache->capacity) {
            index = 0;
        }
    }
    return NULL;
}

static const char* cacheSetEntry(CacheEntry* entries, size_t capacity, const char* key, void* value, size_t* plength) {
    uint64_t hash = FNV1A(key, strlen(key));
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    while(entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            entries[index].value = value;
            return entries[index].key;
        }

        index++;
        if (index >= capacity) {
            index = 0;
        }
    }

    if (plength != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return NULL;
        }
        (*plength)++;
    }
    entries[index].key = (char*)key;
    entries[index].value = value;
    return key;
}

static bool cacheExpand(Cache* cache) {
    size_t new_cap = cache->capacity * 1.5;
    SDL_Log("%ld", new_cap);
    if (new_cap < cache->capacity) {return false;} //overflow

    CacheEntry* newEntries = calloc(new_cap, sizeof(CacheEntry));
    if (newEntries == NULL) {return false;}

    for(size_t i = 0; i < cache->capacity; i++) {
        CacheEntry entry = cache->entries[i];
        if (entry.key != NULL) {
            cacheSetEntry(newEntries, new_cap, entry.key, entry.value, NULL);
        }
    }
    free(cache->entries);
    cache->entries = newEntries;
    cache->capacity = new_cap;
    return true;
}

const char* cacheSet(Cache* cache, const char* key, void* value) {
    assert(value != NULL);
    if (value == NULL) {
        return NULL;
    }
    if (cache->length >= (cache->capacity * LOAD_FACTOR) / 10) {
        SDL_Log("expanding cache");
        if (!cacheExpand(cache)) {
            return NULL;
        }
    }

    return cacheSetEntry(cache->entries, cache->capacity, key, value, &cache->length);
}

size_t getCacheLength(Cache* cache) {
    return cache->length;
}

CacheIterator newCacheIterator(Cache* cache) {
    CacheIterator it;
    it.cache = cache;
    it.index = 0;
    return it;
}

bool cacheIteratorNext(CacheIterator* iterator) {
    Cache* cache = iterator->cache;
    while(iterator->index < cache->capacity) {
        size_t i = iterator->index;
        iterator->index++;
        if (cache->entries[i].key != NULL) {
            CacheEntry entry = cache->entries[i];
            iterator->key = entry.key;
            iterator->value = entry.value;
            return true;
        }
    }
    return false;
}