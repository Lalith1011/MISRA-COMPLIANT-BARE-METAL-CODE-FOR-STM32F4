This project features MISRA Compliant Bare-metal code for the STM32F401. It currently consists of the drivers for the GPIO and UART peripherals along with the newly integrated GCS (GPIO CONTROL SYSTEM) built on top of the GPIO and UART drivers. Take a look at the points below to get an overview about the usage of the drivers. 


**STATUS CODES**

The file status_codes.h contains the different status codes the functions will return. For example, 'STATUS_OK' is returned if a function is well executed without any errors. Otherwise, other status codes are returned based on the function execution.
Take a look at this file at 'Inc/status_codes.h' to get a better overview of it. Create the variable of the type 'status_code_t' and use it to know the status of the function execution. 



**GPIO DRIVER**

The MISRA Compliant Bare-metal GPIO driver can be accessed via the 'gpio_safe.h' file located at 'Inc/gpio_safe.h'. The driver has to be initialized before calling any other function and it can be done using the 
'GPIO_Init()' function. Once initialized, call the 'GPIO_PinInit()' to set a pin's functionality. The other functions to follow will depend on the way a pin is initialized. Refer to the comments for each of the function to get a clear understanding. 


**UART DRIVER**

The MISRA Compliant Bare-metal GPIO driver can be accessed via the 'uart_safe.h' file located at 'Inc/uart_safe.h'. The UART port can be configured by calling the 'UART_Init()' function. Configure the port by using a variable of the structure type 'uart_config_t'. Use the other functions based on the functionality to be implemented. Refer to the comments for each of the function to get a clear understanding. 

Note : The 'FPCLK' used to compute the baudrate is currently hardcoded in the firmware. This has to be varied based on the UART peripheral being used and the chip (Refer to its datasheet for more info).


**GCS**

Gpio Control System (GCS) is a system that can be used to configure the GPIO pins to set/read the output/input states of the GPIOs on the STM32F4 MCU via UART Serial Port. The below commands are to be used in sequence to setup the system and obtain a detailed overview of the additional system commands. 

- 'Intialize System' : This command has to be called first to start the system.
- '- H' : This command fetches a detailed overview of the system. It contains the list of commands the user can execute along with their responses. 
