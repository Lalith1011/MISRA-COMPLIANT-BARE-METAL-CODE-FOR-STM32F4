
/*
 * @file gpio_safe.h
 * @brief This file is used to implement GPIO functionality using MISRA-C guidelines thus making it safe in critical applications.
 * @author Lalith.K
 */
#ifndef GPIO_SAFE
#define GPIO_SAFE

#include <status_codes.h>
#include <stdint.h>
#include <stdio.h>

extern uint32_t input_register_value;

/*
 * @brief enum to define GPIO port
 */

typedef enum
{
	GPIO_PORT_A = 0U,
	GPIO_PORT_B = 1U,
	GPIO_PORT_C = 2U,
	GPIO_PORT_D = 3U,
	GPIO_PORT_E = 4U,
	GPIO_PORT_RESERVED1 = 5U,
	GPIO_PORT_H = 6U,
	GPIO_PORT_COUNT
}gpio_port_t;

/*
 * @brief enum to define GPIO PIN
 */

typedef enum
{
	GPIO_PIN_0 = 0U,
	GPIO_PIN_1 = 1U,
	GPIO_PIN_2 = 2U,
	GPIO_PIN_3 = 3U,
	GPIO_PIN_4 = 4U,
	GPIO_PIN_5 = 5U,
	GPIO_PIN_6 = 6U,
	GPIO_PIN_7 = 7U,
	GPIO_PIN_8 = 8U,
	GPIO_PIN_9 = 9U,
	GPIO_PIN_10 = 10U,
	GPIO_PIN_11 = 11U,
	GPIO_PIN_12 = 12U,
	GPIO_PIN_13 = 13U,
	GPIO_PIN_14 = 14U,
	GPIO_PIN_15 = 15U,
	GPIO_PIN_COUNT
}gpio_pin_t;

/*
 * @brief enum to define GPIO STATE
 */

typedef enum
{
	GPIO_MODE_INPUT = 0U,  /*Input Mode*/
	GPIO_MODE_OUTPUT = 1U, /* Output Mode */
	GPIO_MODE_ALTERNATE_FUNCTION = 2U, /* Alternate Function Mode */
	GPIO_MODE_ANALOG = 3U /* Analog Mode */
}gpio_state_t;

/*
 * @brief enum to define OUTPUT type
 */

typedef enum
{
	GPIO_OUTPUT_PUSH_PULL = 0U, /*Push Pull Output*/
	GPIO_OUTPUT_OPEN_DRAIN = 1U /*Open Drain Output*/
}gpio_output_type_t;

/*
 * @brief enum to define OUTPUT SPEED
 */
typedef enum
{
	GPIO_LOW_SPEED = 0U, /* Low Speed */
	GPIO_MEDIUM_SPEED = 1U, /* Medium Speed */
	GPIO_HIGH_SPEED = 2U, /* High Speed */
	GPIO_VERY_HIGH_SPEED = 3U /* Very High Speed */
}gpio_output_speed_t;

/*
 * @brief enum to define PULL-UP/DOWN
 */
typedef enum
{
	GPIO_NO_PULL_UP_OR_DOWN = 0U, /* Disable Pull_up/Pull_down */
	GPIO_PULL_UP = 1U, /* Enable Pull_up */
	GPIO_PULL_DOWN = 2U /* Enable Pull Down */
}gpio_pull_up_down_t;

/*
 * @brief enum to SET/RESET the I/O pin
 */
typedef enum
{
	GPIO_PIN_RESET = 0U, /* Set the GPIO pin to LOW */
	GPIO_PIN_SET = 1U  /* Set the GPIO pin to HIGH */
}gpio_pin_state_t;

/*
 * @brief enum for I/O alternate functions
 */
typedef enum
{
	ALTERNATE_FUNCTION_0 = 0U,
	ALTERNATE_FUNCTION_1 = 1U,
	ALTERNATE_FUNCTION_2 = 2U,
	ALTERNATE_FUNCTION_3 = 3U,
	ALTERNATE_FUNCTION_4 = 4U,
	ALTERNATE_FUNCTION_5 = 5U,
	ALTERNATE_FUNCTION_6 = 6U,
	ALTERNATE_FUNCTION_7 = 7U,
	ALTERNATE_FUNCTION_8 = 8U,
	ALTERNATE_FUNCTION_9 = 9U,
	ALTERNATE_FUNCTION_10 = 10U,
	ALTERNATE_FUNCTION_11 = 11U,
	ALTERNATE_FUNCTION_12 = 12U,
	ALTERNATE_FUNCTION_13 = 13U,
	ALTERNATE_FUNCTION_14 = 14U,
	ALTERNATE_FUNCTION_15 = 15U,

}gpio_alternate_function_t;

/*
 * @brief Structure to define a GPIO PIN
 */
typedef struct
{
	gpio_port_t port;
	gpio_pin_t pin;
	gpio_state_t state;
	gpio_output_type_t output_type;
	gpio_output_speed_t output_speed;
	gpio_pull_up_down_t pull_up_or_down;
	gpio_alternate_function_t alternate_function;
}gpio_config_t;

/*
 * @brief This function is used to initialise the gpio peripheral
 *
 * @return Status_code_t
 * @retval STATUS_OK gpio init successful
 * @retval STATUS_INIT_ERROR init not successful
 */
Status_code_t GPIO_Init(void);

/*
 * @brief This function is used to define the functionality of a GPIO pin of a particular GPIO Port
 * @param[in] config Pointer of the type gpio_config_t to pass the respective gpio pin configurations to be used.
 * @return Status_code_t
 * @retval STATUS_OK Gpio port and pin defined.
 * @retval STATUS_NULL_ERROR Null pointer passed.
 * @retval STATUS_PARAM_ERROR Parameter is invalid.
 * @retval STATUS_INIT_ERROR Init error
 */
Status_code_t GPIO_PinInit(gpio_config_t *config);

/*
 * @brief This function is used to set the output of a GPIO pin.
 * @param[in] port Gpio port
 * @param[in] pin  Gpio Pin
 * @param[in] state Output state of the Gpio pin.
 * @return Status_code_t
 * @retval STATUS_OK Functionality is implemented.
 * @retval STATUS_PARAM_ERROR Parameter is invalid.
 * @retval STATUS_INIT_ERROR Unable to implement the intended functionality.
 */
Status_code_t GPIO_PinWrite(gpio_port_t port, gpio_pin_t pin, gpio_pin_state_t state);

/*
 * @brief This function is used to get the input state of a GPIO pin.
 * @param[in] port Gpio port
 * @param[in] pin Gpio pin
 * @param[out] state Gpio pin state
 * @return Status_code_t
 * @retval STATUS_OK Functionality is implemented.
 * @retval STATUS_NULL_ERROR Null Pointer passed.
 * @retval STATUS_PARAM_ERROR Parameter is invalid.
 * @retval STATUS_INIT_ERROR Unable to implement the intended functionality.
 */
Status_code_t GPIO_PinRead(gpio_port_t port, gpio_pin_t pin, gpio_pin_state_t *state);

/*
 * @brief This function is used to toggle the output of a GPIO pin.
 * @param[in] port Gpio port
 * @param[in] pin  Gpio Pin
 * @return Status_code_t
 * @retval STATUS_OK Functionality is implemented.
 * @retval STATUS_PARAM_ERROR Parameter is invalid.
 * @retval STATUS_INIT_ERROR Unable to implement the intended functionality.
 */
Status_code_t GPIO_PinToggle(gpio_port_t port, gpio_pin_t pin);

/*
 * @brief This function is used to disable the GPIO peripheral.
 * @return Status_code_t
 * @retval STATUS_OK Functionality is implemented.
 */
Status_code_t GPIO_DeInit(void);

/*
 * @brief This function is used to retrieve the version of the driver
 * @param[in] major Pointer to store the Major version of the driver
 * @param[in] minor Pointer to store the Minor version of the driver
 * @param[in] patch Pointer to store the Patch version of the driver
 * @return Status_code_t
 * @retval STATUS_OK Functionality is implemented
 * @retval STATUS_NULL_ERROR Null Pointer passed.
 */
Status_code_t GPIO_Version(uint8_t *major, uint8_t *minor, uint8_t *patch);


#endif
