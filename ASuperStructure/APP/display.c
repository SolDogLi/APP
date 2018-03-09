/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny
                      
 *******************************************************************************
  文 件 名   : display.c
  版 本 号   : 初稿
  作    者   : LJM
  生成日期   : 2017.12.26
  最近修改   :
  功能描述   : 状态显示任务
               本系统编程时需要注意:
               >>凡是用了raw_printf()的task，一定要注意raw_printf是需要动态变量数组的，
                 所以task的任务堆栈必须能满足要求，最好要128或256以上
  函数列表   :
*******************************************************************************/
/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"

/* 内部自定义数据类型 --------------------------------------------------------*/
/* 内部宏定义 ----------------------------------------------------------------*/

/* 任务相关信息定义-----------------------------------------------------------*/
#define DISPLAY_STK_SIZE		128
OS_TCB	DISPLAYTaskTCB;
CPU_STK	DISPLAY_TASK_STK[DISPLAY_STK_SIZE];

/* 内部常量定义---------------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/
extern int Encoder_TIM2;
extern THROW_INDEX TZ1; //TZ1参数
extern THROW_INDEX TZ2; //TZ2参数
extern THROW_INDEX TZ3; //TZ3参数
extern int angle,past_cycle;
extern int real_speed;
extern char USART3_Flag;
extern volatile char command_flag;
extern volatile char Photoelectric_flag;
extern volatile char GET_flag;
extern int display[8];
extern int TIM3_CCR1;
extern int RL_NUM;
extern int _entercode_3,_entercode_4,_entercode_5;
extern char cali_flag;
extern int Encoder;
extern int accspeed;
/* 外部函数原型声明 ----------------------------------------------------------*/

/* 内部变量 ------------------------------------------------------------------*/

/* 内部函数原型声明 ----------------------------------------------------------*/
/* 任务主体部分 --------------------------------------------------------------*/
static void display_task(void *p_arg)
{	  
  Clear_LCD();	
	while(1)
	{	 			        
	 p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告
		 switch(Robot_Mode)
	 {
		 case MATCH_MODE:{
					LCD_Write_string("  cali_flag",0, 0);
			    LCD_Display_int(cali_flag,15,0);	
					LCD_Write_string("  real_speed",0, 2);
					LCD_Display_int(real_speed,15,2);			
					LCD_Write_string("  TZ1.control_final",0, 3);
					LCD_Display_int(TZ1.control_final,15,3);
					LCD_Write_string("  Encoder",0, 4);
					LCD_Display_int(Encoder,15,4);
			    switch(place)
					{
				   case ONE:{
						 LCD_Write_string("  TZ1_speed",0, 7);
						 LCD_Display_int(TZ1.speed,15,7);		
						 LCD_Write_string("  TZ1_point",0, 6);
						 LCD_Display_int(TZ1.point,15,6);
						 LCD_Write_string("  TZ1_turn",0, 5);
						 LCD_Display_int(TZ1.turn,15,5);
									 
		         }break;
						case TWO:{
						 LCD_Write_string("  TZ2_speed",0, 7);
						 LCD_Display_int(TZ2.speed,15,7);		
						 LCD_Write_string("  TZ2_point",0, 6);
						 LCD_Display_int(TZ2.point,15,6);
						 LCD_Write_string("  TZ2_turn",0, 5);
						 LCD_Display_int(TZ2.turn,15,5);
						 }break;
						 case THREE:{
						 LCD_Write_string("  TZ3_speed",0, 7);
						 LCD_Display_int(TZ3.speed,15,7);		
						 LCD_Write_string("  TZ3_point",0, 6);
						 LCD_Display_int(TZ3.point,15,6);
						 LCD_Write_string("  TZ3_turn",0, 5);
						 LCD_Display_int(TZ3.turn,15,5);
							}break;
							default :{
							}break;
							}
	   switch(command_flag){
			
			/*************等待状态***********************/	
				case _delay:   //等待决策
					LCD_Write_string("  task_name:  delay     ",0, 1);
//				LCD_Write_string("  Photoelectric_flag",0, 5);
          LCD_Display_int(Photoelectric_flag,18,5);
			      
				
				break;
			
			/***********标定状态***********************/	
			  case _calibration:   //进行标定
				LCD_Write_string("  task_name:  calibrate  ",0, 1);
//				LCD_Write_string("  motor_flag",0, 5);
        LCD_Display_int(motor_init_flag,18,3);			
		  	break;
				
			/***********交接状态***********************/	
			case _connect: //进行交接
			  LCD_Write_string("  task_name:  connect   ",0, 1);          
				break;
			
			/***********抛掷状态***********************/	
			case _throw:
				LCD_Write_string("  task_name:  throw    ",0, 1);
        

			   	break;
			
				
			/***********前往交接点**********************/	
			case _ready_connect:
				LCD_Write_string("  task_name: to_connect  ",0, 1);
//			  LCD_Write_string("  Photoelectric",0, 5);
        LCD_Display_int(Photoelectric_flag,18,5);
				break;
			
			/***********前往抛掷点***********************/	
			case _ready_throw:
				LCD_Write_string("  task_name: to_throw  ",0, 1);
				break;
			
			/***********前往摄像头定位点1**********************/		
			case _ready_location_one:
				LCD_Write_string("  task_name: to_throw  ",0, 1);
//				LCD_Write_string("  Photoelectric_flag",0, 5);
				LCD_Display_int(Photoelectric_flag,18,5);
				break;
			
			/***********前往摄像头定位点2**********************/				
			case _ready_location_two:
				LCD_Write_string("  task_name: to_throw  ",0, 1);
//				LCD_Write_string("  Photoelectric_flag",0, 5);
				LCD_Display_int(Photoelectric_flag,18,5);
				break;
			
			default:break;
		}
			                 	
		                 }break;                       //比赛模式
		 case THROW_MODE:{
                       LCD_Write_string("   throw_mode",0, 0);	
                       LCD_Display_int(place,16,0);
			                 switch(place)
											 {
												 case ONE:{
												           LCD_Write_string("  TZ1_speed",0, 2);
                                   LCD_Display_int(TZ1.speed,15,2);		
                                   LCD_Write_string("  TZ1_point",0, 3);
                                   LCD_Display_int(TZ1.point,15,3);
                                   LCD_Write_string("  accspeed",0, 4);
                                   LCD_Display_float(accspeed,11,4);
													         LCD_Write_string("  RL_NUM",0, 5);
                                   LCD_Display_int(RL_NUM,11,5);
													         LCD_Write_string("  TZ1_turn",0, 6);
                                   LCD_Display_int(TZ1.turn,15,6);
                                   LCD_Write_string("  Encoder",0, 7);
                                   LCD_Display_int(Encoder_TIM2*65536 + TIM2->CNT,10,7);	//real_speed=Encoder*1000*60/(4000*15);					 
												 }break;
												 case TWO:{
													         LCD_Write_string("  TZ2_speed",0, 2);
                                   LCD_Display_int(TZ2.speed,15,2);		
                                   LCD_Write_string("  TZ2_point",0, 3);
                                   LCD_Display_int(TZ2.point,15,3);
                                   LCD_Write_string("  TZ2_P",0, 4);
                                   LCD_Display_float(TZ2.P,11,4);
													        LCD_Write_string("  RL_NUM",0, 5);
                                   LCD_Display_int(RL_NUM,11,5);
													         LCD_Write_string("  TZ2_turn",0, 6);
                                   LCD_Display_int(TZ2.turn,15,6);
                                   LCD_Write_string("  Encoder",0, 7);
                                   LCD_Display_int(Encoder_TIM2*65536 + TIM2->CNT,10,7);
												 }break;
												 case THREE:{
																	 LCD_Write_string("  TZ3_speed",0, 2);
																	 LCD_Display_int(TZ3.speed,15,2);		
																	 LCD_Write_string("  TZ3_point",0, 3);
																	 LCD_Display_int(TZ3.point,15,3);
																	 LCD_Write_string("  TZ3_P",0, 4);
																	 LCD_Display_float(TZ3.P,11,4);
																	  LCD_Write_string("  RL_NUM",0, 5);
                                   LCD_Display_int(RL_NUM,11,5);
																	 LCD_Write_string("  TZ3_turn",0, 6);
																	 LCD_Display_int(TZ3.turn,15,6);
																	 LCD_Write_string("  Encoder",0, 7);
																	 LCD_Display_int(Encoder_TIM2*65536 + TIM2->CNT,10,7);
												 }break;
												 default :{
												 }break;
                 		   }
			 
		                 }break;                        //抛掷模式
		 case CONNECT_MODE:{ 
		                   LCD_Write_string("  connect_mode",0, 0);
			                 LCD_Display_int(place,15,0);
			                 LCD_Write_string("  TIM3_CCR1",0, 1);
			                 LCD_Display_int(TIM3_CCR1,15,1);
		                   }break;                      //交接模式
	 }
   
	 delay_ms(10);
  }
}

void create_display_task(uint8_t prio)
{
  OS_ERR  err;	
	OSTaskCreate((OS_TCB 	* )&DISPLAYTaskTCB,		
				 (CPU_CHAR	* )"DISPALY test task", 		
                 (OS_TASK_PTR )display_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )prio,     	
                 (CPU_STK   * )&DISPLAY_TASK_STK[0],	
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE/10,	
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				 
}

/************/