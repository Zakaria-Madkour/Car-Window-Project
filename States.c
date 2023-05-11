# include "States.h"

void create_queues(){
	limit_queue = xQueueCreate( 1, sizeof( ACTION ) );
	while(limit_queue == NULL); // Cant create queue increase heap size
	
	jam_queue = xQueueCreate( 5, sizeof( ACTION ) );
	while(jam_queue == NULL); // Cant create queue increase heap size
	
	driver_queue  = xQueueCreate( 2, sizeof( ACTION ) );
	while(driver_queue == NULL); // Cant create queue increase heap size
	
	passenger_queue  = xQueueCreate( 2, sizeof( ACTION ) );
	while(passenger_queue == NULL); // Cant create queue increase heap size
	
}

void create_semaphores(){
	limit_semaphore = xSemaphoreCreateCounting( 1, 0 );
	while(limit_semaphore == NULL); // Cant create semaphore increase heap size
	
	jam_semaphore = xSemaphoreCreateCounting( 5, 0 );
	while(jam_semaphore == NULL); // Cant create semaphore increase heap size
	
	driver_semaphore = xSemaphoreCreateCounting( 2, 0 );
	while(driver_semaphore == NULL); // Cant create semaphore increase heap size
	
	passenger_semaphore  = xSemaphoreCreateCounting( 2, 0 );
	while(passenger_semaphore == NULL); // Cant create semaphore increase heap size
}

void create_mutex(){
	driver_motor_mutex = xSemaphoreCreateMutex();
	while(driver_motor_mutex == NULL); // Cant create mutex increase heap size
	
	passenger_motor_mutex = xSemaphoreCreateMutex();
	while(passenger_motor_mutex == NULL); // Cant create mutex increase heap size
	
	driver_state_mutex = xSemaphoreCreateMutex();
	while(driver_state_mutex == NULL); // Cant create mutex increase heap size
	
	passenger_state_mutex = xSemaphoreCreateMutex();
	while(passenger_state_mutex == NULL); // Cant create mutex increase heap size
	
}