/*******************************************************************************

                      版权所有 (C), 2017-, Mushiny
                      
 *******************************************************************************
  文 件 名   : hand_task.c
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2017.11.5 星期六
  最近修改   :
  功能描述   : 无线手柄任务
               本系统编程时需要注意:
               >>凡是用了raw_printf()的task，一定要注意raw_printf是需要动态变量数组的，
                 所以task的任务堆栈必须能满足要求，最好要128或256以上
								使用延时不能打开临界区
  函数列表   :
*******************************************************************************/
/* 包含头文件 ----------------------------------------------------------------*/
#include "app.h"

/* 内部自定义数据类型 --------------------------------------------------------*/
/* 内部宏定义 ----------------------------------------------------------------*/

/* 任务相关信息定义-----------------------------------------------------------*/
#define HAND_STK_SIZE		512					//任务堆栈大小
CPU_STK HAND_TASK_STK[HAND_STK_SIZE];	
OS_TCB  hand_TaskTCB;							//任务控制块

/*信号量申明---------------------------------------------------------------*/
   OS_SEM	Handle_Sem;
/* 内部常量定义---------------------------------------------------------------*/

/* 外部变量声明 --------------------------------------------------------------*/
   HandlesStruct UART_CH;
/* 外部函数原型声明 ----------------------------------------------------------*/
OS_ERR	HandleControlTaskErr	 = OS_ERR_NONE;
extern OS_TCB	THROWTaskTCB;
OS_ERR err;
extern THROW_INDEX TZ1; //TZ1参数
extern THROW_INDEX TZ2; //TZ2参数
extern THROW_INDEX TZ3; //TZ3参数
extern int calibration_speed;
extern volatile char command_flag;
extern SERVO_MLCB UNDER1_MLCB;
extern SERVO_MLCB UNDER2_MLCB;
extern int RL_NUM;
/* 内部变量 ------------------------------------------------------------------*/
int TIM3_CCR1=0;
/* 内部函数原型声明 ----------------------------------------------------------*/

/* 任务主体部分 --------------------------------------------------------------*/

extern int printf_flag;
int accspeed=30;

static void hand_task(void *p_arg)
{ 	
	p_arg = p_arg;//任务函数输入参数，此写法是防止编译器因为未使用变量而进行警告	    
	while(1)
	{        
       
       CPU_SR_ALLOC();
	
	OSSemCreate(	(OS_SEM		*)&Handle_Sem,				//需要创建的信号量(需要先声明)
								(CPU_CHAR	*)"Handle_Sem",				//信号量的名字
								(OS_SEM_CTR)0,									//信号量的初始值
								(OS_ERR   *)&HandleControlTaskErr);		//错误返回代码	
		
		            TZ1.P=0.000011f;
	              TZ1.D=0.004f;
	              TZ1.control_num=0;
	              TZ1.speed=190;
	              TZ1.point=100;
		            TZ2.turn=2; 
  
                TZ2.P=0.000011f;
	              TZ2.D=0.004f;
	              TZ2.control_num=0;
	              TZ2.speed=180;
	              TZ2.point=500;
                TZ2.turn=3; 

                TZ3.P=0.000011f;
	              TZ3.D=0.004f;
	              TZ3.control_num=0;
	              TZ3.speed=200;
	              TZ3.point=300;//200  1000未打结   200  1200打结辫子形状
                TZ3.turn=3; 
	while(1)
	{
		OSSemPend(	(OS_SEM	*)&Handle_Sem,						//申请的信号量
								(OS_TICK )0,											//等待时间,0为无限等待,其他为等待的确切时钟节拍数
								(OS_OPT  )OS_OPT_PEND_BLOCKING,		//若没申请到信号量,则任务被挂起
								(CPU_TS *)0,											//获得时间戳
								(OS_ERR *)&HandleControlTaskErr);	//错误代码
	 switch(Robot_Mode)
	 {
		 case MATCH_MODE:{
	                      switch(UART_CH.KEY)
		                  {
 			                  case 0x01:{	
                         command_flag=_delay;BEED_OFF;													
						            	}break;			
			                  case 0x02:{	
                         command_flag=_calibration;calibration_speed=0;																		
						        			}break;
			                  case 0x03:{ 
                         command_flag=_connect;															
									        }break;
			                  case 0x04:{		
                         command_flag=_throw;															
								        	}break;
			                  case 0x05:{   
                         command_flag=_ready_connect;
								        	}break;						
		                  	case 0x06:{
												 command_flag=_ready_throw;											
									       }break;			
			                  case 0x07:{
												 command_flag=_ready_location_one;TIM_Cmd(TIM7, ENABLE);
								       	 }break;
			                  case 0x08:{		
                           command_flag=_ready_location_two;  													
									       }break;
												
												 
			                  case 0x09:{
													
													
									       }break;												
			                  case 0x0A:{
													    switch(place)
															{case ONE:{TZ1.turn=2;}break;
															case TWO:{TZ2.turn+=1;}break;
															case THREE:{TZ3.turn+=1;}break;
															default:{}
															}	
									       }break;
			                  case 0x0B:{
															switch(place)
															{
															case ONE:{RL1_ON;}break;
															case TWO:{RL1_ON;}break;
															case THREE:{RL1_ON;}break;
															default:{}
															}							
									       }break;
			                  case 0x0C:{
                              switch(place)
															{
															case ONE:{RL1_OFF;}break;
															case TWO:{RL1_OFF;}break;
															case THREE:{RL1_OFF;}break;
															default:{}
															}																							
									       }break;
			                 case 0x0D:{
                              switch(place)
															{case ONE:{TZ1.point+=100;}break;
															case TWO:{TZ2.point+=100;}break;
															case THREE:{TZ3.point+=100;}break;
															default:{}
															}																	 										 
									       }break;
			                  case 0x0E:{ 
                              switch(place)
															{case ONE:{TZ1.point-=100;}break;
															case TWO:{TZ2.point-=100;}break;
															case THREE:{TZ3.point-=100;}break;
															default:{}
															}																										
									       }break;
			                  case 0x0F:{
                              switch(place)
															{case ONE:{TZ1.speed-=10;}break;
															case TWO:{TZ2.speed+=10;}break;
															case THREE:{TZ3.speed+=10;}break;
															default:{}
															}													
									       }break;		
			                  case 0x10:{	
                          Send_underpan(0x21);													
									       }break;
			                  case 0x11:{	
                          Send_underpan(0x22);													
									       }break;
			                  case 0x12:{
                          													
									       }break;
			                  case 0x13:{
//                          Send_underpan(0x22);													
									       }break;
			                  case 0x14:{
                          RL1_ON;RL_NUM++;													
									       }break;		
			                  default : {
									       }break;
		                  } 		                 
		                 }break;                       //比赛模式
		 
										 
		 case THROW_MODE:{
                     switch(UART_CH.KEY)
		                  {
 			                  case 0x01:{
                                   switch(place)
																	 {
																		 case ONE:{
																			         if(TZ1.speed<-180){TZ1.speed+=1;}
																			         else{TZ1.speed+=10;}
																		 }break;
																		 case TWO:{
																			         TZ2.speed+=10;
																		 }break;
																		 case THREE:{
																			         TZ3.speed+=10;
																		 }
																		 break;
																		 default:{
																		 }
																	 }																		 
						            	}break;			//调整转速
			                  case 0x02:{
													switch(place)
																	 {
																		  case ONE:{
																			        if(TZ1.speed<-180){TZ1.speed-=1;}
																			         else{TZ1.speed-=10;}
																		 }break;
																		 case TWO:{
																			         TZ2.speed-=10;
																		 }break;
																		 case THREE:{
																			         TZ3.speed-=10;
																		 }
																		 break;
																		 default:{
																		 }
																	 }		
						        			}break;
			                  case 0x03:{
													switch(place)
																	 {
																		 case ONE:{
																			          TZ1.point+=10;
																		 }break;
																		 case TWO:{
																			          TZ2.point+=10;
																		 }break;
																		 case THREE:{
																			          TZ3.point+=10;
																		 }
																		
																		 break;
																		 default:{
																		 }
																	 }		
									        }break;     //调整释放位置，分辨率为10
			                  case 0x04:{
													switch(place)
																	 {
																		 case ONE:{
																			        TZ1.point-=10;
																		 }break;
																		 case TWO:{
																			          TZ2.point-=10;
																		 }break;
																		 case THREE:{
																			          TZ3.point-=10;
																		 }break;
																		 default:{
																		 }
																	 }		
								        	}break;    //调整释放位置，分辨率为10
			                  case 0x05:{ 
													switch(place)
																	{
																		 case ONE:{
																			          TZ1.point+=100;
																		 }break;
																		 case TWO:{
																			          TZ2.point+=100;
																		 }break;
																		 case THREE:{
																			          TZ3.point+=100;
																		 }break;
																		 default:{
																		 }
																	 }		
								        	}break;						 //调整释放位置，分辨率为100
		                  	case 0x06:{
													switch(place)
																	 {
																		 case ONE:{
																			          TZ1.point-=100;
																		 }break;
																		 case TWO:{
																			          TZ2.point-=100;
																		 }break;
																		 case THREE:{
																			          TZ3.point-=100;
																		 }break;
																		 default:{
																		 }
																	 }		
									       }break;			 //调整释放位置，分辨率为100
			                  case 0x07:{
													       switch(place)
																	 {
																		 case ONE:{
																			          TZ1.point+=1000;
																		 }break;
																		 case TWO:{
																			          TZ2.point+=1000;
																		 }break;
																		 case THREE:{
																			          TZ3.point+=1000;
																		 }break;
																		 default:{
																		 }
																	 }											             		
								       	 }break;     //调整释放位置，分辨率为1000
			                  case 0x08:{	
                                	switch(place)
																	 {
																		 case ONE:{
																			          TZ1.point-=1000;
																		 }break;
																		 case TWO:{
																			          TZ2.point-=1000;
																		 }break;
																		 case THREE:{
																			          TZ3.point-=1000;
																		 }break;
																		 default:{
																		 }
																	 }														
									       }break;        //调整释放位置，分辨率为1000
			                  case 0x09:{
													switch(place)
																	 {
																		 case ONE:{
																			          TZ1.turn+=1;
																		 }break;
																		 case TWO:{
																			          TZ2.speed+=1;
																		 }break;
																		 case THREE:{
																			          TZ3.turn+=1;
																		 }break;
																		 default:{
																		 }
																	 }		
									       }break;        //调整加速圈数
			                  case 0x0A:{
                            switch(place)
																	 {
																		 case ONE:{
																			          TZ1.turn-=1;
																		 }break;
																		 case TWO:{
																			          TZ2.speed -=1;
																		 }break;
																		 case THREE:{
																			          TZ3.turn-=1;
																		 }break;
																		 default:{
																		 }
																	 }															
									       }break;         //调整加速圈数
			                  case 0x0B:{			Motor_Speed_Mode(&UNDER1_MLCB);delay_us(600);					            
					                              Motor_MastSpeed(&UNDER1_MLCB,600);delay_us(600);
				                               	Motor_AccelSpeed(&UNDER1_MLCB,accspeed);delay_us(600);
													              Motor_SetSpeed(&UNDER1_MLCB,TZ1.speed);           //调试电机 一直转动  11   													
									                      TIM_Cmd(TIM3, ENABLE);
												}break;
			                  case 0x0C:{		
													          TIM_Cmd(TIM7, ENABLE); 
												           	TIM_Cmd(TIM3, DISABLE);
									       }break;
			                  case 0x0D:{	RL1_ON;RL_NUM++;	
													
									       }break;
			                  case 0x0E:{	RL1_OFF;									
									       }break;
			                  case 0x0F:{		TZ1.speed=-300;				//15														
									       }break;		
			                  case 0x10:{																						
									       }break;
			                  case 0x11:{				                 																		
									       }break;
			                  case 0x12:{																					
									       }break;
			                  case 0x13:{											
									       }break;
			                  case 0x14:{	
													           TIM_Cmd(TIM3, ENABLE);
//													           printf_flag=1;
                                     OSTaskResume(&THROWTaskTCB,&err);//开始抛掷	         										
									       }break;		
			                  default : {
									       }break;
		                  } 
		                 }break;                        //抛掷模式
		 
										 
		 case CONNECT_MODE:{ 
		                     switch(UART_CH.KEY)
		                  {
 			                  case 0x01:{TIM3_CCR1+=10;				                      		
						            	}break;			
			                  case 0x02:{TIM3_CCR1-=10;			
						        			}break;
			                  case 0x03:{
													 TIM3_CCR1+=100;
                           												
									        }break;
			                  case 0x04:{
                           TIM3_CCR1-=100;													
								        	}break;
			                  case 0x05:{
                           TIM_PWM_Config(TIM3, TIM3_CCR1,430, 425, 510);														
								        	}break;						
		                  	case 0x06:{
									       }break;			
			                  case 0x07:{
								       	 }break;
			                  case 0x08:{				              
									       }break;
			                  case 0x09:{
									       }break;
			                  case 0x0A:{																												
									       }break;
			                  case 0x0B:{											
									       }break;
			                  case 0x0C:{											
									       }break;
			                 case 0x0D:{																							
									       }break;
			                  case 0x0E:{												
									       }break;
			                  case 0x0F:{																				
									       }break;		
			                  case 0x10:{																						
									       }break;
			                  case 0x11:{				                 																		
									       }break;
			                  case 0x12:{																					
									       }break;
			                  case 0x13:{											
									       }break;
			                  case 0x14:{		
									       }break;		
			                  default : {
									       }break;
		                  } 
		                   }break;                      //交接模式
	 }   	
		
		delay_ms(100);
	}	        
	}
}


void create_hand_task(uint8_t prio)
{
  OS_ERR  err;
	OSTaskCreate ((OS_TCB    *) &hand_TaskTCB,			//任务控制块
								(CPU_CHAR  *)   "hand_task",			//任务名称,该名字能被调试器显示出来
								(OS_TASK_PTR)    hand_task,				//任务函数
								(void      *)    0,								//传入函数的参数
								(OS_PRIO    )    prio,          	//任务的优先级
								(CPU_STK   *)   &HAND_TASK_STK[0],//任务堆栈基地址
								(CPU_STK_SIZE)   HAND_STK_SIZE/10,//任务堆栈深度限制,用于堆栈溢出检测
								(CPU_STK_SIZE)   HAND_STK_SIZE,		//任务堆栈大小
								(OS_MSG_QTY  )   0,								//任务内部消息队列接收的最大消息数目,0则是禁止消息队列
								(OS_TICK     )   0,								//使能时间片轮转调度时设定当前任务的时间片长度,0为默认长度
								(void       *)   0,								//用户补充的存储区,一般为一个数据结构,用来扩展任务的TCB,如:放浮点运算内容
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //包含任务的特定选项：检测该任务的堆栈|需要堆栈清零
								(OS_ERR     *)  &err);							//指定一个变量存储错误码
}

/************** 版权所有 (C), 2017-, Mushiny **********本文件结束*************/

