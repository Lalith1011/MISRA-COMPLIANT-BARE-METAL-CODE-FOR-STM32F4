
/*
 * @file uart_safe.h
 * @brief This file is a .h file for the safe uart driver implemented using MISRA-C guidelines for safety critical applications.
 * @author Lalith.K
 */

#ifndef UART_SAFE_H
#define UART_SAFE_H

#include <status_codes.h>
#include <stdint.h>
#include <stdio.h>

typedef enum
{
	UART_PORT_1, /* USART1 : PA9 - RX, PA10 - TX */
	UART_PORT_2, /* USART2 : PA3 - RX, PA2 - TX */
	UART_PORT_3  /* USART6 : PC7 - RX, PC6 - TX */
}uart_port_t;

typedef enum
{
	DATA_BITS_8,
	DATA_BITS_9
}uart_data_bits_len_t;

typedef enum
{
	STOP_BITS_1,
	STOP_BITS_2
}uart_stop_bits_t;


typedef enum
{
	ODD_PARITY,
	EVEN_PARITY,
	NO_PARITY
}uart_parity_t;

typedef struct
{
	uart_port_t port;
	uint32_t baudrate;
	uart_parity_t parity;
	uart_data_bits_len_t data_bits;
	uart_stop_bits_t stop_bits;
}uart_config_t;



/*
 * @brief This function is used to initialize UART driver and configure a UART port with the provided configurations.
 * @param[in] config : This parameter is of the type uart_config_t and contains the parameters for uart port configuration.
 * @return Status_code_t
 * @retval STATUS_OK : If configuration is successful.
 * @retval STATUS_NULL_ERROR : If null pointer is passed.
 * @retval STATUS_PARAM_ERROR : If an invalid parameter is passed.
 */
Status_code_t UART_Init(uart_config_t *config);


/*
 * @brief This function is used to transmit a byte from the configured UART port.
 * @param[in] config : This parameter is a pointer of the type uart_config_t and contains the information to transmit data from the provided port.
 * @param[in] byte_to_transmit : This parameter is the data_byte to be transmitted over UART.
 * @return Status_code_t
 * @retval STATUS_OK : If transmission is successful.
 * @retval STATUS_NULL_ERROR : If the passed config parameter is NULL.
 * @retval STATUS_PARAM_ERROR : If invalid parameter is passed.
 * @retval STATUS_TIMEOUT_ERROR : If a timeout occurs.
 * @retval STATUS_BUSY_ERROR : If TX line is busy.
 */
Status_code_t UART_TransmitByte(uart_config_t *config, uint8_t byte_to_transmit);

/*
 * @brief This function is used to transmit a byte from the configured UART port.
 * @param[in] config : This parameter is a pointer of the type uart_config_t and contains the information to transmit data from the provided port.
 * @param[in] data : This parameter is the pointer to the string to be transmitted over UART.
 * @return Status_code_t
 * @retval STATUS_OK : If transmission is successful
 * @retval STATUS_NULL_ERROR : If the passed config parameter or data pointer is NULL.
 * @retval STATUS_PARAM_ERROR : If invalid parameter is passed.
 * @retval STATUS_TIMEOUT_ERROR : If a timeout occurs.
 */
Status_code_t UART_TransmitString(uart_config_t *config, char *data, uint16_t data_len);

/*
 * @brief This function is used to read a byte from the configured UART port.
 * @param[in] config : This parameter is a pointer of the type uart_config_t and contains the information to transmit data from the provided port.
 * @param[in] byte_to_read : This parameter is a pointer that holds the address to the data_byte that is read over UART.
 * @return Status_code_t
 * @retval STATUS_OK : If transmission is successful
 * @retval STATUS_NULL_ERROR : If the passed config parameter or byte_to_read pointer is NULL.
 * @retval STATUS_PARAM_ERROR : If invalid parameter is passed.
 */
Status_code_t UART_ReadByte(uart_config_t *config, char *byte_to_read);

/*
 * @brief This function is used to read a line (string) from the configured UART port.
 * @param[in] config : This parameter is a pointer of the type uart_config_t and contains the information to transmit data from the provided port.
 * @param[in] line_to_read : This parameter is a pointer that holds the address to the string/line that is read over UART.
 * @param[in] data_len : The length of the string that is to be read.
 * @return Status_code_t
 * @retval STATUS_OK : If transmission is successful
 * @retval STATUS_NULL_ERROR : If the passed config parameter or line_to_read pointer is NULL.
 * @retval STATUS_PARAM_ERROR : If invalid parameter is passed.
 * @retval STATUS_TIMEOUT_ERROR : If a timeout occurs.
 *
 */
Status_code_t UART_ReadLine(uart_config_t *config, char *line_to_read, uint16_t *data_len);


/*
 * @brief This function is used to de-initialize the UART driver.
 * @return Status_code_t
 * @retval STATUS_OK : If deinit is successful.
 */
Status_code_t UART_DeInit();

/*
 * @brief This function is used to fetch the version of the UART driver.
 * @param[in] major : Pointer to the major version number of UART driver.
 * @param[in] minor : Pointer to the minor version number of UART driver.
 * @param[in] patch : Pointer to the patch version number of UART driver.
 * @return Status_code_t
 * @retval STATUS_OK : If transmission is successful
 * @retval STATUS_NULL_ERROR : If any of the passed pointers is NULL.
 */
Status_code_t UART_Driver_Version(uint8_t *major, uint8_t *minor, uint8_t *patch);
#endif
