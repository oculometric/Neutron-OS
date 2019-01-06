void memset(char *dest, char src, int len);

void memcpy(char *dest, char *src, int length);

struct heap_block {
    size_t size;
    bool isFree;
    heap_block *next;
} __attribute__((packed));

// typedef struct multiboot_memory_map {
// 	unsigned int size;
// 	unsigned int base_addr_low,base_addr_high;
// 	unsigned long long int base_addr;
// 	unsigned int length_low,length_high;
// 	// You can also use: unsigned long long int length; if supported.
// 	unsigned int type;
// } multiboot_memory_map_t;

void prepMemory ();

void * malloc (size_t size);

void updateAllocationTable ();

void logAllocationTable ();

void free (void *p);

void * calloc(size_t size);

void * operator new(size_t size);

void * operator new[](size_t size);

void operator delete(void *p, unsigned long);

void operator delete[](void *p);
