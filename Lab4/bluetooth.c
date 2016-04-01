#define ARM_MATH_CM4
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include "math.h"
#include "interthread.h"

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */

#define SPI_FLAG_TIMEOUT         ((uint32_t)0x1000)
__IO uint32_t  SPITimeout = SPI_FLAG_TIMEOUT;
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
SPI_HandleTypeDef    Spi2Handle;

/**
  * @brief  SPI Interface pins
  */
#define SPI                       SPI1
#define SPI_CLK                   RCC_APB2Periph_SPI1

#define SPI_SCK_PIN               GPIO_PIN_5                  /* PA.05 */
#define SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define SPI_SCK_SOURCE            GPIO_PinSource5
#define SPI_SCK_AF                GPIO_AF5_SPI1

#define SPI_MISO_PIN              GPIO_PIN_6                  /* PA.6 */
#define SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define SPI_MISO_SOURCE           GPIO_PinSource6
#define SPI_MISO_AF               GPIO_AF5_SPI1

#define SPI_MOSI_PIN              GPIO_PIN_7                  /* PA.7 */
#define SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define SPI_MOSI_SOURCE           GPIO_PinSource7
#define SPI_MOSI_AF               GPIO_AF5_SPI1

#define SPI_CS_PIN                GPIO_PIN_3                  /* PE.03 */
#define SPI_CS_GPIO_PORT          GPIOE                       /* GPIOE */
#define SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE

#define SPI_INT1_PIN              GPIO_PIN_0                  /* PE.00 */
#define SPI_INT1_GPIO_PORT        GPIOE                       /* GPIOE */
#define SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define SPI_INT1_EXTI_LINE        EXTI_Line0
#define SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource0
#define SPI_INT1_EXTI_IRQn        EXTI0_IRQn

#define SPI_INT2_PIN              GPIO_PIN_1                  /* PE.01 */
#define SPI_INT2_GPIO_PORT        GPIOE                       /* GPIOE */
#define SPI_INT2_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define SPI_INT2_EXTI_LINE        EXTI_Line1
#define SPI_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define SPI_INT2_EXTI_PIN_SOURCE  EXTI_PinSource1
#define SPI_INT2_EXTI_IRQn        EXTI1_IRQn

#define CS_LOW()       HAL_GPIO_WritePin(SPI_CS_GPIO_PORT, SPI_CS_PIN, GPIO_PIN_RESET)
#define CS_HIGH()      HAL_GPIO_WritePin(SPI_CS_GPIO_PORT, SPI_CS_PIN, GPIO_PIN_SET)

// Function def
uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi);
void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data);
static uint8_t SPI_SendByte(uint8_t byte);
void SPI_Init(void);

// Variables + function names

osThreadId tid_Thread_Bluetooth;
void Thread_Bluetooth(void const *argument);
osThreadDef(Thread_Bluetooth, osPriorityNormal, 1, 0);

#define data_ready_flag 1

/**
  * @brief  Start bluetooth thread
  * @param  None
  * @retval int
  */
int start_Thread_Bluetooth	(void){
	tid_Thread_Bluetooth = osThreadCreate(osThread(Thread_Bluetooth), NULL); // Start Bluetooth
  if (!tid_Thread_Bluetooth) return(-1); 
  return(0);
}

/**
  * @brief  Bluetooth thread
  * @param  argument
  * @retval None
  */
void Thread_Bluetooth(void const *argument){
	while(1){
		 // 1 ms 
		// Talk to SPI
		// Send accelerometer, temperature
		// Get angles
		uint8_t * rollArr;
		uint8_t * pitchArr;
		uint8_t * tempArr;
		
		float roll = getSetValue(1,0,0);
		float pitch = getSetValue(1,0,1);
		float temp = getSetValue(1,0,2);
		
		uint8_t testBytesArray[12] = {1,1,1,1,2,1,1,1,3,1,1,1};
		
		osDelay(1);
		// Now how do I transmit these values via SPI?
		// We will need some sort of data encoding on this end:
		// 0 -> roll
		// 1 -> pitch
		// 2 -> temp
		
		rollArr = (uint8_t *) &roll;
		pitchArr = (uint8_t *) &pitch;
		tempArr = (uint8_t *) &temp;
		
		HAL_SPI_Transmit(&Spi2Handle, testBytesArray, 12, SPI_FLAG_TIMEOUT);
		
		/**
		
		testBytesArray[0] = 2;
		
		HAL_SPI_Transmit(&Spi2Handle, testBytesArray, 4, SPI_FLAG_TIMEOUT);
		
		testBytesArray[0] = 3;
		
		HAL_SPI_Transmit(&Spi2Handle, testBytesArray, 4, SPI_FLAG_TIMEOUT);
		
		
		**/
		
		//printf("%f", roll);
		/*
		HAL_SPI_TransmitReceive(&SpiHandle, rollArr, 0, 4, SPI_FLAG_TIMEOUT);
		HAL_SPI_TransmitReceive(&SpiHandle, pitchArr, 0, 4, SPI_FLAG_TIMEOUT);
		HAL_SPI_TransmitReceive(&SpiHandle, tempArr, 0, 4, SPI_FLAG_TIMEOUT);
		*/
		//while (roll) {
			//roll *= 100;
			//i = (int *)&roll;
			
			//uint8_t byteToSend = (int)(roll) & 0xff;
			//roll = roll > 8;
			//HAL_SPI_TransmitReceive(&SpiHandle, iarr, 0, 4, SPI_FLAG_TIMEOUT);
		//}
		
	}
}

void SPI_Init(void)
{
  /* Configure the low level interface ---------------------------------------*/
	  /* SPI configuration -------------------------------------------------------*/
	__HAL_RCC_SPI1_CLK_ENABLE();
	
  HAL_SPI_DeInit(&Spi2Handle);
  Spi2Handle.Instance 							  = SPI2;
  Spi2Handle.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_4; 
  Spi2Handle.Init.Direction 					= SPI_DIRECTION_2LINES;
  Spi2Handle.Init.CLKPhase 					= SPI_PHASE_1EDGE;
  Spi2Handle.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  Spi2Handle.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  Spi2Handle.Init.CRCPolynomial 			= 7;
  Spi2Handle.Init.DataSize 					= SPI_DATASIZE_8BIT;
  Spi2Handle.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
  Spi2Handle.Init.NSS 								= SPI_NSS_SOFT;
  Spi2Handle.Init.TIMode 						= SPI_TIMODE_DISABLED;
  Spi2Handle.Init.Mode 							= SPI_MODE_MASTER;
	
	if (HAL_SPI_Init(&Spi2Handle) != HAL_OK) {printf ("ERROR: Error in initialising SPI1 \n");};
  
	__HAL_SPI_ENABLE(&Spi2Handle);
}

void SPI_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit:
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */ 
  CS_LOW();

  /* Send the Address of the indexed register */
  SPI_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    SPI_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }

  /* Set chip select High at the end of the transmission */
  CS_HIGH();
}



/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t SPI_SendByte(uint8_t byte)
{
  /* Loop while DR register in not empty */
  SPITimeout = SPI_FLAG_TIMEOUT;
  while (__HAL_SPI_GET_FLAG(&Spi2Handle, SPI_FLAG_TXE) == RESET)
  {
    if((SPITimeout--) == 0) return 0; // Timeout
  }

  /* Send a Byte through the SPI peripheral */
  SPI_SendData(&Spi2Handle,  byte);

  /* Wait to receive a Byte */
  SPITimeout = SPI_FLAG_TIMEOUT;
  while (__HAL_SPI_GET_FLAG(&Spi2Handle, SPI_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) {
			return 0; // Timeout
		}
  }

  /* Return the Byte read from the SPI bus */ 
  return SPI_ReceiveData(&Spi2Handle);
}

/**
  * @brief  Returns the most recent received data by the SPIx/I2Sx peripheral. 
  * @param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
  * @retval The value of the received data.
  */
//uint8_t SPI_ReceiveData(SPI_HandleTypeDef *hspi)
//{
  /* Return the data in the DR register */
  //return hspi->Instance->DR;
//}

/**
  * @brief  Transmits a Data through the SPIx/I2Sx peripheral.
  * @param  *hspi: Pointer to the SPI handle. Its member Instance can point to either SPI1, SPI2 or SPI3 
  * @param  Data: Data to be transmitted.
  * @retval None
  */
//void SPI_SendData(SPI_HandleTypeDef *hspi, uint16_t Data)
//{ 
  /* Write in the DR register the data to be sent */
  //hspi->Instance->DR = Data;
//}


