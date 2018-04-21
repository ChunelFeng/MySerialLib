/***************************************************************************************************
*
* ��Ȩ��Ϣ����Ȩ���� (c) 2016, ***, ��������Ȩ��
*
* �ļ����ƣ�Bcd_SerialCmdLib.h
* ժ    Ҫ: ���ڿ��ƿ�ͷ�ļ�
*
* ��ǰ�汾��1.1.0
* ��    �ߣ�Chunel
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

/************************************************************************
*  @fn     BCD_SRL_GetLibVersion()
*  @brief  ��ȡ�汾��Ϣ
*  @param  ��
*  @return ���ش��ڿ�汾��Ϣ
************************************************************************/
BCD_SERIALDLL_API unsigned int __stdcall BCD_SRL_GetLibVersion();

/************************************************************************
*  @fn     BCD_SRL_CreateHandle()
*  @brief  �������ڶ�Ӧ���
*  @param  handle                    [OUT]         �������Ĵ��ھ��
*  @param  pstSerialInfo             [IN]          �������Ĵ�����Ϣ
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_CreateHandle(OUT void ** handle,
                                                     IN const BCD_SERIAL_INFO * pstSerialInfo);

/************************************************************************
*  @fn     BCD_SRL_Write
*  @brief  �򴮿�д����Ϣ
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @param  strInfo                   [IN]          д�봮�ڵ���Ϣ
*  @param  nBufLen                   [IN]          д����Ϣ�ĳ���
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_Write(IN void * handle,
                                              IN const char * strInfo,
                                              const unsigned int nBufLen);

/************************************************************************
*  @fn     BCD_SRL_Read
*  @brief  �Ӵ����ж�ȡ��Ϣ
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @param  strInfo                   [OUT]         ��ȡ��������
*  @param  nBufLen                   [IN]          ����Buf��Ӧ�ĳ���
*  @param  nRecvLen                  [OUT]         �յ������ݳ���
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_Read(IN void * handle,
                                             OUT char * strInfo,
                                             IN const unsigned int nBufLen,
                                             OUT unsigned int & nRecvLen);

/************************************************************************
*  @fn     BCD_SRL_ReadBlock
*  @brief  �Ӵ����ж�ȡȷ�����ȵ���Ϣ
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @param  nBlockLen                 [IN]          �趨�ı��Ŀ��С
*  @param  strInfo                   [OUT]         ��ȡ��������
*  @param  nBufLen                   [IN]          �յ������ݳ���
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_ReadBlock(IN void * handle,
                                                  IN const unsigned int nBlockLen,
                                                  OUT char * strInfo,
                                                  IN const unsigned int nBufLen);

/************************************************************************
*  @fn     BCD_SRL_RigisterReadCallBack
*  @brief  ע���ȡ���ں���Ϣ�Ļص�����
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @param  pFunc                     [IN]          ע���ȡ�ص���������ΪNULL��
*  @param  pUser                     [IN]          ע���ȡ�ص�������Ӧ���ࣨ��ΪNULL��
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_RigisterReadCallBack(IN void * handle,
                                                             IN SRL_ReadCallBackFunc pFunc,
                                                             IN void * pUser);

/************************************************************************
*  @fn     BCD_SRL_SetRecvTimeSpan
*  @brief  ����������ȡģʽ�£����ڶ�ȡ��Ϣʱ��������λΪms
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @param  nRecvTimeSpan             [IN]          �������ζ�ȡʱ����
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_SetRecvTimeSpan(IN void * handle,
                                                        IN const unsigned int nRecvTimeSpan);

/************************************************************************
*  @fn     BCD_SRL_StartReading
*  @brief  ����������ȡģʽ
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_StartReading(IN void * handle);

/************************************************************************
*  @fn     BCD_SRL_StartReading
*  @brief  ֹͣ������ȡģʽ
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_StopReading(IN void * handle);

/************************************************************************
*  @fn     BCD_SRL_GetSerialInfo
*  @brief  ��ȡ����������Ϣ
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @param  pstSerialInfo             [OUT]         ��ȡ��Ӧ�Ĵ�����Ϣ
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_GetSerialInfo(IN void * handle,
                                                      OUT BCD_SERIAL_INFO * pstSerialInfo);

/************************************************************************
*  @fn     BCD_SRL_DestoryHandle
*  @brief  ���ٴ��ھ�����ͷ���Ӧ��Դ
*  @param  handle                    [IN]          ��Ӧ�ľ��
*  @return �ɹ�������SRL_RET_OK��ʧ�ܣ������󷵻�ֵ
************************************************************************/
BCD_SERIALDLL_API int __stdcall BCD_SRL_DestoryHandle(IN void * handle);

#ifdef __cplusplus
}
#endif 


#endif