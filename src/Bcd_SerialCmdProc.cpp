/**************************************************************
*
* 版权信息：版权所有 (c) 2016, ***, 保留所有权利
*
* 文件名称：Bcd_SerialCmdProc.cpp
* 摘    要：串口控制库实现部分
*
* 当前版本：1.0.0.0
* 作    者: Chunel
* 日    期：2018-01-25
* 备    注：新建
*　　　　　　　　┏┓　　　┏┓+ +
*　　　　　　　┏┛┻━━━┛┻┓ + +
*　　　　　　　┃　　　　　　　┃
*　　　　　　　┃　　　━　　　┃ ++ + + +
*　　　　　　 ████━████ ┃+
*　　　　　　　┃　　　　　　　┃ +
*　　　　　　　┃　　　┻　　　┃
*　　　　　　　┃　　　　　　　┃ + +
*　　　　　　　┗━┓　　　┏━┛
*　　　　　　　　　┃　　　┃
*　　　　　　　　　┃　　　┃ + + + +
*　　　　　　　　　┃　　　┃　　　　Code is far away from bug with the animal protecting
*　　　　　　　　　┃　　　┃ + 　　　　神兽保佑,代码无bug
*　　　　　　　　　┃　　　┃
*　　　　　　　　　┃　　　┃　　+
*　　　　　　　　　┃　 　 ┗━━━┓ + +
*　　　　　　　　　┃ 　　　　　　　┣┓
*　　　　　　　　　┃ 　　　　　　　┏┛
*　　　　　　　　　┗┓┓┏━┳┓┏┛ + + + +
*　　　　　　　　　　┃┫┫　┃┫┫
*　　　　　　　　　　┗┻┛　┗┻┛+ + + +
*
* ━━━━━━感觉萌萌哒━━━━━━
**************************************************************/

#include <iostream>
#include <process.h>  
#include "Bcd_SerialCmdProc.h"

using namespace std;

inline static void MyDbgView(const char * format, ...)
{
    // 如果是debug模式下，则打印定位LOG信息
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

unsigned int __stdcall StartRecvingThread(void * pUser)
{
    if (NULL != pUser)
    {
        SerialCmdProc * pThis = (SerialCmdProc *)pUser;
        pThis->StartRecvingProc();
    }

    _endthreadex(0);
    return 1;
}

SerialCmdProc::SerialCmdProc()
{
    m_pcSerial = NULL;
    m_pSrlReadCbFunc = NULL;
    m_pSrlCbUser = NULL;

    memset(&m_stSerialInfo, 0, sizeof(BCD_SERIAL_INFO));
    m_bIsCtnReading = FALSE;
    m_hCtnReadHandle = NULL;
    m_bIsExit = FALSE;
    m_nRecvTimeSpan = DEFAULT_RECV_SPAN;
}

SerialCmdProc::~SerialCmdProc()
{
    SrlDeinit();
}

int SerialCmdProc::SrlInit(const BCD_SERIAL_INFO * pstSerialInfo)
{
    if (NULL == pstSerialInfo)
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

            nRet = OpenSerial(pstSerialInfo);
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
    CloseCtnRecvingThread();

    if (NULL != m_pcSerial)
    {
        m_pcSerial->Close();
        delete m_pcSerial;
        m_pcSerial = NULL;
    }

    memset(&m_stSerialInfo, 0, sizeof(BCD_SERIAL_INFO));
    m_bIsCtnReading = FALSE;
    m_pSrlReadCbFunc = NULL;
    m_pSrlCbUser = NULL;

    return SRL_RET_OK;
}

int SerialCmdProc::OpenSerial(const BCD_SERIAL_INFO * pstSerialInfo)
{
    if (NULL == pstSerialInfo)
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
        sprintf_s(strPort, "COM%d", pstSerialInfo->nComNum);
        lRet = m_pcSerial->Open((LPCTSTR)strPort, 0, 0, false);
        if (lRet != ERROR_SUCCESS)
        {
            MyDbgView("[SrlLib] open com[%u] failed. Serial error code = [%d].", pstSerialInfo->nComNum, (int)lRet);
            nRet = SRL_RET_PARAM;
            break;
        }

        lRet = m_pcSerial->Setup((CSerial::EBaudrate)pstSerialInfo->nBaudrate,
                                 (CSerial::EDataBits)pstSerialInfo->nDataBits,
                                 (CSerial::EParity)pstSerialInfo->nParity,
                                 (CSerial::EStopBits)pstSerialInfo->nStopBits);
        if (lRet != ERROR_SUCCESS)
        {
            MyDbgView("[SrlLib] serial set param failed. Serial error code = [%d].", (int)lRet);
            CloseSerial();
            nRet = SRL_RET_PARAM;
            break;
        }

        if (pstSerialInfo->nParity)
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

        m_pcSerial->SetDtrSta(pstSerialInfo->nEnableDtrControl);
        m_pcSerial->SetRtsSta(pstSerialInfo->nEnableRtsControl);

        lRet = m_pcSerial->SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
        if (ERROR_SUCCESS != lRet)
        {
            MyDbgView("[SrlLib] serial SetupReadTimeouts failed, Serial error code = [%d].", (int)lRet);
            CloseSerial();
            nRet = SRL_RET_ERR;
        }
        else
        {
            /* 如果打开串口过程一切正常，则保存串口设置相关信息 */
            memset(&m_stSerialInfo, 0, sizeof(BCD_SERIAL_INFO));
            memcpy(&m_stSerialInfo, pstSerialInfo, sizeof(BCD_SERIAL_INFO));
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

int SerialCmdProc::SrlSendInfo(const char * strInfo, const unsigned int nBufLen)
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
    lRet = m_pcSerial->Write(strInfo, nBufLen);
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
    /* strInfo  是读取到的串口信息
       nBufLen  是传入的Buf的长度
       nRecvLen 是接收到的串口数据的长度 */
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

    if (TRUE == m_bIsCtnReading)
    {
        MyDbgView("[SrlLib] SrlRecvInfo is in CtnReading status.");
        return SRL_RET_ERR;
    }

    if (nBufLen > SRL_RET_BUFLEN)
    {
        MyDbgView("[SrlLib] SrlRecvInfo nBufLen is too big.");
        return SRL_RET_BUFLEN;
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

int SerialCmdProc::SrlRecvBlockInfo(const unsigned int nBlockLen, char * strInfo, const unsigned int nBufLen)
{
    if (NULL == strInfo)
    {
        MyDbgView("[SrlLib] SrlRecvBlockInfo input info is null.");
        return SRL_RET_RES;
    }

    if (NULL == m_pcSerial)
    {
        MyDbgView("[SrlLib] SrlRecvBlockInfo m_pcSerial is null.");
        return SRL_RET_RES;
    }

    if (TRUE == m_bIsCtnReading)
    {
        MyDbgView("[SrlLib] SrlRecvBlockInfo is in CtnReading status.");
        return SRL_RET_ERR;
    }

    if (nBufLen < nBlockLen)
    {
        MyDbgView("[SrlLib] SrlRecvBlockInfo nBufLen is smaller than nBlockLen.");
        return SRL_RET_BUFLEN;
    }

    if (nBufLen > SRL_RET_BUFLEN || nBlockLen > SRL_RET_BUFLEN)
    {
        MyDbgView("[SrlLib] SrlRecvBlockInfo nBufLen or nBlockLen is too big.");
        return SRL_RET_BUFLEN;
    }

    unsigned int nTmpLen = 0;
    DWORD nRecvLen = 0;
    memset(strInfo, 0, nBufLen);
    long lRet = ERROR_SUCCESS;

    while (FALSE == m_bIsExit)
    {
        /* 在循环中实现，是防止在调用等待的时候，外部进行析构 */
        lRet = m_pcSerial->Read(strInfo + nTmpLen, nBufLen - nTmpLen, (DWORD *)&nRecvLen);
        if (ERROR_SUCCESS == lRet && nRecvLen > 0)
        {
            nTmpLen += nRecvLen;
            if (nTmpLen >= nBlockLen)
            {
                strInfo[nBlockLen] = '\0';
                if (NULL != m_pSrlReadCbFunc)
                {
                    m_pSrlReadCbFunc((void *)strInfo, m_pSrlCbUser);
                }
                break;
            }

            Sleep(2);
        }
    }

    return SRL_RET_OK;
}

int SerialCmdProc::SrlSetReadCallBackFunc(SRL_ReadCallBackFunc pFunc, void * pUser)
{
    if (NULL == pFunc)
    {
        MyDbgView("[SrlLib] SrlSetReadCallBackFunc pFunc is null.");
        return SRL_RET_RES;
    }

    m_pSrlReadCbFunc = pFunc;
    m_pSrlCbUser = pUser;

    return SRL_RET_OK;
}

int SerialCmdProc::SrlStartRecving()
{
    /* nRecvSpan 是相邻两次读取串口信息的间隔时间（单位为ms） */
    if (NULL == m_pcSerial)
    {
        MyDbgView("[SrlLib] SrlStartRecving m_pcSerial is null.");
        return SRL_RET_RES;
    }

    int nRet = SRL_RET_OK;
    if (NULL == m_hCtnReadHandle)
    {
        m_bIsExit = FALSE;
        m_bIsCtnReading = TRUE;
        m_hCtnReadHandle = (void *)_beginthreadex(NULL, 0, StartRecvingThread, this, 0, NULL);
        if (NULL == m_hCtnReadHandle)
        {
            MyDbgView("[SrlLib] SrlStartRecving create thread failed.");
            nRet = SRL_RET_RES;
        }
    }
    else
    {
        MyDbgView("[SrlLib] SrlStartRecving m_hCtnReadHandle is not null.");
        nRet = SRL_RET_RES;
    }

    return nRet;
}

void SerialCmdProc::StartRecvingProc()
{
    if (NULL == m_pcSerial)
    {
        MyDbgView("[SrlLib] StartRecvingProc m_pcSerial is null.");
        return;
    }

    char strInfo[MAX_RECV_LEN] = {0};
    DWORD nRecvLen = 0;
    long lRet = ERROR_SUCCESS;

    while (FALSE == m_bIsExit)
    {
        Sleep(m_nRecvTimeSpan);

        // 开始持续读取串口信息，然后通过设置回调，来进行处理
        memset(strInfo, 0, MAX_RECV_LEN);
        nRecvLen = 0;
        lRet = m_pcSerial->Read(strInfo, MAX_RECV_LEN, (DWORD *)&nRecvLen);
        if (ERROR_SUCCESS == lRet && nRecvLen > 0)
        {
            strInfo[nRecvLen] = '\0';
            MyDbgView("[SrlLib] serial continue recv info : [%s].", strInfo);
            if (NULL != m_pSrlReadCbFunc)
            {
                m_pSrlReadCbFunc((void *)strInfo, m_pSrlCbUser);
            }
        }
    }

    return;
}

void SerialCmdProc::CloseCtnRecvingThread()
{
    m_bIsExit = TRUE;
    if (NULL != m_hCtnReadHandle)
    {
        WaitForSingleObject(m_hCtnReadHandle, INFINITE);
        CloseHandle(m_hCtnReadHandle);
        m_hCtnReadHandle = NULL;
    }

    return;
}

int SerialCmdProc::SrlSetRecvTimeSpan(const unsigned int nRecvTimeSpan)
{
    m_nRecvTimeSpan = nRecvTimeSpan;
    return SRL_RET_OK;
}

int SerialCmdProc::SrlGetSerialInfo(BCD_SERIAL_INFO * pstSerialInfo)
{
    if (NULL == pstSerialInfo)
    {
        MyDbgView("[SrlLib] SrlGetSerialInfo pstSerialInfo is null.");
        return SRL_RET_RES;
    }

    memset(pstSerialInfo, 0, sizeof(BCD_SERIAL_INFO));
    memcpy(pstSerialInfo, &m_stSerialInfo, sizeof(BCD_SERIAL_INFO));

    return SRL_RET_OK;
}

int SerialCmdProc::SrlStopRecving()
{
    CloseCtnRecvingThread();
    m_bIsCtnReading = FALSE;

    return SRL_RET_OK;
}