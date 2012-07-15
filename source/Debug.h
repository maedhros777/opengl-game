#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define ASSERT(assertion) __assert((bool) (assertion), #assertion, __LINE__, __FILE__)
#define ASSERT_D(assertion, description) __assert((bool) (assertion), #assertion, description, __LINE__, __FILE__)
#define ASSERT_STR(description) __assert(description, __LINE__, __FILE__)
#else
#define ASSERT(assertion)
#define ASSERT_D(assertion, description)
#define ASSERT_STR(description)
#endif

#define COMPILE_ASSERT(assertion) switch(0) { case 0: case assertion:; }

void __assert(bool, const char *, int, const char *);
void __assert(bool, const char *, const char *, int, const char *);
void __assert(const char *, int, const char *);

#endif
