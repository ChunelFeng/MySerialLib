/**************************************************************
*
* ��Ȩ��Ϣ����Ȩ���� (c) 2016, ***, ��������Ȩ��
*
* �ļ����ƣ�Bcd_SerialCmdProc.cpp
* ժ    Ҫ�����ڿ��ƿ�ʵ�ֲ���
*
* ��ǰ�汾��1.0.0.0
* ��    ��: ***
* ��    �ڣ�2018-01-25
* ��    ע���½�
*������������������������������+ +
*�������������������ߩ��������ߩ� + +
*��������������������������������
*�������������������������������� ++ + + +
*������������ ������������������ ��+
*�������������������������������� +
*�����������������������ߡ�������
*�������������������������������� + +
*��������������������������������
*����������������������������
*���������������������������� + + + +
*������������������������������������Code is far away from bug with the animal protecting
*���������������������������� + �����������ޱ���,������bug
*����������������������������
*��������������������������������+
*���������������������� �� ���������� + +
*�������������������� ���������������ǩ�
*�������������������� ������������������
*�����������������������������ש����� + + + +
*�����������������������ϩϡ����ϩ�
*�����������������������ߩ������ߩ�+ + + +
*
* �������������о������թ�����������
**************************************************************/

#include <iostream>
#include "Bcd_SerialCmdProc.h"

using namespace std;

inline static void MyDbgView(const char * format, ...)
{
    // �����debugģʽ�£����ӡ��λLOG��Ϣ
#if _DEBUG
    if (NULL == format)
    {
        cout << "[SrlLib] MyDbgView input is null." << endl;
        OutputDebugString("[SrlLib] MyDbgView input is null.");
        return;
    }

    char strLogMsg[1024] = {0};
    va_list ap;
    va_start(ap, format);
    vsnprintf_s(strLogMsg, 1024, _TRUNCATE, format, ap);
    va_end(ap);

    cout << strLogMsg << endl;
    OutputDebugString(strLogMsg);
#endif

    return;
}

SerialCmdProc::SerialCmdProc()
{
    m_pcSerial = NULL;
    m_pSrlReadCbFunc = NULL;
    m_pSrlCbUser = NULL;
}

SerialCmdProc::~SerialCmdProc()
{
    SrlDeinit();
}

int SerialCmdProc::SrlInit(const BCD_SERIAL_INFO * pstSeriallInfo)
{
    if (NULL == pstSeriallInfo)
    {
        MyDbgView("[SrlLib] SrlInit input is null.");
        return SRL_RET_RES;
    }

    int nRet = SRL_RET_OK;
    do 
    {
        if (NULL == m_pcSerial)
        {
            m_pcSerial = new CSerial();
            if (NULL == m_pcSerial)
            {
                nRet = SRL_RET_RES;
                break;
            }

            nRet = OpenSerial(pstSeriallInfo);
        }
        else
        {
            nRet = SRL_RET_TWICE;
        }
    } while (0);

    return nRet;
}

int SerialCmdProc::SrlDeinit()
{
    if (NULL != m_pcSerial)
    {
        m_pcSerial->Close();
        delete m_pcSerial;
        m_pcSerial = NULL;
    }

    m_pSrlReadCbFunc = NULL;
    m_pSrlCbUser = NULL;

    return SRL_RET_OK;
}

int SerialCmdProc::OpenSerial(const BCD_SERIAL_INFO * pstSeriallInfo)
{
    if (NULL == pstSeriallInfo)
    {
        MyDbgView("[SrlLib] OpenSeiral input is null.");
        return SRL_RET_RES;
    }

    if (NULL == m_pcSerial)
    {
        MyDbgView("[SrlLib] OpenSeiral m_pcSerial is null.");
        return SRL_RET_RES;
    }

    int nRet = SRL_RET_OK;
    do 
    {
        long lRet = 0;
        char strPort[32] = {0};
        sprintf_s(strPort, "COM%d", pstSeriallInfo->nComNum);
        lRet = m_pcSerial->Open((LPCTSTR)strPort, 0, 0, false);
        if (lRet != ERROR_SUCCESS)
        {
            MyDbgView("[SrlLib] open com[%u] failed. Serial error code = [%d].", pstSeriallInfo->nComNum, (int)lRet);
            nRet = SRL_RET_PARAM;
            break;
        }

        lRet = m_pcSerial->Setup((CSerial::EBaudrate)pstSeriallInfo->nBaudrate,
                                 (CSerial::EDataBits)pstSeriallInfo->nDataBits,
                                 (CSerial::EParity)pstSeriallInfo->nParity,
                                 (CSerial::EStopBits)pstSeriallInfo->nStopBits);
        if (lRet != ERROR_SUCCESS)
        {
            MyDbgView("[SrlLib] serial set param failed. Serial error code = [%d].", (int)lRet);
            CloseSerial();
            nRet = SRL_RET_PARAM;
            break;
        }

        if (pstSeriallInfo->nParity)
        {
            lRet = m_pcSerial->SetMask(CSerial::EEventBreak |
                                       CSerial::EEventCTS   |
                                       CSerial::EEventDSR   |
                                       CSerial::EEventError |
                                       CSerial::EEventRing  |
                                       CSerial::EEventRLSD  |
                                       CSerial::EEventRecv);
            if (ERROR_SUCCESS != nRet)
            {
                MyDbgView("[SrlLib] serial setmask failed, Serial error code = [%d].", (int)lRet);
                CloseSerial();
                nRet = SRL_RET_ERR;
                break;
            }
        }

        m_pcSerial->SetDtrSta(pstSeriallInfo->nEnableDtrControl);
        m_pcSerial->SetRtsSta(pstSeriallInfo->nEnableRtsControl);

        lRet = m_pcSerial->SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
        if (ERROR_SUCCESS != lRet)
        {
            MyDbgView("[SrlLib] serial SetupReadTimeouts failed, Serial error code = [%d].", (int)lRet);
            CloseSerial();
            nRet = SRL_RET_ERR;
        }
    } while (0);

    return nRet;
}

int SerialCmdProc::CloseSerial()
{
    if (NULL == m_pcSerial)
    {
        MyDbgView("[SrlLib] CloseSerial return error res.");
        return SRL_RET_RES;
    }

    m_pcSerial->Close();
    delete m_pcSerial;
    m_pcSerial = NULL;

    MyDbgView("[SrlLib] serial has been closed.");
    return SRL_RET_OK;
}

int SerialCmdProc::SrlSendInfo(const char * strInfo)
{
    if (NULL == strInfo)
    {
        MyDbgView("[SrlLib] SrlSendInfo input info is null.");
        return SRL_RET_RES;
    }

    if (NULL == m_pcSerial)
    {
        MyDbgView("[SrlLib] SrlSendInfo m_pcSerial is null.");
        return SRL_RET_RES;
    }

    long lRet = ERROR_SUCCESS;
    int nRet = SRL_RET_OK;
    lRet = m_pcSerial->Write(strInfo, strlen(strInfo) + 1);
    if (ERROR_SUCCESS == lRet)
    {
        MyDbgView("[SrlLib] serial send info : [%s].", strInfo);
    }
    else
    {
        MyDbgView("[SrlLib] serial send info [%s] failed, Serial error code = [%d]", strInfo, (int)lRet);
        nRet = SRL_RET_ERR;
    }

    return nRet;
}

int SerialCmdProc::SrlRecvInfo(char * strInfo, const unsigned int nBufLen, unsigned int & nRecvLen)
{
    /* strInfo  �Ƕ�ȡ���Ĵ�����Ϣ
       nBufLen  �Ǵ����Buf�ĳ���
       nRecvLen �ǽ��յ��Ĵ������ݵĳ��� */
    if (NULL == strInfo)
    {
        MyDbgView("[SrlLib] SrlRecvInfo input info is null.");
        return SRL_RET_RES;
    }

    if (NULL == m_pcSerial)
    {
        MyDbgView("[SrlLib] SrlRecvInfo m_pcSerial is null.");
        return SRL_RET_RES;
    }

    memset(strInfo, 0, nBufLen);
    int nRet = SRL_RET_OK;
    long lRet = m_pcSerial->Read(strInfo, nBufLen, (DWORD *)&nRecvLen);
    if (ERROR_SUCCESS != lRet)
    {
        nRecvLen = 0;
        MyDbgView("[SrlLib] serial read failed, [%d].", (int)lRet);
        nRet = SRL_RET_ERR;
    }
    else
    {
        if (nRecvLen > 0)
        {
            strInfo[nRecvLen] = '\0';
            MyDbgView("[SrlLib] serial recv info : [%s].", strInfo);
            if (NULL != m_pSrlReadCbFunc)
            {
                m_pSrlReadCbFunc((void *)strInfo, m_pSrlCbUser);
            }
            nRet = SRL_RET_OK;
        }
        else
        {
            nRet = SRL_RET_NODATA;
        }
    }

    return nRet;
}

int SerialCmdProc::SrlSetReadCallBackFunc(SRL_ReadCallBackFunc pFunc, void * pUser)
{
    if (NULL == pFunc)
    {
        MyDbgView("[SrlLib] SrlSetReadCallBackFunc pFunc is null.");
        return SRL_RET_ERR;
    }

    m_pSrlReadCbFunc = pFunc;
    m_pSrlCbUser = pUser;

    return SRL_RET_OK;
}