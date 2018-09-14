
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UVTIMER_H
#define __UVTIMER_H
/* Configuration defines -----------------------------------------------------*/
#define __UCDEV     // оригинальный таймер 
//#define __W1209_CA  // W1209 с общим анодом
//#define __W1209_CC  // W1209 с общим анодом

#if defined(__UCDEV)
  #define DISP_CA
  #define LONG_PRESS      5
  #define DEBOUNCE        0
#elif defined(__W1209_CA)
  #define DISP_CA
  #define LONG_PRESS      5
  #define DEBOUNCE        3
#elif defined(__W1209_CC)
  #define DISP_CC
  #define LONG_PRESS      5
  #define DEBOUNCE        3
#else
 #error "Unsupported Board!"  
#endif


#define STORE_SET       10
/* Board dependent defines ---------------------------------------------------*/
#if defined(__UCDEV)
  #define EXTI_ENC        EXTI_PORT_GPIOD

  #define ENCA_PORT       GPIOD
  #define ENCA_PIN        GPIO_PIN_4
  #define ENCB_PORT       GPIOD
  #define ENCB_PIN        GPIO_PIN_5
  #define ENCC_PORT       GPIOD
  #define ENCC_PIN        GPIO_PIN_6

  #define DIG1_PORT       GPIOD
  #define DIG1_PIN        GPIO_PIN_3
  #define DIG2_PORT       GPIOD
  #define DIG2_PIN        GPIO_PIN_2
  #define DIG3_PORT       GPIOD
  #define DIG3_PIN        GPIO_PIN_1

  #define SEG_A_PORT      GPIOC
  #define SEG_A_PIN       GPIO_PIN_4
  #define SEG_B_PORT      GPIOC
  #define SEG_B_PIN       GPIO_PIN_5
  #define SEG_C_PORT      GPIOC
  #define SEG_C_PIN       GPIO_PIN_6
  #define SEG_D_PORT      GPIOC
  #define SEG_D_PIN       GPIO_PIN_7
  #define SEG_E_PORT      GPIOB
  #define SEG_E_PIN       GPIO_PIN_5
  #define SEG_F_PORT      GPIOB
  #define SEG_F_PIN       GPIO_PIN_4
  #define SEG_G_PORT      GPIOC
  #define SEG_G_PIN       GPIO_PIN_3

#elif defined(__W1209_CA)
  #define EXTI_ENC        EXTI_PORT_GPIOC
  #define BEEP_PORT       GPIOD
  #define BEEP_PIN        GPIO_PIN_6

  #define ENCA_PORT       GPIOC
  #define ENCA_PIN        GPIO_PIN_4
  #define ENCB_PORT       GPIOC
  #define ENCB_PIN        GPIO_PIN_5
  #define ENCC_PORT       GPIOC
  #define ENCC_PIN        GPIO_PIN_3

  #define DIG1_PORT       GPIOA
  #define DIG1_PIN        GPIO_PIN_2
  #define DIG2_PORT       GPIOA
  #define DIG2_PIN        GPIO_PIN_1
  #define DIG3_PORT       GPIOD
  #define DIG3_PIN        GPIO_PIN_4

  #define SEG_A_PORT      GPIOD
  #define SEG_A_PIN       GPIO_PIN_5
  #define SEG_B_PORT      GPIOB
  #define SEG_B_PIN       GPIO_PIN_5
  #define SEG_C_PORT      GPIOC
  #define SEG_C_PIN       GPIO_PIN_7
  #define SEG_D_PORT      GPIOD
  #define SEG_D_PIN       GPIO_PIN_3
  #define SEG_E_PORT      GPIOD
  #define SEG_E_PIN       GPIO_PIN_1
  #define SEG_F_PORT      GPIOB
  #define SEG_F_PIN       GPIO_PIN_4
  #define SEG_G_PORT      GPIOC
  #define SEG_G_PIN       GPIO_PIN_6

#elif defined(__W1209_CC)
  #define EXTI_ENC        EXTI_PORT_GPIOC

  #define BEEP_PORT       GPIOD
  #define BEEP_PIN        GPIO_PIN_6

  #define ENCA_PORT       GPIOC
  #define ENCA_PIN        GPIO_PIN_4
  #define ENCB_PORT       GPIOC
  #define ENCB_PIN        GPIO_PIN_5
  #define ENCC_PORT       GPIOC
  #define ENCC_PIN        GPIO_PIN_3

  #define DIG1_PORT       GPIOB
  #define DIG1_PIN        GPIO_PIN_4
  #define DIG2_PORT       GPIOB
  #define DIG2_PIN        GPIO_PIN_5
  #define DIG3_PORT       GPIOD
  #define DIG3_PIN        GPIO_PIN_4

  #define SEG_A_PORT      GPIOD
  #define SEG_A_PIN       GPIO_PIN_5
  #define SEG_B_PORT      GPIOA
  #define SEG_B_PIN       GPIO_PIN_2
  #define SEG_C_PORT      GPIOC
  #define SEG_C_PIN       GPIO_PIN_7
  #define SEG_D_PORT      GPIOD
  #define SEG_D_PIN       GPIO_PIN_3
  #define SEG_E_PORT      GPIOD
  #define SEG_E_PIN       GPIO_PIN_1
  #define SEG_F_PORT      GPIOA
  #define SEG_F_PIN       GPIO_PIN_1
  #define SEG_G_PORT      GPIOC
  #define SEG_G_PIN       GPIO_PIN_6
#else
 #error "Unsupported Board!"  
#endif

enum state { timer_run, timer_pause, timer_set };

typedef struct {
  u16 debounce;
  u8  flashTmr;
  bool  flashRdy;
  u32 timeSet;
  u32 timeCurrent;
  u8  timerState;
  u8  timerLongPress;
  u8  prevA;
  u8  prevB;
  u8 expired;
  u8 delay;
  u8 a;
  u8 b;
  u8 c;

} uvTimerTypedef;

typedef struct {
  u8  index;
  u8  dig1;
  u8  dig2;
  u8  dig3;
} ledDisplayTypedef;

extern uvTimerTypedef uvTimer;
extern ledDisplayTypedef ledDisplay;


void seven_segment(void);
void delay_100ms(u16);

#ifndef __UCDEV
  void beep(u8);
#endif


/**
  * @}
  */
#endif /* __UVTIMER_H */
