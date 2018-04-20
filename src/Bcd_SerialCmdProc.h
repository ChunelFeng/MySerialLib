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
    explicit SerialCmdProc();
    virtual ~SerialCmdProc();

	/* �����ṩ�Ľӿں������ڲ�ʵ�� */
    int SrlInit(const BCD_SERIAL_INFO * pstSeriallInfo);
    int SrlDeinit();

    int SrlSendInfo(const char * strInfo);
    int SrlRecvInfo(char * strInfo, const unsigned int nBufLen, unsigned int & nRecvLen);
    int SrlSetReadCallBackFunc(SRL_ReadCallBackFunc pFunc, void * pUser);
	int SrlStartRecving(const unsigned int nRecvTimeSpan);
	int SrlStopRecving();
	int SrlSetRecvTimeSpan(const unsigned int nRecvTimeSpan);

	/* �̴߳����� */
	void StartRecvingProc();

protected:
    int  OpenSerial(const BCD_SERIAL_INFO * pstSeriallInfo);
    int  CloseSerial();
	void CloseCtnRecvingThread();

private:
    CSerial                   * m_pcSerial;
    SRL_ReadCallBackFunc      m_pSrlReadCbFunc;
    void                      * m_pSrlCbUser;

	BOOL                      m_bIsCtnReading;    // �ж��Ƿ���������ȡ״̬
	void                      * m_hCtnReadHandle;    // ������ȡ�߳�
	BOOL                      m_bIsThreadExit;
	unsigned int              m_nRecvTimeSpan;    // �������δ�����Ϣ����ʱ��
};

#endif