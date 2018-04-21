#ifndef _BCD_SERIAL_CMD_DEFINE_H_
#define _BCD_SERIAL_CMD_DEFINE_H_

// ��������Ϣ
#define SRL_RET_OK        (0)
#define SRL_RET_ERR       (-1)    // �ڲ����̳���
#define SRL_RET_RES       (-2)    // �ڲ���Դ�������
#define SRL_RET_ORDER     (-3)    // ����˳�����
#define SRL_RET_TWICE     (-4)    // �ظ�����
#define SRL_RET_PARAM     (-5)    // ���ô��ڲ���ʧ��
#define SRL_RET_NODATA    (-6)    // �޴�������
#define SRL_RET_BUFLEN    (-7)    // �������ݳ��ȳ�����󻺴泤��(1024)

typedef struct _BCD_SERIAL_INFO_
{
    // ���������Ϣ
    unsigned int nComNum;               // �˿ں�
    unsigned int nBaudrate;             // ������
    unsigned int nDataBits;             // ����λ
    unsigned int nParity;               // �Ƿ�У�顣��������0����ʾУ��λΪNo��No-0;Odd-1;Even-2;Mark-3;Space-4��
    unsigned int nStopBits;             // ֹͣλ����������0����ʾУ��λΪ1��1-0;1.5-2;2-2��
    unsigned int nEnableEvent;          // �Ƿ������¼���
    unsigned int nEnableDtrControl;     // Dtr����ʹ��
    unsigned int nEnableRtsControl;     // Rts����ʹ��
}BCD_SERIAL_INFO;

// ��ȡ����ʱ�������Ļص���������
typedef void (__stdcall * SRL_ReadCallBackFunc)(void * pInfo, void * pUser);

#endif