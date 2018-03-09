/*******************************************************************************

                      ��Ȩ���� (C), 2017-, Mushiny
                      
 *******************************************************************************
  �� �� ��   : hand_task.c
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2017.11.5 ������
  ����޸�   :
  ��������   : �����ֱ�����
               ��ϵͳ���ʱ��Ҫע��:
               >>��������raw_printf()��task��һ��Ҫע��raw_printf����Ҫ��̬��������ģ�
                 ����task�������ջ����������Ҫ�����Ҫ128��256����
								ʹ����ʱ���ܴ��ٽ���
  �����б�   :
*******************************************************************************/
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "app.h"

/* �ڲ��Զ����������� --------------------------------------------------------*/
/* �ڲ��궨�� ----------------------------------------------------------------*/

/* ���������Ϣ����-----------------------------------------------------------*/
#define HAND_STK_SIZE		512					//�����ջ��С
CPU_STK HAND_TASK_STK[HAND_STK_SIZE];	
OS_TCB  hand_TaskTCB;							//������ƿ�

/*�ź�������---------------------------------------------------------------*/
   OS_SEM	Handle_Sem;
/* �ڲ���������---------------------------------------------------------------*/

/* �ⲿ�������� --------------------------------------------------------------*/
   HandlesStruct UART_CH;
/* �ⲿ����ԭ������ ----------------------------------------------------------*/
OS_ERR	HandleControlTaskErr	 = OS_ERR_NONE;
extern OS_TCB	THROWTaskTCB;
OS_ERR err;
extern THROW_INDEX TZ1; //TZ1����
extern THROW_INDEX TZ2; //TZ2����
extern THROW_INDEX TZ3; //TZ3����
extern int calibration_speed;
extern volatile char command_flag;
extern SERVO_MLCB UNDER1_MLCB;
extern SERVO_MLCB UNDER2_MLCB;
extern int RL_NUM;
/* �ڲ����� ------------------------------------------------------------------*/
int TIM3_CCR1=0;
/* �ڲ�����ԭ������ ----------------------------------------------------------*/

/* �������岿�� --------------------------------------------------------------*/

extern int printf_flag;
int accspeed=30;

static void hand_task(void *p_arg)
{ 	
	p_arg = p_arg;//�����������������д���Ƿ�ֹ��������Ϊδʹ�ñ��������о���	    
	while(1)
	{        
       
       CPU_SR_ALLOC();
	
	OSSemCreate(	(OS_SEM		*)&Handle_Sem,				//��Ҫ�������ź���(��Ҫ������)
								(CPU_CHAR	*)"Handle_Sem",				//�ź���������
								(OS_SEM_CTR)0,									//�ź����ĳ�ʼֵ
								(OS_ERR   *)&HandleControlTaskErr);		//���󷵻ش���	
		
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
	              TZ3.point=300;//200  1000δ���   200  1200��������״
                TZ3.turn=3; 
	while(1)
	{
		OSSemPend(	(OS_SEM	*)&Handle_Sem,						//������ź���
								(OS_TICK )0,											//�ȴ�ʱ��,0Ϊ���޵ȴ�,����Ϊ�ȴ���ȷ��ʱ�ӽ�����
								(OS_OPT  )OS_OPT_PEND_BLOCKING,		//��û���뵽�ź���,�����񱻹���
								(CPU_TS *)0,											//���ʱ���
								(OS_ERR *)&HandleControlTaskErr);	//�������
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
		                 }break;                       //����ģʽ
		 
										 
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
						            	}break;			//����ת��
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
									        }break;     //�����ͷ�λ�ã��ֱ���Ϊ10
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
								        	}break;    //�����ͷ�λ�ã��ֱ���Ϊ10
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
								        	}break;						 //�����ͷ�λ�ã��ֱ���Ϊ100
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
									       }break;			 //�����ͷ�λ�ã��ֱ���Ϊ100
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
								       	 }break;     //�����ͷ�λ�ã��ֱ���Ϊ1000
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
									       }break;        //�����ͷ�λ�ã��ֱ���Ϊ1000
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
									       }break;        //��������Ȧ��
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
									       }break;         //��������Ȧ��
			                  case 0x0B:{			Motor_Speed_Mode(&UNDER1_MLCB);delay_us(600);					            
					                              Motor_MastSpeed(&UNDER1_MLCB,600);delay_us(600);
				                               	Motor_AccelSpeed(&UNDER1_MLCB,accspeed);delay_us(600);
													              Motor_SetSpeed(&UNDER1_MLCB,TZ1.speed);           //���Ե�� һֱת��  11   													
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
                                     OSTaskResume(&THROWTaskTCB,&err);//��ʼ����	         										
									       }break;		
			                  default : {
									       }break;
		                  } 
		                 }break;                        //����ģʽ
		 
										 
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
		                   }break;                      //����ģʽ
	 }   	
		
		delay_ms(100);
	}	        
	}
}


void create_hand_task(uint8_t prio)
{
  OS_ERR  err;
	OSTaskCreate ((OS_TCB    *) &hand_TaskTCB,			//������ƿ�
								(CPU_CHAR  *)   "hand_task",			//��������,�������ܱ���������ʾ����
								(OS_TASK_PTR)    hand_task,				//������
								(void      *)    0,								//���뺯���Ĳ���
								(OS_PRIO    )    prio,          	//��������ȼ�
								(CPU_STK   *)   &HAND_TASK_STK[0],//�����ջ����ַ
								(CPU_STK_SIZE)   HAND_STK_SIZE/10,//�����ջ�������,���ڶ�ջ������
								(CPU_STK_SIZE)   HAND_STK_SIZE,		//�����ջ��С
								(OS_MSG_QTY  )   0,								//�����ڲ���Ϣ���н��յ������Ϣ��Ŀ,0���ǽ�ֹ��Ϣ����
								(OS_TICK     )   0,								//ʹ��ʱ��Ƭ��ת����ʱ�趨��ǰ�����ʱ��Ƭ����,0ΪĬ�ϳ���
								(void       *)   0,								//�û�����Ĵ洢��,һ��Ϊһ�����ݽṹ,������չ�����TCB,��:�Ÿ�����������
								(OS_OPT      )   OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����������ض�ѡ���������Ķ�ջ|��Ҫ��ջ����
								(OS_ERR     *)  &err);							//ָ��һ�������洢������
}

/************** ��Ȩ���� (C), 2017-, Mushiny **********���ļ�����*************/

