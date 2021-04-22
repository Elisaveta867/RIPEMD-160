#define _CRT_SECURE_NO_WARNINGS
#include "ripemd.h"

int main() {
	srand(time(NULL));

	byte *hashcode;
	int inputmode;

	cout << "Implementing RIPEMD-160 hash-function \n \n";
	hashcode = RIPEMD("test.txt");
	cout << "Your hashcode: \n";
	for (int j = 0; j < 160 / 8; j++)
		cout << hex << static_cast<int>(hashcode[j]);
	cout << endl;

	//cout << "Analysis in progress... \n" << endl;
	/*
	ofstream out("results.txt");
	int bits[] = { 8, 12, 16, 20, 24 };
	for (int i = 0; i < 5; i++) {
		double N = 0;
		double M = 0;
		for (int j = 0; j < 1000; j++) {
			N += findPreimage(bits[i], "test.txt");
			M += findCollision();
		}
		out << (N / 1000.0) << " " << (M / 1000.0) << '\n';
	}
	out.close();
	*/

	system("pause");
	return 0;
}