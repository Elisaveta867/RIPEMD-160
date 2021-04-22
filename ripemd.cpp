#define _CRT_SECURE_NO_WARNINGS


#include "ripemd.h"

#define hash 160
#define ROL(x, n)        (((x) << (n)) | ((x) >> (32-(n))))
#define F1(x, y, z)        ((x) ^ (y) ^ (z))				
#define F2(x, y, z)        (((x) & (y)) | (~(x) & (z)))  
#define F3(x, y, z)        (((x) | ~(y)) ^ (z))			
#define F4(x, y, z)        (((x) & (z)) | ((y) & ~(z)))	
#define F5(x, y, z)        ((x) ^ ((y) | ~(z)))			

dword K[] = { 0x00000000, 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xa953fd4e };
dword K1[] = { 0x00000000, 0x7a6d76e9, 0x6d703ef3, 0x5c4dd124, 0x50a28be6 };
vector<vector<int>> hashes;


void inputarrays(int* arr, string filename) {
	ifstream input(filename);
	for (int i = 0; i < 80; i++) {
		input >> arr[i];
	}
	input.close();
}

void init(dword *buf) {
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;
	buf[4] = 0xc3d2e1f0;

	return;
}

void compress(dword *MDbuf, dword *X) {
	int r[80];
	int r1[80];
	int s[80];
	int s1[80];
	inputarrays(r, "r.txt");
	inputarrays(r1, "r1.txt");
	inputarrays(s, "s.txt");
	inputarrays(s1, "s1.txt");

	dword a = MDbuf[0], b = MDbuf[1], c = MDbuf[2], d = MDbuf[3], e = MDbuf[4];
	dword a1 = MDbuf[0], b1 = MDbuf[1], c1 = MDbuf[2], d1 = MDbuf[3], e1 = MDbuf[4];

	dword *p[5] = { &a, &b,  &c, &d, &e };
	dword *p1[5] = { &a1, &b1,  &c1, &d1, &e1 };


	for (int j = 0; j < 80; j++) {

		if (j < 16)
			*p[0] += F1(*p[1], *p[2], *p[3]) + X[r[j]] + K[0];
		else if (j < 32)
			*p[0] += F2(*p[1], *p[2], *p[3]) + X[r[j]] + K[1];
		else if (j < 48)
			*p[0] += F3(*p[1], *p[2], *p[3]) + X[r[j]] + K[2];
		else if (j < 64)
			*p[0] += F4(*p[1], *p[2], *p[3]) + X[r[j]] + K[3];
		else if (j < 80)
			*p[0] += F5(*p[1], *p[2], *p[3]) + X[r[j]] + K[4];

		*p[0] = ROL(*p[0], s[j]) + *p[4];
		*p[2] = ROL(*p[2], 10);

		dword *temp = p[4];
		for (int i = 4; i >= 0; i--) {
			p[i] = p[i - 1];
		}
		p[0] = temp;
	//}

	/* parallel  */
	//for (int j = 0; j < 80; j++) {
		if (j < 16)
			*p1[0] += F5(*p1[1], *p1[2], *p1[3]) + X[r1[j]] + K1[4];
		else if (j < 32)
			*p1[0] += F4(*p1[1], *p1[2], *p1[3]) + X[r1[j]] + K1[3];
		else if (j < 48)
			*p1[0] += F3(*p1[1], *p1[2], *p1[3]) + X[r1[j]] + K1[2];
		else if (j < 64)
			*p1[0] += F2(*p1[1], *p1[2], *p1[3]) + X[r1[j]] + K1[1];
		else if (j < 80)
			*p1[0] += F1(*p1[1], *p1[2], *p1[3]) + X[r1[j]] + K1[0];

		*p1[0] = ROL(*p1[0], s1[j]) + *p1[4];
		*p1[2] = ROL(*p1[2], 10);

		//dword *
		temp = p1[4];
		for (int i = 4; i >= 0; i--) {
			p1[i] = p1[i - 1];
		}
		p1[0] = temp;
	}


	d1 += c + MDbuf[1];
	MDbuf[1] = MDbuf[2] + d + e1;
	MDbuf[2] = MDbuf[3] + e + a1;
	MDbuf[3] = MDbuf[4] + a + b1;
	MDbuf[4] = MDbuf[0] + b + c1;
	MDbuf[0] = d1;

	return;
}

void finish(dword *MDbuf, byte *strptr, dword lswlen, dword mswlen)
{
	unsigned int i;
	dword X[16];

	memset(X, 0, 16 * sizeof(dword));

	for (i = 0; i < (lswlen & 63); i++) {
		X[i >> 2] ^= (dword)*strptr++ << (8 * (i & 3));
	}

	X[(lswlen >> 2) & 15] ^= (dword)1 << (8 * (lswlen & 3) + 7);

	if ((lswlen & 63) > 55) {
		compress(MDbuf, X);
		memset(X, 0, 16 * sizeof(dword));
	}

	X[14] = lswlen << 3;
	X[15] = (lswlen >> 29) | (mswlen << 3);
	compress(MDbuf, X);

	return;
}

dword BYTES_TO_DWORD(byte* n) {
	(((dword) *((n)+3) << 24) | ((dword) *((n)+2) << 16) | ((dword) *((n)+1) << 8) | ((dword) *(n)));
	return *n;
}

byte *RIPEMD(const char *fname) {

	FILE *inputf;
	byte data[1024];
	dword nbytes;
	dword buf[hash / 32];
	static byte hashcode[hash / 8];
	dword X[16];
	unsigned int  i, j;
	dword length[2] = { 0 };
	dword offset;

	if ((inputf = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "\nRMDbinary: cannot open file .\n", fname);
		exit(1);
	}

	init(buf);

	while ((nbytes = fread(data, 1, 1024, inputf)) != 0) {
		for (i = 0; i < (nbytes >> 6); i++) {
			for (j = 0; j < 16; j++)
				X[j] = BYTES_TO_DWORD(data + 64 * i + 4 * j);
			compress(buf, X);
		}
		if (length[0] + nbytes < length[0])
			length[1]++;
		length[0] += nbytes;
	}
	offset = length[0] & 0x3C0;

	finish(buf, data + offset, length[0], length[1]);

	for (i = 0; i < hash / 8; i += 4)
	{
		hashcode[i] = buf[i >> 2];
		hashcode[i + 1] = (buf[i >> 2] >> 8);
		hashcode[i + 2] = (buf[i >> 2] >> 16);
		hashcode[i + 3] = (buf[i >> 2] >> 24);
	}

	fclose(inputf);
	return (byte *)hashcode;
}

void random_str(int size) {
	string key = "";
	static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < size; ++i) {
		key += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	ofstream fout;
	fout.open("hash.txt");
	fout << key << endl;
	fout.close();
}

vector<int> getNBits(int n, byte h) {
	vector<int> x (n);
	for (int i = 0; i < n; i++) {
		bool b = (bool((1 << i)  &  h));
		x[i] = b;
	}
	return x;
}

int findPreimage(int n, const char *test) {
	byte y0;
	byte y;
	byte *p;
	vector<int> x0(n);
	vector<int> x(n);
	hashes.clear();

	p = RIPEMD(test);
	y0 = *p;
	x0 = getNBits(n, *p);

	do {
		random_str(12);
		p = RIPEMD("hash.txt");
		x = getNBits(n, *p);
		hashes.push_back(x);
	} while (x != x0);

	return hashes.size();
}

int findCollision() {
	int count = 0;
	for (int i = 1; i < hashes.size(); i++) {
		for (int j =i + 1; j < hashes.size(); j++) {
			count++;
			if (hashes[i] == hashes[i])
				break;
		}
	}
	return count;
}