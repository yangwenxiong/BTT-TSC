#include "includes.h"
#include "spi_slave.h"

//TODO:
//now support SPI2 and PB12 CS only
//more compatibility changes are needed
//Config for SPI Channel
#if ST7920_SPI == _SPI1
  #define ST7920_SPI_NUM          SPI1
  #define W25QXX_SPI_DMA_RCC_AHB  RCC_AHBPeriph_DMA1
  #define W25QXX_SPI_DMA          DMA1
  #define W25QXX_SPI_DMA_CHANNEL  DMA1_Channel2
  #define W25QXX_SPI_DMA_IFCR_BIT 5
#elif ST7920_SPI == _SPI2
  #define ST7920_SPI_NUM          SPI2
  #define W25QXX_SPI_DMA          DMA1
  #define W25QXX_SPI_DMA_RCC_AHB  RCC_AHBPeriph_DMA1
  #define W25QXX_SPI_DMA_CHANNEL  DMA1_Channel4
  #define W25QXX_SPI_DMA_IFCR_BIT 13
#elif ST7920_SPI == _SPI3
  #define W25QXX_SPI_NUM          SPI3
  #define W25QXX_SPI_DMA          DMA2
  #define W25QXX_SPI_DMA_RCC_AHB  RCC_AHBPeriph_DMA2
  #define W25QXX_SPI_DMA_CHANNEL  DMA2_Channel1
  #define W25QXX_SPI_DMA_IFCR_BIT 1
#endif

//#define _SPI_SLAVE_IRQ(n)  n##_IRQHandler
//#define SPI_SLAVE_IRQ  _SPI_SLAVE_IRQ(W25QXX_SPI_NUM)

SPI_QUEUE SPISlave;

void SPI_ReEnable(u8 mode)
{
  ST7920_SPI_NUM->CR1 = (0<<15)  //0:˫��˫�� 1:����˫��
                      | (0<<14)  //����˫��ʱʹ�ã�0:ֻ�� 1:ֻ����
                      | (0<<13)  //1:����Ӳ��CRC
                      | (0<<12)  //0:��һ�����͵�ֵ���Է��ͻ����� 1:��һ�����͵�ֵ���Է���CRC�Ĵ�����
                      | (0<<11)  //0:8λ����֡ 1:16λ����֡
                      | (1<<10)  //0:ȫ˫�� 1:ֻ����
                      | (1<<9)   //0:Ӳ��NSS 1:���NSS
                      | (0<<8)   //SSMλΪ��1��ʱ�����塣��������NSS�ϵĵ�ƽ
                      | (0<<7)   //0:MSB 1:LSB
                      | (7<<3)   //bit3-5   000�� fPCLK/2 001�� fPCLK/4 010�� fPCLK/8 011�� fPCLK/16
                                 //         100�� fPCLK/32 101�� fPCLK/64 110�� fPCLK/128 111�� fPCLK/256
                      | (0<<2)   //0:���豸 1:���豸
                      | (mode<<1)   //CPOL
                      | (mode<<0);  //CPHA
            
  ST7920_SPI_NUM->CR2 |= 1<<6;  //�����ж�ʹ�� SPI_I2S_IT_RXNE
}

void SPI_Slave(void) 
{ 
  GPIO_InitTypeDef GPIO_InitStructure; 
  NVIC_InitTypeDef   NVIC_InitStructure; 

  //Enable SPI2 clock and GPIO clock for SPI2 and SPI 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); 
  //IO��ʼ�� 

  //Configure SPI2 pins: SCK, MISO and MOSI 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

  SPI_ReEnable(1);
}

void SPI_SlaveDeInit(void)
{  
  NVIC_InitTypeDef   NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE); 
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);  
}


void SPI2_IRQHandler(void) 
{ 
  SPISlave.data[SPISlave.wIndex] =  ST7920_SPI_NUM->DR;
  SPISlave.wIndex = (SPISlave.wIndex + 1) % SPI_SLAVE_MAX;
} 

/* �ⲿ�ж����� */
void SPI_Slave_CS_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure; 

  /* ��GPIOA_0���ж������� */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

  /* �����ж���0λ�ⲿ�½����ж� */
  EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);


  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure); 
}


/* �ⲿ�ж� */
void EXTI15_10_IRQHandler(void)
{
  if((GPIOB->IDR & (1<<12)) != 0)
  {
    SPI_ReEnable(!!(GPIOB->IDR & (1<<13))); //����Ӧ spi mode0/mode3
    ST7920_SPI_NUM->CR1 |= (1<<6);
  }
  else
  {
    RCC->APB1RSTR |= 1<<14;	//��λSPI1
    RCC->APB1RSTR &= ~(1<<14);
  }
/* ����ж�״̬�Ĵ��� */
  EXTI->PR = 1<<12;
}
