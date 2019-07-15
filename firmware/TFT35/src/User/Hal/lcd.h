#ifndef _LCD_H_
#define _LCD_H_

#include "stdint.h"
#include "Configuration.h"

#ifdef STM32_HAS_FSMC

  typedef struct
  {
    volatile uint16_t LCD_REG;
    volatile uint16_t LCD_RAM;
  } LCD_TypeDef;
                                
  #define LCD_BASE        ((uint32_t)(0x60000000 | 0x00FFFFFE))  	//1111 1111 1111 1111 1111 1110	
  #define LCD             ((LCD_TypeDef *) LCD_BASE)

  #define LCD_WR_REG(regval) do{ LCD->LCD_REG = regval; }while(0)
  #define LCD_WR_DATA(data)  do{ LCD->LCD_RAM = data; }while(0)
  
#else
  
  /* 配置控制线
  * PB6   :LCD-RD
  * PB7   :LCD-WR
  * PB9   :LCD-CS
  * PB8   :LCD-RS   LCD-RS发送的数据代表是命令还是显示数据
  */
  #define	LCD_CS_SET  GPIOB->BSRR=1<<9    //片选端口    PB9
  #define	LCD_RS_SET	GPIOB->BSRR=1<<8    //数据/命令 	PB8	   
  #define	LCD_WR_SET	GPIOB->BSRR=1<<7    //写数据			PB7
  #define	LCD_RD_SET	GPIOB->BSRR=1<<6    //读数据			PB6
                      
  #define	LCD_CS_CLR  GPIOB->BRR=1<<9     //片选端口  	PB9
  #define	LCD_RS_CLR	GPIOB->BRR=1<<8     //数据/命令		PB8	   
  #define	LCD_WR_CLR	GPIOB->BRR=1<<7     //写数据			PB7
  #define	LCD_RD_CLR	GPIOB->BRR=1<<6     //读数据			PB6   

  //PB0~15,作为数据线
  #define DATAOUT(x) do{GPIOC->ODR = x;}while(0) //数据输出
  #define DATAIN()     GPIOC->IDR                //数据输入	

  void LCD_WR_REG(uint16_t data);
  void LCD_WR_DATA(uint16_t data);
  
#endif

//TFT35 V1.0 V1.1 RM68042 8bit
//TFT35 V1.2 ili9488 16bit
//TFT28 TFT24 ili9341 16bit
#if LCD_DATA_16BIT == 1
  #define LCD_WR_16BITS_DATA(c) do{ LCD_WR_DATA(c); }while(0)
#else
  #define LCD_WR_16BITS_DATA(c) do{ LCD_WR_DATA(((c)>>8)&0xFF); LCD_WR_DATA((c)&0xFF); }while(0)	
#endif


//
#define WHITE                 0xFFFF
#define BLACK                 0x0000    
#define BLUE                  0x001F  
#define BRED                  0XF81F
#define GRED                  0XFFE0
#define GBLUE                 0X07FF
#define RED                   0xF800
#define MAGENTA               0xF81F
#define GREEN                 0x07E0
#define CYAN                  0x7FFF
#define YELLOW                0xFFE0
#define BROWN                 0XBC40 //
#define BRRED                 0XFC07 //
#define GRAY                  0X8430 //


void LCD_Init(void);          //

#endif
