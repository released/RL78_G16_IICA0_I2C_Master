/*_____ I N C L U D E S ____________________________________________________*/

/*_____ D E C L A R A T I O N S ____________________________________________*/
#define bool  _Bool

#ifndef PASS
#define PASS              					            (1)
#endif

#ifndef FAIL
#define FAIL              					            (0)
#endif

#ifndef OK
#define OK              					            (1)
#endif

#ifndef NG
#define NG              					            (0)
#endif

#ifndef TRUE
#define TRUE              					            (1)
#endif

#ifndef FALSE
#define FALSE              					            (0)
#endif

// #ifndef true
// #define true              					            (1)
// #endif

// #ifndef false
// #define false              					            (0)
// #endif

#ifndef ACTIVE_LOW
#define ACTIVE_LOW              					    (0)
#endif

#ifndef ACTIVE_HIGH
#define ACTIVE_HIGH              					    (1)
#endif

#ifndef HIGH
#define HIGH              					            (1)
#endif

#ifndef LOW
#define LOW              					            (0)
#endif

#ifndef PRESSED
#define PRESSED              					        (1)
#endif

#ifndef RELEASED
#define RELEASED              					        (0)
#endif


#ifndef BIT0
#define BIT0     (0x00000001UL)       ///< Bit 0 mask of an 32 bit integer
#endif

#ifndef BIT1
#define BIT1     (0x00000002UL)       ///< Bit 1 mask of an 32 bit integer
#endif

#ifndef BIT2
#define BIT2     (0x00000004UL)       ///< Bit 2 mask of an 32 bit integer
#endif

#ifndef BIT3
#define BIT3     (0x00000008UL)       ///< Bit 3 mask of an 32 bit integer
#endif

#ifndef BIT4
#define BIT4     (0x00000010UL)       ///< Bit 4 mask of an 32 bit integer
#endif

#ifndef BIT5
#define BIT5     (0x00000020UL)       ///< Bit 5 mask of an 32 bit integer
#endif

#ifndef BIT6
#define BIT6     (0x00000040UL)       ///< Bit 6 mask of an 32 bit integer
#endif

#ifndef BIT7
#define BIT7     (0x00000080UL)       ///< Bit 7 mask of an 32 bit integer
#endif

/*_____ D E F I N I T I O N S ______________________________________________*/

#define ENALBE_SPECIFIC_FUNCTION

#define TOUCH_SYNC                                      (P4_bit.no1)    //OUTPUT


#define ENABLE_BTN1
// #define ENABLE_BTN2
// #define ENABLE_BTN3
// #define ENABLE_BTN4
// #define ENABLE_BTN5
// #define ENABLE_BTN6
// #define ENABLE_BTN7
// #define ENABLE_BTN8

// button , schematic : pull high
#if defined (ENABLE_BTN1)
#define BTN1                                            (P13_bit.no7)   //(P9_bit.no0)
#endif 
#if defined (ENABLE_BTN2)
#define BTN2                                            (P9_bit.no1)
#endif 
#if defined (ENABLE_BTN3)
#define BTN3                                            (P9_bit.no2)
#endif 
#if defined (ENABLE_BTN4)
#define BTN4                                            (P9_bit.no3)
#endif 
#if defined (ENABLE_BTN5)
#define BTN5                                            (P9_bit.no4)
#endif 
#if defined (ENABLE_BTN6)
#define BTN6                                            (P9_bit.no5)
#endif 
#if defined (ENABLE_BTN7)
#define BTN7                                            (P9_bit.no6)
#endif 
#if defined (ENABLE_BTN8)
#define BTN8                                            (P9_bit.no7)
#endif 



// #define ENABLE_BUTTON_EXTI
#define ENABLE_BUTTON_DEBOUNCE

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void IICA0_send_seq_cmd(void);
void IICA0_send_multi_cmd(void);

void Timer_TAU0_Channel0_1ms_IRQ(void);

void set_TIMER_PERIOD_1000MS(void);
void reset_TIMER_PERIOD_1000MS(void);
bool Is_TIMER_PERIOD_1000MS_Trig(void);
void Timer_1ms_IRQ(void);

#if defined (ENABLE_BUTTON_EXTI)
void Button_Process_long_counter(void);
void Button_Process_in_polling(void);
void Button_Process_in_IRQ(void);
#endif

void UARTx_Process(unsigned char rxbuf);

void LED_Toggle(void);
void loop(void);

void check_reset_source(void);
void RL78_soft_reset(unsigned char flag);
void hardware_init(void);
