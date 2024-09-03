/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include "r_cg_userdefine.h"
#include "r_smc_entry.h"

#include "misc_config.h"
#include "custom_func.h"

#include "button_sw_debounce.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_16bit flag_BTN_CTL;
#define FLAG_BTN_1_SHORT_PRESSED                 	    (flag_BTN_CTL.bit0)
#define FLAG_BTN_2_SHORT_PRESSED                 	    (flag_BTN_CTL.bit1)
#define FLAG_BTN_3_SHORT_PRESSED                 	    (flag_BTN_CTL.bit2)
#define FLAG_BTN_4_SHORT_PRESSED                 	    (flag_BTN_CTL.bit3)
#define FLAG_BTN_5_SHORT_PRESSED                 	    (flag_BTN_CTL.bit4)
#define FLAG_BTN_6_SHORT_PRESSED                 	    (flag_BTN_CTL.bit5)
#define FLAG_BTN_7_SHORT_PRESSED                 	    (flag_BTN_CTL.bit6)
#define FLAG_BTN_8_SHORT_PRESSED                 	    (flag_BTN_CTL.bit7)
#define FLAG_BTN_1_LONG_PRESSED                 	    (flag_BTN_CTL.bit8)
#define FLAG_BTN_2_LONG_PRESSED                 	    (flag_BTN_CTL.bit9)
#define FLAG_BTN_3_LONG_PRESSED                 	    (flag_BTN_CTL.bit10)
#define FLAG_BTN_4_LONG_PRESSED                 	    (flag_BTN_CTL.bit11)
#define FLAG_BTN_5_LONG_PRESSED                 	    (flag_BTN_CTL.bit12)
#define FLAG_BTN_6_LONG_PRESSED                 	    (flag_BTN_CTL.bit13)
#define FLAG_BTN_7_LONG_PRESSED                 	    (flag_BTN_CTL.bit14)
#define FLAG_BTN_8_LONG_PRESSED                 	    (flag_BTN_CTL.bit15)


/*_____ D E F I N I T I O N S ______________________________________________*/

/*
    interval : 10 ms
    shift 8 times = 10 ms*8

    target :
    8*2 = 16 ms
    8*4 = 32 ms
    8*6 = 48 ms
    8*8 = 64 ms
    8*10 = 80 ms
*/

#define USE_DEBOUNCE_16MS
// #define USE_DEBOUNCE_32MS
// #define USE_DEBOUNCE_48MS
// #define USE_DEBOUNCE_64MS
// #define USE_DEBOUNCE_80MS

#if defined (USE_DEBOUNCE_16MS)
#define DEBOUNCE_INTERVAL_TIME                          (2)  
#define DEBOUNCE_SHORT_TIME                             (DEBOUNCE_INTERVAL_TIME*8)      //Press and release button settling time ( ms )

#elif defined (USE_DEBOUNCE_32MS)
#define DEBOUNCE_INTERVAL_TIME                          (4)  
#define DEBOUNCE_SHORT_TIME                             (DEBOUNCE_INTERVAL_TIME*8)      //Press and release button settling time ( ms )

#elif defined (USE_DEBOUNCE_48MS)
#define DEBOUNCE_INTERVAL_TIME                          (6)  
#define DEBOUNCE_SHORT_TIME                             (DEBOUNCE_INTERVAL_TIME*8)      //Press and release button settling time ( ms )

#elif defined (USE_DEBOUNCE_64MS)
#define DEBOUNCE_INTERVAL_TIME                          (8)  
#define DEBOUNCE_SHORT_TIME                             (DEBOUNCE_INTERVAL_TIME*8)      //Press and release button settling time ( ms )

#elif defined (USE_DEBOUNCE_80MS)
#define DEBOUNCE_INTERVAL_TIME                          (10)  
#define DEBOUNCE_SHORT_TIME                             (DEBOUNCE_INTERVAL_TIME*8)      //Press and release button settling time ( ms )

#endif

#define DEBOUNCE_LONG_TIME                              (1500)                          //Press and release button settling time ( ms )
#define DEBOUNCE_SHIFT_BITS                             (0xFF)

typedef struct BtnEvent_t
{
    unsigned char ButtonState;
    unsigned char ButtonShortPressed;
    unsigned char ButtonLongPressed;
    unsigned char ButtonDebounceState;
    unsigned char ButtonBitShift;
    unsigned long ButtonDebounceCnt;
    unsigned long ButtonDebounceTimer;
} BtnEvent_t;

typedef enum  
{
    BTN_IDX_1 = 0,
    BTN_IDX_2,
    BTN_IDX_3,
    BTN_IDX_4,
    BTN_IDX_5,
    BTN_IDX_6,
    BTN_IDX_7,
    BTN_IDX_8,
}BTN_IDX_t;

BtnEvent_t event_btn[8] = {0};

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/



void btnX_Debounce(BTN_IDX_t idx,unsigned char IOState)
{
    // printf_tiny("%d,%2d\r\n",event_btn[idx].ButtonState,event_btn[idx].ButtonDebounceTimer);

    if ((event_btn[idx].ButtonState == IOState) && 
        (event_btn[idx].ButtonDebounceTimer++ == DEBOUNCE_INTERVAL_TIME))
    {
        event_btn[idx].ButtonDebounceTimer = 0;
        event_btn[idx].ButtonDebounceState |= 1 << event_btn[idx].ButtonBitShift;

        // printf_tiny("0x%02X,%d\r\n",event_btn[idx].ButtonDebounceState,event_btn[idx].ButtonBitShift);
        if (event_btn[idx].ButtonDebounceState == DEBOUNCE_SHIFT_BITS)
        {
            event_btn[idx].ButtonShortPressed = PRESSED;
            event_btn[idx].ButtonDebounceState = 0;
            event_btn[idx].ButtonDebounceCnt++;
        }

        if ((event_btn[idx].ButtonShortPressed == PRESSED) && 
            (event_btn[idx].ButtonDebounceCnt >=  (DEBOUNCE_LONG_TIME/DEBOUNCE_SHORT_TIME)  ) )    // over 80ms* __ = long
        {
            event_btn[idx].ButtonLongPressed = PRESSED;
            event_btn[idx].ButtonDebounceCnt = 0;            
        }
        event_btn[idx].ButtonBitShift = (event_btn[idx].ButtonBitShift >= 7) ? (0) : (event_btn[idx].ButtonBitShift+1) ;
    }
}

void btnX_init(BTN_IDX_t idx)
{
    event_btn[idx].ButtonDebounceTimer = 0;
    event_btn[idx].ButtonDebounceState = 0;
    event_btn[idx].ButtonDebounceCnt = 0;
    event_btn[idx].ButtonShortPressed = RELEASED;
    event_btn[idx].ButtonLongPressed = RELEASED;
    event_btn[idx].ButtonBitShift = 0;
}

#if defined (ENABLE_BTN8)
void btn8_timer_irq(void)
{
    if (BTN8 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_8,BTN8);
    }
    else
    {
        FLAG_BTN_8_SHORT_PRESSED = 0;
        FLAG_BTN_8_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_8);
        
        event_btn[BTN_IDX_8].ButtonState = ACTIVE_LOW; //target state
    }
}

void btn8_task(void)
{

    /********************* */
    if ((FLAG_BTN_8_SHORT_PRESSED == 0) && 
        (event_btn[BTN_IDX_8].ButtonShortPressed))
    {
        FLAG_BTN_8_SHORT_PRESSED = 1;
        printf_tiny("button 8 SHORT pressed\r\n");
    }

    if ((FLAG_BTN_8_LONG_PRESSED == 0 ) && 
        (event_btn[BTN_IDX_8].ButtonLongPressed))
    {
        FLAG_BTN_8_LONG_PRESSED = 1;
        printf_tiny("button 8 LONG pressed\r\n");
    }
}
#endif

#if defined (ENABLE_BTN7)
void btn7_timer_irq(void)
{
    if (BTN7 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_7,BTN7);
    }
    else
    {
        FLAG_BTN_7_SHORT_PRESSED = 0;
        FLAG_BTN_7_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_7);
        
        event_btn[BTN_IDX_7].ButtonState = ACTIVE_LOW; //target state
    }
}

void btn7_task(void)
{

    /********************* */
    if ((FLAG_BTN_7_SHORT_PRESSED == 0) && 
        (event_btn[BTN_IDX_7].ButtonShortPressed))
    {
        FLAG_BTN_7_SHORT_PRESSED = 1;
        printf_tiny("button 7 SHORT pressed\r\n");
    }

    if ((FLAG_BTN_7_LONG_PRESSED == 0 ) && 
        (event_btn[BTN_IDX_7].ButtonLongPressed))
    {
        FLAG_BTN_7_LONG_PRESSED = 1;
        printf_tiny("button 7 LONG pressed\r\n");
    }
}
#endif

#if defined (ENABLE_BTN6)
void btn6_timer_irq(void)
{
    if (BTN6 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_6,BTN6);
    }
    else
    {
        FLAG_BTN_6_SHORT_PRESSED = 0;
        FLAG_BTN_6_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_6);
        
        event_btn[BTN_IDX_6].ButtonState = ACTIVE_LOW; //target state
    }
}

void btn6_task(void)
{

    /********************* */
    if ((FLAG_BTN_6_SHORT_PRESSED == 0) && 
        (event_btn[BTN_IDX_6].ButtonShortPressed))
    {
        FLAG_BTN_6_SHORT_PRESSED = 1;
        printf_tiny("button 6 SHORT pressed\r\n");
    }

    if ((FLAG_BTN_6_LONG_PRESSED == 0 ) && 
        (event_btn[BTN_IDX_6].ButtonLongPressed))
    {
        FLAG_BTN_6_LONG_PRESSED = 1;
        printf_tiny("button 6 LONG pressed\r\n");
    }
}
#endif

#if defined (ENABLE_BTN5)
void btn5_timer_irq(void)
{
    if (BTN5 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_5,BTN5);
    }
    else
    {
        FLAG_BTN_5_SHORT_PRESSED = 0;
        FLAG_BTN_5_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_5);
        
        event_btn[BTN_IDX_5].ButtonState = ACTIVE_LOW; //target state
    }
}

void btn5_task(void)
{

    /********************* */
    if ((FLAG_BTN_5_SHORT_PRESSED == 0) && 
        (event_btn[BTN_IDX_5].ButtonShortPressed))
    {
        FLAG_BTN_5_SHORT_PRESSED = 1;
        printf_tiny("button 5 SHORT pressed\r\n");
    }

    if ((FLAG_BTN_5_LONG_PRESSED == 0 ) && 
        (event_btn[BTN_IDX_5].ButtonLongPressed))
    {
        FLAG_BTN_5_LONG_PRESSED = 1;
        printf_tiny("button 5 LONG pressed\r\n");
    }
}
#endif

#if defined (ENABLE_BTN4)
void btn4_timer_irq(void)
{
    if (BTN4 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_4,BTN4);
    }
    else
    {
        FLAG_BTN_4_SHORT_PRESSED = 0;
        FLAG_BTN_4_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_4);
        
        event_btn[BTN_IDX_4].ButtonState = ACTIVE_LOW; //target state
    }
}

void btn4_task(void)
{

    /********************* */
    if ((FLAG_BTN_4_SHORT_PRESSED == 0) && 
        (event_btn[BTN_IDX_4].ButtonShortPressed))
    {
        FLAG_BTN_4_SHORT_PRESSED = 1;
        printf_tiny("button 4 SHORT pressed\r\n");
    }

    if ((FLAG_BTN_4_LONG_PRESSED == 0 ) && 
        (event_btn[BTN_IDX_4].ButtonLongPressed))
    {
        FLAG_BTN_4_LONG_PRESSED = 1;
        printf_tiny("button 4 LONG pressed\r\n");
    }
}
#endif

#if defined (ENABLE_BTN3)
void btn3_timer_irq(void)
{
    if (BTN3 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_3,BTN3);
    }
    else
    {
        FLAG_BTN_3_SHORT_PRESSED = 0;
        FLAG_BTN_3_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_3);
        
        event_btn[BTN_IDX_3].ButtonState = ACTIVE_LOW; //target state
    }
}

void btn3_task(void)
{

    /********************* */
    if ((FLAG_BTN_3_SHORT_PRESSED == 0) && 
        (event_btn[BTN_IDX_3].ButtonShortPressed))
    {
        FLAG_BTN_3_SHORT_PRESSED = 1;
        printf_tiny("button 3 SHORT pressed\r\n");
    }

    if ((FLAG_BTN_3_LONG_PRESSED == 0 ) && 
        (event_btn[BTN_IDX_3].ButtonLongPressed))
    {
        FLAG_BTN_3_LONG_PRESSED = 1;
        printf_tiny("button 3 LONG pressed\r\n");
    }
}
#endif

#if defined (ENABLE_BTN2)
void btn2_timer_irq(void)
{
    if (BTN2 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_2,BTN2);
    }
    else
    {
        FLAG_BTN_2_SHORT_PRESSED = 0;
        FLAG_BTN_2_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_2);
        
        event_btn[BTN_IDX_2].ButtonState = ACTIVE_LOW; //target state
    }
}

void btn2_task(void)
{

    /********************* */    
    if ((FLAG_BTN_2_SHORT_PRESSED == 0) && 
        (event_btn[BTN_IDX_2].ButtonShortPressed))
    {
        FLAG_BTN_2_SHORT_PRESSED = 1;
        printf_tiny("button 2 SHORT pressed\r\n");
    }

    if ((FLAG_BTN_2_LONG_PRESSED == 0 ) && 
        (event_btn[BTN_IDX_2].ButtonLongPressed))
    {
        FLAG_BTN_2_LONG_PRESSED = 1;
        printf_tiny("button 2 LONG pressed\r\n");
    }
}
#endif

#if defined (ENABLE_BTN1)
void btn1_timer_irq(void)
{
    if (BTN1 == LOW)  // pressed
    {               
        btnX_Debounce(BTN_IDX_1,BTN1);
    }
    else
    {
        FLAG_BTN_1_SHORT_PRESSED = 0;
        FLAG_BTN_1_LONG_PRESSED = 0;
        btnX_init(BTN_IDX_1);

        event_btn[BTN_IDX_1].ButtonState = ACTIVE_LOW; //target state
    }
}

void btn1_task(void)
{

    /********************* */
    if ((FLAG_BTN_1_SHORT_PRESSED == 0) && 
        (event_btn[BTN_IDX_1].ButtonShortPressed))
    {
        FLAG_BTN_1_SHORT_PRESSED = 1;
        printf_tiny("button 1 SHORT pressed\r\n");
        // IICA0_send_multi_cmd();
        IICA0_send_seq_cmd();
    }

    if ((FLAG_BTN_1_LONG_PRESSED == 0 ) && 
        (event_btn[BTN_IDX_1].ButtonLongPressed))
    {
        FLAG_BTN_1_LONG_PRESSED = 1;
        printf_tiny("button 1 LONG pressed\r\n");
    }
}
#endif

void btn_timer_irq(void)
{
    #if defined (ENABLE_BTN1)
    btn1_timer_irq();
    #endif
    #if defined (ENABLE_BTN2)
    btn2_timer_irq();
    #endif
    #if defined (ENABLE_BTN3)
    btn3_timer_irq();
    #endif
    #if defined (ENABLE_BTN4)
    btn4_timer_irq();
    #endif
    #if defined (ENABLE_BTN5)
    btn5_timer_irq();
    #endif
    #if defined (ENABLE_BTN6)
    btn6_timer_irq();
    #endif
    #if defined (ENABLE_BTN7)
    btn7_timer_irq();
    #endif
    #if defined (ENABLE_BTN8)
    btn8_timer_irq();
    #endif
}

void btn_init(void)
{ 
    #if defined (ENABLE_BTN1)
    btnX_init(BTN_IDX_1);
    #endif
    #if defined (ENABLE_BTN2)
    btnX_init(BTN_IDX_2);
    #endif
    #if defined (ENABLE_BTN3)
    btnX_init(BTN_IDX_3);
    #endif
    #if defined (ENABLE_BTN4)
    btnX_init(BTN_IDX_4);
    #endif
    #if defined (ENABLE_BTN5)
    btnX_init(BTN_IDX_5);
    #endif
    #if defined (ENABLE_BTN6)
    btnX_init(BTN_IDX_6);
    #endif
    #if defined (ENABLE_BTN7)
    btnX_init(BTN_IDX_7);  
    #endif
    #if defined (ENABLE_BTN8)  
    btnX_init(BTN_IDX_8);
    #endif
    
}

void btn_task(void)
{
    #if defined (ENABLE_BTN1)
    btn1_task();
    #endif
    #if defined (ENABLE_BTN2)
    btn2_task();
    #endif
    #if defined (ENABLE_BTN3)
    btn3_task();
    #endif
    #if defined (ENABLE_BTN4)
    btn4_task();
    #endif
    #if defined (ENABLE_BTN5)
    btn5_task();
    #endif
    #if defined (ENABLE_BTN6)
    btn6_task();
    #endif
    #if defined (ENABLE_BTN7)
    btn7_task();
    #endif
    #if defined (ENABLE_BTN8)
    btn8_task();
    #endif
}


