#include "../desDec/stdafx.h"
#include<stdio.h>
#include<string.h>
#include <windows.h>
#include <time.h>
#include <sys/timeb.h>


//Time counting in seconds
int getNowTime_s()
{
	clock_t time;
	time = clock();
	return time;
}

//Time counting in milliseconds
//Milliseconds is better because encryption is too fast
int getNowTime_ms()
{
	struct timeb tb;
	struct tm *now = NULL;
	char sec[16];
	char mss[4];
	int min, s, ms;

	ftime(&tb);
	now = localtime(&tb.time);
	//	sprintf(sec, "%02d-%02d", now.tm_min, now.tm_sec);
	//	sprintf(mss, "%03d", tb.millitm);
	min = now->tm_min;
	s = now->tm_sec;
	ms = tb.millitm;

	return ms + s * 1000 + min * 1000 * 60;
}
// Data writing to file
int ReadLocalFile(const char *filePath, void *fileData, int *fileDataLen)
{
	// Offset bytes of the current position relative to the beginning of the file
	int buflen = 0;

	// Open a binary file, read only
	FILE *fp = fopen((const char*)filePath, "rb");
	if (fp == NULL)
	{
		return -1;
	}
	fseek(fp, 0, SEEK_END);// Find the end of a file
	buflen = ftell(fp);// Determine the current location
	fseek(fp, 0, SEEK_SET);// Find the beginning of a file
	if (!fileData)
	{
		*fileDataLen = buflen;
		fclose(fp);
		return 0;
	}
	fread(fileData, buflen, 1, fp);// Read file data
	fclose(fp);
	*fileDataLen = buflen;
	return 0;
}
// Read data from local files
int WriteLocalFile(const char *filePath, void *data,int dataLen)
{
	int buflen = 0;
	// Open a binary file, allowing read and write
	FILE *fp = fopen((const char*)filePath, "wb+");
	if (fp == NULL)
	{
		return -1;
	}
	fwrite(data, 1, dataLen, fp);
	fclose(fp);
	return 0;
}
// Convert hexadecimal string to unsigned char*
void _str_to_hex(unsigned char *pbDest, char *pbSrc, int nLen)// nLen - Number of bytes of hexadecimal numbers
{
	char h1, h2;
	unsigned char s1, s2;
	int i;

	for (i = 0; i < nLen; i++)
	{
		h1 = pbSrc[2 * i];
		h2 = pbSrc[2 * i + 1];
		// Hexadecimal is 0-9, A-F, so the case of greater than 9 is considered
		s1 = toupper(h1) - 0x30;// Lower case to upper case, hex 0x30
		if (s1 > 9)
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;

		pbDest[i] = s1 * 16 + s2;
	}
}
// Convert strings to hexadecimal strings
void _hex_to_str(char *pbDest, unsigned char *pbSrc, int nLen)// nLen - Number of bytes in hexadecimal (length of string / 2)
{
	char ddl, ddh;
	int i;

	for (i = 0; i < nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i * 2] = ddh;
		pbDest[i * 2 + 1] = ddl;
	}

	pbDest[nLen * 2] = '\0';
}
// The initial permutation IP matrix
int IP_Table[64] = {                                    
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };
// E bit-selection table
int E_Table[48] = {                                    
	32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1 };
// The permutation function P table
int P_Table[32] = {                                            
	16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
	2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25 };
// The inverse of the initial permutation matrix
int IPR_Table[64] = {                                   
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25 };
// Permuted choice 1 table - PC1
int PC1_Table[56] = {                              
	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4 };
// Permuted choice 2 table - PC2
int PC2_Table[48] = {                          
	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };
// The primitive functions S1,...,S8
int S_Box[8][4][16] = {                   
											// S1
	14, 4,  13,     1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	// S2
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	// S3
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	// S4
	7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
	// S5
	2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
	// S6
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
	// S7
	4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
	// S8
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

// Convert char into int
void CharToBit(char input[], int output[]) {
	int i, j;
	unsigned char tempInput[8] = { 0 };
	memcpy(tempInput, input, 8);
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			//output[8 * j + 7 - i] = (input[j] >> i) & 1;
			output[8 * j + 7 - i] = abs((tempInput[j] % 2));
			tempInput[j] /= 2;
		}
	}
};

// Convert int to char
void BitToChar(int intput[], char output[]) {
	int i, j;
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			output[j] = output[j] * 2 + intput[i + 8 * j];
		}
	}
};

// XOR operation
void Xor(int *INA, int *INB, int len) {
	for (int i = 0; i < len; i++) {
		*(INA + i) = *(INA + i) ^ *(INB + i);
	}
};
//Initial permutation IP, both input and output are 64 bits.
void IP(const int input[64], int output[64], int table[64]) {
	for (int i = 0; i < 64; i++) {
		// Here table[i] - 1, why - 1, because you can see the table value of IP_Table, the minimum value of 1, the maximum value of 64
		// output and input length is 64, if you want to swap the bits, start from bit 0
		// and the IP_Table represents from 1, so -1 is needed
		// The following substitution is the same
		output[i] = input[table[i] - 1];
	}
};
//E expansion [32 -> 48]
void E(const int input[32], int output[48], int table[48]) {
	for (int i = 0; i < 48; i++) {
		output[i] = input[table[i] - 1];
	}
};
// The permutation function P
void P(const int input[32], int output[32], int table[32]) {
	for (int i = 0; i < 32; i++) {
		output[i] = input[table[i] - 1];
	}
};
// The inverse of the initial permutation
void IP_In(const int input[64], int output[64], int table[64]) {
	for (int i = 0; i < 64; i++) {
		output[i] = input[table[i] - 1];
	}
};
// Permuted choice 1 for key permutation
void PC_1(const int input[64], int output[56], int table[56]) {
	for (int i = 0; i < 56; i++) {
		output[i] = input[table[i] - 1];
	}
};
// Permuted choice 2
void PC_2(const int input[56], int output[48], int table[48]) {
	for (int i = 0; i < 48; i++) {
		output[i] = input[table[i] - 1];
	}
};
// S-box compression
void S(const int input[48], int output[32], int table[8][4][16]) {
	int m, n, s_ele;
	// Divide the resulting 48-bit number into eight 6-bit numbers. 1-6 bits are Z 1, 7-12 bits are Z 2,......43-48 bits are Z 8.
	for (int i = 0; i < 8; i++)          //S-box substitution
	{
		// Replace Z j with the value in the S-box, starting from j=1. The value in the S-box is a 4-digit number, for a total of 8 S-boxes.
		// Take the 1st and 6th bits of Z j and concatenate them to form a 2-digit number, m. m is the number of rows in the Sj box where the number of Z j is replaced.
		m = (input[i * 6] * 2) + (input[i * 6 + 5] * 1);   // The 1st and 6th bits form the row number
														  // Take bits 2 to 5 of Z j and concatenate them to form a 4-digit number, denoted n. n is the number of columns in the Sj box where the number used to replace Z j is located. 
		n = (input[i * 6 + 1] * 8) + (input[i * 6 + 2] * 4) + (input[i * 6 + 3] * 2) + (input[i * 6 + 4] * 1);  // Bits 2, 3, 4, and 5 form the column number
																												// Replace with the value of coordinates (n, m) in the S-box.
		s_ele = S_Box[i][m][n];//Get the decimal number in the S-box
		
		// Convert decimal to binary
		for (int j = 3; j >= 0; j--)// Decreasing from 3 because binary takes the remainder from the bottom to the top.
		{
			output[i * 4 + j] = s_ele % 2;
			s_ele /= 2;
		}
	}
};


// Complete the DES algorithm  transformation, the internal implementation of the f function, which is an iso-or
void F_func(int input[32], int output[32], int subkey[48]) {
	int len = 48;
	int temp[48] = { 0 };
	int temp_1[32] = { 0 };
	// Using extended substitution E, extend 32 bits into 48 bits
	E(input, temp, E_Table);//32->48
	// Use E{R (i-1)} to make a by-position iso-or operation with the subkey K(i).
	
	// Enter the S function
	S(temp, temp_1, S_Box);//48->32
						   // The output of the eight selection functions Sj (1 ¡Ü j ¡Ü 8) is spliced into a 32-bit binary data area group, which is used as the input of the P-box permutation to obtain the output
	P(temp_1, output, P_Table);// No change in digits
};


// Key loop left shift
void RotateL(int input[28], int output[28], int leftCount) {
	int i;
	int len = 28;// Because it is not a bitwise operation, it can be used without unsigned
	for (i = 0; i < len; i++) {
		output[i] = input[(i + leftCount) % len];
	}
};

// Sub-key generation
void subKey_fun(const int input[64], int Subkey[16][48]) {// Note the number of bits of input and output, int only store 01, the key is 18 bits 16 rounds
	int loop = 1, loop_2 = 2;
	int i, j;
	int c[28], d[28];
	int pc_1[56] = { 0 };
	int pc_2[16][56] = { 0 };
	int rotatel_c[16][28] = { 0 };
	int rotatel_d[16][28] = { 0 };
	// The key is transformed, and after the transformation of the subkey permutation table PC
	PC_1(input, pc_1, PC1_Table);

	// The transformed key is divided into two equal parts, and the first 28 bits are recorded as C0 and the last 28 bits are recorded as D0.
	for (i = 0; i < 28; i++) {
		c[i] = pc_1[i];//L
		d[i] = pc_1[i + 28];//R
	}
	int leftCount = 0;
	// Compute the subkeys (16 in total), starting from i = 1.
	// Generate Ci and Di by circular left shift of Ci-1 and Di-1 respectively (16 times in total).
	for (i = 1; i < 17; i++) {
		if (i == 1 || i == 2 || i == 9 || i == 16) {// Shift one position left
			leftCount += loop;
			RotateL(c, rotatel_c[i - 1], leftCount);
			RotateL(d, rotatel_d[i - 1], leftCount);
		}
		else {// Shift two positions left
			leftCount += loop_2;
			RotateL(c, rotatel_c[i - 1], leftCount);
			RotateL(d, rotatel_d[i - 1], leftCount);
		}
	}
	//The 56-bit number is obtained by concatenating Ci and Di, and then a subkey permutation table PC-2 transformation is performed to generate a 48-bit subkey Ki.
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 28; j++) {
			pc_2[i][j] = rotatel_c[i][j];
			pc_2[i][j + 28] = rotatel_d[i][j];
		}
	}
	for (i = 0; i < 16; i++) {
		PC_2(pc_2[i], Subkey[i], PC2_Table);
	}
	//Calculate the 16 subkeys by the above method 

};

//Encryption implementation
void  DES_Encrypt(int input[64], int key_in[64], int output[64]) {
	int Ip[64] = { 0 };//Store the matrix after the initial permutation
	int output_1[64] = { 0 };
	int subkeys[16][48];
	int key[64];
	int l[17][32], r[17][32];

	//For details, go to the subKey_fun function
	subKey_fun(key_in, subkeys);

	//Transformation of data blocks using the initial transformation IP table
	IP(input, Ip, IP_Table);
	//The transformed data block is divided into two equal parts, the first 32 bits are recorded as L0 and the second 32 bits are recorded as R0
	for (int i = 0; i < 32; i++) {
		l[0][i] = Ip[i];
		r[0][i] = Ip[32 + i];
	}
	//Encryption of data with 16 subkeys
	for (int j = 1; j < 17; j++) {
		F_func(r[j - 1], r[j], subkeys[j - 1]);
		//The result of the computation is the same as L(i - 1). Assign the result of the calculation to R(i)
		Xor(r[j], l[j - 1], 32);
		//Assign the value of R(i-1) to L(i) to complete the 1st round of product transformation.
		for (int k = 0; k < 32; k++) {
			l[j][k] = r[j - 1][k];
		}
	}
	// The loop executes until K(16) is also used. 2~16 rounds is this for loop above
	// Put R(16) and L(16) in series in sequence to get a 64-bit number. Implement the inverse transform IP - 1 of the 2.2 transform on this number.
	for (int t = 0; t < 32; t++)
	{
		output_1[t] = r[16][t];
		output_1[32 + t] = l[16][t];
	}

	IP_In(output_1, output, IPR_Table);
};

// Decryption implementation 
void  DES_Decrypt(int input[64], int key_in[64], int output[64]) {
	int Ip[64] = { 0 };// Store the matrix after the initial permutation
	int output_1[64] = { 0 };
	int output_2[64] = { 0 };
	int subkeys[16][48];
	int key[64];
	int r[17][32], l[17][32];
	IP(input, Ip, IP_Table);// IP initial permutation
							// Sub-keys generation
	subKey_fun(key_in, subkeys);

	// Splitting r l
	for (int i = 0; i < 32; i++) {
		r[16][i] = Ip[i];
		l[16][i] = Ip[32 + i];
	}
	// 16 rounds of F operation or r0 l0
	for (int j = 16; j > 0; j--)
	{
		F_func(l[j], l[j - 1], subkeys[j - 1]);
		Xor(l[j - 1], r[j], 32);
		for (int k = 0; k < 32; k++)
		{
			r[j - 1][k] = l[j][k];
		}
	}
	// Combine r0 l0 to output_1
	for (int t = 0; t < 32; t++)
	{
		output_1[t] = l[0][t];
		output_1[32 + t] = r[0][t];
	}
	// IP permutation to get plaintext
	IP_In(output_1, output, IPR_Table);
};

void DES_EncDec(char* data, int dataLen, char *key, char *output, bool encMode)
{
	int _key[64] = { 0 };
	int _data[64] = { 0 };s
	int _output[64] = { 0 };
	char key_parity[9] = { 0 };

	int cPar;
	strcpy(key_parity, key);
	for (int i = 0; i < strlen(key_parity); i++)
	{
		cPar = 0;
		for (int j = 0; j < 8; j++)
		{
			if (key_parity[i] & (0x01 << j))
				cPar = !cPar;
		}
		if (!cPar)
			key_parity[i] ^= 0x01;
	}

	CharToBit(key_parity, _key);
	// group division is the same as data.length() % 8 ? (data.length() / 8 + 1) * 8
	int blockCount = 0;
	if (dataLen % 8 == 0)
		blockCount = dataLen / 8;
	else
		blockCount = dataLen / 8 + 1;

	for (int i = 0; i < blockCount; i++)
	{
		CharToBit(data + i * 8, _data);

		if (encMode)
		{
			DES_Encrypt(_data, _key, _output);
		}
		else
		{
			DES_Decrypt(_data, _key, _output);
		}

		// Convert the encrypted cipher text 64 bits, to char
		BitToChar(_output, output + i * 8);
	}
}