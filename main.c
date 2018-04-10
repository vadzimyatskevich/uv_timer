/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  Vadzim Yatskevich
  * @version V0.0.1
  * @date    04-june-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "uv_timer.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void TIM1_Config(void);
static void TIM2_Config(void);
void seven_segment(void);
void store_set(void);
/* Private functions ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uvTimerTypedef uvTimer;
ledDisplayTypedef ledDisplay;






void main(void)
{ 
  uint32_t f_data = 0;
  
  uvTimer.debounce = DEBOUNCE;
  uvTimer.timeSet = 1200;
  uvTimer.timerState = timer_set;
  uvTimer.flashRdy = TRUE;
  uvTimer.expired = FALSE;
  ledDisplay.index = 0;
  ledDisplay.dig1 = 1;
  ledDisplay.dig2 = 2;
  ledDisplay.dig3 = 3;
  
  /* Clock configuration -----------------------------------------------------*/
  CLK_Config();
  GPIO_Config();
  TIM1_Config();
  TIM2_Config();
  
  FLASH_Unlock  ( FLASH_MEMTYPE_DATA )  ;
  f_data = FLASH_ReadByte(0x00004000);
  f_data = (f_data << 8) + FLASH_ReadByte(0x00004001);
  f_data = (f_data << 8) + FLASH_ReadByte(0x00004002);
  f_data = (f_data << 8) + FLASH_ReadByte(0x00004003);
  FLASH_Lock  ( FLASH_MEMTYPE_DATA )  ;

  if (f_data > 594000 /*|| f_data == 0*/) {
    f_data = 1200;
  }

  uvTimer.timeSet = f_data;
  enableInterrupts(); 

  /* Infinite loop */
  while (1)
  {
    if ( (uvTimer.flashRdy == FALSE) & (uvTimer.flashTmr == 0) ){
        uvTimer.flashRdy = TRUE;
        store_set();
    }
#ifndef __UCDEV
    if (uvTimer.expired == TRUE){
        uvTimer.expired = FALSE;
        beep(5);
    }
#endif
  }
  
}



void store_set(){
  FLASH_Unlock  ( FLASH_MEMTYPE_DATA )  ;
  FLASH_ProgramWord(0x00004000, (uint32_t)uvTimer.timeSet);
  FLASH_Lock  ( FLASH_MEMTYPE_DATA )  ;
}

#if defined (__W1209_CA) || defined(__W1209_CC)
  void beep(u8 i){
    for (; i>0; i--){
        GPIO_WriteHigh(BEEP_PORT, (GPIO_Pin_TypeDef)BEEP_PIN); // dig on
        delay_100ms(5);
        GPIO_WriteLow(BEEP_PORT, (GPIO_Pin_TypeDef)BEEP_PIN); // dig on
        delay_100ms(2);
    }
  }
#endif

              
void delay_100ms(u16 ms){
  uvTimer.delay = ms;
  while (uvTimer.delay != 0){
  }
}

void seven_segment(){
  u8 digit, dig1,dig2,dig3;
  u32 value;
   // index
   if ( ++ledDisplay.index > 2) {
     ledDisplay.index = 0;
   }

  if(uvTimer.timerState == timer_set){
    value = uvTimer.timeSet;
  } else {
    value = uvTimer.timeCurrent;
  }
  
  if ( value >= 6000 ){
    dig3 = value/60000;
    value %= 60000;
    dig2 = value/6000;
    value %= 6000;
    dig1 = value/600;
  } else if ( value >= 10 ){
    dig3 = value / 1000;
    value %= 1000;
    dig2 = value/100;
    value %= 100;
    dig1 = value/10;
  } else {
    dig1 = value%10;
    dig2 = ',';
    dig3 = value/10;
  }
  switch ( ledDisplay.index){
    case 0:
      digit = dig1;
      break;
    case 1:
      digit = dig2;
      break;
    case 2:
      digit = dig3;
      break;
    default:
    break;
  }
  
  /* Check the used compiler */
#if defined(DISP_CA)
  //  all digits off
  GPIO_WriteLow(DIG1_PORT, (GPIO_Pin_TypeDef)DIG1_PIN);
  GPIO_WriteLow(DIG2_PORT, (GPIO_Pin_TypeDef)DIG2_PIN);
  GPIO_WriteLow(DIG3_PORT, (GPIO_Pin_TypeDef)DIG3_PIN);
  // segments on
  GPIO_WriteLow(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg ON
  GPIO_WriteLow(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg ON
  GPIO_WriteLow(SEG_C_PORT, (GPIO_Pin_TypeDef)SEG_C_PIN); // seg ON
  GPIO_WriteLow(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg ON
  GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg ON
  GPIO_WriteLow(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg ON
  GPIO_WriteLow(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg ON
  switch (digit){
    case 0:
      GPIO_WriteHigh(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    case 1:
      GPIO_WriteHigh(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg off
      GPIO_WriteHigh(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg off
      GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteHigh(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
      GPIO_WriteHigh(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    case 2:
      GPIO_WriteHigh(SEG_C_PORT, (GPIO_Pin_TypeDef)SEG_C_PIN); // seg off
      GPIO_WriteHigh(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
    break;
    case 3:
      GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteHigh(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
    break;
    case 4:
      GPIO_WriteHigh(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg off
      GPIO_WriteHigh(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg off
      GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
    break;
    case 5:
      GPIO_WriteHigh(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg off
      GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
    break;
    case 6:
      GPIO_WriteHigh(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg off
    break;
    case 7:
      GPIO_WriteHigh(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg off
      GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteHigh(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
      GPIO_WriteHigh(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    case 8:
    break;
    case 9:
      GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
    break;
    case ',':
      GPIO_WriteHigh(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg off
      GPIO_WriteHigh(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg off
      GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteHigh(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
      GPIO_WriteHigh(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    default:
      GPIO_WriteHigh(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg off
      GPIO_WriteHigh(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg off
      GPIO_WriteHigh(SEG_C_PORT, (GPIO_Pin_TypeDef)SEG_C_PIN); // seg off
      GPIO_WriteHigh(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg off
      GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteHigh(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
      GPIO_WriteHigh(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    }
  
  switch (ledDisplay.index){
    case 0:
      GPIO_WriteHigh(DIG1_PORT, (GPIO_Pin_TypeDef)DIG1_PIN); // dig on
    break;
    case 1:
      GPIO_WriteHigh(DIG2_PORT, (GPIO_Pin_TypeDef)DIG2_PIN); // dig on
    break;
    case 2:
      GPIO_WriteHigh(DIG3_PORT, (GPIO_Pin_TypeDef)DIG3_PIN); // dig on
    break;
    default:
    break;
  }
#elif defined(DISP_CC)
  //  all digits off
  GPIO_WriteHigh(DIG1_PORT, (GPIO_Pin_TypeDef)DIG1_PIN);
  GPIO_WriteHigh(DIG2_PORT, (GPIO_Pin_TypeDef)DIG2_PIN);
  GPIO_WriteHigh(DIG3_PORT, (GPIO_Pin_TypeDef)DIG3_PIN);
  // segments on
  GPIO_WriteHigh(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg ON
  GPIO_WriteHigh(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg ON
  GPIO_WriteHigh(SEG_C_PORT, (GPIO_Pin_TypeDef)SEG_C_PIN); // seg ON
  GPIO_WriteHigh(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg ON
  GPIO_WriteHigh(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg ON
  GPIO_WriteHigh(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg ON
  GPIO_WriteHigh(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg ON
  switch (digit){
    case 0:
      GPIO_WriteLow(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    case 1:
      GPIO_WriteLow(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg off
      GPIO_WriteLow(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg off
      GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteLow(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
      GPIO_WriteLow(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    case 2:
      GPIO_WriteLow(SEG_C_PORT, (GPIO_Pin_TypeDef)SEG_C_PIN); // seg off
      GPIO_WriteLow(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
    break;
    case 3:
      GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteLow(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
    break;
    case 4:
      GPIO_WriteLow(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg off
      GPIO_WriteLow(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg off
      GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
    break;
    case 5:
      GPIO_WriteLow(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg off
      GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
    break;
    case 6:
      GPIO_WriteLow(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg off
    break;
    case 7:
      GPIO_WriteLow(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg off
      GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteLow(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
      GPIO_WriteLow(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    case 8:
    break;
    case 9:
      GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
    break;
    case ',':
      GPIO_WriteLow(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg off
      GPIO_WriteLow(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg off
      GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteLow(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
      GPIO_WriteLow(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    default:
      GPIO_WriteLow(SEG_A_PORT, (GPIO_Pin_TypeDef)SEG_A_PIN); // seg off
      GPIO_WriteLow(SEG_B_PORT, (GPIO_Pin_TypeDef)SEG_B_PIN); // seg off
      GPIO_WriteLow(SEG_C_PORT, (GPIO_Pin_TypeDef)SEG_C_PIN); // seg off
      GPIO_WriteLow(SEG_D_PORT, (GPIO_Pin_TypeDef)SEG_D_PIN); // seg off
      GPIO_WriteLow(SEG_E_PORT, (GPIO_Pin_TypeDef)SEG_E_PIN); // seg off
      GPIO_WriteLow(SEG_F_PORT, (GPIO_Pin_TypeDef)SEG_F_PIN); // seg off
      GPIO_WriteLow(SEG_G_PORT, (GPIO_Pin_TypeDef)SEG_G_PIN); // seg off
    break;
    }
  
  switch (ledDisplay.index){
    case 0:
      GPIO_WriteLow(DIG1_PORT, (GPIO_Pin_TypeDef)DIG1_PIN); // dig on
    break;
    case 1:
      GPIO_WriteLow(DIG2_PORT, (GPIO_Pin_TypeDef)DIG2_PIN); // dig on
    break;
    case 2:
      GPIO_WriteLow(DIG3_PORT, (GPIO_Pin_TypeDef)DIG3_PIN); // dig on
    break;
    default:
    break;
  }
#endif

}

  
/**
  * @brief  Configure system clock to run at 16Mhz and output the system clock on
  *         CCO pin
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  CLK_DeInit();
  /* Clock divider to HSI/1 */
#if defined(__UCDEV)
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
#elif defined(__W1209_CA)
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
#elif defined(__W1209_CC)
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
#else
 #error "Unsupported Board!"  
#endif


}

/**
  * @brief  Configure GPIO for buttons available on the evaluation board
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  // encoder
  GPIO_Init(ENCA_PORT, (GPIO_Pin_TypeDef)ENCA_PIN, GPIO_MODE_IN_PU_IT);
  GPIO_Init(ENCB_PORT, (GPIO_Pin_TypeDef)ENCB_PIN, GPIO_MODE_IN_PU_IT);
  GPIO_Init(ENCC_PORT, (GPIO_Pin_TypeDef)ENCC_PIN, GPIO_MODE_IN_PU_IT);
  EXTI_SetExtIntSensitivity(EXTI_ENC, EXTI_SENSITIVITY_FALL_ONLY);
  // LED cathodes
  GPIO_Init(DIG1_PORT, (GPIO_Pin_TypeDef)(DIG1_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(DIG2_PORT, (GPIO_Pin_TypeDef)(DIG2_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(DIG3_PORT, (GPIO_Pin_TypeDef)(DIG3_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  // LED segments
  GPIO_Init(SEG_A_PORT, (GPIO_Pin_TypeDef)(SEG_A_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SEG_B_PORT, (GPIO_Pin_TypeDef)(SEG_B_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SEG_C_PORT, (GPIO_Pin_TypeDef)(SEG_C_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SEG_D_PORT, (GPIO_Pin_TypeDef)(SEG_D_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SEG_E_PORT, (GPIO_Pin_TypeDef)(SEG_E_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SEG_F_PORT, (GPIO_Pin_TypeDef)(SEG_F_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(SEG_G_PORT, (GPIO_Pin_TypeDef)(SEG_G_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  // led module output
  GPIO_Init(GPIOA, (GPIO_Pin_TypeDef)(GPIO_PIN_3) , GPIO_MODE_OUT_PP_LOW_FAST);
  #ifndef __UCDEV
  //beeper
  GPIO_Init(BEEP_PORT, (GPIO_Pin_TypeDef)(BEEP_PIN) , GPIO_MODE_OUT_PP_LOW_FAST);
  #endif
}

/**
  * @brief  Configure TIM1 to generate interrupt
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
    CLK_PeripheralClockConfig (CLK_PERIPHERAL_TIMER1 , ENABLE);     
    TIM1_DeInit(); 
    TIM1_TimeBaseInit(800, TIM1_COUNTERMODE_DOWN, 1000-1, 0); //timer freq = (clock CPU/800) -> 1bit = 1uS -> 92*1uS=92uS 
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); 
    TIM1_Cmd(ENABLE); 
}

/**
  * @brief  Configure TIM1 to generate interrupt
  * @param  None
  * @retval None
  */
static void TIM2_Config(void)
{
    CLK_PeripheralClockConfig (CLK_PERIPHERAL_TIMER2 , ENABLE);     
    TIM2_DeInit(); 
    TIM2_TimeBaseInit(TIM2_PRESCALER_2, 20000); //timer freq = (clock CPU/8) -> 1bit = 1uS -> 100*1uS=100uS 
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE); 
    TIM2_Cmd(ENABLE); 
}



#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
