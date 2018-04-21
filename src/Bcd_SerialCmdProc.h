/**
*
* 版权信息：版权所有 (c) 2016, ***, 保留所有权利
*
* 文件名称：Bcd_SerialCmdProc.h
* 摘    要：串口控制库实现部分对应头文件
*
* 当前版本：1.1.0
* 作    者: Chunel
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

const unsigned int DEFAULT_RECV_SPAN = 32;      // 连续接收模式下，默认的串口接收间隔时间
const unsigned int MAX_RECV_LEN      = 1024;    // 串口一次接收数据的最大值

class SerialCmdProc
{
public:
    explicit SerialCmdProc();
    virtual ~SerialCmdProc();

    /* 对外提供的接口函数的内部实现 */
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

    /* 线程处理函数 */
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

    BOOL                      m_bIsCtnReading;       // 判断是否是连续读取状态
    void                      * m_hCtnReadHandle;    // 连续读取线程
    BOOL                      m_bIsExit;             // 结束所有流程的标志位
    unsigned int              m_nRecvTimeSpan;       // 相邻两次串口信息接收时间
};

#endif