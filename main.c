# include "FreeRTOS.h"
# include "task.h"
# include "FreeRTOSConfig.h"

# include "MotorDriver.h"	
# include "BoardPorting.h"
# include "States.h"

//--------------------- Semaphores ------------------------						 
extern xSemaphoreHandle driver_semaphore = NULL;
extern xSemaphoreHandle passenger_semaphore = NULL;
extern xSemaphoreHandle limit_semaphore = NULL;
extern xSemaphoreHandle jam_semaphore = NULL;							 

//--------------------- Queues ----------------------------							 
extern xQueueHandle driver_queue = NULL;
extern xQueueHandle passenger_queue = NULL;	
extern xQueueHandle limit_queue = NULL;							 
extern xQueueHandle jam_queue = NULL;
							 
//--------------------- Mutex Locks -----------------------						 
extern xSemaphoreHandle driver_motor_mutex = NULL;
extern xSemaphoreHandle passenger_motor_mutex = NULL;	

extern xSemaphoreHandle driver_state_mutex = NULL;
extern xSemaphoreHandle passenger_state_mutex = NULL;		



int main(){
	// Create the required semaphores, queues, locks  ---> All the interrupts must be disabled until queues and semaphores are created	
	
	/*create_queues();
	create_semaphores();
	create_mutex();
	// Create the required tasks
	*/
	
	// Initialization of the used ports
	/* Note-> Board initialization must come at last because 
	if a port throws an interrupt the semaphores and queues must be already created
	*/
	
	motor_module_init();
	BOARD_init();
	
	// Start scheduler
	
	
	// If the program is working correctly this part wont be reached
	while(1){
		__asm(" nop");
	}
	return 0;
}
