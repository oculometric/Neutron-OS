#ifndef MEMORYMANAGER
#define MEMORYMANAGER

void memset(char *dest, char src, int len);

void memset(char *dest, char src);

void memcpy(char *dest, char *src, int length);

typedef struct heap_block {
    size_t size;
    bool isFree;
    void *x;
} header;

void prepMemory ();

void * malloc (size_t size);

void updateAllocationTable ();

void logAllocationTable ();

void free (void *p);

void * calloc(size_t size);

void * operator new(size_t size);

void * operator new[](size_t size);

void operator delete(void *p);

void operator delete[](void *p);

#endif
