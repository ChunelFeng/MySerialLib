/**
*
* 版权信息：版权所有 (c) 2016, ***, 保留所有权利
*
* 文件名称：Bcd_SerialCmdProc.h
* 摘    要：串口控制库实现部分对应头文件
*
* 当前版本：1.0.0.0
* 作    者: ***
* 日    期：2018-01-25
* 备    注：新建

* ━━━━━━神兽出没━━━━━━
* 　　　┏┓　　　┏┓
* 　　┏┛┻━━━┛┻┓
* 　　┃　　　　　　　┃
* 　　┃　　　━　　　┃
* 　　┃　┳┛　┗┳　┃
* 　　┃　　　　　　　┃
* 　　┃　　　┻　　　┃
* 　　┃　　　　　　　┃
* 　　┗━┓　　　┏━┛Code is far away from bug with the animal protecting
* 　　　　┃　　　┃
* 　　　　┃　　　┃
* 　　　　┃　　　┗━━━┓
* 　　　　┃　　　　　　　┣┓
* 　　　　┃　　　　　　　┏┛
* 　　　　┗┓┓┏━┳┓┏┛
* 　　　　　┃┫┫　┃┫┫
* 　　　　　┗┻┛　┗┻┛
*
* ━━━━━━感觉萌萌哒━━━━━━
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