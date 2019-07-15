#include "includes.h"

FATFS fs;
extern u8 USBH_USR_ApplicationState;
int _main(void)
{
  __IO uint32_t i = 0;
  USART1_Config(115200);
//  USART1_Puts("init ok!\r\n");
  printf("init ok!\r\n");
  /* !< At this stage the microcontroller clock setting is already configured,
   * this is done through SystemInit() function which is called from startup
   * file (startup_stm32fxxx_xx.s) before to branch to application main. To
   * reconfigure the default setting of SystemInit() function, refer to
   * system_stm32fxxx.c file */

  printf("udisk mount: %d\r\n", f_mount(&fs, "0:", 1));
//  printf("sd mount: %d\r\n", f_mount(&fs, "1:", 1));
  /* Init Host Library */
  USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_cb);


   FIL file;
   static u8 aaa = 0;
//  printf("write£º %d\r\n", f_open(&file, "0:1.TXT", FA_CREATE_ALWAYS | FA_WRITE));
    
  while (1)
  {
    /* Host Task handler */
    USBH_Process(&USB_OTG_Core, &USB_Host);

    if(USBH_USR_ApplicationState == USH_USR_FS_DRAW && aaa == 0)
    {
      aaa = 1;
      printf("udisk mount: %d\r\n", f_mount(&fs, "0:", 1));
//      printf("udisk mount: %d\r\n", f_mount(&fs, "", 0));
      if(f_open(&file, "222.a", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
      {
        printf("open ok!\r\n");
        f_close(&file);
      }
      else
        printf("open failed!\r\n");
    }
    if (i++ == 0x100000)
    {
      printf("Process...\r\n");
//      STM_EVAL_LEDToggle(0);
//      STM_EVAL_LEDToggle(LED2);
//      STM_EVAL_LEDToggle(LED3);
//      STM_EVAL_LEDToggle(LED4);
      i = 0;
    }
  }
}


#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
   * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file, 
   * line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
