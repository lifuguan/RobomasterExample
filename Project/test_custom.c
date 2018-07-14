#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "pid.h"
#include "sys.h"

//left为左边摇杆
//right为右边摇杆


int16_t test_moto_speed_right = 0;
int16_t test_moto_speed_left = 0;
int16_t test_moto_current[2];
uint8_t test_servo;
uint8_t key1;

void moto_control(void)
{
	//LED控制函数
	write_led_io(LED_IO1, LED_ON);	
	
	//电机的速度给定
	test_moto_speed_right = rc.ch2 / RC_MAX_VALUE * MAX_WHEEL_RPM;
	test_moto_speed_left = rc.ch4 / RC_MAX_VALUE * MAX_WHEEL_RPM;
	
	
	//闭环计算电机电流
	test_moto_current[0] = pid_calc(&pid_wheel_spd[0], moto_chassis[0].speed_rpm, test_moto_speed_right);
	test_moto_current[1] = pid_calc(&pid_wheel_spd[1], moto_chassis[1].speed_rpm, test_moto_speed_left);
	
	//发送电机的电流
	set_test_motor_current_left(test_moto_current);
	
	
	
	//舵机控制函数周期设定
	set_pwm_group_param(PWM_GROUP1, 20000);
	
	//开启控制端口
	start_pwm_output(PWM_IO1);
    
	//舵机控制命令
		if(test_servo == 0)
	{
		set_pwm_param(PWM_IO1, 2200);
	}
	else
	{
		set_pwm_param(PWM_IO1, 1500);
	}
   

	//  扩展板的按键控制电机
	//      read_key_io(KEY_IO1,&key1);  
	//			if(key1 ==1)
	//		{
	//			 set_test_motor_current(test_moto_current);
	//			}
			
			
}
//电机初始化参数设定
void test_moto_init(void)
{
	//PID参数初始化		 
	pid_init(&pid_test_moto, 7000, 0, 1, 0, 0);

}
void left_moto_init(void)
{
	//PID参数初始化		 
	pid_init(&pid_wheel_spd[0], 7000, 0, 1, 0, 0);
}
void right_moto_init(void)
{
	//PID参数初始化		 
	pid_init(&pid_wheel_spd[1], 7000, 0, 1, 0, 0);
}
