#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

# include "tm4c123gh6pm.h"
# include "DIO.h"

# define PORT_USED PORT_B
# define MOTOR1_UP 0
# define MOTOR1_DOWN 1
# define MOTOR2_UP 2
# define MOTOR2_DOWN 3

//-------------- Interface Functions-----------------------
void motor_module_init();

void motor1_up();
void motor1_down();
void motor1_off();

void motor2_up();
void motor2_down();
void motor2_off();
//---------------------------------------------------------


void port_init();
void config_PWM_MOTOR();
void test_motor();

#endif