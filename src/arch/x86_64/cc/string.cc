#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "string.h"
#include "log.h"
#include "memory.h"

#define INITIAL_SIZE 10

string::string () {
	strPointer = (char *)calloc (sizeof (char)*INITIAL_SIZE);
	ccapacity = INITIAL_SIZE;
	length = 0;
}

string::string (long long i, int base) {
	int c = i;
	int n = 0;
	while (c /= base) n++;
	strPointer = (char *)calloc (sizeof(char)*(n+4));
	ccapacity = n+4;
	length = n;
	intToString(strPointer, i, base);
}

string::string (int initSize) {
	strPointer = (char *)calloc (sizeof (char)*initSize);
	ccapacity = initSize;
	length = 0;
}

string::string (const char *c) {
	size_t len = strlen((char *)c);
	strPointer = (char *)calloc (sizeof (char)*(len+4));
	memcpy (strPointer, (char *)c, len);
	ccapacity = len+4;
	length = len;
}

string::string (const string *s) {
	strPointer = (char *)calloc (sizeof (char)*s->ccapacity);
	memcpy(strPointer, s->strPointer, s->length);
	ccapacity = s->ccapacity;
	length = s->length;
}

string string::operator=(const string inst) { // TODO: Upon reassignment, old value is not freed
	delete strPointer;
	strPointer = inst.strPointer;
	this->length = inst.length;
	this->ccapacity = inst.length+4;
}

bool string::isEmpty() {
	return this->length;
}

int string::size () {
	return this->length;
}

int string::capacity () {
	return this->ccapacity;
}

char * string::getData () {
	return this->strPointer;
}

string::~string() {
    delete [] this->strPointer;
}

string * string::appending (const string other) {
	int newLen = this->length + other.length;
	string *s = new string (newLen);
	memcpy(s->strPointer, this->strPointer, this->length);
	memcpy (s->strPointer+this->length, other.strPointer, other.length);
	return s;
}

string * string::appending (const char *other) {
	int slen = strlen ((char *)other);
	int newLen = this->length + slen;
	string *s = new string (newLen);
	memcpy (s->strPointer, this->strPointer, this->length);
	memcpy (s->strPointer+this->length, (char *)other, slen);
	return s;
}

string * string::appending (const char other) {
	int newLen = this->length + 1;
	string *s = new string (newLen);
	memcpy (s->strPointer, this->strPointer, this->length);
	*(s->strPointer+this->length) = other;
	return s;
}

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
