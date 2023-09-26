#pragma once

int getNowTime_ms();
int ReadLocalFile(const char *filePath, void *fileData, int *fileDataLen);
int WriteLocalFile(const char *filePath, void *data, int dataLen);
void _str_to_hex(unsigned char *pbDest, char *pbSrc, int nLen);
void _hex_to_str(char *pbDest, unsigned char *pbSrc, int nLen);
void BitToChar(int intput[], char output[]);
void DES_Encrypt(char input[8], char key_in[8], int output[64]);
void DES_Decrypt(int input[64], char key_in[8], char output[8]);
void DES_EncDec(char* data, int dataLen, char *key, char *output, bool encMode);