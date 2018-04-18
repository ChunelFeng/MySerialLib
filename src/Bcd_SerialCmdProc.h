/**
*
* ��Ȩ��Ϣ����Ȩ���� (c) 2016, ***, ��������Ȩ��
*
* �ļ����ƣ�Bcd_SerialCmdProc.h
* ժ    Ҫ�����ڿ��ƿ�ʵ�ֲ��ֶ�Ӧͷ�ļ�
*
* ��ǰ�汾��1.0.0.0
* ��    ��: ***
* ��    �ڣ�2018-01-25
* ��    ע���½�

* ���������������޳�û������������
* ��������������������
* ���������ߩ��������ߩ�
* ����������������������
* ����������������������
* ���������ש������ס���
* ����������������������
* �������������ߡ�������
* ����������������������
* ����������������������Code is far away from bug with the animal protecting
* ������������������
* ������������������
* ��������������������������
* �������������������������ǩ�
* ����������������������������
* �������������������ש�����
* �������������ϩϡ����ϩ�
* �������������ߩ������ߩ�
*
* �������������о������թ�����������
*/

#ifndef _BCD_SERIAL_CMD_PROC_H_
#define _BCD_SERIAL_CMD_PROC_H_

#include <Windows.h>
#include "Bcd_SerialCmdDefine.h"
#include "Serial.h"

class SerialCmdProc
{
public:
    SerialCmdProc();
    virtual ~SerialCmdProc();

    int SrlInit(const BCD_SERIAL_INFO * pstSeriallInfo);
    int SrlDeinit();
    int SrlSendInfo(const char * strInfo);
    int SrlRecvInfo(char * strInfo, const unsigned int nBufLen, unsigned int & nRecvLen);
    int SrlSetReadCallBackFunc(SRL_ReadCallBackFunc pFunc, void * pUser);

protected:
    int OpenSerial(const BCD_SERIAL_INFO * pstSeriallInfo);
    int CloseSerial();

private:
    CSerial                   * m_pcSerial;
    SRL_ReadCallBackFunc      m_pSrlReadCbFunc;
    void                      * m_pSrlCbUser;
};

#endif