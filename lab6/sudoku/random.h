// random.h
#ifndef RANDOM_HEADER
#define RANDOM_HEADER

void random_Init();

void random_Exit();

char random_GetChar();

short random_GetShort();

int random_GetInt();

long random_GetLong();

long long random_GetLongLong();

#define random_GetUnsignedChar()     (unsigned char)random_GetChar()
#define random_GetUnsignedShort()    (unsigned Short)random_GetShort()
#define random_GetUnsignedInt()      (unsigned int)random_GetInt()
#define random_GetUnsignedLong()     (unsigned long)random_GetLong()
#define random_GetUnsignedLongLong() (unsigned long long)random_GetLongLong()

#endif /* RANDOM_HEADER */