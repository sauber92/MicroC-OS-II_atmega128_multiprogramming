/*
*********************************************************************************************************
*                                               uC/OS-II
*                                        The Real-Time Kernel
*
*                                          AVR Specific code
*
* File         : INCLUDES.H
* By           : Ole Saether
* Port Version : V1.01
*
* Modifications by Julius Luukko 07-21-2003 (Julius.Luukko@lut.fi) to get this compiled with 
* uC/OS-II 2.52 and 2.70.
*
*********************************************************************************************************
*/

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>


#include <stdarg.h>
#define F_CPU 16000000UL
#include <util/delay.h>



/* If you have uC/OS-II version below 2.70 uncomment the includes for os_cpu.h and os_cfg.h */
#include  "os_cpu.h"
#include  "os_cfg.h"
#include  <ucos_ii.h>

/* ISR support macros */
#include <avr_isr.h>

#define  COLOR_BLACK     0                       /* VT102 Color Codes                                  */
#define  COLOR_RED       1
#define  COLOR_GREEN     2
#define  COLOR_BROWN     3
#define  COLOR_BLUE      4
#define  COLOR_MAGENTA   5
#define  COLOR_CYAN      6
#define  COLOR_WHITE     7

void  VT102Init(void);
void  VT102SemInit(void);
void  VT102DispClrScr(void);                           
void  VT102DispChar(INT8U, INT8U, char);  
void  VT102DispStr(INT8U, INT8U, char *); 
void SPrintDec(char *s, INT16U x, INT8U n);
INT8U random(INT8U n);
