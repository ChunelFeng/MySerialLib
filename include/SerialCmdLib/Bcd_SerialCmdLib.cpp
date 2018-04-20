#include <iostream>
#include "Bcd_SerialCmdLib.h"
#include "Bcd_SerialCmdProc.h"

using namespace std;

BCD_SERIALDLL_API unsigned int __stdcall BCD_SRL_GetLibVersion()
{
    return 20180123;
}

BCD_SERIALDLL_API int __stdcall BCD_SRL_CreateHandle(OUT void ** handle, IN const BCD_SERIAL_INFO * pstSerialInfo)
{
    if (NULL == handle)
    {
        return SRL_RET_RES;
    }

    SerialCmdProc * pcSrlProc = new SerialCmdProc();
    if (NULL == pcSrlProc)
    {
        return SRL_RET_RES;
    }

    int nRet = pcSrlProc->SrlInit(pstSerialInfo);
    if (SRL_RET_OK != nRet)
    {
        return nRet;
    }
    *handle = pcSrlProc;

    return SRL_RET_OK;
}

BCD_SERIALDLL_API int __stdcall BCD_SRL_Write(IN void * handle, IN const char * strInfo)
{
    if (NULL == handle)
    {
        return SRL_RET_RES;
    }

    SerialCmdProc * pcSrlProc = (SerialCmdProc *)handle;
    return pcSrlProc->SrlSendInfo(strInfo);
}

BCD_SERIALDLL_API int __stdcall BCD_SRL_Read(IN void * handle, OUT char * strInfo, IN const unsigned int nBufLen, OUT unsigned int & nRecvLen)
{
    if (NULL == handle)
    {
        return SRL_RET_RES;
    }

    SerialCmdProc * pcSrlProc = (SerialCmdProc *)handle;    // 后面添加函数
    return pcSrlProc->SrlRecvInfo(strInfo, nBufLen, nRecvLen);
}

BCD_SERIALDLL_API int __stdcall BCD_SRL_RigisterReadCallBack(IN void * handle, IN SRL_ReadCallBackFunc pFunc, IN void * pUser)
{
    if (NULL == handle)
    {
        return SRL_RET_RES;
    }

    SerialCmdProc * pcSrlProc = (SerialCmdProc *)handle;
    return pcSrlProc->SrlSetReadCallBackFunc(pFunc, pUser);
}

BCD_SERIALDLL_API int __stdcall BCD_SRL_SetRecvTimeSpan(IN void * handle, IN const unsigned int nRecvTimeSpan)
{
	if (NULL == handle)
	{
		return SRL_RET_RES;
	}

	SerialCmdProc * pcSrlProc = (SerialCmdProc *)handle;
	return pcSrlProc->SrlSetRecvTimeSpan(nRecvTimeSpan);
}

BCD_SERIALDLL_API int __stdcall BCD_SRL_StartReading(IN void * handle, IN const unsigned int nRecvTimeSpan)
{
	if (NULL == handle)
	{
		return SRL_RET_RES;
	}

	SerialCmdProc * pcSrlProc = (SerialCmdProc *)handle;
	return pcSrlProc->SrlStartRecving(nRecvTimeSpan);
}

BCD_SERIALDLL_API int __stdcall BCD_SRL_StopReading(IN void * handle)
{
	if (NULL == handle)
	{
		return SRL_RET_RES;
	}

	SerialCmdProc * pcSrlProc = (SerialCmdProc *)handle;
	return pcSrlProc->SrlStopRecving();
}

BCD_SERIALDLL_API int __stdcall BCD_SRL_DestoryHandle(IN void * handle)
{
    if (NULL == handle)
    {
        return SRL_RET_RES;
    }

    SerialCmdProc * pcSrlProc = (SerialCmdProc *)handle;
    pcSrlProc->SrlDeinit();
    delete pcSrlProc;

    return SRL_RET_OK;
}
