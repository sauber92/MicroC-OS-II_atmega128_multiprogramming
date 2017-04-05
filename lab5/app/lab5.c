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
  // 마이크로 C/OS-II 에서 사용할 기능 초기화
  OSInit();

  // 크리티컬 섹션을 보호하기 위한 부분
  // 인터럽트를 Disable해서 Context-switching이 안 일어나게 하는 방법
  // 싱글프로세스일 때 사용
  OS_ENTER_CRITICAL();
  
  // 타이머 레지스터 사용
  TCCR0=0x07;  
  TIMSK=_BV(TOIE0);                  
  TCNT0=256-(CPU_CLOCK_HZ/OS_TICKS_PER_SEC/ 1024);   
  
  // 크리티컬 섹션 끝
  OS_EXIT_CRITICAL();
  
  // OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio);
  // void (*task)(void *pd) : Task 명
  // void *pdata : Task의 파라미터
  // OS_STK *ptos : Task가 사용할 Stack. Stack은 메모리 공간이므로 배열 형식
  // INT8U prio : Task의 ID이자 우선순위. 0이 최고우선순위이며 Task마다 ID가 같을 수 없다.
  OSTaskCreate(LedTask, (void *)0, (void *)&LedTaskStk[TASK_STK_SIZE - 1], 0);
  OSTaskCreate(FndTask, (void *)0, (void *)&FndTaskStk[TASK_STK_SIZE - 1], 1);

  // Dormant 상태에 있는 Task를 Ready 상태로 바꾼다
  OSStart();                         
  
  return 0;
}

// LED ON/OFF를 10101010과 01010101로 반복하는 Task
void LedTask (void *data) {
  data = data;                                   

  // Main task
  DDRA = 0xFF;		
  while (1)  {
    PORTA = 0xAA;
	// OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U milli);
	// Micro C/OS-II에서 사용하는 Delay function
    OSTimeDlyHMSM(0, 0, 0, 500);
    
	PORTA = 0x55;
    OSTimeDlyHMSM(0, 0, 0, 500);
  }
}

// FND 4자리를 모두 8888로 디스플레이하는 Task
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