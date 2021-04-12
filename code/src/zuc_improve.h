/************************************************************************
File name:	zuc_improve.h
Date:	Oct 19,2019
Description:  This headfile provide macro defination,parameter definition and function declaration needed in ZUC stream cipher algorithm implementation.
Function List:
1.AddMod_use_by_init_mode	// calculate a+b mod 2^31-1 used by initmode 
2.AddMod_use_by_work_mode   //calculate a+b mod 2^31-1 used by workmode 
3.L1	// linear transformation L1:X^(X<<< 2)^(X<<<10)^(X<<<18)^(X<<<24)
4.L2	// linear transformation L2:X^(X<<< 8)^(X<<<14)^(X<<<22)^(X<<<30)
5.Init_S_box //Initialize S box
6.Init_S_box_reset //Reset S box
7.BR_use_by_Init_mode//Bit Reconstruction used by initmode
8.BR_use_by_work_mode//Bit Reconstruction used by workmode
9.F_use_by_init_mode//nonlinear function used by initmode
10.F_use_by_work_mode//nonlinear function used by workmode
11.LFSRWithInitMode//Initialisation mode,refresh the current state of LFSR
12.ZUC_Init//Initialisation process of ZUC
13.ZUC_Work//working stage of ZUC
14.producer//LFSR and BR  producer
15.consumer//F consumer
16.ZUC_GenKeyStream//generate key stream
17.ZUC_Confidentiality//the ZUC-based condifentiality algorithm
18.ZUC_GenKeyStream_speed//Test the time which is used by the key stream generating
19.ZUC_Confidentiality_speed//Test the time which is used by enciphering data
20.en_decryption//Encrypt or decrypt a file
**************************************************************************/
#pragma once
#include <immintrin.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<ctime>
#include<iostream>
#include <windows.h>
#include <thread>
#include<atomic>
#include<fstream>
#include"lock_free_queue.h"
#include<string>
//rotate n bits to the left in a 32bit buffer
#define ZUC_rotl32(x, k)   (((x) << k) | ((x) >> (32 - k)))
//si = ki|||di|||ivi,in key loading
#define ZUC_LinkToS(a, b, c)	(((unsigned int)(a) << 23)|((unsigned int)(b) << 8)|(unsigned int)(c))
//Sliding window sizemask
#define windows_size_mask 255
//Sliding window size
#define windows_size 256
namespace ZUC_LEARN {
	//__declspec(align(64))
		struct BR_X {
		unsigned int BR_X_data_0;
		unsigned int BR_X_data_1;
		unsigned int BR_X_data_2;
		unsigned int BR_X_data_3;
		unsigned int BR_X_data_4;
		unsigned int BR_X_data_5;
		unsigned int BR_X_data_6;
		unsigned int BR_X_data_7;
		unsigned int BR_X_data_8;
		unsigned int BR_X_data_9;
		unsigned int BR_X_data_10;
		unsigned int BR_X_data_11;
		unsigned int BR_X_data_12;
		unsigned int BR_X_data_13;
		unsigned int BR_X_data_14;
		unsigned int BR_X_data_15;
	};
	extern unsigned int ZUC_S0[];
	extern unsigned int ZUC_S1[];
	extern unsigned short ZUC_S2[];
	extern unsigned char ZUC_S3[];
	extern unsigned int ZUC_d[];
	inline unsigned int L1(unsigned int X)
	{

		return X ^ ZUC_rotl32(X, 2) ^ ZUC_rotl32(X, 10) ^ ZUC_rotl32(X, 18) ^ ZUC_rotl32(X, 24);
	}
	inline unsigned int L2(unsigned int X)
	{
		return X ^ ZUC_rotl32(X, 8) ^ ZUC_rotl32(X, 14) ^ ZUC_rotl32(X, 22) ^ ZUC_rotl32(X, 30);
	}
	inline unsigned int AddMod_use_by_init_mode(unsigned int a, unsigned int b)
	{
		unsigned int c = a + b;

		if (c >> 31)
		{
			c = (c & 0x7fffffff) + 1;
		}
		if (c >> 31)
		{
			c = (c & 0x7fffffff) + 1;
		}
		return c;

	}
    inline unsigned long long AddMod_use_by_work_mode(unsigned long long a, unsigned long long b)
	{
		unsigned long long v = a + b;
		v = (v & 0x7fffffff) + (v >> 31);
		v = (v & 0x7fffffff) + (v >> 31);
		//v = v % 0x7fffffff;
		return v;
	}
	void Init_S_box();
	void Init_S_box_reset();
	void BR_use_by_Init_mode(unsigned int LFSR_S[], unsigned int BR_X[]);
	void BR_use_by_work_mode(unsigned long long LFSR_S[], unsigned int BR_X[], int& Sliding_window_index);
	unsigned int F_use_by_init_mode(unsigned int BR_X[], unsigned int F_R[]);
	unsigned int F_use_by_work_mode(unsigned int BR_X[], unsigned int F_R[]);
	void LFSRWithInitMode(unsigned int LFSR_S[], unsigned int u);
	void ZUC_Init(unsigned char k[], unsigned char iv[], unsigned int LFSR_S[], unsigned int BR_X[], unsigned int F_R[]);
	void ZUC_Work(unsigned long long  LFSR_S[], unsigned int BR_X[], unsigned int F_R[], unsigned int pKeyStream[], int KeyStreamLen, int& Sliding_window_index);
	void producer(unsigned long long  LFSR_S[], int Sliding_window_index, int item_total, LockFreeQueue<BR_X>* queue);
	void consumer(int  item_total, unsigned int pKeyStream[], unsigned int F_R[], LockFreeQueue<BR_X>* queue);
	void ZUC_GenKeyStream(unsigned char k[], unsigned char iv[], unsigned int KeyStream[], int KeyStreamLen);
	void ZUC_Confidentiality(unsigned char CK[], unsigned int COUNT, unsigned char BEARER, unsigned char DIRECTION, unsigned int IBS[], unsigned int LENGTH, unsigned int OBS[]);
	double ZUC_GenKeyStream_speed(int LENGTH);
	double ZUC_Confidentiality_speed(int LENGTH);
	int en_decryption(std::wstring infile, std::wstring outfile, unsigned char* key, unsigned int COUNT, unsigned char BEARER, unsigned char DIRECTION);

}