#include <iostream>
#include <Windows.h>
#include "Bcd_SerialCmdLib.h"

using namespace std;

static void * g_hSerial = NULL;    // ȫ�ִ��ھ�������ڴ��ھ���Ķ�д

void __stdcall SerialReadCallBack(void * pInfo, void * pUser)
{
    // �ص�������������յ��Ĵ�����Ϣ���ȴ���3����ظ�"Hello, serial."��Ϣ
    if (strlen((char *)pInfo) >= 3)
    {
        BCD_SRL_Write(g_hSerial, "Hello, serial.", strlen("Hello, serial.") + 1);
    }

    return;
}

void main()
{
    // ��ʼ����ز���
    unsigned int nRecvLen = 0;    // ���ڷ�����Ϣ�ĳ���
    char strRecvInfo[128] = {0};    // �յ��Ĵ�����Ϣ

    // ���ô�����Ϣ
    BCD_SERIAL_INFO stSerialInfo = {0};
    stSerialInfo.nComNum = 4;
    stSerialInfo.nBaudrate = 9600;
    stSerialInfo.nDataBits = 8;
    stSerialInfo.nParity = 0;
    stSerialInfo.nStopBits = 0;
    stSerialInfo.nEnableEvent = 0;
    stSerialInfo.nEnableDtrControl = 0;
    stSerialInfo.nEnableRtsControl = 0;

    // ���ݴ�����Ϣ���������ھ��
    int nRet = BCD_SRL_CreateHandle(&g_hSerial, &stSerialInfo);
    if (nRet != SRL_RET_OK)
    {
        cout << "BCD_SRL_CreateHandle failed..." << endl;
        Sleep(3000);
        return;
    }
    cout << "BCD_SRL_CreateHandle success..." << endl;

    // ���ô��ڶ�ȡ��Ϣʱ�Ļص�������������ص�����ʡ�ԣ�
    nRet = BCD_SRL_RigisterReadCallBack(g_hSerial, SerialReadCallBack, NULL);
    if (nRet != SRL_RET_OK)
    {
        cout << "BCD_SRL_RigisterReadCallBack failed..." << endl;
        Sleep(3000);
        return;
    }
    cout << "BCD_SRL_RigisterReadCallBack success..." << endl;

    // ������ȡ������Ϣ
    while (1)
    {
        Sleep(10);
        nRet = BCD_SRL_Read(g_hSerial, strRecvInfo, sizeof(strRecvInfo), nRecvLen);
        if (SRL_RET_OK == nRet)
        {
            // ��ȡ������Ϣ�ɹ��󣬴����ص�
            cout << "Serial recv : " << strRecvInfo << endl;
        }
    }

    BCD_SRL_DestoryHandle(g_hSerial);

    return;
}