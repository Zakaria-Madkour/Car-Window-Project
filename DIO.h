#ifndef DIO_H
#define DIO_H

#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "DataTypes.h"


enum PORT { PORT_A = 0,
            PORT_B = 1,
            PORT_C = 2,
            PORT_D = 3,
            PORT_E = 4,
            PORT_F = 5} ;

enum DIRECTION { IN  = 0,
                 OUT = 1};

void DIO_Init(enum PORT port, int pin, enum DIRECTION dir);
void DIO_WritePin( enum PORT port, int pin, int value);
void DIO_WritePort(enum PORT port, int32 value);

int32 DIO_ReadPin(enum PORT port, int pin);
int32 DIO_ReadPort(enum PORT port);
								 
#endif