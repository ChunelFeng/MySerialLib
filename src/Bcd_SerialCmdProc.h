/**
*
* ��Ȩ��Ϣ����Ȩ���� (c) 2016, ***, ��������Ȩ��
*
* �ļ����ƣ�Bcd_SerialCmdProc.h
* ժ    Ҫ�����ڿ��ƿ�ʵ�ֲ��ֶ�Ӧͷ�ļ�
*
* ��ǰ�汾��1.1.0
* ��    ��: Chunel
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

const unsigned int DEFAULT_RECV_SPAN = 32;      // ��������ģʽ�£�Ĭ�ϵĴ��ڽ��ռ��ʱ��
const unsigned int MAX_RECV_LEN      = 1024;    // ����һ�ν������ݵ����ֵ

class SerialCmdProc
{
public:
    explicit SerialCmdProc();
    virtual ~SerialCmdProc();

    /* �����ṩ�Ľӿں������ڲ�ʵ�� */
    int SrlInit(const BCD_SERIAL_INFO * pstSerialInfo);
    int SrlDeinit();

    int SrlSendInfo(const char * strInfo, const unsigned int nBufLen);
    int SrlRecvInfo(char * strInfo, const unsigned int nBufLen, unsigned int & nRecvLen);
    int SrlRecvBlockInfo(const unsigned int nBlockLen, char * strInfo, unsigned int nBufLen);
    int SrlSetReadCallBackFunc(SRL_ReadCallBackFunc pFunc, void * pUser);
    int SrlStartRecving();
    int SrlStopRecving();
    int SrlSetRecvTimeSpan(const unsigned int nRecvTimeSpan);
    int SrlGetSerialInfo(BCD_SERIAL_INFO * pstSerialInfo);

    /* �̴߳����� */
    void StartRecvingProc();

protected:
    int  OpenSerial(const BCD_SERIAL_INFO * pstSerialInfo);
    int  CloseSerial();
    void CloseCtnRecvingThread();

private:
    CSerial                   * m_pcSerial;
    SRL_ReadCallBackFunc      m_pSrlReadCbFunc;
    void                      * m_pSrlCbUser;
    BCD_SERIAL_INFO           m_stSerialInfo;

    BOOL                      m_bIsCtnReading;       // �ж��Ƿ���������ȡ״̬
    void                      * m_hCtnReadHandle;    // ������ȡ�߳�
    BOOL                      m_bIsExit;             // �����������̵ı�־λ
    unsigned int              m_nRecvTimeSpan;       // �������δ�����Ϣ����ʱ��
};

#endif