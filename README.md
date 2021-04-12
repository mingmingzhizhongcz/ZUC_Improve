# ZUC_Improve
    这个课题对ZUC(祖冲之算法)的优化主要分三个层次，第一个层次是用循环展开、小函数内联、减少内存引用等通用优化方法以及取模延迟、滑动窗口和改进S盒的针对性优化方法，对串行代码进行了基本的优化。第二个层次是利用AVX、AVX2等SIMD指令进行向量化优化，充分发挥单处理器的效能。第三个层次是进行多线程优化，利用生产者-消费者模型，克服了并行编程中任务分配与执行过程高度耦合的问题。为了克服传统线程同步方法的低效，利用了原子操作实现了生产者线程与消费者线程对FIFO队列缓冲区操作的同步，而且结合多核处理器的情况，引入CacheLinePad较好地解决了Cache伪共享对性能的影响。
    经优化后程序加速比可达到3.3,  最终优化后的ZUC算法实现在单组密钥与初始向量的要求下在Intel @Core(TM)i7-9700K CPU @ 3.60GHz 3.60GHz处理器的情况下，产生1密钥流的平均吞吐率可达到3.85Gbps,峰值吞吐率可达到4.46Gbps.
# 一、文件说明
1、bin目录的zuc_improve.dll是ZUC算法实现的动态链接库文件；
zuc_test.exe,zuc_test_speed.exe是测试时用的可执行文件；
zuc_test.exe测试标准文档中的六组数据，用来检验正确性；
zuc_test_speed.exe结合Python脚本测试算法性能，输出8个值，分别表示产生密钥流0.5Gbit,1Gbit,1.5Gbit,2Gbit以及加密明文流0.5Gbit,1Gbit,1.5Gbit,2Gbit的运行时间，单位为ms。
2、lib目录中的zuc_improve.lib是zuc_improve.dll的动态导入库文件。
3、include目录中的zuclib.h是zuc_improve.dll对应的包含目录。
4、src目录是全部的源文件。
# 二、命令行使用方式
本程序在Windows 10 1903版本操作系统下用用于 x64 的 Microsoft (R) C/C++ 优化编译器 19.22.27905 版编译通过，编译选项为/O2  /std:c++17。
如已将msvc工具集(cl.exe所在目录)和msvc工具集及Windows 10 SDK的库目录和包含目录加入环境变量，就可使用msvc去编译。
例如编译zuc_test.cpp 应在src目录下打开PowerShell,用命令行
cl zuc_test.cpp /O2  /std:c++17  /Fe"../bin/zuc_test.exe" /link"../lib/zuc_improve.lib" 编译，如下图：
 
# 三、Visual Studio 2019使用方式
1、选择解决方案平台为x64，打开“调试”中的项目属性表。选择“C/C++”的“常规”，在附加包含目录中添加“include”目录路径；

2、选择“链接器”中的“常规”，在“附加库目录”中添加“lib”目录路径；
 
3、选择“链接器”中的“输入”，在“附加依赖项”中添加“zuc_improve.lib”
 
4、编写代码（注意包含头文件zuclib.h），生成解决方案但不运行。
 
5、在程序生成的可执行文件目录下添加“zuc_improve.dll”，完成之后即可运行。
  
# 四、程序接口
1.void ZUC_GenKeyStream(unsigned char k[], unsigned char iv[], unsigned int KeyStream[], int KeyStreamLen)
这是用于产生密钥流的函数。其中，k表示初始密钥，iv表示初始向量，KeyStream表示生成的密钥流，KeyStreamLen表示以32比特分组的密钥流长度，且需是4的倍数；
2. void ZUC_Confidentiality(unsigned char CK[], unsigned int COUNT, unsigned char BEARER, unsigned char DIRECTION, unsigned int IBS[], unsigned int LENGTH, unsigned int OBS[])
这是加密函数。其中，CK、COUNT、BEARER、DIRECTION是算法相应参数，IBS是明文比特流，LENGTH是明文比特长度，OBS是密文比特流；
3. double ZUC_GenKeyStream_speed(int LENGTH)
此函数返回产生LENGTH比特密钥流长度的时间，单位ms。为了方便，此函数指定了初始密钥和初始向量。
4. double ZUC_Confidentiality_speed(int LENGTH)
此函数返回加密LENGTH比特长度明文的时间，单位ms。为了方便，此函数明文以及相应的参数不提供额外的接口。

