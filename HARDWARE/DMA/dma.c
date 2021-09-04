#include "dma.h"
#include "usart3.h"


u16 receivebuf[5];

static void _uart3_dma_configuration()
{
  DMA_InitTypeDef DMA_InitStructure;
 
  /* DMA1 Channel 6 (triggered by USART1 Rx event) Config */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 ,ENABLE);
 
  /* DMA1 Channel5 (triggered by USART3 Rx event) Config */
  DMA_DeInit(DMA1_Channel3);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;// 初始化外设地址，相当于“哪家快递”  
  DMA_InitStructure.DMA_MemoryBaseAddr =(u32)receivebuf;// 内存地址，相当于几号柜
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为数据来源，即为收快递
  DMA_InitStructure.DMA_BufferSize = DMASIZE ;// 缓存容量，即柜子大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不递增，即柜子对应的快递不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 内存递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设字节宽度，即快递运输快件大小度量（按重量算，还是按体积算） 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 内存字节宽度，即店主封装快递的度量(按重量，还是按体质进行封装)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 正常模式，即满了就不在接收了，而不是循环存储
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;// 优先级很高，对应快递就是加急
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 内存与外设通信，而非内存到内存 
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);// 把参数初始化，即拟好与快递公司的协议
 
  DMA_Cmd(DMA1_Channel3, ENABLE);// 启动DMA，即与快递公司签订合同，正式生效
 
//  /* DMA1 Channel4 (triggered by USART1 Tx event) Config */
//  DMA_DeInit(DMA1_Channel3);
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;  // 外设地址，串口1， 即发件的快递
//  DMA_InitStructure.DMA_MemoryBaseAddr =(u32)sendbuf;// 发送内存地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;// 外设为传送数据目的地，即发送数据，即快递是发件
//  DMA_InitStructure.DMA_BufferSize = 0;  //发送长度为0，即未有快递需要发送
//  DMA_Init(DMA1_Channel3, &DMA_InitStructure);//初始化
 
  USART_ITConfig(USART3, USART_IT_TC, ENABLE);// 使能串口发送完成中断
  USART_DMACmd(USART3, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);// 使能DMA串口发送和接受请求
}


