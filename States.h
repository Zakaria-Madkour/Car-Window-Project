#ifndef STATES_H
#define STATES_H

# include "FreeRTOS.h"
# include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core


typedef enum { FULLY_UP,
							 FULLY_DOWN,
							 IN_BETWEEN,
				  	 	 MOVING_UP,
							 MOVING_DOWN,
							 AUTOMATIC_UP,
							 AUTOMATIC_DOWN}STATE;

typedef enum { DRIVER_UP,
							 DRIVER_DOWN,
							 DRIVER_STOP,
							 DRIVER_STOP_UP,
							 DRIVER_STOP_DOWN,
							 PASSENGER_UP,
							 PASSENGER_DOWN,
							 PASSENGER_STOP,
							 PASSENGER_STOP_UP,
							 PASSENGER_STOP_DOWN}ACTION ;

							 
extern STATE driver_state;
extern STATE passenger_state;						 
//--------------------- Semaphores ------------------------						 
extern xSemaphoreHandle driver_semaphore;
extern xSemaphoreHandle passenger_semaphore;
extern xSemaphoreHandle limit_semaphore;
extern xSemaphoreHandle jam_semaphore;							 

//--------------------- Queues ----------------------------							 
extern xQueueHandle driver_queue;
extern xQueueHandle passenger_queue;	
extern xQueueHandle limit_queue;							 
extern xQueueHandle jam_queue;
							 
//--------------------- Mutex Locks -----------------------						 
extern xSemaphoreHandle driver_motor_mutex;
extern xSemaphoreHandle passenger_motor_mutex;	

extern xSemaphoreHandle driver_state_mutex;
extern xSemaphoreHandle passenger_state_mutex;		




//--------------------- Prototypes -----------------------	
void create_queues();
void create_semaphores();
void create_mutex();
#endif