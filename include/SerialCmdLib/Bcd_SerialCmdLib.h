/***************************************************************************************************
*
* ��Ȩ��Ϣ����Ȩ���� (c) 2016, ***, ��������Ȩ��
*
* �ļ����ƣ�Bcd_SerialCmdLib.h
* ժ    Ҫ: ���ڿ��ƿ�ͷ�ļ�
*
* ��ǰ�汾��1.0.0.0
* ��    �ߣ�***
* ��    �ڣ�2018-01-25
* ��    ע���½�
* ˵    �������ڿ��ƶ�̬����Ҫ������¹��ܣ�1����ȡ������Ϣ��2��д�봮����Ϣ��3����ȡ������Ϣʱ�Ļص�����
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

// ��ȡ��̬��汾��Ϣ
BCD_SERIALDLL_API unsigned int __stdcall BCD_SRL_GetLibVersion();

// ������̬�������������趨��Ϣ�򿪴���
BCD_SERIALDLL_API int __stdcall BCD_SRL_CreateHandle(OUT void ** handle, IN const BCD_SERIAL_INFO * pstSerialInfo);

// �򴮿�д����Ϣ
BCD_SERIALDLL_API int __stdcall BCD_SRL_Write(IN void * handle, IN const char * strInfo);

// �Ӵ����ж�ȡ��Ϣ
BCD_SERIALDLL_API int __stdcall BCD_SRL_Read(IN void * handle, OUT char * strInfo, IN const unsigned int nBufLen, OUT unsigned int & nRecvLen);

// ע���ȡ���ں���Ϣ�Ļص�����
BCD_SERIALDLL_API int __stdcall BCD_SRL_RigisterReadCallBack(IN void * handle, IN SRL_ReadCallBackFunc pFunc, IN void * pUser);

// ���ٴ��ھ�����ͷ���Ӧ��Դ
BCD_SERIALDLL_API int __stdcall BCD_SRL_DestoryHandle(IN void * handle);

#ifdef __cplusplus
}
#endif 


#endif