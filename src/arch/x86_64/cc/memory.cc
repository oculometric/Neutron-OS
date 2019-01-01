#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "memory.h"
#include "log.h"

void *usableStart = (void *) 0x1000000;
heap_block *headers = (heap_block *) 0x000007E00;
int numHeaderBlocks = 0;

size_t allocatedSpace = 0;
size_t freeSpace = 0;

unsigned long long heapSpace = 4294967296/2;

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

void prepMemory () {
  heap_block b = heap_block ();
  b.size = heapSpace;
  b.isFree = true;
  b.x = usableStart;
  headers[0] = b;
  numHeaderBlocks++;
  freeSpace = heapSpace;
}

void * malloc (size_t size) {
  if (size == 0 || size > freeSpace) return 0x0;
  int i = 0;
  while (i < numHeaderBlocks) {
    if (headers[i].isFree) {
      if (headers[i].size >= size) {
        // Found a free block large enough
        void *returnValue = headers[i].x;
        headers[i].size = headers[i].size - size;
        headers[i].x += size;

        int x = numHeaderBlocks;
        while (x > i) {
          headers[x] = headers[x-1];
          x--;
        }

        heap_block b = heap_block ();
        b.size = size;
        b.isFree = false;
        b.x = returnValue;
        headers[i] = b;
        numHeaderBlocks++;

        freeSpace -= size;
        allocatedSpace += size;
        return returnValue;
      }
    }
    i++;
  }

  return 0x0;
}

void updateAllocationTable () {
  // TODO: Merge adjacent free blocks if possible

}

void logAllocationTable () {
  logLn ("Logging allocation table:");
  for (int i = 0; i < numHeaderBlocks; i++) {
    log ("Item "); logInt(i);
    log ("Memory Address="); logHex ((long int)headers[i].x);
    log ("Is Free="); logBool (headers[i].isFree);
    log ("Size="); logInt (headers[i].size);
    logLn ("");
  }
}

void free (void *p) {
  int i = 0;
  while (i < numHeaderBlocks) {
    if (headers[i].x == p) {
      headers[i].isFree = true;
      freeSpace += headers[i].size;
      allocatedSpace -= headers[i].size;
      break;
    }
    i++;
  }
  updateAllocationTable();
}

void * calloc(size_t size) {
  void *ptr = malloc(size);
  memset((char *)ptr, 0x00000000, size);
	return ptr;
}

void * operator new(size_t size) { return malloc(size); }

void * operator new[](size_t size) { return malloc(size); }

void operator delete(void *p) { free(p); } // FIXME: Fix delete operator

void operator delete[](void *p) { free(p); }
