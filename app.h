/*******************************************************************************

                      ��Ȩ���� (C), 2017- Mushiny.

 *******************************************************************************
  �� �� ��   : main.h
  �� �� ��   : ����
  ��    ��   : hzh
  ��������   :2017.11.5
  ����޸�   :
  ��������   : Ӧ�ó���ӿ�����ģ��
  �����б�   :
  �޸���ʷ   :
*******************************************************************************/
#ifndef _APP_H
#define _APP_H

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "includes.h"
#include "sys.h"
#include "mylib.h"
#include <rsh.h>

/* ��ģ�����ⲿ�ṩ�ĺ궨�� --------------------------------------------------*/
#define LOGO_STRING                     "NCURoboteam-A Robot# "
#define _MAIN_DEBUG_                  1
#define _CLI_ENABLE_                  1

/* ��ģ�����ⲿ�ṩ�ĺ궨�� --------------------------------------------------*/
typedef volatile struct
{
  uint8_t  control_num;     //��ǰ���ƴ���	
	uint8_t  control_final;   //����ֹͣ����
	float    P;               //�ջ�Pֵ
  float    D;               //�ջ�Dֵ
	float    Xtime;           //����x��ʱ��
	int      turn;            //������תȦ��
	int      speed;           //�����ٶ�
	int      Vspeed;          //����y���ٶ�
	int      Aspeed;          //����y���ٶ�
	int      point;           //�ͷŵ�
	int      init_point;      //��ʼλ��
	int      target_distance; //���پ���
	int      real_distance;   //ʵ��λ��
	int      theo_distance;   //����λ��
	int      offe_distance;   //ƫ�����
	int      offe_lastance;   //ƫ�����
	int      time;            //ֹͣʱ��
}THROW_INDEX;  //��������

 typedef struct {
	/*****************����΢��������**********************/
  float r;//�������� ʱ��߶�
	float h;//�˲����� ����ʱ��
	float n;
  float h0;//h0=n*h��
	
	float x1;//����΢���������ź�
	float x2;//����΢����΢�ָ����ź�
	float fh;
/********************��������ϲ���*********************/
	float b1;
	float theta;
	float b2;
	float alphal;
	
/**********************����ź�************************/
  float u0;	
	float e0;
	float e1;
	float e2;
	
}adrc;


/* ��ģ�����ⲿ�ṩ�Ľӿڳ������� --------------------------------------------*/

#define Pi 3.14159f               //��
#define TIME_cyc 20.0f            //�������� ms
#define cycle   64000          //�������תһȦ�����������ص�����ֵ
#define throw_speed_max   200     //��������ٶ�
#define throw_speed_min  -200     //��������ٶ�  
#define CAN_Delay        50     //CAN��ʱ����

#define ONE    1  //TZ1״̬.
#define TWO    2  //TZ2״̬
#define THREE  3  //TZ3״̬ 


#define _delay 0        //�ȴ�����״̬
#define _calibration 1  //�궨״̬
#define _connect 2      //����״̬
#define _throw 3        //����״̬
#define _ready_connect 4//ǰ�����ӵ�
#define _ready_throw 5  //ǰ��������
#define _ready_location_one 6//ǰ����λ��
#define _ready_location_two 7//ǰ����λ��
/* ��ģ�����ⲿ�ṩ�Ľӿڱ������� --------------------------------------------*/
extern uint8_t motor_init_flag;
extern uint8_t throw_flag;
extern uint8_t Robot_Mode;
extern uint8_t place;
/* ��ģ�����ⲿ�ṩ�Ľӿں���ԭ������ ----------------------------------------*/
void BSP_Init(void);
void retarget_init(void);
int  asy_printf(const char *fmt, ...);
char put_char(char ch);

void motor_all_Init(void);
void motor_move_setvmmps(int dstVmmps_X,int dstVmmps_Y,int dstVmmps_W);
void motor_move_stop(void);
void IWDG_Feed(void);
void throw_init(uint8_t area);
void throw_ball(uint8_t area);
void throw_stop(uint8_t area);
void throw_adjust(void);

void create_star_task(uint8_t prio);
void create_IWDG_task(uint8_t prio);
void create_motor_driver_task(uint8_t prio);
void create_hand_task(uint8_t prio);
void create_key_task(uint8_t prio);
void create_display_task(uint8_t prio);
void create_command_task(uint8_t prio);
void create_throw_task(uint8_t prio);
void create_connect_task(uint8_t prio);

/* ȫ�������� ----------------------------------------*/


//λ��������
#ifndef BIT
#define BIT(n)                (1 << (n))
#define bit(n)                (1 << (n))
#endif

#define setbit(reg,bits)      (reg) |= (bits)
#define clearbit(reg,bits)    (reg) &= ~(bits)
#define readbit(reg,bits)     ((reg) & (bits))
#define togglebit(reg,bits)   (reg) ^= (bits)

// ���Ժ꣬printf״̬���ٵ���
//�����ڼĴ�����ַ
#define UART1_DR_Addr         (USART1_BASE + 0x04)
#define UART2_DR_Addr         (USART2_BASE + 0x04)
#define UART3_DR_Addr         (USART3_BASE + 0x04)
#define UART4_DR_Addr         (UART4_BASE  + 0x04)
#define UART5_DR_Addr         (UART5_BASE  + 0x04)
#define UART6_DR_Addr         (USART6_BASE + 0x04)

#define SYSTEM_DEBUG                  1
#define DEBUG_TRACE                   asy_printf
#define os_printf                     asy_printf

// Printf UART ����
#define PrintfUART_AF                 GPIO_AF_UART4
#define PrintfUART_RCC                RCC_APB1Periph_UART4

#define PrintfUART_TXD_GPIO           GPIOC
#define PrintfUART_TXD_PIN            GPIO_Pin_10

#define PrintfUART_RXD_GPIO           GPIOC
#define PrintfUART_RXD_PIN            GPIO_Pin_11

#define PrintfUART                    UART4
#define PrintfUART_DR_Addr            UART4_DR_Addr
#define PrintfUART_BAUD               115200
#define PrintfUART_TxBufSize          128
#define PrintfUART_RxBufSize          128
#define PrintfUART_IRQn               UART4_IRQn
#define PrintfUART_IRQHandler         UART4_IRQHandler

#endif
/************** ��Ȩ���� (C), 2017- Mushiny. ***********���ļ�����***********/


