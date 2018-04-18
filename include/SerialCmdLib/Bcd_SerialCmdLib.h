/***************************************************************************************************
*
* 版权信息：版权所有 (c) 2016, ***, 保留所有权利
*
* 文件名称：Bcd_SerialCmdLib.h
* 摘    要: 串口控制库头文件
*
* 当前版本：1.0.0.0
* 作    者：***
* 日    期：2018-01-25
* 备    注：新建
* 说    明：串口控制动态库主要完成以下功能：1，读取串口信息；2，写入串口信息；3，读取串口信息时的回调处理。
****************************************************************************************************/
#ifndef _BCD_SERIAL_CMD_LIB_H_
#define _BCD_SERIAL_CMD_LIB_H_

#include "Bcd_SerialCmdDefine.h"

#ifndef BCD_SERIALDLL_API
    #if defined(BCD_SERIALDLL_EXPORTS)
        #define BCD_SERIALDLL_API __declspec(dllexport)
    #else
        #define BCD_SERIALDLL_API __declspec(dllimport)
    #endif
#endif


#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

#ifdef __cplusplus
extern "C" {
#endif 

// 获取动态库版本信息
BCD_SERIALDLL_API unsigned int __stdcall BCD_SRL_GetLibVersion();

// 创建动态库句柄，并根据设定信息打开串口
BCD_SERIALDLL_API int __stdcall BCD_SRL_CreateHandle(OUT void ** handle, IN const BCD_SERIAL_INFO * pstSerialInfo);

// 向串口写入信息
BCD_SERIALDLL_API int __stdcall BCD_SRL_Write(IN void * handle, IN const char * strInfo);

// 从串口中读取信息
BCD_SERIALDLL_API int __stdcall BCD_SRL_Read(IN void * handle, OUT char * strInfo, IN const unsigned int nBufLen, OUT unsigned int & nRecvLen);

// 注册读取串口后信息的回调函数
BCD_SERIALDLL_API int __stdcall BCD_SRL_RigisterReadCallBack(IN void * handle, IN SRL_ReadCallBackFunc pFunc, IN void * pUser);

// 销毁串口句柄，释放相应资源
BCD_SERIALDLL_API int __stdcall BCD_SRL_DestoryHandle(IN void * handle);

#ifdef __cplusplus
}
#endif 


#endif