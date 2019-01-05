#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "memory.h"
#include "log.h"

void memset(char *dest, char src, int len) {
	char *p = dest;
  int c = len;
  while (c) {
    *(p++) = src;
		c--;
  }
}

void memcpy(char *dest, char *src, int length) {
	char *p = dest;
	int w = length;
	while (w > 0) {
		*(p++) = *(src++);
		w--;
	}
}

#define HEAP_BLOCK_SIZE sizeof(struct heap_block)

size_t allocatedSpace = 0;
size_t freeSpace = 4294967296-HEAP_BLOCK_SIZE;
unsigned long long heapSpace = 4294967296;
heap_block *availableStart = (heap_block *)0x01000000;


void prepMemory () {
	availableStart->next = NULL;
	availableStart->isFree = true;
	availableStart->size = heapSpace-HEAP_BLOCK_SIZE;
}

void * malloc (size_t size) {
  if (size == 0 || size > freeSpace) return 0x0;
	struct heap_block *h = (struct heap_block *)availableStart;

  do {
    if (h->isFree) {
      if (h->size >= size) {
        // Found a free block large enough
				size_t sizeDif = h->size-size;
				if (sizeDif > HEAP_BLOCK_SIZE + 50) {
					h->isFree = false;
					void *n = (void *)h;
					n += HEAP_BLOCK_SIZE;
					n += size;
					h->next = (heap_block *)n;

					h->next->size = h->size-(size+HEAP_BLOCK_SIZE);
					h->next->isFree = true;
					h->next->next = NULL;

					h->size = size;
				} else {
					h->isFree = false;
				}
				freeSpace -= h->size;
				allocatedSpace += h->size;
				void *r = (void *)h;
				r += HEAP_BLOCK_SIZE;
        return r;
      }
    }
		h = h->next;
  } while (h != NULL);
	fatal ("Oh no! We couldn't find anywhere to allocate!");
  return 0x0;
}

void updateAllocationTable () {
	struct heap_block *h = (struct heap_block *)availableStart;
	while (h != NULL) {
		while (h != NULL && h->isFree && h->next != NULL && h->next->isFree) {
			h->size += HEAP_BLOCK_SIZE;
			h->size += h->next->size;
			h->next = h->next->next;

			h = h->next;
		}
		if (h != 0x0) {
			h = h->next;
		}
	}
}

void logAllocationTable () {
  logLn ("Logging allocation table:");
	struct heap_block *h = (struct heap_block *)availableStart;
  do {
		log ("Meta Address="); logHex ((long int)(h));
    log ("Memory Address="); logHex ((long int)(h)+HEAP_BLOCK_SIZE);
    log ("Is Free="); logBool (h->isFree);
    log ("Size="); logInt (h->size);
    logLn ("");
		h = h->next;
  } while (h != NULL);
}

void free (void *p) {
	p -= HEAP_BLOCK_SIZE;
	struct heap_block *h = (struct heap_block *)p;
	h->isFree = true;
  updateAllocationTable();
}

void * calloc(size_t size) {
  void *ptr = malloc(size);
  memset((char *)ptr, 0x00000000, size);
	return ptr;
}

void * operator new (size_t size) { return malloc(size); }

void * operator new[] (size_t size) { return malloc(size); }

void operator delete (void *p, unsigned long) {free(p); }

void operator delete[] (void *p, unsigned long) { free(p); }
