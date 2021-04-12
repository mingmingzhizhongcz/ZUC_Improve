/************************************************************************
File name:	test_improvezuc.cpp
Date:	Oct 19,2019
Description: zuc Algorithm speed test file
Other:
**************************************************************************/
#include"../include/zuclib.h"
#include<iostream>
int main(){
    std::cout<<ZUC_LEARN::ZUC_GenKeyStream_speed(500000000)<<std::endl;
    std::cout<<ZUC_LEARN::ZUC_GenKeyStream_speed(1000000000)<<std::endl;
    std::cout<<ZUC_LEARN::ZUC_GenKeyStream_speed(1500000000)<<std::endl;
    std::cout<<ZUC_LEARN::ZUC_GenKeyStream_speed(2000000000)<<std::endl;

    std::cout<<ZUC_LEARN::ZUC_Confidentiality_speed(500000000)<<std::endl;
    std::cout<<ZUC_LEARN::ZUC_Confidentiality_speed(1000000000)<<std::endl;
    std::cout<<ZUC_LEARN::ZUC_Confidentiality_speed(1500000000)<<std::endl;
    std::cout<<ZUC_LEARN::ZUC_Confidentiality_speed(2000000000)<<std::endl;
    return 0;
}