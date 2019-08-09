//Authors: Nicholas Daniel Jagdeo, Juan Sanchez, Kiyah Brooks, Joel Thomas, Thong Nguyen
//Function: Uses UART to send to COM a hex code when Button1/Button2 is pressed. Also sends Analog Stick Values when a Timer Interrupt happens.

//Change so Analog Stick doesnt send data if a button is pressed - and add 2 more button interrupts for LEFT AND RIGHT - FIND OUT SERIAL BAUD RATE FOR TIVAC
//----------------------------------------
// BIOS header files
//----------------------------------------
#include <xdc/std.h>                        //Mandatory for Bios
#include <ti/sysbios/BIOS.h>                //Mandatory for BIOS_start
#include <xdc/runtime/Log.h>                //For Log_info()
#include <xdc/cfg/global.h>                 //Header for Statically Defined Objects
#include <ti/sysbios/knl/task.h>            //Header for Tasks/Semaphore

// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "uart_library.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "string.h"
#include "inc/hw_ints.h"

//---------------------------------------------------
//                      global variables
uint32_t UART_BASE = UART0_BASE; // Can be switched to UART0_BASE for usb connection - CHANGED FROM UART3_BASE TO UART0_BASE

//---------------------------------------------------
//                          Swi Function
void toggleCalcSwi(){
    //determine analog stick and send a hex code as a string to USB UART
    UARTPutString(UART_BASE, "ANALOGXXXYYY");   //Analog Stick - NEED TO FINISH - Replace XXX and YYY with string concatentaion from ADC Values to hex - WHEN PUSHING DOWN AND LEFT IS NEGATIVE VALUES - WHEN PUSHING UP AND RIGHT SEND POSITIVE VALUES
}

void toggleCalcSwi2(){
    //determine analog stick click - MAYE ONLY NEED SWI1 DEPENDS ON ADC VALUE THE STICK SENDS
    UARTPutString(UART_BASE, "CLICK");   //STRING IS Click no hex code necessary
}
//-----------------------------------------------
//                  Interrupt Function
void Timer2AIntHandle(uint32_t UART_BASE){
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);     //Needs to be in ISR function - Interrupt seems to get stuck without
    //check for button press here?
    Swi_post(swi0);                                 //Posts to Swi
}

//-------------------------------------------------
//                  Button1 Interrupt
unsigned long i;
    void Pin_Int(void)
    {
    /*Clear interrupt register*/
    GPIOPinIntClear(GPIO_PORTA_BASE, GPIO_PIN_2);
    //Check for debouncing of switch
    for(i=0;i<=2000;i++)	//Might be a bit large - this is like a third of a second
        {
        }
    if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2)==GPIO_PIN_2)
        {
        UARTPutString(UART_BASE, "UP"); // Button 1
        }
    }
//------------------------------------------------
//                  Button2 Interrupt
unsigned long j;
    void Pin_Int2(void)
    {
    /*Clear interrupt register*/
    GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_2);   //may need to change pin/////////////
    //Check for debouncing of switch
    for(j=0;j<=2000;j++)	//same issue as above
        {
        }
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)==GPIO_PIN_2)
        {
        UARTPutString(UART_BASE, "DOWN"); // Button 2
        }
    }
//-------------------------------------------------
//                      Main Function
void main(void) {
//-------------------------------------------------
//                      Hardware Initialization
	uint32_t ui32Period = 0;
	uint32_t INT_TIMER2AEN = 0;

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	//ANALOG STICK STUFF HERE
	uint32_t ui32ADC0Value[8];//space for ADC values - Probably dont need all of this
//    // Enable the GPIO port that is used for the sensor port

      SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);                   //I DONT REMEMBER WHICH PINS THESE ARE ON THE BOARD WILL HAVE TO LOOK IT UP
      ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
      ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);   //will have to modify to figure out x and y
      ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH0);
      ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH0);
      ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH0);

      ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH1);   //possibly dont need this but am leaving it for the possibility that we need it for x and y
      ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH1);
      ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH1);
      ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
      ADCSequenceEnable(ADC0_BASE, 0);
      //BUTTON STUFF HERE
     /* Set the clock for the GPIO Port F */

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /* Set the type of the GPIO Pin of PORTF */
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 );
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);

    /*Register Interrupt to call Interrupt Handler*/
    GPIOPortIntRegister(GPIO_PORTA_BASE, Pin_Int);
    GPIOPortIntRegister(GPIO_PORTF_BASE, Pin_Int2);

    /*Clear interrupt register*/
    GPIOPinIntClear(GPIO_PORTA_BASE, GPIO_PIN_2);
    GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_2);

    /*Configure GPIO pad with internal pull-up enabled*/
    //GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

    /*Set interrupt triggering sequence*/
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_2,GPIO_FALLING_EDGE);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_2,GPIO_FALLING_EDGE);

    //UART STARTUP AND INTERRUPT STUFF HERE
	Configure_UART0();//calling function from UART_library to configure - CHANGED FROM UART3 TO UART0

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2); //Configure Timer
	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC); //32-bit timer, periodic

	ui32Period = (SysCtlClockGet() / 40);          //40[Hz] Target Freq - Could change this to get more accurate values,(if you are using counter then you need to change how capture is calculated)
	TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period-1);

	IntEnable(INT_TIMER2AEN);                             //Interrupt Enable
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);    //Timer 2A Interrupt Enable
	//
	/*Enable interrupts on selected pin - BUTTON STUFF*/
    GPIOPinIntEnable(GPIO_PORTA_BASE, GPIO_PIN_2);
    GPIOPinIntEnable(GPIO_PORTF_BASE, GPIO_PIN_2);

    /*Enable interrupts on selected port - BUTTON STUFF*/
    IntEnable(INT_GPIOA);
    IntEnable(INT_GPIOF);
    //
    /*Enable global interrupts*/
    IntMasterEnable();

	TimerEnable(TIMER2_BASE, TIMER_A);  //Enable Timer

	Swi_enable();                       //Enable Swi

//-------------------------------------------------------

	while (1){
	    //do ADC for analog stick
	 ADCIntClear(ADC0_BASE, 0);
     ADCProcessorTrigger(ADC0_BASE, 0);
     while(! ADCIntStatus(ADC0_BASE, 0, false))
     {
     }
     ADCSequenceDataGet(ADC0_BASE, 0, ui32ADC0Value);
    charge1 = (ui32ADC0Value[0]+ui32ADC0Value[1]+ui32ADC0Value[2]+ui32ADC0Value[3])/4;  //probs dont need
    charge2 = (ui32ADC0Value[4]+ui32ADC0Value[5]+ui32ADC0Value[6]+ui32ADC0Value[7])/4;  //probs dont need
	}

}
