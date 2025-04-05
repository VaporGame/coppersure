#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

typedef struct {
    const char* key;
    void* value;
} CacheEntry;

typedef struct {
    int w,h;
    SDL_Texture* texture;
} TextureCacheEntry;

typedef struct Cache Cache;

uint64_t FNV1A(const void* data, size_t size);

Cache* createCache();
void freeCache(Cache* cache);
void freeTextureCache(Cache* cache);

void* cacheGet(Cache* cache, const char* key);
const char* cacheSet(Cache* cache, const char* key, void* value);

size_t getCacheLength(Cache* cache);

typedef struct {
    const char* key;
    void* value;

    //dont use directly
    Cache* cache;
    size_t index;
} CacheIterator;

CacheIterator newCacheIterator(Cache* cache);
bool cacheIteratorNext(CacheIterator* iterator);

#endif