#ifndef    _AES_H_
#define    _AES_H_
#include  "stdlib.h"
#include  "string.h"



//AES����  16�ֽ�һ�����ݿ�
//IV_IN_OUT:        ��������  �������
//State_IN_OUT��    ��������  �������
//key128bit:        ��Կ  128bit  16�ֽ�
void Aes_IV_key128bit_Encrypt(unsigned char *IV_IN_OUT, unsigned char *State_IN_OUT, unsigned char *key128bit);


//AES����  16�ֽ�һ�����ݿ�
//IV_IN_OUT:        ��������  ԭ�������
//State_IN_OUT��    ��������  �������
//key128bit:        ��Կ  128bit  16�ֽ�
void Aes_IV_key128bit_Decode(unsigned char *IV_IN_OUT, unsigned char *State_IN_OUT, unsigned char *key128bit);

//AES����  16�ֽ�һ�����ݿ�
//IV_IN_OUT:        ��������  �������
//State_IN_OUT��    ��������  �������
//key192bit:        ��Կ  192bit  24�ֽ�
void Aes_IV_key192bit_Encrypt(unsigned char *IV_IN_OUT, unsigned char *State_IN_OUT, unsigned char *key192bit);

//AES����  16�ֽ�һ�����ݿ�
//IV_IN_OUT:        ��������  ԭ�������
//State_IN_OUT��    ��������  �������
//key192bit:        ��Կ  192bit  24�ֽ�
void Aes_IV_key192bit_Decode(unsigned char *IV_IN_OUT, unsigned char *State_IN_OUT, unsigned char *key192bit);


//AES����  16�ֽ�һ�����ݿ�
//IV_IN_OUT:        ��������  �������
//State_IN_OUT��    ��������  �������
//key256bit:        ��Կ  256bit  32�ֽ�
void Aes_IV_key256bit_Encrypt(unsigned char *IV_IN_OUT, unsigned char *State_IN_OUT, unsigned char *key256bit);


//AES����  16�ֽ�һ�����ݿ�
//IV_IN_OUT:        ��������  ԭ�������
//State_IN_OUT��    ��������  �������
//key256bit:        ��Կ  256bit  32�ֽ�
void Aes_IV_key256bit_Decode(unsigned char *IV_IN_OUT, unsigned char *State_IN_OUT, unsigned char *key256bit);



#endif



