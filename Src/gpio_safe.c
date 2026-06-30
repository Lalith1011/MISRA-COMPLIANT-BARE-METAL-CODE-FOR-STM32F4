/*
 * gpio_safe.c
 *
 *  Created on: May 14, 2026
 *      Author: lalithk
 *
 *  @brief This is the .c file of the gpio driver implemented based on MISRA C guidelines for a safe register access process.
 *  @version 1.0.0
 */

#include "gpio_safe.h"
#include "stm32f4xx.h"
#include <stddef.h>
#include <stdbool.h>

/* File Version */
#define MAJOR_VERSION (1U)
#define MINOR_VERSION (0U)
#define PATCH_VERSION (0U)

/* Internal Constants */
#define GPIO_MODE_BITS (2U)
#define GPIO_OTYPE_BITS (1U)
#define GPIO_OSPEED_BITS (2U)
#define GPIO_PUPD_BITS (2U)
#define GPIO_AFR_BITS (4U)

/* GPIO PIN/PORT MASK */
#define GPIO_PIN_MASK(pin) (0x01UL << (pin))

static uint8_t gpio_initialised = 0U;

/* Pin initialisation and output count */
static uint8_t gpio_output_pins[GPIO_PORT_COUNT][GPIO_PIN_COUNT] = { 0U };
static uint8_t gpio_initialised_pins[GPIO_PORT_COUNT][GPIO_PIN_COUNT] = { 0U };

/*
 * @brief GPIO get port register address
 * This function provides the Gpio port register for the provided port from enum.
 *
 * @param[in] port: The port name from the enum gpio_port_t
 * @return : This function returns a pointer of the type GPIO_TypeDef*
 */
static GPIO_TypeDef* GPIO_GetPortRegister(gpio_port_t port) {
	GPIO_TypeDef *gpio_regs = NULL;
	switch (port) {
	case GPIO_PORT_A:
		gpio_regs = GPIOA;
		break;
	case GPIO_PORT_B:
		gpio_regs = GPIOB;
		break;
	case GPIO_PORT_C:
		gpio_regs = GPIOC;
		break;
	case GPIO_PORT_D:
		gpio_regs = GPIOD;
		break;
	case GPIO_PORT_E:
		gpio_regs = GPIOE;
		break;
	case GPIO_PORT_H:
		gpio_regs = GPIOH;
		break;
	case GPIO_PORT_COUNT:
		gpio_regs = NULL;
		break;
	default:
		gpio_regs = NULL;
		break;
	}

	return gpio_regs;
}

/*
 * @brief This function is used to check if a port is valid or not.
 *
 * @param[in] port: This param specifies the port from the enum gpio_port_t
 * @param[in] pin: This param specifies the port from the enum gpio_pin_t
 *
 * @return : If the port and pin are valid, it returns true else it returns false.
 */
static bool GPIO_IsPortAndPinValid(gpio_port_t port, gpio_pin_t pin) {
	if (port < GPIO_PORT_COUNT) {
		if (port == GPIO_PORT_RESERVED1) /* Invalid port */
		{
			return false;
		} else /* valid port */
		{
			if (port == GPIO_PORT_H) {
				if (pin >= GPIO_PIN_2) /* Invalid pin for Port H */
				{
					return false;
				} else /* Valid pin for Port H */
				{
					return true;
				}
			} else /* For other ports */
			{
				if (pin < GPIO_PIN_COUNT) /* Valid pin */
				{
					return true;
				} else /* Invalid pin */
				{
					return false;
				}
			}
		}

	} else /* Invalid port */
	{
		return false;
	}

}

/*
 * @brief This function is used to check if the alternate function number is valid or not.
 *
 * @param [in] alternate_function_number: This param specifies the alternate_function_number of a I/O pin.
 *
 * @return bool
 * @retval true if Alternate function is valid
 * @retval false if Alternate function is invalid.
 */
static bool GPIO_IsAlternateFunctionValid(
		gpio_alternate_function_t alternate_function_number) {

	if (alternate_function_number <= ALTERNATE_FUNCTION_15) {
		return true;
	} else {
		return false;
	}
}

/*
 * @brief Check if Output Type is valid or not.
 *
 * @param[in] output_type : This input is of the type gpio_output_type_t that specifies the
 *
 * @return bool
 * @retval true if output_type is valid
 * @retval false if output_type is invalid
 */
static bool GPIO_IsOutputTypeValid(gpio_output_type_t output_type) {
	if (output_type <= GPIO_OUTPUT_OPEN_DRAIN) {
		return true;
	} else {
		return false;
	}
}

/*
 * @brief Check if Output Speed is valid or not.
 *
 * @param[in] output_speed : This input is of the type gpio_output_speed_t that specifies the output_speed of an I/O pin.
 *
 * @return bool
 * @retval true if output_speed is valid
 * @retval false if output_speed is invalid
 */
static bool GPIO_IsOutputSpeedValid(gpio_output_speed_t output_speed) {
	if (output_speed <= GPIO_VERY_HIGH_SPEED) {
		return true;
	} else {
		return false;
	}
}

/*
 * @brief Check if State is valid or not.
 *
 * @param[in] state : This input is of the type gpio_state_t that specifies the state of the pin.
 *
 * @return bool
 * @retval true if state is valid
 * @retval false if state is invalid
 */
static bool GPIO_IsStateValid(gpio_state_t state) {
	if (state <= GPIO_MODE_ANALOG) {
		return true;
	} else {
		return false;
	}
}

/*
 * @brief Check if the pull_up_or_down param is valid or not.
 *
 * @param[in] pull_up_or_down : This input is of the type gpio_pull_up_down_t that specifies the pull_up_or_down for an I/O.
 *
 * @return bool
 * @retval true if pull_up_or_down is valid
 * @retval false if pull_up_or_down is invalid
 */
static bool GPIO_IsPullUpOrDownValid(gpio_pull_up_down_t pull_up_or_down) {
	if (pull_up_or_down <= GPIO_PULL_DOWN) {
		return true;
	} else {
		return false;
	}
}

/*
 * @brief This function is used to enable the clock to a gpio port.
 * @param[in] port : This parameter is of the type gpio_port_t and is used to specify the port to which the clock has to be enabled.
 * @return Status_code_t
 * @retval STATUS_PARAM_ERROR if parameter is invalid
 * @retval STATUS_OK if parameter is valid and clock is set.
 */
static Status_code_t GPIO_EnableClock(gpio_port_t port) {
	Status_code_t code = STATUS_OK;

	if (!GPIO_IsPortAndPinValid(port, 0)) {
		code = STATUS_PARAM_ERROR;
	} else {
		switch (port) {
		case GPIO_PORT_A:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
			break;
		case GPIO_PORT_B:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
			break;
		case GPIO_PORT_C:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
			break;
		case GPIO_PORT_D:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
			break;
		case GPIO_PORT_E:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
			break;
		case GPIO_PORT_H:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
			break;
		}

	}

	return code;
}

static void GPIO_SetAlternateFunction(GPIO_TypeDef *gpio_peripheral,
		gpio_pin_t pin, gpio_alternate_function_t alternate_function) {
	int i = 0;

	if (pin > GPIO_PIN_7) {
		i = 1;
	} else {
		i = 0;
	}
	switch (alternate_function) {
	case ALTERNATE_FUNCTION_0:
		gpio_peripheral->AFR[i] &= ~((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_1:
		gpio_peripheral->AFR[i] |= ((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_2:
		gpio_peripheral->AFR[i] &= ~((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_3:
		gpio_peripheral->AFR[i] |= ((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_4:
		gpio_peripheral->AFR[i] &= ~((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_5:
		gpio_peripheral->AFR[i] |= ((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_6:
		gpio_peripheral->AFR[i] &= ~((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_7:
		gpio_peripheral->AFR[i] |= ((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_8:
		gpio_peripheral->AFR[i] &= ~((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_9:
		gpio_peripheral->AFR[i] |= ((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_10:
		gpio_peripheral->AFR[i] &= ~((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_11:
		gpio_peripheral->AFR[i] |= ((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_12:
		gpio_peripheral->AFR[i] &= ~((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_13:
		gpio_peripheral->AFR[i] |= ((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] &= ~((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_14:
		gpio_peripheral->AFR[i] &= ~((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	case ALTERNATE_FUNCTION_15:
		gpio_peripheral->AFR[i] |= ((0x01UL) << (pin * GPIO_AFR_BITS));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 1));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 2));
		gpio_peripheral->AFR[i] |= ((0x01UL) << ((pin * GPIO_AFR_BITS) + 3));
		break;

	}

}

Status_code_t GPIO_Init(void) {
	Status_code_t init_code = STATUS_OK;

	/* Reset both the arrays */
	for(int j = 0; j < GPIO_PORT_COUNT; j ++)
	{
	for (int i = 0; i < 15; i++) {
		gpio_output_pins[j][i] = 0;
		gpio_initialised_pins[j][i] = 0;
	}
	}

	/* Set the variable as driver initialised */
	gpio_initialised = 1U;

	return init_code;

}

Status_code_t GPIO_PinInit(gpio_config_t *config) {

	if (!gpio_initialised) /* check if gpio is initialised */
	{
		return STATUS_INIT_ERROR;
	}


	if (config == NULL) {
		return STATUS_NULL_ERROR;
	}

	if (!GPIO_IsPortAndPinValid(config->port, config->pin)
			|| !GPIO_IsStateValid(config->state)) {
		return STATUS_PARAM_ERROR;
	}

	if ((config->state == GPIO_MODE_OUTPUT)
			&& (!GPIO_IsOutputTypeValid(config->output_type)
					|| !GPIO_IsOutputSpeedValid(config->output_speed))) {
		return STATUS_PARAM_ERROR;
	}

	if ((config->state == GPIO_MODE_ALTERNATE_FUNCTION)
			&& (!GPIO_IsAlternateFunctionValid(config->alternate_function))) {
		return STATUS_PARAM_ERROR;
	}

	if (!GPIO_IsPullUpOrDownValid(config->pull_up_or_down)) {
		return STATUS_PARAM_ERROR;
	}

	GPIO_TypeDef *gpio_peripheral = GPIO_GetPortRegister(config->port);

	/* Enable the clock to the port */
	if (GPIO_EnableClock(config->port) != STATUS_OK) {
		return STATUS_INIT_ERROR;
	}

	/* Set the mode and their respective settings*/
	if (config->state == GPIO_MODE_INPUT) {
		gpio_peripheral->MODER &= ~((0x01UL) << (config->pin * GPIO_MODE_BITS));
		gpio_peripheral->MODER &= ~((0x01UL)
				<< ((config->pin * GPIO_MODE_BITS) + 1));
	} else if (config->state == GPIO_MODE_OUTPUT) {
		gpio_output_pins[config->port][config->pin] = 1U;
		gpio_peripheral->MODER |= ((0x01UL) << (config->pin * GPIO_MODE_BITS));
		gpio_peripheral->MODER &= ~((0x01UL)
				<< ((config->pin * GPIO_MODE_BITS) + 1));

		/*Set OutputType*/
		if (config->output_type == GPIO_OUTPUT_PUSH_PULL) {
			gpio_peripheral->OTYPER &= ~((0x01UL)
					<< (config->pin * GPIO_OTYPE_BITS));
		} else if (config->output_type == GPIO_OUTPUT_OPEN_DRAIN) {
			gpio_peripheral->OTYPER |= ((0x01UL)
					<< (config->pin * GPIO_OTYPE_BITS));
		}

		/*Set OutputSpeed*/
		if (config->output_speed == GPIO_LOW_SPEED) {
			gpio_peripheral->OSPEEDR &= ~((0x01UL)
					<< (config->pin * GPIO_OSPEED_BITS));
			gpio_peripheral->OSPEEDR &= ~((0x01UL)
					<< ((config->pin * GPIO_OSPEED_BITS) + 1));
		} else if (config->output_speed == GPIO_MEDIUM_SPEED) {
			gpio_peripheral->OSPEEDR |= ((0x01UL)
					<< (config->pin * GPIO_OSPEED_BITS));
			gpio_peripheral->OSPEEDR &= ~((0x01UL)
					<< ((config->pin * GPIO_OSPEED_BITS) + 1));
		} else if (config->output_speed == GPIO_HIGH_SPEED) {
			gpio_peripheral->OSPEEDR &= ~((0x01UL)
					<< (config->pin * GPIO_OSPEED_BITS));
			gpio_peripheral->OSPEEDR |= ((0x01UL)
					<< ((config->pin * GPIO_OSPEED_BITS) + 1));
		} else if (config->output_speed == GPIO_VERY_HIGH_SPEED) {
			gpio_peripheral->OSPEEDR |= ((0x01UL)
					<< (config->pin * GPIO_OSPEED_BITS));
			gpio_peripheral->OSPEEDR |= ((0x01UL)
					<< ((config->pin * GPIO_OSPEED_BITS) + 1));
		}
	} else if (config->state == GPIO_MODE_ALTERNATE_FUNCTION) {
		gpio_peripheral->MODER &= ~((0x01UL) << (config->pin * GPIO_MODE_BITS));
		gpio_peripheral->MODER |= ((0x01UL)
				<< ((config->pin * GPIO_MODE_BITS) + 1));

		GPIO_SetAlternateFunction(gpio_peripheral, config->pin,
				config->alternate_function);

	} else if (config->state == GPIO_MODE_ANALOG) {
		gpio_peripheral->MODER |= ((0x01UL) << (config->pin * GPIO_MODE_BITS));
		gpio_peripheral->MODER |= ((0x01UL)
				<< ((config->pin * GPIO_MODE_BITS) + 1));
	}

	/* set pull up/down setting */

	if (config->pull_up_or_down == GPIO_NO_PULL_UP_OR_DOWN) {
		gpio_peripheral->PUPDR &= ~((0x01UL) << (config->pin * GPIO_PUPD_BITS));
		gpio_peripheral->PUPDR &= ~((0x01UL)
				<< ((config->pin * GPIO_PUPD_BITS) + 1));
	} else if (config->pull_up_or_down == GPIO_PULL_UP) {
		gpio_peripheral->PUPDR |= ((0x01UL) << (config->pin * GPIO_PUPD_BITS));
		gpio_peripheral->PUPDR &= ~((0x01UL)
				<< ((config->pin * GPIO_PUPD_BITS) + 1));
	} else if (config->pull_up_or_down == GPIO_PULL_DOWN) {
		gpio_peripheral->PUPDR &= ~((0x01UL) << (config->pin * GPIO_PUPD_BITS));
		gpio_peripheral->PUPDR |= ((0x01UL)
				<< ((config->pin * GPIO_PUPD_BITS) + 1));
	}

	/* Update the array */
	gpio_initialised_pins[config->port][config->pin] = 1U;

	return STATUS_OK;

}

Status_code_t GPIO_PinWrite(gpio_port_t port, gpio_pin_t pin, gpio_pin_state_t state)
{
	GPIO_TypeDef *gpio;

	   if(!GPIO_IsPortAndPinValid(port, pin))
	   {
		   return STATUS_PARAM_ERROR;
	   }
//
	if (gpio_output_pins[port][pin] == 0U) /* check if gpio is set as output */
	{
		return STATUS_INIT_ERROR;
	}


   if((state != GPIO_PIN_RESET) && (state != GPIO_PIN_SET))
   {
	   return STATUS_PARAM_ERROR;
   }

   gpio = GPIO_GetPortRegister(port);

   if(state == GPIO_PIN_RESET)
   {
	   gpio->BSRR &= ~(0x01UL << pin);
	   gpio->BSRR |= 0x01UL << (16 + pin);
   }
   else
   {
	   gpio->BSRR |= (0x01UL << pin);
	   gpio->BSRR &= 0x01UL << (16 + pin);
   }

   return STATUS_OK;



}

Status_code_t GPIO_PinRead(gpio_port_t port, gpio_pin_t pin, gpio_pin_state_t *state)
{
	GPIO_TypeDef *gpio;
	uint32_t idr_value = 0;

	   if(!GPIO_IsPortAndPinValid(port, pin))
	   {
		   return STATUS_PARAM_ERROR;
	   }

	if (!gpio_initialised_pins[port][pin]) /* check if gpio is initialised */
	{
		return STATUS_INIT_ERROR;
	}

	if(state == NULL)
	{
		return STATUS_NULL_ERROR;
	}



   /* Fetch GPIO Port address and read the input */
   gpio = GPIO_GetPortRegister(port);
   idr_value = gpio->IDR;

   if(((idr_value >> pin) & 0x01UL) == 0)
   {
	   *state = GPIO_PIN_RESET;
   }
   else
   {
	   *state = GPIO_PIN_SET;
   }



   return STATUS_OK;



}

Status_code_t GPIO_PinToggle(gpio_port_t port, gpio_pin_t pin)
{
	GPIO_TypeDef *gpio;
	uint32_t idr_value;

	   if(!GPIO_IsPortAndPinValid(port, pin))
	   {
		   return STATUS_PARAM_ERROR;
	   }

	if (!gpio_output_pins[port][pin]) /* check if gpio is set as output */
	{
		return STATUS_INIT_ERROR;
	}

	   /* Fetch GPIO Port address, read the input and toggle the output */
	   gpio = GPIO_GetPortRegister(port);
	   idr_value = gpio->IDR;

	   if(((idr_value >> pin) & 0x01UL) == 0)
	   {
		   gpio->BSRR |= (0x01UL << pin);
		   gpio->BSRR &= 0x01UL << (16 + pin);
	   }
	   else
	   {
		   gpio->BSRR &= ~(0x01UL << pin);
		   gpio->BSRR |= 0x01UL << (16 + pin);
	   }

	   return STATUS_OK;

}


Status_code_t GPIO_DeInit(void)
{
	Status_code_t init_code = STATUS_OK;

	/* Reset both the arrays */
	for(int j = 0; j < GPIO_PORT_COUNT; j ++)
	{
	for (int i = 0; i < 15; i++) {
		gpio_output_pins[j][i] = 0;
		gpio_initialised_pins[j][i] = 0;
	}
	}

	/* Set the variable as driver initialised */
	gpio_initialised =0U;

	/* Disable the clock */
	RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIODEN;
	RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOEEN;
	RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOHEN;
	return init_code;
}

Status_code_t GPIO_Version(uint8_t *major, uint8_t *minor, uint8_t *patch)
{
	if((major == NULL) || (minor == NULL) || (patch == NULL))
	{
		return STATUS_NULL_ERROR;
	}

	*major = MAJOR_VERSION;
	*minor = MINOR_VERSION;
	*patch = PATCH_VERSION;

	return STATUS_OK;
}
