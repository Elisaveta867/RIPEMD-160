#ifndef  ripemdH
#define  ripemdH

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

typedef    unsigned char        byte;
typedef    unsigned long        dword;

dword BYTES_TO_DWORD(byte* n);

void init(dword *buf);

void compress(dword *MDbuf, dword *X);

void finish(dword *MDbuf, byte *strptr, dword lswlen, dword mswlen);

byte *RIPEMD(const char *fname);

void random_str(int size);

int findPreimage(int n, const char *test);

int findCollision();

vector<int> getNBits(int n, byte h);

#endif
