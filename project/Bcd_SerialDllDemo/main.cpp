#include <iostream>
#include <Windows.h>
#include "Bcd_SerialCmdLib.h"

using namespace std;

static void * g_hSerial = NULL;    // 全局串口句柄，用于串口句柄的读写

void __stdcall SerialReadCallBack(void * pInfo, void * pUser)
{
    // 回调函数。如果接收到的串口信息长度大于3，则回复"Hello, serial."信息
    if (strlen((char *)pInfo) >= 3)
    {
        BCD_SRL_Write(g_hSerial, "Hello, serial.", strlen("Hello, serial.") + 1);
    }

    return;
}

void main()
{
    // 初始化相关参数
    unsigned int nRecvLen = 0;    // 串口返回信息的长度
    char strRecvInfo[128] = {0};    // 收到的串口信息

    // 设置串口信息
    BCD_SERIAL_INFO stSerialInfo = {0};
    stSerialInfo.nComNum = 4;
    stSerialInfo.nBaudrate = 9600;
    stSerialInfo.nDataBits = 8;
    stSerialInfo.nParity = 0;
    stSerialInfo.nStopBits = 0;
    stSerialInfo.nEnableEvent = 0;
    stSerialInfo.nEnableDtrControl = 0;
    stSerialInfo.nEnableRtsControl = 0;

    // 根据串口信息，创建串口句柄
    int nRet = BCD_SRL_CreateHandle(&g_hSerial, &stSerialInfo);
    if (nRet != SRL_RET_OK)
    {
        cout << "BCD_SRL_CreateHandle failed..." << endl;
        Sleep(3000);
        return;
    }
    cout << "BCD_SRL_CreateHandle success..." << endl;

    // 设置串口读取信息时的回调函数（如无需回调，可省略）
    nRet = BCD_SRL_RigisterReadCallBack(g_hSerial, SerialReadCallBack, NULL);
    if (nRet != SRL_RET_OK)
    {
        cout << "BCD_SRL_RigisterReadCallBack failed..." << endl;
        Sleep(3000);
        return;
    }
    cout << "BCD_SRL_RigisterReadCallBack success..." << endl;

    // 持续读取串口信息
    while (1)
    {
        Sleep(10);
        nRet = BCD_SRL_Read(g_hSerial, strRecvInfo, sizeof(strRecvInfo), nRecvLen);
        if (SRL_RET_OK == nRet)
        {
            // 读取串口信息成功后，触发回调
            cout << "Serial recv : " << strRecvInfo << endl;
        }
    }

    BCD_SRL_DestoryHandle(g_hSerial);

    return;
}