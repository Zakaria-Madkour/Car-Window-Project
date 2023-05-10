#ifndef BOARDPORTING_H
#define BOARDPORTING_H

# include "DIO.h"
# include "tm4c123gh6pm.h"
# include "stdbool.h"
# include "TM4C123.h"                    // Device header
# include "States.h"


//-------------------------- PORTD ----------------------------------------
# define DRIVERUPLIMIT         6
# define DRIVERDOWNLIMIT       7
# define PASSENGERUPLIMIT      2
# define PASSENGERDOWNLIMIT    3

//-------------------------- PORTA ----------------------------------------
# define DRIVERUPBUTTON        2
# define DRIVERDOWNBUTTON      3
# define ON_OFFSWITCH          5

//-------------------------- PORTE ----------------------------------------
# define PASSENGERUP_DP        1
# define PASSENGERDOWN_DP      2
# define PASSENGERUP_PP        4
# define PASSENGERDOWN_PP      5

//-------------------------- PORTC ----------------------------------------
# define DRIVERJAM             4
# define PASSENGERJAM          5




// -------------------- Interface Functions -------------------------------
void BOARD_init();//Initialize the whole Board --> recommended 

void PORTD_init();

void PORTA_init();
bool driver_up_pressed();
bool driver_down_pressed();

void PORTE_init();
bool passenger_up_pressed();
bool passenger_down_pressed();

void PORTC_init();

//---------------------- Internal Functions -------------------------------
void off_switch();
void on_switch();


#endif