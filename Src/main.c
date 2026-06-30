#include "stm32f4xx.h"
#include "gpio_safe.h"
#include "uart_safe.h"
#include <string.h>

void delay()
{
	for(int i = 0; i < 1000000; i ++)
	{

	}
}


uint32_t pin_state = 0;
gpio_config_t config = {0};
gpio_config_t input_config = {0};
uart_config_t uart_port_config = {0};
Status_code_t status;
uint8_t gpio_version_major = 0;
uint8_t gpio_version_minor = 0;
uint8_t gpio_version_patch = 0;
int test = 0;
uint8_t data_len_uart = 0;
char data_to_read[256];
char uart_read_byte = ' ';
int main(void)
{

  if(GPIO_Init() == STATUS_OK)
  {
	  /* UART Pins config */
  config.port = GPIO_PORT_A;
  config.pin = GPIO_PIN_2;
  config.state = GPIO_MODE_ALTERNATE_FUNCTION;
  config.pull_up_or_down = GPIO_NO_PULL_UP_OR_DOWN;
  config.output_type = GPIO_OUTPUT_PUSH_PULL;
  config.output_speed = GPIO_LOW_SPEED;
  config.alternate_function = ALTERNATE_FUNCTION_7;

  status = GPIO_PinInit(&config);

  input_config.port = GPIO_PORT_A;
  input_config.pin = GPIO_PIN_3;
  input_config.state = GPIO_MODE_ALTERNATE_FUNCTION;
  input_config.pull_up_or_down = GPIO_NO_PULL_UP_OR_DOWN;
  input_config.output_type = GPIO_OUTPUT_PUSH_PULL;
  input_config.output_speed = GPIO_LOW_SPEED;
  input_config.alternate_function = ALTERNATE_FUNCTION_7;

  status = GPIO_PinInit(&input_config);

  /* LED pin PA5 */
  config.port = GPIO_PORT_A;
  config.pin = GPIO_PIN_5;
  config.state = GPIO_MODE_OUTPUT;
  config.pull_up_or_down = GPIO_NO_PULL_UP_OR_DOWN;
  config.output_type = GPIO_OUTPUT_PUSH_PULL;
  config.output_speed = GPIO_LOW_SPEED;

  status = GPIO_PinInit(&config);

  uart_port_config.baudrate = 115200;
  uart_port_config.data_bits = DATA_BITS_8;
  uart_port_config.parity = NO_PARITY;
  uart_port_config.port = UART_PORT_2;
  uart_port_config.stop_bits = STOP_BITS_1;

  status = UART_Init(&uart_port_config);
  if(status == STATUS_OK)
  {
	  test = 2;

//	  char *data_to_transmit = "This is UART driver based on MISRA-C\n";
//	   data_len_uart = strlen(data_to_transmit);
//	  status = UART_TransmitString(&uart_port_config, data_to_transmit, data_len_uart);

//	  if(status == STATUS_OK)
//	  {
		  test = 3;
		  while(1)
		  {
//		  UART_ReadByte(&uart_port_config, &uart_read_byte);
		  status = UART_ReadLine(&uart_port_config, data_to_read, &data_len_uart);
		  data_to_read[data_len_uart] = '\0';
//		  delay();
		  if(status == STATUS_OK)
		  {
			  test = 4;
			  if(strcmp(data_to_read, "TURN_ON") == 0)
			  {
				  test = 5;
				  GPIO_PinWrite(GPIO_PORT_A, GPIO_PIN_5, GPIO_PIN_SET);
				  strcpy(data_to_read, "");
			  }
			  else if(strcmp(data_to_read, "TURN_OFF") == 0)
			  {
				  test = 6;
				  GPIO_PinWrite(GPIO_PORT_A, GPIO_PIN_5, GPIO_PIN_RESET);
				  strcpy(data_to_read, "");

			  }

		  }
		  }
//	  }
	  /* Test all the functions here */
//      while(1)
//      {
//       if(GPIO_PinRead(input_config.port, input_config.pin, &btn_state) == STATUS_OK)
//		{
//    	   if(GPIO_PIN_RESET == btn_state)
//    	   {
//    		   test = 3;
//    		   GPIO_PinWrite(config.port, config.pin, GPIO_PIN_SET);
//    	   }
//    	   else
//    	   {
//    		   test = 4;
//    		   GPIO_PinWrite(config.port, config.pin, GPIO_PIN_RESET);
//
//    	   }
//		}
//      }
//	  delay();
//	  GPIO_PinWrite(config.port, config.pin, GPIO_PIN_RESET);
  }


  }
	 return 0;
}
