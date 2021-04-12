/************************************************************************
File name:	zuclib.h
Date:	Oct 19,2019
Description:Used to generate dynamic link libraries
            Interface declaration ofzuc_improve.dll
Function List:1.ZUC_GenKeyStream//generate key stream
              2.ZUC_Confidentiality//the ZUC-based condifentiality algorithm
              3.ZUC_GenKeyStream_speed//Test the time which is used by the key stream generating
              4.ZUC_Confidentiality_speed//Test the time which is used by enciphering data
              5.en_decryption//Encrypt or decrypt a file
Other:
**************************************************************************/
#ifndef MYLIB_H
#define MYLIB_H
#include<string>
namespace ZUC_LEARN {
__declspec(dllexport) void ZUC_GenKeyStream(unsigned char k[], unsigned char iv[], unsigned int KeyStream[], int KeyStreamLen);
__declspec(dllexport) void ZUC_Confidentiality(unsigned char CK[], unsigned int COUNT, unsigned char BEARER, unsigned char DIRECTION, unsigned int IBS[], unsigned int LENGTH, unsigned int OBS[]);
__declspec(dllexport)  double ZUC_GenKeyStream_speed(int LENGTH);
__declspec(dllexport) double ZUC_Confidentiality_speed(int LENGTH);
__declspec(dllexport)  int en_decryption(std::wstring infile, std::wstring outfile, unsigned char* key, unsigned int COUNT, unsigned char BEARER, unsigned char DIRECTION);
}
#endif
