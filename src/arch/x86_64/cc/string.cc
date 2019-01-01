#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "string.h"
#include "memory.h"

int strlen (char *s) { // Determine the length of a string
	int ret = 0;
	while (s[ret] != '\0')
		ret++;
	return ret;
}

int strcmp (const char *s1, char *s2) {
	int i = 0;
	while (s1[i] == s2[i]) {
		if (s2[i++] == '\0') return true;
	}
	return false;
}

void strcpy (char *dst, const char *src) {
	int i = 0;
	while (dst[i] = src[i++]);
	//return i;
}

void strcat (char *dest, const char *src) {
	char *destStart = dest;
	destStart += strlen(dest);
	memcpy(destStart, (char*)src, strlen((char*)src));
}

void strncpy (char *destString, const char *sourceString, int maxLength) {
	int i = 0;
	while ((i < maxLength) && (sourceString[i] != '\0')) {
		destString[i] = sourceString[i++];
	}
}

bool strncmp (const char *s1, const char *s2, int c) {
	int i = 0;
	while (s1[i] == s2[i++]) {
		if (i == c) return true;
	}
	return false;
}

void intToString (char *buf, unsigned long int n, int base) {
	unsigned long int tmp;
	int i, j;
	tmp = n;
	i = 0;

	do {
		tmp = n % base;
		buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
	} while (n /= base);
	buf[i--] = 0;

	for (j = 0; j < i; j++, i--) {
		tmp = buf[j];
		buf[j] = buf[i];
		buf[i] = tmp;
	}
}
