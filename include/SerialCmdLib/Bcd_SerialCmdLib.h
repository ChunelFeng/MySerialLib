/***************************************************************************************************
*
* 版权信息：版权所有 (c) 2016, ***, 保留所有权利
*
* 文件名称：Bcd_SerialCmdLib.h
* 摘    要: 串口控制库头文件
*
* 当前版本：1.1.0
* 作    者：Chunel
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

/************************************************************************
*  @fn     BCD_SRL_GetLibVersion()
*  @brief  获取版本信息
*  @param  无
*  @return 返回串口库版本信息
************************************************************************/
BCD_SERIALDLL_API unsigned int __stdcall BCD_SRL_GetLibVersion();

/************************************************************************
*  @fn     BCD_SRL_CreateHandle()
*  @brief  创建串口对应句柄
*  @param  handle                    [OUT]         待创建的串口句柄
*  @param  pstSerialInfo             [IN]          待创建的串口信息
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_CreateHandle(OUT void ** handle,
                                                     IN const BCD_SERIAL_INFO * pstSerialInfo);

/************************************************************************
*  @fn     BCD_SRL_Write
*  @brief  向串口写入信息
*  @param  handle                    [IN]          对应的句柄
*  @param  strInfo                   [IN]          写入串口的信息
*  @param  nBufLen                   [IN]          写入信息的长度
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_Write(IN void * handle,
                                              IN const char * strInfo,
                                              const unsigned int nBufLen);

/************************************************************************
*  @fn     BCD_SRL_Read
*  @brief  从串口中读取信息
*  @param  handle                    [IN]          对应的句柄
*  @param  strInfo                   [OUT]         读取串口数据
*  @param  nBufLen                   [IN]          输入Buf对应的长度
*  @param  nRecvLen                  [OUT]         收到的数据长度
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_Read(IN void * handle,
                                             OUT char * strInfo,
                                             IN const unsigned int nBufLen,
                                             OUT unsigned int & nRecvLen);

/************************************************************************
*  @fn     BCD_SRL_ReadBlock
*  @brief  从串口中读取确定长度的信息
*  @param  handle                    [IN]          对应的句柄
*  @param  nBlockLen                 [IN]          设定的报文块大小
*  @param  strInfo                   [OUT]         读取串口数据
*  @param  nBufLen                   [IN]          收到的数据长度
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_ReadBlock(IN void * handle,
                                                  IN const unsigned int nBlockLen,
                                                  OUT char * strInfo,
                                                  IN const unsigned int nBufLen);

/************************************************************************
*  @fn     BCD_SRL_RigisterReadCallBack
*  @brief  注册读取串口后信息的回调函数
*  @param  handle                    [IN]          对应的句柄
*  @param  pFunc                     [IN]          注册读取回调函数（可为NULL）
*  @param  pUser                     [IN]          注册读取回调函数对应的类（可为NULL）
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_RigisterReadCallBack(IN void * handle,
                                                             IN SRL_ReadCallBackFunc pFunc,
                                                             IN void * pUser);

/************************************************************************
*  @fn     BCD_SRL_SetRecvTimeSpan
*  @brief  设置连续读取模式下，串口读取信息时间间隔，单位为ms
*  @param  handle                    [IN]          对应的句柄
*  @param  nRecvTimeSpan             [IN]          相邻两次读取时间间隔
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_SetRecvTimeSpan(IN void * handle,
                                                        IN const unsigned int nRecvTimeSpan);

/************************************************************************
*  @fn     BCD_SRL_StartReading
*  @brief  开启连续读取模式
*  @param  handle                    [IN]          对应的句柄
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_StartReading(IN void * handle);

/************************************************************************
*  @fn     BCD_SRL_StartReading
*  @brief  停止连续读取模式
*  @param  handle                    [IN]          对应的句柄
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_StopReading(IN void * handle);

/************************************************************************
*  @fn     BCD_SRL_GetSerialInfo
*  @brief  获取串口设置信息
*  @param  handle                    [IN]          对应的句柄
*  @param  pstSerialInfo             [OUT]         获取对应的串口信息
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_GetSerialInfo(IN void * handle,
                                                      OUT BCD_SERIAL_INFO * pstSerialInfo);

/************************************************************************
*  @fn     BCD_SRL_DestoryHandle
*  @brief  销毁串口句柄，释放相应资源
*  @param  handle                    [IN]          对应的句柄
*  @return 成功，返回SRL_RET_OK；失败，见错误返回值
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_DestoryHandle(IN void * handle);

#ifdef __cplusplus
}
#endif 


#endif