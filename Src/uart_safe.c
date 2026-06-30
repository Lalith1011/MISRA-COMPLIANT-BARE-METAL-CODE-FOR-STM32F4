/*
 * uart_safe.c
 *
 *  Created on: Jun 17, 2026
 *      Author: lalithk
 */


#include "stm32f4xx.h"
#include "uart_safe.h"
#include <stdbool.h>



uint8_t is_uart_initialized = 0U;
char    uart_read_buffer[256] = {0};
#define MAJOR_VERSION (1U)
#define MINOR_VERSION (0U)
#define PATCH_VERSION (0U)

#define UART_TRANSMIT_TIMEOUT_IN_TICKS 100000000

static bool Is_PortValid(uart_port_t port)
{
	if(port <= UART_PORT_3)
	{
		return true;
	}

	return false;
}


static bool Is_ParityValid(uart_parity_t parity)
{
	if(parity <= NO_PARITY)
	{
		return true;
	}

	return false;
}

static bool Is_DataBitsLenValid(uart_data_bits_len_t data_bit)
{
	if(data_bit <= DATA_BITS_9)
	{
		return true;
	}

	return false;
}

static bool Is_StopBitsLenValid(uart_stop_bits_t stop_bit)
{
	if(stop_bit <= STOP_BITS_2)
	{
		return true;
	}

	return false;
}

static bool EnableClock(uart_port_t uart_port)
{
	if(!Is_PortValid(uart_port))
	{
		return false;
	}


	switch(uart_port)
	{
	case UART_PORT_1 : RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	break;
	case UART_PORT_2 : RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	break;
	case UART_PORT_3 : RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	break;
	}

	return true;

}

static USART_TypeDef* GetUARTPortAddr(uart_port_t uart_port)
{
	USART_TypeDef *uart_port_addr = NULL;
	switch(uart_port)
	{
	case UART_PORT_1 : uart_port_addr = USART1;
	break;
	case UART_PORT_2 : uart_port_addr = USART2;
	break;
	case UART_PORT_3 : uart_port_addr = USART6;
	break;
	}

	return uart_port_addr;
}

uint16_t compute_uart_div(uint32_t periph_clk, uint32_t baud_rate)
{
    return ((periph_clk + (baud_rate/2)) / baud_rate);
}

void set_baudrate_in_register(USART_TypeDef *USARTX, uint32_t periph_clk, uint32_t baud_rate)
{
	USARTX->BRR = compute_uart_div(periph_clk, baud_rate);
}


Status_code_t UART_Init(uart_config_t *config)
{
	uint32_t Fpclk = 0;
    float value = 0;
    uint16_t mantissa = 0;
    uint8_t fraction = 0;

	if(config == NULL)
	{
		return STATUS_NULL_ERROR;
	}

	if((!Is_PortValid(config->port)) || (!Is_ParityValid(config->parity)) || (!Is_DataBitsLenValid(config->data_bits)) || (!Is_StopBitsLenValid(config->stop_bits)))
	{
		return STATUS_PARAM_ERROR;
	}

    if(!EnableClock(config->port))
    {
    	return STATUS_PARAM_ERROR;
    }

    USART_TypeDef *uart_port_addr = GetUARTPortAddr(config->port);

    /* Get Fpclk */
    if(config->port == UART_PORT_1 || config->port == UART_PORT_3)
    {
    	Fpclk = 16000000; //Compute clock frequency here. In this case it has been calculated from the STM32F401 Datasheet.
    }
    else //For UART_PORT_2
    {
    	Fpclk = 16000000;
    }

    if(config->baudrate == 0)
    {
    	return STATUS_PARAM_ERROR;
    }

    /* Set baudrate in the BRR */
//    value = ((float)Fpclk) / ((16.0f * config->baudrate));

    set_baudrate_in_register(uart_port_addr, 16000000, config->baudrate);
//    mantissa = 8;
////   value = ((value - mantissa) * 16);
//   fraction = 10;
//
//   uart_port_addr->BRR |= (fraction & 0x0F);
//   uart_port_addr->BRR |= (mantissa & 0xFFF0);

   /* Set parity control bit and data_len bit */
   if(config->data_bits == DATA_BITS_9)
   {
	   uart_port_addr->CR1 |= (0x01UL << 12);
	   if(config->parity == EVEN_PARITY)
	   {
		   uart_port_addr->CR1 |= (0x01UL << 10);
		   uart_port_addr->CR1 &= ~(0x01UL << 9);
	   }
	   else if(config->parity == ODD_PARITY)
	   {
		   uart_port_addr->CR1 |= (0x01UL << 10);
		   uart_port_addr->CR1 |= (0x01UL << 9);
	   }
	   else if(config->parity == NO_PARITY)
	   {
		   uart_port_addr->CR1 &= ~(0x01UL << 10);
	   }
   }
   else if(config->data_bits == DATA_BITS_8)
   {
	   uart_port_addr->CR1 &= ~(0x01UL << 10);
	   uart_port_addr->CR1 &= ~(0x01UL << 12);
   }

   /* Set stop bits */
   if(config->stop_bits == STOP_BITS_1)
   {
	   uart_port_addr->CR2 &= ~(0x01UL << 12);
	   uart_port_addr->CR2 &= ~(0x01UL << 13);
   }
   else if(config->stop_bits == STOP_BITS_2)
   {
	   uart_port_addr->CR2 &= ~(0x01UL << 12);
	   uart_port_addr->CR2 |= (0x01UL << 13);
   }

   /* Enable Transmitter and Receiver */
   uart_port_addr->CR1 |= (0x01UL << 2);
   uart_port_addr->CR1 |= (0x01UL << 3);

   /* Enable additional UART hardware */
   uart_port_addr->CR1 |= (0x01UL << 13);

   is_uart_initialized = 1U;

   return STATUS_OK;





}


Status_code_t UART_TransmitByte(uart_config_t *config, uint8_t byte_to_transmit)
{
   if(is_uart_initialized == 0U)
   {
	   return STATUS_INIT_ERROR;
   }

   if(config == NULL)
   {
	   return STATUS_NULL_ERROR;
   }

	if((!Is_PortValid(config->port)) || (!Is_ParityValid(config->parity)) || (!Is_DataBitsLenValid(config->data_bits)) || (!Is_StopBitsLenValid(config->stop_bits)))
	{
		return STATUS_PARAM_ERROR;
	}

	USART_TypeDef *uart_port_addr = GetUARTPortAddr(config->port);

	/* Check if line is busy */
	if(((uart_port_addr->SR >> 0x06) & 0x01) == 0x00)
	{
		return STATUS_BUSY_ERROR;
	}

	/* Place the data in the DR */
    uart_port_addr->DR |= (byte_to_transmit & 0xFF);

    uint32_t i = 0;

    /* Wait for transmission to complete */
    while(((uart_port_addr->SR >> 0x06) & 0x01) == 0x00)
    {
    	i += 1;

    	if(i >= UART_TRANSMIT_TIMEOUT_IN_TICKS)
    	{
    		break;
    	}
    }

    if(i >= UART_TRANSMIT_TIMEOUT_IN_TICKS)
    {
		return STATUS_TIMEOUT_ERROR;
    }

    return STATUS_OK;



}

Status_code_t UART_TransmitString(uart_config_t *config, char *data, uint8_t data_len)
{
	uint32_t tick_count = 0;

	  /* Check if uart is initialized */
	   if(is_uart_initialized == 0U)
	   {
		   return STATUS_INIT_ERROR;
	   }

    /* Check if pointers are null */
	if((config == NULL) || (data == NULL))
	{
		return STATUS_NULL_ERROR;
	}

	/* Check if parameter values have invalid */
	if((data_len == 0) || (config->baudrate == 0))
	{
		return STATUS_PARAM_ERROR;
	}

	if((!Is_PortValid(config->port)) || (!Is_ParityValid(config->parity)) || (!Is_DataBitsLenValid(config->data_bits)) || (!Is_StopBitsLenValid(config->stop_bits)))
	{
		return STATUS_PARAM_ERROR;
	}

	USART_TypeDef *uart_port_addr = GetUARTPortAddr(config->port);

	/* Check if line is busy */
	if(((uart_port_addr->SR >> 0x06) & 0x01) == 0x00)
	{
		return STATUS_BUSY_ERROR;
	}

	for(int i = 0; i < data_len; i ++)
	{
	/* Place the data in the DR */
    uart_port_addr->DR |= (((uint8_t)*(data + i)) & 0xFF);

    tick_count = 0;

    /* Wait for transmission to complete */
    while(((uart_port_addr->SR >> 0x06) & 0x01) == 0x00)
    {
    	tick_count += 1;

    	if(tick_count >= UART_TRANSMIT_TIMEOUT_IN_TICKS)
    	{
    		break;
    	}
    }


	}

	/* If timeout occurs */
    if(tick_count >= UART_TRANSMIT_TIMEOUT_IN_TICKS)
    {
		return STATUS_TIMEOUT_ERROR;
    }

    return STATUS_OK;

}

Status_code_t UART_ReadByte(uart_config_t *config, char *byte_to_read)
{
	  /* Check if uart is initialized */
	   if(is_uart_initialized == 0U)
	   {
		   return STATUS_INIT_ERROR;
	   }

	   /* Check if pointers are null */
	   if((config == NULL) || (byte_to_read == NULL))
	   {
		   return STATUS_NULL_ERROR;
	   }

		/* Check if parameter values have invalid */
		if(config->baudrate == 0)
		{
			return STATUS_PARAM_ERROR;
		}

		if((!Is_PortValid(config->port)) || (!Is_ParityValid(config->parity)) || (!Is_DataBitsLenValid(config->data_bits)) || (!Is_StopBitsLenValid(config->stop_bits)))
		{
			return STATUS_PARAM_ERROR;
		}

		USART_TypeDef *uart_port_addr = GetUARTPortAddr(config->port);

		while(((uart_port_addr->SR >> 5) & 0x01) == 0x00)
		{

		}

		*byte_to_read = (uart_port_addr->DR & 0xFF);

		return STATUS_OK;
}

Status_code_t UART_ReadLine(uart_config_t *config, char *line_to_read, uint8_t *data_len)
{
	  /* Check if uart is initialized */
	   if(is_uart_initialized == 0U)
	   {
		   return STATUS_INIT_ERROR;
	   }

	   /* Check if pointers are null */
	   if((config == NULL) || (data_len == NULL))
	   {
		   return STATUS_NULL_ERROR;
	   }

		/* Check if parameter values have invalid */
		if(config->baudrate == 0)
		{
			return STATUS_PARAM_ERROR;
		}

		if((!Is_PortValid(config->port)) || (!Is_ParityValid(config->parity)) || (!Is_DataBitsLenValid(config->data_bits)) || (!Is_StopBitsLenValid(config->stop_bits)))
		{
			return STATUS_PARAM_ERROR;
		}

		char a;

		USART_TypeDef *uart_port_addr = GetUARTPortAddr(config->port);

		for(int i = 0; i < 256; i ++)
		{

			/* Wait for a byte to be received */
		while(((uart_port_addr->SR >> 5) & 0x01) == 0x00)
		{

		}



		a = (char)(uart_port_addr->DR & 0xFF);
		if((a != '\n') && (a != '\r'))
		{
			line_to_read[i] = a;
			*data_len += 1;
		}
		else
		{
			uint8_t received_data;
			/* Check if any byte is yet to be read and clear the flag*/
			if(((uart_port_addr->SR >> 0x05) & 0x01) == 0x01)
			{
				 received_data = uart_port_addr->DR;
			}

			/* Check if over_run took place and clear the flag */
			if(((uart_port_addr->SR >> 0x03) & 0x01) == 0x01)
			{
				 received_data = uart_port_addr->DR;
				 received_data = uart_port_addr->SR;
			}
						break;
		}

		}

		return STATUS_OK;
}



