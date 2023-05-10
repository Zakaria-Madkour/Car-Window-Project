#include "DIO.h"


void DIO_Init(enum PORT port, int pin, enum DIRECTION dir){

    //Step 1 : clock gating
    Set_Bit(SYSCTL_RCGCGPIO_R, port);

    //Step 2 : wait for enable
    while( (SYSCTL_PRGPIO_R & (1U<<port)) == 0 ){}

    switch (port)
    {
    case PORT_A:

        //Step 3 : Deactivate Lock
        GPIO_PORTA_LOCK_R = 0x4C4F434B;

        //Step 4 : Allow changes for the needed pins in the commit register
        Set_Bit(GPIO_PORTA_CR_R, pin);

        //Step 5 : Setting the pin Direction
        if(dir){
            Set_Bit(GPIO_PORTA_DIR_R,pin);
        }
        else{
            Clear_Bit(GPIO_PORTA_DIR_R,pin);
            //Step 5.1 : In case of input we need pull up resistance
            Set_Bit(GPIO_PORTA_PUR_R,pin);
        }

        //Step 6 : Digital Enable
        Set_Bit(GPIO_PORTA_DEN_R,pin);

        break;
    

    case PORT_B:

        //Step 3 : Deactivate Lock
        GPIO_PORTB_LOCK_R = 0x4C4F434B;

        //Step 4 : Allow changes for the needed pins in the commit register
        Set_Bit(GPIO_PORTB_CR_R, pin);

        //Step 5 : Setting the pin Direction
        if(dir){
            Set_Bit(GPIO_PORTB_DIR_R,pin);
        }
        else{
            Clear_Bit(GPIO_PORTB_DIR_R,pin);
            //Step 5.1 : In case of input we need pull up resistance
            Set_Bit(GPIO_PORTB_PUR_R,pin);
        }

        //Step 6 : Digital Enable
        Set_Bit(GPIO_PORTB_DEN_R,pin);

        break;
    

    case PORT_C:

        //Step 3 : Deactivate Lock
        GPIO_PORTC_LOCK_R = 0x4C4F434B;

        //Step 4 : Allow changes for the needed pins in the commit register
        Set_Bit(GPIO_PORTC_CR_R, pin);

        //Step 5 : Setting the pin Direction
        if(dir){
            Set_Bit(GPIO_PORTC_DIR_R,pin);
        }
        else{
            Clear_Bit(GPIO_PORTC_DIR_R,pin);
            //Step 5.1 : In case of input we need pull up resistance
            Set_Bit(GPIO_PORTC_PUR_R,pin);
        }

        //Step 6 : Digital Enable
        Set_Bit(GPIO_PORTC_DEN_R,pin);

        break;
    

    case PORT_D:

    //Step 3 : Deactivate Lock
        GPIO_PORTD_LOCK_R = 0x4C4F434B;

        //Step 4 : Allow changes for the needed pins in the commit register
        Set_Bit(GPIO_PORTD_CR_R, pin);

        //Step 5 : Setting the pin Direction
        if(dir){
            Set_Bit(GPIO_PORTD_DIR_R,pin);
        }
        else{
            Clear_Bit(GPIO_PORTD_DIR_R,pin);
            //Step 5.1 : In case of input we need pull up resistance
            Set_Bit(GPIO_PORTD_PUR_R,pin);
        }

        //Step 6 : Digital Enable
        Set_Bit(GPIO_PORTD_DEN_R,pin);

        break;


    case PORT_E:

        //Step 3 : Deactivate Lock
        GPIO_PORTE_LOCK_R = 0x4C4F434B;

        //Step 4 : Allow changes for the needed pins in the commit register
        Set_Bit(GPIO_PORTE_CR_R, pin);

        //Step 5 : Setting the pin Direction
        if(dir){
            Set_Bit(GPIO_PORTE_DIR_R,pin);
        }
        else{
            Clear_Bit(GPIO_PORTE_DIR_R,pin);
            //Step 5.1 : In case of input we need pull up resistance
            Set_Bit(GPIO_PORTE_PUR_R,pin);
        }

        //Step 6 : Digital Enable
        Set_Bit(GPIO_PORTE_DEN_R,pin);

        break;
    

    case PORT_F:

        //Step 3 : Deactivate Lock
        GPIO_PORTF_LOCK_R = 0x4C4F434B;

        //Step 4 : Allow changes for the needed pins in the commit register
        Set_Bit(GPIO_PORTF_CR_R, pin);

        //Step 5 : Setting the pin Direction
        if(dir){
            Set_Bit(GPIO_PORTF_DIR_R,pin);
        }
        else{
            Clear_Bit(GPIO_PORTF_DIR_R,pin);
            //Step 5.1 : In case of input we need pull up resistance
            Set_Bit(GPIO_PORTF_PUR_R,pin);
        }

        //Step 6 : Digital Enable
        Set_Bit(GPIO_PORTF_DEN_R,pin);

        break;

    default:
        break;
    }

}



void DIO_WritePin( enum PORT port, int pin, int value){
    switch (port)
    {
    case PORT_A:
        if(value){
            Set_Bit(GPIO_PORTA_DATA_R , pin);
        }
        else{
            Clear_Bit(GPIO_PORTA_DATA_R , pin);
        }
        break;

    case PORT_B:
        if(value){
            Set_Bit(GPIO_PORTB_DATA_R , pin);
        }
        else{
            Clear_Bit(GPIO_PORTB_DATA_R , pin);
        }
        break;

    case PORT_C:
        if(value){
            Set_Bit(GPIO_PORTC_DATA_R , pin);
        }
        else{
            Clear_Bit(GPIO_PORTC_DATA_R , pin);
        }
        break;

    case PORT_D:
        if(value){
            Set_Bit(GPIO_PORTD_DATA_R , pin);
        }
        else{
            Clear_Bit(GPIO_PORTD_DATA_R , pin);
        }
        break;

    case PORT_E:
        if(value){
            Set_Bit(GPIO_PORTE_DATA_R , pin);
        }
        else{
            Clear_Bit(GPIO_PORTE_DATA_R , pin);
        }
        break;

    case PORT_F:
        if(value){
            Set_Bit(GPIO_PORTF_DATA_R , pin);
        }
        else{
            Clear_Bit(GPIO_PORTF_DATA_R , pin);
        }
        break;

    default:
        break;
    }
}


void DIO_WritePort(enum PORT port, int32 value){
    switch (port)
    {
    case PORT_A:
        GPIO_PORTA_DATA_R = value;
        break;
    case PORT_B:
        GPIO_PORTB_DATA_R = value;
        break;

    case PORT_C:
        GPIO_PORTC_DATA_R = value;
        break;

    case PORT_D:
        GPIO_PORTD_DATA_R = value;
        break;

    case PORT_E:
        GPIO_PORTE_DATA_R = value;
        break;

    case PORT_F:
        GPIO_PORTF_DATA_R = value;
        break;

    default:
        break;
    }
}


int32 DIO_ReadPin(enum PORT port, int pin){
    switch (port)
    {
    case PORT_A:
        return Get_Bit(GPIO_PORTA_DATA_R,pin);
        break;
    case PORT_B:
        return Get_Bit(GPIO_PORTB_DATA_R,pin);
        break;

    case PORT_C:
        return Get_Bit(GPIO_PORTC_DATA_R,pin);
        break;

    case PORT_D:
        return Get_Bit(GPIO_PORTD_DATA_R,pin);
        break;

    case PORT_E:
        return Get_Bit(GPIO_PORTE_DATA_R,pin);
        break;

    case PORT_F:
        return Get_Bit(GPIO_PORTF_DATA_R,pin);
        break;

    default:
        return 0;
        break;
    }
}


int32 DIO_ReadPort(enum PORT port){
    switch (port)
    {
    case PORT_A:
        return GPIO_PORTA_DATA_R;
        break;

    case PORT_B:
        return GPIO_PORTB_DATA_R;
        break;

    case PORT_C:
        return GPIO_PORTC_DATA_R;
        break;

    case PORT_D:
        return GPIO_PORTD_DATA_R;
        break;

    case PORT_E:
        return GPIO_PORTE_DATA_R;
        break;

    case PORT_F:
        return GPIO_PORTF_DATA_R;
        break;
        
    default:
        break;
    }
}