/*!*********************************************************************************************************************
@file user_app.c
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1Run(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u8 G_u8UserAppFlags;                             /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_<type>" and be declared as static.
***********************************************************************************************************************/


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------
void TimeXus(INPUT_PARAMETER_)
Sets Timer0 to count u16Microseconds_

Requires:
- Timer0 configured such that each timer tick is 1 microsecond
- INPUT_PARAMETER_ is the value in microseconds to time from 1 to 65535

Promises:
- Pre-loads TMR0H:L to clock out desired period
- TMR0IF cleared
- Timer0 enabled
*/
void TimeXus(u16 u16InputParameter)
{
    // Stop the timer
    T0CON0bits.EN = 0;

    // Subtract the input parameter from the max Timer0 value to find out...
    // ...what we need to set TMR0H/TMR0L to
    u16 u16TimerSet = 65536 - u16InputParameter;

    // Pre-set TMR0H/TMR0L by splitting up the 16 bit "u16TimerSet"...
    // ...into two 8 bit chunks
    TMR0H = u16TimerSet >> 8;
    TMR0L = u16TimerSet & 0x00FF;

    // Clear the TMR0IF bit in PIR3
    PIR3bits.TMR0IF = 0;

    // Start the timer
    T0CON0bits.EN = 1;

} /* end TimeXus () */


/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserAppInitialize(void)

@brief
Initializes the application's variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserAppInitialize(void)
{
    /* LED initialization */
    LATA = 0x80;

    /* Timer0 control register initialization to turn timer on, asynch...
       ...mode, 16 bit, Fosc/4, 1:16 prescaler, 1:1 postscaler */
    T0CON0 = 0b10010000;
    T0CON1 = 0b01010100;

} /* end UserAppInitialize() */


/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserAppRun(void)

@brief Application code that runs once per system loop

Requires:
-

Promises:
-

*/
void UserAppRun(void)
{
    /*

    // This is the block of code I used to test the 1 Hz blinking LED.
    // I've commented it out because it is not needed for the LED pattern.

    static u16 u16Counter = 0;

    u16Counter++;

    if (u16Counter >= 500)
    {
       u16Counter = 0;

       LATA = LATA ^ 0b00000001;
    }

    */


    //Initializations for the LED pattern index, length, and array
    static u8 u8PatternIndex = 0;
    static u8 u8PatternLength = 24;

    static u8 au8Pattern[] = {
        0b00000000,
        0b00000001,
        0b00000011,
        0b00000111,
        0b00001111,
        0b00011111,
        0b00111111,
        0b00111110,
        0b00111100,
        0b00111000,
        0b00110000,
        0b00100000,
        0b00000000,
        0b00100000,
        0b00110000,
        0b00111000,
        0b00111100,
        0b00111110,
        0b00111111,
        0b00011111,
        0b00001111,
        0b00000111,
        0b00000011,
        0b00000001,
    };

    // The delay in TimeXus is still set to 1 ms. "u16Counter" will count...
    // ...the number of milliseconds and switch the LED states every 250 ms
    static u16 u16Counter = 0;

    u16Counter++;

    if (u16Counter >= 250)
    {
        // Reset u16 Counter
        u16Counter = 0;

        // Set LATA to the next LED pattern in the array
        LATA = au8Pattern[u8PatternIndex];

        // Increment the array index for next time
        u8PatternIndex++;

        // Reset the array index if it goes past the length of the array
        if (u8PatternIndex == u8PatternLength)
        {
            u8PatternIndex = 0;
        }
    }

} /* end UserAppRun */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
