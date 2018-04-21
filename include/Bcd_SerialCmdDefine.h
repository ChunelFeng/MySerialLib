#ifndef _BCD_SERIAL_CMD_DEFINE_H_
#define _BCD_SERIAL_CMD_DEFINE_H_

// 错误码信息
#define SRL_RET_OK        (0)
#define SRL_RET_ERR       (-1)    // 内部流程出错
#define SRL_RET_RES       (-2)    // 内部资源分配出错
#define SRL_RET_ORDER     (-3)    // 调用顺序出错
#define SRL_RET_TWICE     (-4)    // 重复调用
#define SRL_RET_PARAM     (-5)    // 设置串口参数失败
#define SRL_RET_NODATA    (-6)    // 无串口数据
#define SRL_RET_BUFLEN    (-7)    // 缓存数据长度超过最大缓存长度(1024)

typedef struct _BCD_SERIAL_INFO_
{
    // 串口相关信息
    unsigned int nComNum;               // 端口号
    unsigned int nBaudrate;             // 波特率
    unsigned int nDataBits;             // 数据位
    unsigned int nParity;               // 是否校验。例：设置0，表示校验位为No（No-0;Odd-1;Even-2;Mark-3;Space-4）
    unsigned int nStopBits;             // 停止位。例：设置0，表示校验位为1（1-0;1.5-2;2-2）
    unsigned int nEnableEvent;          // 是否利用事件打开
    unsigned int nEnableDtrControl;     // Dtr控制使能
    unsigned int nEnableRtsControl;     // Rts控制使能
}BCD_SERIAL_INFO;

// 读取数据时，触发的回调函数类型
typedef void (__stdcall * SRL_ReadCallBackFunc)(void * pInfo, void * pUser);

#endif