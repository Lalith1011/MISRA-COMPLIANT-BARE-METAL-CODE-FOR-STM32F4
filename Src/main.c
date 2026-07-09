#include "stm32f4xx.h"
#include "uart_safe.h"
#include <string.h>
#include "gpio_control_system.h"
void delay()
{
	for(int i = 0; i < 1000000; i ++)
	{

	}
}

uint32_t input_register_value = 0;
uint32_t pin_state = 0;
gpio_config_t config = {0};
gpio_config_t input_config = {0};
uart_config_t uart_port_config = {0};
Status_code_t status;
uint8_t gpio_version_major = 0;
uint8_t gpio_version_minor = 0;
uint8_t gpio_version_patch = 0;
int test = 0;
uint16_t data_len_uart = 0;
char data_to_read[256];
char helper_string[2048];
uint16_t helper_string_size = 0;
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


  uart_port_config.baudrate = 115200;
  uart_port_config.data_bits = DATA_BITS_8;
  uart_port_config.parity = NO_PARITY;
  uart_port_config.port = UART_PORT_2;
  uart_port_config.stop_bits = STOP_BITS_1;
//
  status = UART_Init(&uart_port_config);
  if(status == STATUS_OK)
  {
	  test = 2;


		  test = 3;
		  while(1)
		  {
//		  UART_ReadByte(&uart_port_config, &uart_read_byte);
		  status = UART_ReadLine(&uart_port_config, data_to_read, &data_len_uart);
		  data_to_read[data_len_uart] = '\0';
//		  delay();
		  if(status == STATUS_OK)
		  {
             if(strcmp(data_to_read, "Initialize System") == 0)
             {
                 System_status_code_t sys_status = GCS_Init();
                 if(sys_status == STATUS_EXECUTION_SUCCESSFUL)
                 {
                 char resp[512] = "\nInit Successful, Press \"- H\" to get a detailed overview of the system\n\n";
                 UART_TransmitString(&uart_port_config, resp, strlen(resp));
                 }
                 else
                 {
                	 char resp[100] = "\nInit Failed, Kindly try after sometime\n\n";
                	 UART_TransmitString(&uart_port_config, resp, strlen(resp));
                 }
             }
             else if(strcmp(data_to_read, "- H") == 0)
             {

            	 System_status_code_t sys_status = GCS_Helper(helper_string);
            	 if(sys_status == STATUS_EXECUTION_SUCCESSFUL)
            	 {
           		  test = 4;
           		helper_string_size = strlen(helper_string);
            		 status = UART_TransmitString(&uart_port_config, helper_string, helper_string_size);
            		 if(status != STATUS_OK)
            		 {
//                		 status = UART_TransmitString(&uart_port_config, helper_string, strlen(helper_string));
            			 test = 5;
            		 }
            		 else
            		 {
            			 test = 6;
            		 }
            	 }
            	 else
            	 {
            		 test = 7;
            		 char response[100] = "\nUNABLE TO FETCH \"HELPER\"\n";
            		 UART_TransmitString(&uart_port_config, response, strlen(response));
            	 }


             }
             else if(strstr(data_to_read,"COMMAND") != 0) /* Command Received, Parse and execute it */
             {
            	 char response[300] = "";
            	 GCS_CommandParser(data_to_read, response);
            	 UART_TransmitString(&uart_port_config, response, strlen(response));
             }

             data_len_uart = 0;


		  }
		  }
  }


  }

#ifdef test
  GPIO_Init();
  gpio_config_t config;
  gpio_config_t input_config;
  gpio_pin_state_t btn_state;
  config.port = GPIO_PORT_A;
  config.pin = GPIO_PIN_5;
  config.pull_up_or_down = GPIO_NO_PULL_UP_OR_DOWN;
  config.output_type = GPIO_OUTPUT_PUSH_PULL;
  config.state = GPIO_MODE_OUTPUT;
  config.output_speed = GPIO_LOW_SPEED;
  GPIO_PinInit(&config);

  input_config.port = GPIO_PORT_C;
  input_config.pin = GPIO_PIN_13;
  input_config.pull_up_or_down = GPIO_PULL_UP;
  input_config.state = GPIO_MODE_INPUT;
  GPIO_PinInit(&input_config);

        while(1)
        {
         if(GPIO_PinRead(input_config.port, input_config.pin, &btn_state) == STATUS_OK)
  		{
      	   if(GPIO_PIN_RESET == btn_state)
      	   {
      		   test = 3;
      		   GPIO_PinWrite(config.port, config.pin, GPIO_PIN_SET);
      	   }
      	   else
      	   {
      		   test = 4;
      		   GPIO_PinWrite(config.port, config.pin, GPIO_PIN_RESET);

      	   }
  		}
        }
#endif
	 return 0;
}
