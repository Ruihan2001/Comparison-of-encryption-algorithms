#include "../desDec/stdafx.h"
#include "aes.h"
#include <string>

//S Box
const unsigned char sBox[256] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
//Inverse S Box
const unsigned char invSBox[256] =
{
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};


//Rcon array used for Key Expansion
const unsigned char RC[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };


//The processing of the last column in the single-round key expansion
void keyExpansionProcess(unsigned char keyPart[],int index)
{

	//4-bit left shift by one
	char temp = keyPart[0];
	keyPart[0] = keyPart[1];
	keyPart[1] = keyPart[2];
	keyPart[2] = keyPart[3];
	keyPart[3] = temp;

	
	for (int i = 0;i < 4;i++)
	{
		keyPart[i] = sBox[keyPart[i]];
	}

	keyPart[0] ^= RC[index];
}


//Key expansion (generation of subkeys)
void keyExpansion(unsigned char key[],unsigned char roundKey[11][16])
{

	//Convert {k0,k1,k2,k3,k4,k5,k6,k7,k8,k9.k10,k11,k12,k13,k14,k15} to
	/*{ k0,k4,k8,k12,
		k1,k5,k9,k13,
		k2,k6,k10,k14,
		k3,k7,k11,k15
	}*/
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4;col++)
		{
			roundKey[0][row * 4 + col] = key[row + col * 4];
		}
	}
	unsigned char temp[4] = { 0 };

	//10 rounds of key transformation
	for (int i = 1; i < 11;i++)
	{
		for (int j = 0;j < 4;j++)
		{
			for (int row = 0; row < 4; row++)
			{
			
				//j=0, temp is the last column at the beginning of the i-th subkey
				if (j == 0)
					temp[row] = roundKey[i - 1][3 + row * 4];
		
				//j ! = 0, temp is the first column of the key to be generated
				else
					temp[row] = roundKey[i][j - 1 + row * 4];
			}
			if (j == 0)
			{
		
				//Handling g operations
				keyExpansionProcess(temp, i - 1);
			}
		
			//To generate a subkey, we have to use the completed temp to dissociate with the corresponding column of the previous round of keys to get a new subkey
			for (int row = 0; row < 4; row++)
				roundKey[i][j + row * 4] = roundKey[i - 1][j + row * 4] ^ temp[row];
		}

	}
}


//Add round key, permutation of the data with the key after XOR operation
void addRoundKey(unsigned char data[16], unsigned char key[16])
{
	for (int i = 0; i < 16;i++)
	{
		data[i] ^= key[i];
	}
}


//Follow the encryption rule to shift the second row left by one, 
//the third row left by two, and the third row left by three to generate a new matrix
void shiftRowsLeft(unsigned char data[])
{
	unsigned char temp;
	unsigned char temp1;
	unsigned char temp2;

	temp = data[4];
	data[4] = data[5];
	data[5] = data[6];
	data[6] = data[7];
	data[7] = temp;

	temp = data[8];
	temp1 = data[9];
	data[8] = data[10];
	data[9] = data[11];
	data[10] = temp;
	data[11] = temp1;

	temp = data[15];
	data[15] = data[14];
	data[14] = data[13];
	data[13] = data[12];
	data[12] = temp;
}


//According to the decryption rule, the second row is shifted to the right by one, 
//the third row is shifted to the right by two, 
//and the third row is shifted to the right by three to generate a new matrix.
void shiftRowsRight(unsigned char data[])
{
	unsigned char temp;
	unsigned char temp1;
	unsigned char temp2;

	temp = data[7];
	data[7] = data[6];
	data[6] = data[5];
	data[5] = data[4];
	data[4] = temp;

	temp = data[8];
	temp1 = data[9];
	data[8] = data[10];
	data[9] = data[11];
	data[10] = temp;
	data[11] = temp1;

	temp = data[12];
	data[12] = data[13];
	data[13] = data[14];
	data[14] = data[15];
	data[15] = temp;
}

//judge if m2 is converted to binary and the first one is 1
//If it is 1, then the value will be shifted to the left first and then do an iso-or with 0x1B, otherwise it will be shifted to the left only.
unsigned char judgeMix(unsigned char m2)
{
	if ((m2 & 0x80) == 0x00)
	{
		return m2 << 1;
	}
	return (m2 << 1) ^ 0x1B;
}

//The data were processed in the order of 2 3 1 1 for each column
unsigned char mixCol(unsigned char m2, unsigned char m3,unsigned char m11,unsigned char m12)
{
	return judgeMix(m2) ^ judgeMix(m3) ^ m3 ^ m11 ^ m12;
}


//Mix Columns (Encryption)
void mixColumns(unsigned char data[])
{
	unsigned char temp[4] = { 0 };
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0;col < 4;col++)
		{
			temp[col] = data[row + col * 4];
		}

		data[row + 0] = mixCol(temp[0], temp[1], temp[2], temp[3]);
		data[row + 4] = mixCol(temp[1], temp[2], temp[3], temp[0]);
		data[row + 8] = mixCol(temp[2], temp[3], temp[0], temp[1]);
		data[row + 12] = mixCol(temp[3], temp[0], temp[1], temp[2]);
	}
}

//https://www.cnblogs.com/chenshikun/p/11667438.html
//Inverse Mix Columns
//Multiplicative computation in Galois domain for computing the inverse mix columns (when decrypting)
unsigned char GFMul(unsigned char a, unsigned char b)
{
	//Define Variable
	 unsigned char Result = 0;      //Result of multiplication in the Galois domain
	 //a != 0
	 while (a)
	 {
		 //If the lowest bit of a is 1, it is anomalous to b, which is equivalent to adding b * 1
		 if (a & 0x01)
		 {
			 Result ^= b;
		 }
		 //a is shifted one place to the right, which is equivalent to dividing by 2
		 a = a >> 1;
		 //If the highest bit of b is 1
		 if (b & 0x80)
		 {
			 //Shifting one place left is equivalent to multiplying by two
			 b = b << 1;     //Note: The highest bit will be lost here, but don't worry

			 b ^= 0x1B;   //Calculate the division in the Galois domain b = b / (x^8 (just missing the highest bit) + x^4 + x^3 + x^1 + 1)
		 }
		 else
		 {
			 // shifting left by one is equivalent to multiplying by two
			 b = b << 1;
		 }
	}
	return Result;
}

//https://www.sohu.com/a/433871771_675634
//Inverse column mixing
//process data in the order e b d 9 for each column
unsigned char mixInverseMul(unsigned char me, unsigned char mb, unsigned char md, unsigned char m9)
{
	return GFMul(me, 0x0e) ^ GFMul(mb, 0x0b) ^ GFMul(md, 0x0d) ^ GFMul(m9, 0x09);
}
void mixInverseColumns(unsigned char data[])
{
	unsigned char temp[4] = { 0 };
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			temp[col] = data[row + col * 4];
		}
		data[row + 0] = mixInverseMul(temp[0], temp[1], temp[2], temp[3]);
		data[row + 4] = mixInverseMul(temp[1], temp[2], temp[3], temp[0]);
		data[row + 8] = mixInverseMul(temp[2], temp[3], temp[0], temp[1]);
		data[row + 12] = mixInverseMul(temp[3], temp[0], temp[1], temp[2]);
	}
}

//Encryption Process of Single Block
void encryptBlock(unsigned char data[16], unsigned char key[11][16])
{
	unsigned char enc_data[16] = { 0 };
	//1 row->4 columns conversion (same meaning as k0..k15 note at line 76)
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			enc_data[row * 4 + col] = data[row + col * 4];
		}
	}

	//Key substitution 
	//The plaintext does the XOR operation with the first round of keys
	addRoundKey(enc_data, key[0]);

	//10 rounds of encryption process
	for (int i = 1;i < 11;i++)
	{

		//S-box substitution
		for (int j = 0; j < 16; j++)
		{
			enc_data[j] = sBox[enc_data[j]];
		}

		//Shift left
		shiftRowsLeft(enc_data);
		//No column mix opeartion in the last round (specified in the regulations)
		if (i == 10)
		{
			break;
		}
		mixColumns(enc_data);
		addRoundKey(enc_data, key[i]);

	}
	// The last round does not do column mix but requires key encryption, which is done after jumping out of the loop
	addRoundKey(enc_data, key[10]);
	// then convert k0,k4,k8,k12.... .k16, to k0,k1,k2,k3...
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			data[row * 4 + col] = enc_data[row + col * 4];
		}
	}

}

// Decryption Process of Single Block
// the order of the keys used, the S-box and the shift and mix columns are the opposite of the encryption
void decryptBlock(unsigned char data[16], unsigned char key[11][16])
{
	unsigned char dec_data[16] = { 0 };
	//1 row->4 columns conversion (same as the k0..k15 comment at line 76)
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			dec_data[row * 4 + col] = data[row + col * 4];
		}
	}
	// Reverse order key replacement, the key order starts from the last one
	addRoundKey(dec_data, key[10]);
	for (int i = 1; i < 11; i++)
	{
		// Move right first
		shiftRowsRight(dec_data);

		// Inverse S-box substitution
		for (int j = 0; j < 16; j++)
		{
			dec_data[j] = invSBox[dec_data[j]];
		}
		//Note that 10 - i is decreasing from 9
		addRoundKey(dec_data, key[10 - i]);
		// No inverse mix columns in the last round
		if (i == 10)
		{
			break;
		}
		//Inverse column mixing
		mixInverseColumns(dec_data);

	}

	// then convert k0,k4,k8,k12.... .k16, to k0,k1,k2,k3...
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			data[row * 4 + col] = dec_data[row + col * 4];
		}
	}

}


void AES_EncDec(unsigned char* data, int dataLen, char *key, bool encMode)
{
	unsigned char roundKey[11][16] = { 0 };
	//group division, one group of 16 bytes
	int blockCount = 0;
	if (dataLen % 16 == 0)
		blockCount = dataLen / 16;
	else
		blockCount = dataLen / 16 + 1;

	keyExpansion((unsigned char*)key, roundKey);
	for (int i = 0; i < blockCount; i++)
	{
		if (encMode)
		{
			encryptBlock(data + i * 16, roundKey);
		}
		else
		{
			decryptBlock(data + i * 16, roundKey);
		}
	}
}