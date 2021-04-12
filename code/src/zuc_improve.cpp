/************************************************************************
File name:	zuc_improve.h
Date:	Oct 19,2019
Description:  This code provide the implement of ZUC algorithm,which consist of three parts:key stream generation,confidentiality algorithm.
Function List:
1.Init_S_box //Initialize S box
2.Init_S_box_reset //Reset S box
3.BR_use_by_Init_mode//Bit Reconstruction used by initmode
4.BR_use_by_work_mode//Bit Reconstruction used by workmode
5.F_use_by_init_mode//nonlinear function used by initmode
6.F_use_by_work_mode//nonlinear function used by workmode
7.LFSRWithInitMode//Initialisation mode,refresh the current state of LFSR
8.ZUC_Init//Initialisation process of ZUC
9.ZUC_Work//working stage of ZUC
10.producer//LFSR and BR  producer
11.consumer//F consumer
12.ZUC_GenKeyStream//generate key stream
13.ZUC_Confidentiality//the ZUC-based condifentiality algorithm
14.ZUC_GenKeyStream_speed//Test the time which is used by the key stream generating
15.ZUC_Confidentiality_speed//Test the time which is used by enciphering data
16.en_decryption//Encrypt or decrypt a file
**************************************************************************/

#include"../include/zuclib.h"
#include"zuc_improve.h"

namespace ZUC_LEARN {
	unsigned int ZUC_S0[256] =
	{ 0x3e,0x72,0x5b,0x47,0xca,0xe0,0x00,0x33,0x04,0xd1,0x54,0x98,0x09,0xb9,0x6d,0xcb,
	0x7b,0x1b,0xf9,0x32,0xaf,0x9d,0x6a,0xa5,0xb8,0x2d,0xfc,0x1d,0x08,0x53,0x03,0x90,
	0x4d,0x4e,0x84,0x99,0xe4,0xce,0xd9,0x91,0xdd,0xb6,0x85,0x48,0x8b,0x29,0x6e,0xac,
	0xcd,0xc1,0xf8,0x1e,0x73,0x43,0x69,0xc6,0xb5,0xbd,0xfd,0x39,0x63,0x20,0xd4,0x38,
	0x76,0x7d,0xb2,0xa7,0xcf,0xed,0x57,0xc5,0xf3,0x2c,0xbb,0x14,0x21,0x06,0x55,0x9b,
	0xe3,0xef,0x5e,0x31,0x4f,0x7f,0x5a,0xa4,0x0d,0x82,0x51,0x49,0x5f,0xba,0x58,0x1c,
	0x4a,0x16,0xd5,0x17,0xa8,0x92,0x24,0x1f,0x8c,0xff,0xd8,0xae,0x2e,0x01,0xd3,0xad,
	0x3b,0x4b,0xda,0x46,0xeb,0xc9,0xde,0x9a,0x8f,0x87,0xd7,0x3a,0x80,0x6f,0x2f,0xc8,
	0xb1,0xb4,0x37,0xf7,0x0a,0x22,0x13,0x28,0x7c,0xcc,0x3c,0x89,0xc7,0xc3,0x96,0x56,
	0x07,0xbf,0x7e,0xf0,0x0b,0x2b,0x97,0x52,0x35,0x41,0x79,0x61,0xa6,0x4c,0x10,0xfe,
	0xbc,0x26,0x95,0x88,0x8a,0xb0,0xa3,0xfb,0xc0,0x18,0x94,0xf2,0xe1,0xe5,0xe9,0x5d,
	0xd0,0xdc,0x11,0x66,0x64,0x5c,0xec,0x59,0x42,0x75,0x12,0xf5,0x74,0x9c,0xaa,0x23,
	0x0e,0x86,0xab,0xbe,0x2a,0x02,0xe7,0x67,0xe6,0x44,0xa2,0x6c,0xc2,0x93,0x9f,0xf1,
	0xf6,0xfa,0x36,0xd2,0x50,0x68,0x9e,0x62,0x71,0x15,0x3d,0xd6,0x40,0xc4,0xe2,0x0f,
	0x8e,0x83,0x77,0x6b,0x25,0x05,0x3f,0x0c,0x30,0xea,0x70,0xb7,0xa1,0xe8,0xa9,0x65,
	0x8d,0x27,0x1a,0xdb,0x81,0xb3,0xa0,0xf4,0x45,0x7a,0x19,0xdf,0xee,0x78,0x34,0x60 };

	unsigned int ZUC_S1[256] =
	{ 0x55,0xc2,0x63,0x71,0x3b,0xc8,0x47,0x86,0x9f,0x3c,0xda,0x5b,0x29,0xaa,0xfd,0x77,
	0x8c,0xc5,0x94,0x0c,0xa6,0x1a,0x13,0x00,0xe3,0xa8,0x16,0x72,0x40,0xf9,0xf8,0x42,
	0x44,0x26,0x68,0x96,0x81,0xd9,0x45,0x3e,0x10,0x76,0xc6,0xa7,0x8b,0x39,0x43,0xe1,
	0x3a,0xb5,0x56,0x2a,0xc0,0x6d,0xb3,0x05,0x22,0x66,0xbf,0xdc,0x0b,0xfa,0x62,0x48,
	0xdd,0x20,0x11,0x06,0x36,0xc9,0xc1,0xcf,0xf6,0x27,0x52,0xbb,0x69,0xf5,0xd4,0x87,
	0x7f,0x84,0x4c,0xd2,0x9c,0x57,0xa4,0xbc,0x4f,0x9a,0xdf,0xfe,0xd6,0x8d,0x7a,0xeb,
	0x2b,0x53,0xd8,0x5c,0xa1,0x14,0x17,0xfb,0x23,0xd5,0x7d,0x30,0x67,0x73,0x08,0x09,
	0xee,0xb7,0x70,0x3f,0x61,0xb2,0x19,0x8e,0x4e,0xe5,0x4b,0x93,0x8f,0x5d,0xdb,0xa9,
	0xad,0xf1,0xae,0x2e,0xcb,0x0d,0xfc,0xf4,0x2d,0x46,0x6e,0x1d,0x97,0xe8,0xd1,0xe9,
	0x4d,0x37,0xa5,0x75,0x5e,0x83,0x9e,0xab,0x82,0x9d,0xb9,0x1c,0xe0,0xcd,0x49,0x89,
	0x01,0xb6,0xbd,0x58,0x24,0xa2,0x5f,0x38,0x78,0x99,0x15,0x90,0x50,0xb8,0x95,0xe4,
	0xd0,0x91,0xc7,0xce,0xed,0x0f,0xb4,0x6f,0xa0,0xcc,0xf0,0x02,0x4a,0x79,0xc3,0xde,
	0xa3,0xef,0xea,0x51,0xe6,0x6b,0x18,0xec,0x1b,0x2c,0x80,0xf7,0x74,0xe7,0xff,0x21,
	0x5a,0x6a,0x54,0x1e,0x41,0x31,0x92,0x35,0xc4,0x33,0x07,0x0a,0xba,0x7e,0x0e,0x34,
	0x88,0xb1,0x98,0x7c,0xf3,0x3d,0x60,0x6c,0x7b,0xca,0xd3,0x1f,0x32,0x65,0x04,0x28,
	0x64,0xbe,0x85,0x9b,0x2f,0x59,0x8a,0xd7,0xb0,0x25,0xac,0xaf,0x12,0x03,0xe2,0xf2
	};
	unsigned short ZUC_S2[256] =
	{ 0x3e,0x72,0x5b,0x47,0xca,0xe0,0x00,0x33,0x04,0xd1,0x54,0x98,0x09,0xb9,0x6d,0xcb,
	0x7b,0x1b,0xf9,0x32,0xaf,0x9d,0x6a,0xa5,0xb8,0x2d,0xfc,0x1d,0x08,0x53,0x03,0x90,
	0x4d,0x4e,0x84,0x99,0xe4,0xce,0xd9,0x91,0xdd,0xb6,0x85,0x48,0x8b,0x29,0x6e,0xac,
	0xcd,0xc1,0xf8,0x1e,0x73,0x43,0x69,0xc6,0xb5,0xbd,0xfd,0x39,0x63,0x20,0xd4,0x38,
	0x76,0x7d,0xb2,0xa7,0xcf,0xed,0x57,0xc5,0xf3,0x2c,0xbb,0x14,0x21,0x06,0x55,0x9b,
	0xe3,0xef,0x5e,0x31,0x4f,0x7f,0x5a,0xa4,0x0d,0x82,0x51,0x49,0x5f,0xba,0x58,0x1c,
	0x4a,0x16,0xd5,0x17,0xa8,0x92,0x24,0x1f,0x8c,0xff,0xd8,0xae,0x2e,0x01,0xd3,0xad,
	0x3b,0x4b,0xda,0x46,0xeb,0xc9,0xde,0x9a,0x8f,0x87,0xd7,0x3a,0x80,0x6f,0x2f,0xc8,
	0xb1,0xb4,0x37,0xf7,0x0a,0x22,0x13,0x28,0x7c,0xcc,0x3c,0x89,0xc7,0xc3,0x96,0x56,
	0x07,0xbf,0x7e,0xf0,0x0b,0x2b,0x97,0x52,0x35,0x41,0x79,0x61,0xa6,0x4c,0x10,0xfe,
	0xbc,0x26,0x95,0x88,0x8a,0xb0,0xa3,0xfb,0xc0,0x18,0x94,0xf2,0xe1,0xe5,0xe9,0x5d,
	0xd0,0xdc,0x11,0x66,0x64,0x5c,0xec,0x59,0x42,0x75,0x12,0xf5,0x74,0x9c,0xaa,0x23,
	0x0e,0x86,0xab,0xbe,0x2a,0x02,0xe7,0x67,0xe6,0x44,0xa2,0x6c,0xc2,0x93,0x9f,0xf1,
	0xf6,0xfa,0x36,0xd2,0x50,0x68,0x9e,0x62,0x71,0x15,0x3d,0xd6,0x40,0xc4,0xe2,0x0f,
	0x8e,0x83,0x77,0x6b,0x25,0x05,0x3f,0x0c,0x30,0xea,0x70,0xb7,0xa1,0xe8,0xa9,0x65,
	0x8d,0x27,0x1a,0xdb,0x81,0xb3,0xa0,0xf4,0x45,0x7a,0x19,0xdf,0xee,0x78,0x34,0x60 };


	unsigned char ZUC_S3[256] =
	{ 0x55,0xc2,0x63,0x71,0x3b,0xc8,0x47,0x86,0x9f,0x3c,0xda,0x5b,0x29,0xaa,0xfd,0x77,
	0x8c,0xc5,0x94,0x0c,0xa6,0x1a,0x13,0x00,0xe3,0xa8,0x16,0x72,0x40,0xf9,0xf8,0x42,
	0x44,0x26,0x68,0x96,0x81,0xd9,0x45,0x3e,0x10,0x76,0xc6,0xa7,0x8b,0x39,0x43,0xe1,
	0x3a,0xb5,0x56,0x2a,0xc0,0x6d,0xb3,0x05,0x22,0x66,0xbf,0xdc,0x0b,0xfa,0x62,0x48,
	0xdd,0x20,0x11,0x06,0x36,0xc9,0xc1,0xcf,0xf6,0x27,0x52,0xbb,0x69,0xf5,0xd4,0x87,
	0x7f,0x84,0x4c,0xd2,0x9c,0x57,0xa4,0xbc,0x4f,0x9a,0xdf,0xfe,0xd6,0x8d,0x7a,0xeb,
	0x2b,0x53,0xd8,0x5c,0xa1,0x14,0x17,0xfb,0x23,0xd5,0x7d,0x30,0x67,0x73,0x08,0x09,
	0xee,0xb7,0x70,0x3f,0x61,0xb2,0x19,0x8e,0x4e,0xe5,0x4b,0x93,0x8f,0x5d,0xdb,0xa9,
	0xad,0xf1,0xae,0x2e,0xcb,0x0d,0xfc,0xf4,0x2d,0x46,0x6e,0x1d,0x97,0xe8,0xd1,0xe9,
	0x4d,0x37,0xa5,0x75,0x5e,0x83,0x9e,0xab,0x82,0x9d,0xb9,0x1c,0xe0,0xcd,0x49,0x89,
	0x01,0xb6,0xbd,0x58,0x24,0xa2,0x5f,0x38,0x78,0x99,0x15,0x90,0x50,0xb8,0x95,0xe4,
	0xd0,0x91,0xc7,0xce,0xed,0x0f,0xb4,0x6f,0xa0,0xcc,0xf0,0x02,0x4a,0x79,0xc3,0xde,
	0xa3,0xef,0xea,0x51,0xe6,0x6b,0x18,0xec,0x1b,0x2c,0x80,0xf7,0x74,0xe7,0xff,0x21,
	0x5a,0x6a,0x54,0x1e,0x41,0x31,0x92,0x35,0xc4,0x33,0x07,0x0a,0xba,0x7e,0x0e,0x34,
	0x88,0xb1,0x98,0x7c,0xf3,0x3d,0x60,0x6c,0x7b,0xca,0xd3,0x1f,0x32,0x65,0x04,0x28,
	0x64,0xbe,0x85,0x9b,0x2f,0x59,0x8a,0xd7,0xb0,0x25,0xac,0xaf,0x12,0x03,0xe2,0xf2 };
	unsigned int ZUC_d[16] = { 0x44D7, 0x26BC, 0x626B, 0x135E, 0x5789, 0x35E2, 0x7135, 0x09AF,0x4D78, 0x2F13, 0x6BC4, 0x1AF1, 0x5E26, 0x3C4D, 0x789A, 0x47AC };
/************************************************************
Function:     Init_S_box
Description:  Initialisation mode,set Sbox
Called By:    ZUC_GenKeyStream
Input:        Null
Output:		  Null
Return:       Null Others:
************************************************************/
	void Init_S_box()
	{
		for (int i = 0; i < 256; i++)
		{
			ZUC_S2[i] <<= 8;
			ZUC_S1[i] <<= 16;
			ZUC_S0[i] <<= 24;
		}
	};
/************************************************************
Function:     Init_S_box_reset
Description:  Initialisation mode,reset Sbox
Called By:    ZUC_GenKeyStream
Input:        Null
Output:		  Null
Return:       Null 
************************************************************/
	void Init_S_box_reset()
	{
		for (int i = 0; i < 256; i++)
		{
			ZUC_S2[i] >>= 8;
			ZUC_S1[i] >>= 16;
			ZUC_S0[i] >>= 24;
		}
	}
/************************************************************
Function:	BR_use_by_Init_mode
Description:  Bit Reconstruction used by init mode
Called By:	ZUC_Init
Input:	LFSR_S:current state of LFSR
Output:	BR_X[]:achieve X0,X1,X2,X3
Return:	Null
************************************************************/
	void BR_use_by_Init_mode(unsigned int LFSR_S[], unsigned int BR_X[])
	{
		BR_X[0] = ((LFSR_S[15] & 0x7fff8000) << 1) | (LFSR_S[14] & 0x0000ffff);
		BR_X[1] = ((LFSR_S[11] & 0x0000ffff) << 16) | ((LFSR_S[9] & 0x7fff8000) >> 15);
		BR_X[2] = ((LFSR_S[7] & 0x0000ffff) << 16) | ((LFSR_S[5] & 0x7fff8000) >> 15);
		BR_X[3] = ((LFSR_S[2] & 0x0000ffff) << 16) | ((LFSR_S[0] & 0x7fff8000) >> 15);
	}
/************************************************************
Function:	BR_use_by_work_mode
Description:  Bit Reconstruction used by work mode
Called By:	ZUC_Work
Input:	LFSR_S:current state of LFSR,Current index of Sliding windows
Output:	BR_X[]:achieve X0,X1,X2,X3
Return:	Null
************************************************************/
	void BR_use_by_work_mode(unsigned long long LFSR_S[], unsigned int BR_X[], int& Sliding_window_index)
	{
		int t = windows_size - 15;
		if (Sliding_window_index < t) {
			BR_X[0] = ((LFSR_S[Sliding_window_index + 15] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index)] & 0x0000ffff);
			BR_X[1] = ((LFSR_S[Sliding_window_index + 11] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index)] & 0x7fff8000) >> 15);
			BR_X[2] = ((LFSR_S[Sliding_window_index + 7] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index)] & 0x7fff8000) >> 15);
			BR_X[3] = ((LFSR_S[Sliding_window_index + 2] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index)] & 0x7fff8000) >> 15);
		}
		else {
			BR_X[0] = ((LFSR_S[(Sliding_window_index + 15) & windows_size_mask] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index) & windows_size_mask] & 0x0000ffff);
			BR_X[1] = ((LFSR_S[(Sliding_window_index + 11) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15);
			BR_X[2] = ((LFSR_S[(Sliding_window_index + 7) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15);
			BR_X[3] = ((LFSR_S[(Sliding_window_index + 2) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15);
		}
	}
/************************************************************
Function:	F_use_by_init_mode
Description:  nonlinear function used by  init mode
Calls:
Called By:	ZUC_Init
Input:	BR_X[]:words X0,X1,X2,X3 from BR F_R[]:F_R[0]=R1,F_R[1]=R2
Output:
Return:	W
************************************************************/
	unsigned int F_use_by_init_mode(unsigned int BR_X[], unsigned int F_R[])
	{
		unsigned int W, W1, W2;
		W = (BR_X[0] ^ F_R[0]) + F_R[1];
		W1 = F_R[0] + BR_X[1];
		W2 = F_R[1] ^ BR_X[2];
		F_R[0] = L1((W1 << 16) | (W2 >> 16));
		F_R[0] = (ZUC_S2[(F_R[0] >> 24) & 0xFF]) << 24
			| (ZUC_S3[(F_R[0] >> 16) & 0xFF]) << 16
			| (ZUC_S2[(F_R[0] >> 8) & 0xFF]) << 8
			| (ZUC_S3[F_R[0] & 0xFF]);
		F_R[1] = L2((W2 << 16) | (W1 >> 16));
		F_R[1] = (ZUC_S2[(F_R[1] >> 24) & 0xFF]) << 24
			| (ZUC_S3[(F_R[1] >> 16) & 0xFF]) << 16
			| (ZUC_S2[(F_R[1] >> 8) & 0xFF]) << 8
			| (ZUC_S3[F_R[1] & 0xFF]);
		return W;
	};
/************************************************************
Function:	F_use_by_work_mode
Description:  nonlinear function used by work mode
Calls:
Called By:	ZUC_Init
Input:	BR_X[]:words X0,X1,X2,X3 from BR F_R[]:F_R[0]=R1,F_R[1]=R2
Output:
Return:	W
************************************************************/
	unsigned int F_use_by_work_mode(unsigned int BR_X[], unsigned int F_R[])
	{
		unsigned int W, W1, W2;
		W = (BR_X[0] ^ F_R[0]) + F_R[1];
		W1 = F_R[0] + BR_X[1];
		W2 = F_R[1] ^ BR_X[2];
		F_R[0] = L1((W1 << 16) | (W2 >> 16));
		F_R[0] = (ZUC_S0[(F_R[0] >> 24)])
			| (ZUC_S1[(F_R[0] >> 16) & 0xFF])
			| (ZUC_S2[(F_R[0] >> 8) & 0xFF])
			| (ZUC_S3[F_R[0] & 0xFF]);
		F_R[1] = L2((W2 << 16) | (W1 >> 16));
		F_R[1] = (ZUC_S0[(F_R[1] >> 24)])
			| (ZUC_S1[(F_R[1] >> 16) & 0xFF])
			| (ZUC_S2[(F_R[1] >> 8) & 0xFF])
			| (ZUC_S3[F_R[1] & 0xFF]);
		return W;
	}
/************************************************************
Function:     LFSRWithInitMode
Description:  Initialisation mode,refresh the current state of LFSR 
Calls:	AddMod_use_by_init_mode
Called By:    ZUC_Init
Input:        LFSR_S:current state of LFSR
			  u:u=W>>1
Output:		  Null
Return:       Null Others:
************************************************************/
	void LFSRWithInitMode(unsigned int LFSR_S[], unsigned int u)
	{
		unsigned long long v = LFSR_S[0];
		v += (unsigned long long(LFSR_S[15]) << 15);
		v += (unsigned long long(LFSR_S[13]) << 17);
		v += (unsigned long long(LFSR_S[10]) << 21);
		v += (unsigned long long(LFSR_S[4]) << 20);
		v += (unsigned long long(LFSR_S[0]) << 8);
		v = (v & 0x7fffffff) + (v >> 31);
		v = (v & 0x7fffffff) + (v >> 31);
		for (int i = 0; i < 15; i++)
		{
			LFSR_S[i] = LFSR_S[i + 1];
		}
		LFSR_S[15] = AddMod_use_by_init_mode(v, u);


		if (!LFSR_S[15])
		{
			LFSR_S[15] = 0x7fffffff;
		}
	};
/************************************************************
Function:	ZUC_Init
Description:  Initialisation process of ZUC
Calls:	ZUC_LinkToS,BR_use_by_Init_mode,F_use_by_init_mode,LFSRWithInitMode
Called By:	ZUC_GenKeyStream
Input:	k:initial key
		iv:initial vector
Output:	LFSR_S[]:the state of LFSR after initialisation:s0,s1,s2,..s15
		BR_X[] : the current value:X0,X1,X2,X3
		F_R[]:the current value:R1,R2,F_R[0]=R1,F_R[1]=R2
Return:	Null
Others:
************************************************************/
	void ZUC_Init(unsigned char k[], unsigned char iv[], unsigned int LFSR_S[], unsigned int BR_X[], unsigned int F_R[])
	{
		unsigned char count = 32;
		int i;
		//loading key to the LFSR s0,s1,s2....s15
		for (i = 0; i < 16; i++)
		{
			LFSR_S[i] = ZUC_LinkToS(k[i], ZUC_d[i], iv[i]);
		}
		F_R[0] = 0x00;	//R1
		F_R[1] = 0x00;	//R2
		while (count)	//32 times
		{
			unsigned int W;
			BR_use_by_Init_mode(LFSR_S, BR_X); //BitReconstruction
			W = F_use_by_init_mode(BR_X, F_R);	//nonlinear function
			LFSRWithInitMode(LFSR_S, W >> 1);
			count--;
		}
	}
/************************************************************
Function:	producer
Description:  Producers in the producer/consumer model,Generates BR to write to the loop queue buffer
Calls:	AddMod_use_by_work_mode
Called By:	ZUC_Work
Input:	Sliding_window_index: current sliding windows index
		LFSR_S[]:the state of LFSR after initialisation
		item_total:The number of groups that produce BR
		queue:Circular queue buffer
Output:	BR_X : Four groups of the current value:X0,X1,X2,X3
Return:	Null
Others:
************************************************************/
	void producer(unsigned long long  LFSR_S[], int Sliding_window_index, int item_total, LockFreeQueue<BR_X>* queue) {
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		SetThreadAffinityMask(GetCurrentThread(), 0x000000C0);
		int barrier = windows_size - 19;
		int BR_barrier= windows_size - 15;
		for (int i = 0; i < item_total ; i+=4) {
			
			/*******************************YMM寄存器加载************************************************/
			if (Sliding_window_index  < barrier) {

				LFSR_S[(16 + Sliding_window_index)] = 0;//为了初始化ymm4={S13,S14,S15,0};
				__m256i ymm1 = _mm256_load_si256((__m256i*)(LFSR_S + (Sliding_window_index)));//ymm1={S0,S1,S2,S3};
				__m256i ymm2 = _mm256_load_si256((__m256i*)(LFSR_S + (Sliding_window_index + 4)));//ymm2={S4,S5,S6,S7};
				__m256i ymm3 = _mm256_load_si256((__m256i*)(LFSR_S + (Sliding_window_index + 10)));//ymm3={S10,S11,S12,S13};
				__m256i ymm4 = _mm256_load_si256((__m256i*)(LFSR_S + (Sliding_window_index + 13))); //ymm4={S13,S14,S15,0}


				__m256i ymma = _mm256_add_epi64(ymm1, _mm256_slli_epi64(ymm1, 8));//ymm1=ymm1+ymm1<<8
				__m256i ymmb = _mm256_slli_epi64(ymm2, 20);//ymm2=ymm2<<20;
				__m256i ymmc = _mm256_slli_epi64(ymm3, 21);// ymm3=ymm3<<21; 
				__m256i ymmd = _mm256_slli_epi64(ymm4, 17);//ymm4=ymm4<<17;

				ymm1 = _mm256_add_epi64(ymma, ymmb);

				ymm3 = _mm256_add_epi64(ymmc, ymmd);
	
				//ymm5=ymm1+ymm2+ymm3+ymm4;// {S16,S17,S18,S19}=ymm5
				*((__m256i*)(LFSR_S + (16 + Sliding_window_index))) = _mm256_add_epi64(_mm256_add_epi64(ymma, ymmb), _mm256_add_epi64(ymmc, ymmd));
				
				
				LFSR_S[(16 + Sliding_window_index)] = AddMod_use_by_work_mode(LFSR_S[(15 + Sliding_window_index)] << 15, LFSR_S[(16 + Sliding_window_index)]);
				//S16 =(2^15*S_15+S_16)mod(2^31-1)
				LFSR_S[(17 + Sliding_window_index)] = AddMod_use_by_work_mode(LFSR_S[(16 + Sliding_window_index)] << 15, LFSR_S[(17 + Sliding_window_index)]);
				//S17 =(2^15*S_16+S_17)mod(2^31-1);
				LFSR_S[(18 + Sliding_window_index)] = AddMod_use_by_work_mode(LFSR_S[(17 + Sliding_window_index)] << 15, LFSR_S[(18 + Sliding_window_index)]);
				//S18 =(2^15*S_17+S_18)mod(2^31-1);
				LFSR_S[(19 + Sliding_window_index)] = AddMod_use_by_work_mode((LFSR_S[(18 + Sliding_window_index)] << 15) + (LFSR_S[(16 + Sliding_window_index)] << 17), LFSR_S[(19 + Sliding_window_index)]);
				// S19 =(2^15*S_18+2^17*S_16+S_19)mod(2^31-1); 
			}
			else
			{
				LFSR_S[(16 + Sliding_window_index) & windows_size_mask] = 0;//为了初始化ymm4={S13,S14,S15,0};
				__m256i temp0 = _mm256_set_epi64x((Sliding_window_index + 3) & windows_size_mask, (Sliding_window_index + 2) & windows_size_mask, (Sliding_window_index + 1) & windows_size_mask, (Sliding_window_index) & windows_size_mask);
				__m256i ymm1 = _mm256_i64gather_epi64((long long const*)LFSR_S, temp0, 8);//ymm1={S0,S1,S2,S3};
				__m256i temp1 = _mm256_set_epi64x((Sliding_window_index + 7) & windows_size_mask, (Sliding_window_index + 6) & windows_size_mask, (Sliding_window_index + 5) & windows_size_mask, (Sliding_window_index + 4) & windows_size_mask);
				__m256i ymm2 = _mm256_i64gather_epi64((long long const*)(LFSR_S), temp1, 8);//ymm2={S4,S5,S6,S7};
				__m256i temp2 = _mm256_set_epi64x((Sliding_window_index + 13) & windows_size_mask, (Sliding_window_index + 12) & windows_size_mask, (Sliding_window_index + 11) & windows_size_mask, (Sliding_window_index + 10) & windows_size_mask);
				__m256i ymm3 = _mm256_i64gather_epi64((long long const*)(LFSR_S), temp2, 8);//ymm3={S10,S11,S12,S13};
				__m256i temp3 = _mm256_set_epi64x((Sliding_window_index + 16) & windows_size_mask, (Sliding_window_index + 15) & windows_size_mask, (Sliding_window_index + 14) & windows_size_mask, (Sliding_window_index + 13) & windows_size_mask);
				__m256i ymm4 = _mm256_i64gather_epi64((long long const*)LFSR_S, temp3, 8); //ymm4={S13,S14,S15,0}
				ymm1 = _mm256_add_epi64(ymm1, _mm256_slli_epi64(ymm1, 8));//ymm1=ymm1+ymm1<<8
				ymm2 = _mm256_slli_epi64(ymm2, 20);//ymm2=ymm2<<20;
				ymm3 = _mm256_slli_epi64(ymm3, 21);// ymm3=ymm3<<21; 
				ymm4 = _mm256_slli_epi64(ymm4, 17);//ymm4=ymm4<<17;
				__m256i ymm5 = _mm256_add_epi64(_mm256_add_epi64(ymm1, ymm2), _mm256_add_epi64(ymm3, ymm4));//ymm5=ymm1+ymm2+ymm3+ymm4;
				unsigned long long LFSR_S_16_19[4];
				*((__m256i*)(LFSR_S_16_19)) = ymm5;// {S16,S17,S18,S19}=ymm5
				LFSR_S[(16 + Sliding_window_index) & windows_size_mask] = AddMod_use_by_work_mode(LFSR_S[(15 + Sliding_window_index) & windows_size_mask] << 15, LFSR_S_16_19[0]);
				//S16 =(2^15*S_15+S_16)mod(2^31-1)
				LFSR_S[(17 + Sliding_window_index) & windows_size_mask] = AddMod_use_by_work_mode(LFSR_S[(16 + Sliding_window_index) & windows_size_mask] << 15, LFSR_S_16_19[1]);
				//S17 =(2^15*S_16+S_17)mod(2^31-1);
				LFSR_S[(18 + Sliding_window_index) & windows_size_mask] = AddMod_use_by_work_mode(LFSR_S[(17 + Sliding_window_index) & windows_size_mask] << 15, LFSR_S_16_19[2]);
				//S18 =(2^15*S_17+S_18)mod(2^31-1);
				LFSR_S[(19 + Sliding_window_index) & windows_size_mask] = AddMod_use_by_work_mode((LFSR_S[(18 + Sliding_window_index) & windows_size_mask] << 15) + (LFSR_S[(16 + Sliding_window_index) & windows_size_mask] << 17), LFSR_S_16_19[3]);
				// S19 =(2^15*S_18+2^17*S_16+S_19)mod(2^31-1); 
			}

			BR_X t;
			Sliding_window_index = (Sliding_window_index + 1) & windows_size_mask;
			if (!(unsigned int)LFSR_S[(15 + Sliding_window_index) & windows_size_mask])
			{
				LFSR_S[(15 + Sliding_window_index) & windows_size_mask] = 0x7fffffff;
			}

			if (Sliding_window_index  < BR_barrier) {
				t.BR_X_data_0 = (((LFSR_S[Sliding_window_index + 15] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index)] & 0x0000ffff));
				t.BR_X_data_1 = ((((LFSR_S[Sliding_window_index + 11] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index)] & 0x7fff8000) >> 15)));
				t.BR_X_data_2 = ((((LFSR_S[Sliding_window_index + 7] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index)] & 0x7fff8000) >> 15)));
				t.BR_X_data_3 = ((((LFSR_S[Sliding_window_index + 2] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index)] & 0x7fff8000) >> 15)));
			}
			else {
				t.BR_X_data_0 = ((((LFSR_S[(Sliding_window_index + 15) & windows_size_mask] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index) & windows_size_mask] & 0x0000ffff)));
				t.BR_X_data_1 = ((((LFSR_S[(Sliding_window_index + 11) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
				t.BR_X_data_2 = ((((LFSR_S[(Sliding_window_index + 7) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
				t.BR_X_data_3 = ((((LFSR_S[(Sliding_window_index + 2) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
			}


			Sliding_window_index = (Sliding_window_index + 1) & windows_size_mask;
			if (!(unsigned int)LFSR_S[(15 + Sliding_window_index) & windows_size_mask])
			{
				LFSR_S[(15 + Sliding_window_index) & windows_size_mask] = 0x7fffffff;
			}


			if (Sliding_window_index  < BR_barrier) {
				t.BR_X_data_4 = (((LFSR_S[Sliding_window_index + 15] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index)] & 0x0000ffff));
				t.BR_X_data_5 = ((((LFSR_S[Sliding_window_index + 11] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index)] & 0x7fff8000) >> 15)));
				t.BR_X_data_6 = ((((LFSR_S[Sliding_window_index + 7] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index)] & 0x7fff8000) >> 15)));
				t.BR_X_data_7 = ((((LFSR_S[Sliding_window_index + 2] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index)] & 0x7fff8000) >> 15)));
			}
			else {
				t.BR_X_data_4 = ((((LFSR_S[(Sliding_window_index + 15) & windows_size_mask] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index) & windows_size_mask] & 0x0000ffff)));
				t.BR_X_data_5 = ((((LFSR_S[(Sliding_window_index + 11) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
				t.BR_X_data_6 = ((((LFSR_S[(Sliding_window_index + 7) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
				t.BR_X_data_7 = ((((LFSR_S[(Sliding_window_index + 2) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
			}



			Sliding_window_index = (Sliding_window_index + 1) & windows_size_mask;
			if (!(unsigned int)LFSR_S[(15 + Sliding_window_index) & windows_size_mask])
			{
				LFSR_S[(15 + Sliding_window_index) & windows_size_mask] = 0x7fffffff;
			}

			if (Sliding_window_index < BR_barrier) {
				t.BR_X_data_8 = (((LFSR_S[Sliding_window_index + 15] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index)] & 0x0000ffff));
				t.BR_X_data_9 = ((((LFSR_S[Sliding_window_index + 11] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index)] & 0x7fff8000) >> 15)));
				t.BR_X_data_10 = ((((LFSR_S[Sliding_window_index + 7] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index)] & 0x7fff8000) >> 15)));
				t.BR_X_data_11 = ((((LFSR_S[Sliding_window_index + 2] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index)] & 0x7fff8000) >> 15)));
			}
			else {
				t.BR_X_data_8 = ((((LFSR_S[(Sliding_window_index + 15) & windows_size_mask] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index) & windows_size_mask] & 0x0000ffff)));
				t.BR_X_data_9 = ((((LFSR_S[(Sliding_window_index + 11) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
				t.BR_X_data_10 = ((((LFSR_S[(Sliding_window_index + 7) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
				t.BR_X_data_11 = ((((LFSR_S[(Sliding_window_index + 2) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
			}


			Sliding_window_index = (Sliding_window_index + 1) & windows_size_mask;
			if (!(unsigned int)LFSR_S[(15 + Sliding_window_index) & windows_size_mask])
			{
				LFSR_S[(15 + Sliding_window_index) & windows_size_mask] = 0x7fffffff;
			}


			if (Sliding_window_index < BR_barrier) {
				t.BR_X_data_12 = (((LFSR_S[Sliding_window_index + 15] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index)] & 0x0000ffff));
				t.BR_X_data_13 = ((((LFSR_S[Sliding_window_index + 11] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index)] & 0x7fff8000) >> 15)));
				t.BR_X_data_14 = ((((LFSR_S[Sliding_window_index + 7] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index)] & 0x7fff8000) >> 15)));
				t.BR_X_data_15 = ((((LFSR_S[Sliding_window_index + 2] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index)] & 0x7fff8000) >> 15)));
			}
			else {
				t.BR_X_data_12 = ((((LFSR_S[(Sliding_window_index + 15) & windows_size_mask] & 0x7fff8000) << 1) | (LFSR_S[(14 + Sliding_window_index) & windows_size_mask] & 0x0000ffff)));
				t.BR_X_data_13 = ((((LFSR_S[(Sliding_window_index + 11) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(9 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
				t.BR_X_data_14 = ((((LFSR_S[(Sliding_window_index + 7) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(5 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
				t.BR_X_data_15 = ((((LFSR_S[(Sliding_window_index + 2) & windows_size_mask] & 0x0000ffff) << 16) | ((LFSR_S[(0 + Sliding_window_index) & windows_size_mask] & 0x7fff8000) >> 15)));
			}


			while (!queue->push(t));

		}
	}
/************************************************************
Function:	consumer
Description:  Consumer consumes the BR read from the buffer, generating the key stream
Calls:	L1,L2
Called By:	ZUC_Work
Input:	queue:Circular queue buffer
		item_total:The number of groups that produce BR
		F_R[]:The R register values of nonlinear function F After the initialization 
Output:	pKeyStream[]:The generated key stream
Return:	Null
Others:
************************************************************/
	void consumer(int  item_total, unsigned int pKeyStream[], unsigned int F_R[], LockFreeQueue<BR_X>* queue) {

		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		SetThreadAffinityMask(GetCurrentThread(), 0x00000030);
		for (uint32_t i = 0; i < item_total; i += 4) {
			BR_X t;
			unsigned int W, W1, W2;

			while (!queue->pop(t))
				;//自旋
			W = (t.BR_X_data_0 ^ F_R[0]) + F_R[1];

			W1 = F_R[0] + t.BR_X_data_1;

			W2 = F_R[1] ^ t.BR_X_data_2;

			F_R[0] = L1((W1 << 16) | (W2 >> 16));
			F_R[0] = (ZUC_S0[(F_R[0] >> 24) & 0xFF])
				| (ZUC_S1[(F_R[0] >> 16) & 0xFF])
				| (ZUC_S2[(F_R[0] >> 8) & 0xFF])
				| (ZUC_S3[F_R[0] & 0xFF]);
			F_R[1] = L2((W2 << 16) | (W1 >> 16));
			F_R[1] = (ZUC_S0[(F_R[1] >> 24) & 0xFF])
				| (ZUC_S1[(F_R[1] >> 16) & 0xFF])
				| (ZUC_S2[(F_R[1] >> 8) & 0xFF])
				| (ZUC_S3[F_R[1] & 0xFF]);

			pKeyStream[i] = W ^ t.BR_X_data_3;

			W = (t.BR_X_data_4 ^ F_R[0]) + F_R[1];

			W1 = F_R[0] + t.BR_X_data_5;

			W2 = F_R[1] ^ t.BR_X_data_6;

			F_R[0] = L1((W1 << 16) | (W2 >> 16));
			F_R[0] = (ZUC_S0[(F_R[0] >> 24) & 0xFF])
				| (ZUC_S1[(F_R[0] >> 16) & 0xFF])
				| (ZUC_S2[(F_R[0] >> 8) & 0xFF])
				| (ZUC_S3[F_R[0] & 0xFF]);
			F_R[1] = L2((W2 << 16) | (W1 >> 16));
			F_R[1] = (ZUC_S0[(F_R[1] >> 24) & 0xFF])
				| (ZUC_S1[(F_R[1] >> 16) & 0xFF])
				| (ZUC_S2[(F_R[1] >> 8) & 0xFF])
				| (ZUC_S3[F_R[1] & 0xFF]);

			pKeyStream[i + 1] = W ^ t.BR_X_data_7;

			W = (t.BR_X_data_8 ^ F_R[0]) + F_R[1];

			W1 = F_R[0] + t.BR_X_data_9;

			W2 = F_R[1] ^ t.BR_X_data_10;

			F_R[0] = L1((W1 << 16) | (W2 >> 16));
			F_R[0] = (ZUC_S0[(F_R[0] >> 24) & 0xFF])
				| (ZUC_S1[(F_R[0] >> 16) & 0xFF])
				| (ZUC_S2[(F_R[0] >> 8) & 0xFF])
				| (ZUC_S3[F_R[0] & 0xFF]);
			F_R[1] = L2((W2 << 16) | (W1 >> 16));
			F_R[1] = (ZUC_S0[(F_R[1] >> 24) & 0xFF])
				| (ZUC_S1[(F_R[1] >> 16) & 0xFF])
				| (ZUC_S2[(F_R[1] >> 8) & 0xFF])
				| (ZUC_S3[F_R[1] & 0xFF]);

			pKeyStream[i + 2] = W ^ t.BR_X_data_11;



			W = (t.BR_X_data_12 ^ F_R[0]) + F_R[1];

			W1 = F_R[0] + t.BR_X_data_13;

			W2 = F_R[1] ^ t.BR_X_data_14;

			F_R[0] = L1((W1 << 16) | (W2 >> 16));
			F_R[0] = (ZUC_S0[(F_R[0] >> 24) & 0xFF])
				| (ZUC_S1[(F_R[0] >> 16) & 0xFF])
				| (ZUC_S2[(F_R[0] >> 8) & 0xFF])
				| (ZUC_S3[F_R[0] & 0xFF]);
			F_R[1] = L2((W2 << 16) | (W1 >> 16));
			F_R[1] = (ZUC_S0[(F_R[1] >> 24) & 0xFF])
				| (ZUC_S1[(F_R[1] >> 16) & 0xFF])
				| (ZUC_S2[(F_R[1] >> 8) & 0xFF])
				| (ZUC_S3[F_R[1] & 0xFF]);

			pKeyStream[i + 3] = W ^ t.BR_X_data_15;

		}

	}
/************************************************************
Function:	ZUC_work
Description:  working stage of ZUC
Calls:BR_use_by_work_mode,F_use_by_work_mode,prodece,consume,LockFreeQueue
Called By:	ZUC_GenKeyStream
Input:	LFSR_S[]:the state of LFSR after initialisation:s0,s1,s2,..s15
		BR_X[] : X0,X1,X2,X3
		F_R[]:R1,R2
		Sliding_window_index:The index of sliding window 
		KeyStreamLen:the length of KeyStream,exporting 32bit for a beat and it is a multiple of four
Output:	pKeyStream[]:key stream
		
Return:	Null
Others:
************************************************************/
	void ZUC_Work(unsigned long long  LFSR_S[], unsigned int BR_X[], unsigned int F_R[], unsigned int pKeyStream[], int KeyStreamLen, int& Sliding_window_index)
	{

		int i = 0;
		BR_use_by_work_mode(LFSR_S, BR_X, Sliding_window_index);
		F_use_by_work_mode(BR_X, F_R);
		LockFreeQueue<struct BR_X>* queue = new  LockFreeQueue<struct BR_X>(1024 * 100);
		std::thread prodece(producer, LFSR_S, Sliding_window_index, KeyStreamLen, queue);
		std::thread consume(consumer, KeyStreamLen, pKeyStream, F_R, queue);
		prodece.join();
		consume.join();
		delete queue;
	}
/****************************************************************
Function:	ZUC_GenKeyStream 
Description:generate key stream
Calls:	ZUC_Init,ZUC_Work,Init_S_box_reset,Init_S_box
Called By:ZUC_Confidentiality,ZUC_GenKeyStream_speed
Input:	k[]	:initial key,128bit 
        iv[]:initial iv,128bit
		KeyStreamLen:the byte length of KeyStream,exporting 32bit for a beat and it is a multiple of four
Output:	KeyStream[]:key strem to be outputed
Return:	null
Others:
****************************************************************/
	void ZUC_GenKeyStream(unsigned char k[], unsigned char iv[], unsigned int KeyStream[], int KeyStreamLen)
	{
		unsigned int LFSR_S[16]; //LFSR state s0,s1,s2,...s15
		unsigned long long LFSR_S_64[windows_size];
		int Sliding_window_index = 0;
		unsigned int BR_X[4];	//Bit Reconstruction X0,X1,X2,X3
		unsigned int F_R[2];	//R1,R2,variables of nonlinear function F
		//Initialisation
		ZUC_Init(k, iv, LFSR_S, BR_X, F_R);

		for (int i = 0; i < 16; i++)
		{
			LFSR_S_64[i] = LFSR_S[i];
		}
		Init_S_box();
		//Working
		ZUC_Work(LFSR_S_64, BR_X, F_R, KeyStream, KeyStreamLen, Sliding_window_index);
		Init_S_box_reset();
	}
/****************************************************************
Function:	ZUC_Confidentiality
Description:	the ZUC-based condifentiality algorithm
Calls:	ZUC_GenKeyStream
Called By:	
Input:	CK[]:initial key,128bit,uesed to gain the key of ZUC KeyStream generation algorithm
		COUNT:128bit
		BEARER:5bit,bearing layer identification, 
		DIRECTION:1bit
		IBS[]:input bit stream, 
		LENGTH:the bit length of IBS
Output:	OBS[]:output bit stream 
Return:null
Others:
****************************************************************/
	void ZUC_Confidentiality(unsigned char CK[], unsigned int COUNT, unsigned char BEARER, unsigned char DIRECTION, unsigned int IBS[], unsigned int LENGTH, unsigned int OBS[])
	{
		unsigned int* k;
		int L, i, t;
		unsigned char iv[16];
		//generate vector iv1,iv2,...iv15
		iv[0] = (unsigned char)(COUNT >> 24);
		iv[1] = (unsigned char)((COUNT >> 16) & 0xff);
		iv[2] = (unsigned char)((COUNT >> 8) & 0xff);
		iv[3] = (unsigned char)(COUNT & 0xff);
		iv[4] = (((BEARER << 3) | (DIRECTION << 2)) & 0xfc);
		iv[5] = 0x00;
		iv[6] = 0x00;
		iv[7] = 0x00;
		iv[8] = iv[0];
		iv[9] = iv[1];
		iv[10] = iv[2];
		iv[11] = iv[3];
		iv[12] = iv[4];
		iv[13] = iv[5];
		iv[14] = iv[6];
		iv[15] = iv[7];
		//L,the length of key stream,taking 32bit as a unit
		L = (LENGTH + 31) / 32;
		int L1 = ((((LENGTH + 31) / 32) + 3) / 4) * 4;
		k = (unsigned int*)malloc(sizeof(unsigned int) * L1);
		//generate key stream k
		ZUC_GenKeyStream(CK, iv, k, L1);  //generate key stream
		//OBS=IBS^k
		for (i = 0; i < L; i++)
		{
			OBS[i] = IBS[i] ^ k[i];
		}
		t = LENGTH % 32;
		if (t)
		{
			OBS[L - 1] = ((OBS[L - 1] >> (32 - t)) << (32 - t));
		}
		free(k);

	}
/****************************************************************
Function:	ZUC_GenKeyStream_speed
Description:	Test the key stream generation speed of ZUC
Calls:	ZUC_GenKeyStream
Called By:
Input:	LENGTH:the bit length to test
Output:null
Return:The time it takes to generate  key stream of LENGTH bit,In milliseconds
Others:null
****************************************************************/
	double ZUC_GenKeyStream_speed(int LENGTH)
	{
		unsigned char k[16] = { 0x3d,0x4c,0x4b,0xe9,0x6a,0x82,0xfd,0xae,0xb5,0x8f,0x64,0x1d,0xb1,0x7b,0x45,0x5b };
		unsigned char iv[16] = { 0x84,0x31,0x9a,0xa8,0xde,0x69,0x15,0xca,0x1f,0x6b,0xda,0x6b,0xfb,0xd8,0xc7,0x66 };
		int L = (LENGTH + 31) / 32;
		int L1 = ((((LENGTH + 31) / 32) + 3) / 4) * 4;
		unsigned int*  keystream = (unsigned int*)malloc(sizeof(unsigned int) * L1);
		
		std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();//开始时间
		ZUC_GenKeyStream(k, iv, keystream, L1);
		std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();//结束时间
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
		return time_span.count() * 1000;
	}
/****************************************************************
Function:	ZUC_Confidentiality_speed
Description:	Test the  encrypting data speed of ZUC
Calls:	ZUC_Confidentiality
Called By:
Input:	LENGTH:the bit length to test
Output:null
Return:The time it takes to  encrypt data  of LENGTH bit,In milliseconds
Others:null
****************************************************************/
	double ZUC_Confidentiality_speed(int LENGTH)
	{

	 int L = (LENGTH + 31) / 32;
	int L1 = ((((LENGTH + 31) / 32) + 3) / 4) * 4; 
	unsigned char key[16] = { 0xe1, 0x3f, 0xed, 0x21, 0xb4, 0x6e, 0x4e, 0x7e, 0xc3, 0x12, 0x53, 0xb2, 0xbb, 0x17, 0xb3, 0xe0 };
	unsigned int COUNT = 0x2738cdaa;
	unsigned char BEARER = 0x1a;
	unsigned char DIRECTION = 0x00;
	unsigned int* plain = new unsigned int[L1];
	for (int i = 0; i < L1; i++) {
		plain[i] = i;
	}
	unsigned int* cipher = new unsigned int[L1];
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();//开始时间
	ZUC_Confidentiality(key, COUNT, BEARER, DIRECTION, plain, LENGTH, cipher);
	std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();//结束时间
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
	return time_span.count() * 1000;
	}
	/****************************************************************
	Function:	 en_decryption
	Description:Use zuc to encrypt or decrypt  a file
	Calls:	ZUC_Confidentiality
	Called By:
	Input:	key:initial key,128bit,uesed to gain the key of ZUC KeyStream generation algorithm
			COUNT:128bit
			BEARER:5bit,bearing layer identification,
			DIRECTION:1bit
			infile:A file that needs to be encrypted or decrypted
	Output:	outfile:The encrypted or decrypted target file
	Return:A sign of successful encryption or decryption,0  representatives success,Other representatives succeed
	Others:
	****************************************************************/
	int en_decryption(std::wstring infile, std::wstring outfile, unsigned char* key, unsigned int COUNT, unsigned char BEARER, unsigned char DIRECTION )
	{

		std::wcout << L"输入文件名: " << infile << L"   " <<L"输出文件名: " << outfile << std::endl;
		std::fstream fio(infile.c_str(), std::ios::in | std::ios::out| std::ios::binary);

		if (!fio)
		{
			std::cerr << "文件打开失败" << std::endl;
			return -1;
		}

		std::ofstream fout(outfile.c_str(), std::ios::trunc| std::ios::binary);
		if (!fout)
		{
			std::cerr << "文件打开失败" << std::endl;
			return -1;
		}
		int size = 0;
		
		HANDLE handle = CreateFileW(infile.data(), FILE_READ_EA,
			 FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
		 if (handle != INVALID_HANDLE_VALUE)
			{
			      size = GetFileSize(handle, NULL);
			     std::cout << "CreateFile:  " << size << std::endl;
			     CloseHandle(handle);
			 }
		 int size1 = (size + 3) / 4;

		char* plain=new char[size1*4];
		char* cipher = new char[size1*4];
			fio.read(plain, size);		
		    ZUC_LEARN::ZUC_Confidentiality(key, COUNT, BEARER, DIRECTION, (unsigned int*)plain, size*8, (unsigned int*)cipher);
			fout.write(cipher, size);
		fout.close();
		fio.close();
		std::cout << "加密完成" << std::endl;
		return 0;
	}

}
