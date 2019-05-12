/**
  ******************************************************************************
  * @file    stm8s_it.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service 
  *          routine.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "uv_timer.h"
    
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
extern void seven_segment(void);


#ifdef _COSMIC_
/**
  * @brief Dummy Interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#if defined (__W1209_CA) || defined (__W1209_CC)
  if (uvTimer.debounce == 0) {
    if (((GPIO_ReadInputData(ENCA_PORT) & ENCA_PIN) == 0x00) & (uvTimer.timerState == timer_set)){
        uvTimer.prevA = 0;
        if (uvTimer.timeSet <= 1) {
        } else if(uvTimer.timeSet <= 10) {
          uvTimer.timeSet -= 1;
        } else if (uvTimer.timeSet <= 300){
          uvTimer.timeSet -= 10;
        } else if (uvTimer.timeSet <= 600){
          uvTimer.timeSet -= 50;
        } else if (uvTimer.timeSet <= 1200){
          uvTimer.timeSet -= 100;
        } else if (uvTimer.timeSet <= 54000){
          uvTimer.timeSet -= 600;
        } else {
          uvTimer.timeSet -= 6000;
        }
        /* trigger the set saving */
        uvTimer.flashTmr = STORE_SET;
        uvTimer.flashRdy = FALSE;
      } 
     if (((GPIO_ReadInputData(ENCB_PORT) & ENCB_PIN) == 0x00) & (uvTimer.timerState == timer_set)){
        uvTimer.prevB = 0;
        if(uvTimer.timeSet < 10){
          uvTimer.timeSet += 1;
        } else if (uvTimer.timeSet < 300){
          uvTimer.timeSet += 10;
        } else if (uvTimer.timeSet < 600){
          uvTimer.timeSet += 50;
        } else if (uvTimer.timeSet < 1200){
          uvTimer.timeSet += 100;
        } else if (uvTimer.timeSet < 54000){
          uvTimer.timeSet += 600;
        }  else if (uvTimer.timeSet < 594000){
          uvTimer.timeSet += 6000;
        } 
      /* trigger the set saving */
      uvTimer.flashTmr = STORE_SET;
      uvTimer.flashRdy = FALSE;
    }

    if ((GPIO_ReadInputData(ENCC_PORT) & ENCC_PIN) == 0x00)
    {
      if (uvTimer.timerState == timer_set){
        uvTimer.timeCurrent = uvTimer.timeSet;
        uvTimer.timerState = timer_run;
      } else if (uvTimer.timerState == timer_run){
        uvTimer.timerState = timer_pause;
      } else if (uvTimer.timerState == timer_pause){
        uvTimer.timerState = timer_run;
      } 

    }
  }
  if (uvTimer.debounce == 0) {
    uvTimer.debounce = DEBOUNCE;
  }
#endif
}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef __UCDEV
  u8 a,b,c;
  a = (GPIO_ReadInputPin(ENCA_PORT, ENCA_PIN) == RESET);
  b = (GPIO_ReadInputPin(ENCB_PORT, ENCB_PIN) == RESET);
  c = (GPIO_ReadInputPin(ENCC_PORT, ENCC_PIN) == RESET);
  if (uvTimer.debounce == 0) {
    if ((a != TRUE) & (b == TRUE) & (uvTimer.timerState == timer_set)){
        if (uvTimer.timeSet <= 1) {
        } else if(uvTimer.timeSet <= 10) {
          uvTimer.timeSet -= 1;
        } else if (uvTimer.timeSet <= 300){
          uvTimer.timeSet -= 10;
        } else if (uvTimer.timeSet <= 600){
          uvTimer.timeSet -= 50;
        } else if (uvTimer.timeSet <= 1200){
          uvTimer.timeSet -= 100;
        } else {
          uvTimer.timeSet -= 600;
        } 
      /* trigger the set saving */
      uvTimer.flashTmr = STORE_SET;
      uvTimer.flashRdy = FALSE;
      } 
    if ((a == TRUE) & (b != TRUE) & (uvTimer.timerState == timer_set)){
        if(uvTimer.timeSet < 10){
          uvTimer.timeSet += 1;
        } else if (uvTimer.timeSet < 300){
          uvTimer.timeSet += 10;
        } else if (uvTimer.timeSet < 600){
          uvTimer.timeSet += 50;
        } else if (uvTimer.timeSet < 1200){
          uvTimer.timeSet += 100;
        } else if (uvTimer.timeSet < 54000){
          uvTimer.timeSet += 600;
        } 
      /* trigger the set saving */
      uvTimer.flashTmr = STORE_SET;
      uvTimer.flashRdy = FALSE;
    }

    if ((GPIO_ReadInputPin(ENCC_PORT, ENCC_PIN) == RESET))
    {
      if (uvTimer.timerState == timer_set){
        uvTimer.timeCurrent = uvTimer.timeSet;
        uvTimer.timerState = timer_run;
      } else if (uvTimer.timerState == timer_run){
        uvTimer.timerState = timer_pause;
      } else if (uvTimer.timerState == timer_pause){
        uvTimer.timerState = timer_run;
      } 

    }
    uvTimer.debounce = DEBOUNCE;
  }
  
#endif
}
/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x) 
/**
  * @brief External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8AF52Ax) */

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

  if (uvTimer.debounce > 0) {
    uvTimer.debounce--;
  }
  if (uvTimer.flashTmr > 0) {
    uvTimer.flashTmr--;
  }

  if (uvTimer.delay > 0) {
    uvTimer.delay--;
  }
  
  if((uvTimer.timeCurrent > 0) & (uvTimer.timerState == timer_run) & ((GPIO_ReadInputData(ENCC_PORT) & ENCC_PIN) != 0x00)){
    GPIO_WriteHigh(GPIOA, (GPIO_Pin_TypeDef)GPIO_PIN_3);
    uvTimer.timeCurrent--;
  } else if((uvTimer.timeCurrent == 0) & (uvTimer.timerState == timer_run)){
    GPIO_WriteLow(GPIOA, (GPIO_Pin_TypeDef)GPIO_PIN_3);
    uvTimer.timerState = timer_set;
    uvTimer.expired = TRUE;
  } else if(uvTimer.timerState == timer_pause){
    GPIO_WriteLow(GPIOA, (GPIO_Pin_TypeDef)GPIO_PIN_3);
  }
  
  // long press detection
  if ((GPIO_ReadInputData(ENCC_PORT) & ENCC_PIN) == 0x00){
    uvTimer.timerLongPress++;
  } else {
    uvTimer.timerLongPress = 0;
  }
  
  if (uvTimer.timerLongPress > LONG_PRESS) {
    GPIO_WriteLow(GPIOA, (GPIO_Pin_TypeDef)GPIO_PIN_3);
    uvTimer.timerState = timer_set;
    uvTimer.timeCurrent = uvTimer.timeSet;
    uvTimer.timerLongPress = 0;
  }
#if defined (__W1209_CA) || defined(__W1209_CC)
  /* ENCA & ENCB is a buttons "+" & "-" */
      if (((GPIO_ReadInputData(ENCA_PORT) & ENCA_PIN) == 0x00) 
          & (uvTimer.timerState == timer_set)
          & (uvTimer.prevA == 0)
          & (uvTimer.debounce == 0)){
        uvTimer.prevA = 0;
        if (uvTimer.timeSet <= 1) {
        } else if(uvTimer.timeSet <= 10) {
          uvTimer.timeSet -= 1;
        } else if (uvTimer.timeSet <= 300){
          uvTimer.timeSet -= 10;
        } else if (uvTimer.timeSet <= 600){
          uvTimer.timeSet -= 50;
        } else if (uvTimer.timeSet <= 1200){
          uvTimer.timeSet -= 100;
        } else if (uvTimer.timeSet <= 54000){
          uvTimer.timeSet -= 600;
        } else {
          uvTimer.timeSet -= 6000;
        }
        /* trigger the set saving */
        uvTimer.flashTmr = STORE_SET;
        uvTimer.flashRdy = FALSE;
      } else if ( (GPIO_ReadInputData(ENCA_PORT) & ENCA_PIN) != 0x00){
        uvTimer.prevA = 1;
      }
  
     if (((GPIO_ReadInputData(ENCB_PORT) & ENCB_PIN) == 0x00) 
          & (uvTimer.timerState == timer_set)
          & (uvTimer.prevB == 0)
          & (uvTimer.debounce == 0)){
        uvTimer.prevB = 0;
        if(uvTimer.timeSet < 10){
          uvTimer.timeSet += 1;
        } else if (uvTimer.timeSet < 300){
          uvTimer.timeSet += 10;
        } else if (uvTimer.timeSet < 600){
          uvTimer.timeSet += 50;
        } else if (uvTimer.timeSet < 1200){
          uvTimer.timeSet += 100;
        } else if (uvTimer.timeSet < 54000){
          uvTimer.timeSet += 600;
        } else if (uvTimer.timeSet < 594000){
          uvTimer.timeSet += 6000;
        } 
      /* trigger the set saving */
      uvTimer.flashTmr = STORE_SET;
      uvTimer.flashRdy = FALSE;
    }  else if ( (GPIO_ReadInputData(ENCB_PORT) & ENCB_PIN) != 0x00){
        uvTimer.prevB = 1;
      }
#endif
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE); 
}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
 
/**
  * @brief Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
/**
  * @brief Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
   seven_segment();
   TIM2_ClearITPendingBit  ( TIM2_IT_UPDATE )  ;
 }

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S903) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8AF622x)
/**
  * @brief UART4 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART4 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8AF622x) */

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#else /* STM8S105 or STM8S103 or STM8S903 or STM8AF626x or STM8AF622x */
/**
  * @brief ADC1 interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer6 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#else /* STM8S208 or STM8S207 or STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
  * @brief Timer4 Update/Overflow Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x)*/

/**
  * @brief Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/