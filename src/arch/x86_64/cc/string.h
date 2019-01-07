#ifndef STRING
#define STRING

int strlen (char *s);

int strcmp (const char *s1, char *s2);

void strcpy (char *dst, const char *src);

void strcat (char *dest, const char *src);

void strncpy (char *destString, const char *sourceString, int maxLength);

bool strncmp (const char *s1, const char *s2, int c);

void intToString (char *buf, unsigned long int n, int base);

class string {
private:
	char *strPointer;												// Stores the real address of the character array
	int ccapacity;													// The maximum buffer size of the string at the moment
	const static int npos = -1;							//
	int length;															// The used length of the string
public:
	string (long long i, int base);					// Initialise a string object from an integer
	string (int initSize);
	string ();															// Initialise an empty string of length INITIAL_SIZE
	string (const char *c);									// Create a string from a character array
	string (const string *s);								// Create a copy of an existing string
	string operator=(const string inst);		// Assign a string to another string
	string * appending (const string other);
	string * appending (const char *other);
	string * appending (const char other);
	void append (const string other);
	void append (const char *other);
	void append (const char other);
	void copyFrom (const string inst);
	void copyFrom (const char *chrs);
	bool isEmpty ();												// Detect if the string is empty
	int size ();														// Get the size of the string (i.e. how many characters there are)
	int capacity ();												// Get the maximum buffer size of the string at the moment
	char * getData ();											// Get a pointer to the character array
	~string();
};

#endif
