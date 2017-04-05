#include "includes.h"

#define F_CPU	16000000UL	// CPU frequency = 16 Mhz
#include <avr/io.h>	
#include <util/delay.h>

#define  TASK_STK_SIZE  OS_TASK_DEF_STK_SIZE            

// Task Stack
OS_STK LedTaskStk[TASK_STK_SIZE];
OS_STK FndTaskStk[TASK_STK_SIZE];

// Task
void LedTask(void *data);
void FndTask(void *data);


int main (void) {
  // ����ũ�� C/OS-II ���� ����� ��� �ʱ�ȭ
  OSInit();

  // ũ��Ƽ�� ������ ��ȣ�ϱ� ���� �κ�
  // ���ͷ�Ʈ�� Disable�ؼ� Context-switching�� �� �Ͼ�� �ϴ� ���
  // �̱����μ����� �� ���
  OS_ENTER_CRITICAL();
  
  // Ÿ�̸� �������� ���
  TCCR0=0x07;  
  TIMSK=_BV(TOIE0);                  
  TCNT0=256-(CPU_CLOCK_HZ/OS_TICKS_PER_SEC/ 1024);   
  
  // ũ��Ƽ�� ���� ��
  OS_EXIT_CRITICAL();
  
  // OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio);
  // void (*task)(void *pd) : Task ��
  // void *pdata : Task�� �Ķ����
  // OS_STK *ptos : Task�� ����� Stack. Stack�� �޸� �����̹Ƿ� �迭 ����
  // INT8U prio : Task�� ID���� �켱����. 0�� �ְ�켱�����̸� Task���� ID�� ���� �� ����.
  OSTaskCreate(LedTask, (void *)0, (void *)&LedTaskStk[TASK_STK_SIZE - 1], 0);
  OSTaskCreate(FndTask, (void *)0, (void *)&FndTaskStk[TASK_STK_SIZE - 1], 1);

  // Dormant ���¿� �ִ� Task�� Ready ���·� �ٲ۴�
  OSStart();                         
  
  return 0;
}

// LED ON/OFF�� 10101010�� 01010101�� �ݺ��ϴ� Task
void LedTask (void *data) {
  data = data;                                   

  // Main task
  DDRA = 0xFF;		
  while (1)  {
    PORTA = 0xAA;
	// OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U milli);
	// Micro C/OS-II���� ����ϴ� Delay function
    OSTimeDlyHMSM(0, 0, 0, 500);
    
	PORTA = 0x55;
    OSTimeDlyHMSM(0, 0, 0, 500);
  }
}

// FND 4�ڸ��� ��� 8888�� ���÷����ϴ� Task
void FndTask(void * data) {
	data = data;
	
	unsigned char FND_DATA[ ]= {0x3F, 0x06, 0x5B, 0x4F, 0x66,
								0x6D, 0x7D, 0x27, 0x7F, 0x6F,
								0x77, 0x7C, 0x39, 0x5E, 0x79,
								0x71, 0x80, 0x40, 0x08};

	DDRC = 0xFF;
	DDRG = 0x0F;

	while(1) {
		PORTC = FND_DATA[8];
		PORTG = 0x0f;
	}
}