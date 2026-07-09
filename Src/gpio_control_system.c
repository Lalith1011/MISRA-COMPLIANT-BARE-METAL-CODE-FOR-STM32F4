/*
 * gpio_control_system.c
 *
 *  Created on: Jul 1, 2026
 *      Author: lalithk
 */

#include "gpio_control_system.h"
#include <stdbool.h>
#include <string.h>

#define MAJOR_VERSION 1U
#define MINOR_VERSION 0U
#define PATCH_VERSION 0U

bool is_gcs_initialized = false;

static char commands[3][20] = { "CONFIG", "OUTPUT", "INPUT" };

#define GPIO_PORT_ERROR_CODE 8
#define GPIO_PIN_ERROR_CODE 16
#define GPIO_MODE_ERROR_CODE 2
#define GPIO_PULL_UP_OR_DOWN_ERROR_CODE 3
#define GPIO_OUTPUT_STATE_ERROR_CODE 2
static gpio_port_t compute_gpio_port(char *port) {
	if (strstr(port, "GPIO_PORT_A") != 0) {
		return GPIO_PORT_A;
	} else if (strstr(port, "GPIO_PORT_B") != 0) {
		return GPIO_PORT_B;
	} else if (strstr(port, "GPIO_PORT_C") != 0) {
		return GPIO_PORT_C;
	} else if (strstr(port, "GPIO_PORT_D") != 0) {
		return GPIO_PORT_D;
	} else if (strstr(port, "GPIO_PORT_E") != 0) {
		return GPIO_PORT_E;
	}

	return GPIO_PORT_ERROR_CODE;

}

static gpio_pin_t compute_gpio_pin(char *pin) {
	if (strstr(pin, "GPIO_PIN_1;") != 0) {
		return GPIO_PIN_1;
	} else if (strstr(pin, "GPIO_PIN_2;") != 0) {
		return GPIO_PIN_2;
	} else if (strstr(pin, "GPIO_PIN_3;") != 0) {
		return GPIO_PIN_3;
	} else if (strstr(pin, "GPIO_PIN_4;") != 0) {
		return GPIO_PIN_4;
	} else if (strstr(pin, "GPIO_PIN_5;") != 0) {
		return GPIO_PIN_5;
	} else if (strstr(pin, "GPIO_PIN_6;") != 0) {
		return GPIO_PIN_6;
	} else if (strstr(pin, "GPIO_PIN_7;") != 0) {
		return GPIO_PIN_7;
	} else if (strstr(pin, "GPIO_PIN_8;") != 0) {
		return GPIO_PIN_8;
	} else if (strstr(pin, "GPIO_PIN_9;") != 0) {
		return GPIO_PIN_9;
	} else if (strstr(pin, "GPIO_PIN_10;") != 0) {
		return GPIO_PIN_10;
	} else if (strstr(pin, "GPIO_PIN_11;") != 0) {
		return GPIO_PIN_11;
	} else if (strstr(pin, "GPIO_PIN_12;") != 0) {
		return GPIO_PIN_12;
	} else if (strstr(pin, "GPIO_PIN_13;") != 0) {
		return GPIO_PIN_13;
	} else if (strstr(pin, "GPIO_PIN_14;") != 0) {
		return GPIO_PIN_14;
	} else if (strstr(pin, "GPIO_PIN_15;") != 0) {
		return GPIO_PIN_15;
	}
	return GPIO_PIN_ERROR_CODE;
}

static gpio_state_t compute_gpio_mode(char *mode) {
	if (strstr(mode, "INPUT") != 0) {
		return GPIO_MODE_INPUT;
	} else if (strstr(mode, "OUTPUT") != 0) {
		return GPIO_MODE_OUTPUT;
	}

	return GPIO_MODE_ERROR_CODE;

}

static gpio_pull_up_down_t compute_gpio_pull_up_or_down(char *pull_up_or_down) {
	if (strstr(pull_up_or_down, "PULL_UP") != 0) {
		return GPIO_PULL_UP;
	} else if (strstr(pull_up_or_down, "PULL_DOWN") != 0) {
		return GPIO_PULL_DOWN;
	}
	return GPIO_NO_PULL_UP_OR_DOWN;

}

static gpio_pin_state_t compute_output_state(char *output_state) {
	if (strstr(output_state, "HIGH") != 0) {
		return GPIO_PIN_SET;
	} else if (strstr(output_state, "LOW") != 0) {
		return GPIO_PIN_RESET;
	}

	return GPIO_OUTPUT_STATE_ERROR_CODE;

}

System_status_code_t GCS_Init() {
	is_gcs_initialized = true;
//    GPIO_Init();
	return STATUS_EXECUTION_SUCCESSFUL;
}

System_status_code_t GCS_Helper(char *resp) {
	if (resp == NULL) {
		return STATUS_EXECUTION_FAILED;
	}

	if (!is_gcs_initialized) /*If GCS is not initialized */
	{
		return STATUS_EXECUTION_FAILED;
	}

	char inst_string[100] = "";

	strcpy(resp, "");
	strcat(resp, "  \n COMMAND LIST\n\n");
	sprintf(inst_string, "1.%s - To configure a pin\n", commands[0]);
	strcat(resp, inst_string);
	sprintf(inst_string, "2.%s - To set the output of a pin\n", commands[1]);
	strcat(resp, inst_string);
	sprintf(inst_string, "3.%s - To read the input state of a pin\n\n",
			commands[2]);
	strcat(resp, inst_string);
	strcat(resp, "COMMAND FORMAT : \n\n");
	strcat(resp,
			"NOTE : Port names are to be provided in the commands as follows : GPIO_PORT_A, GPIO_PORT_B etc. \nPin numbers are to be provided in the commands as follows : GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2 etc.\n Mode is to be provided as follows : OUTPUT, INPUT. \nPULL_UP/DOWN is to be provided as follows : PULL_UP, PULL_DOWN.\n State is to be provided as : HIGH, LOW.\n\n");
	strcat(resp,
			"For CONFIG - \"COMMAND:CONFIG;PORT:{PORT_NAME};PIN:{PIN_NUMBER};MODE:{output_or_input};PULLUP/DOWN:{pull_up_or_down};\"\n       Example : COMMAND:CONFIG;PORT:GPIO_PORT_A;PIN:GPIO_PIN_5;MODE:OUTPUT;PULLUP/DOWN:PULL_UP\n\n");
	strcat(resp,
			"For OUTPUT - \"COMMAND:OUTPUT;PORT:{PORT_NAME};PIN:{PIN_NUMBER};STATE:{OUTPUT_STATE};\"\n       Example : COMMAND:OUTPUT;PORT:GPIO_PORT_A;PIN:GPIO_PIN_5;STATE:HIGH\n\n");
	strcat(resp,
			"For INPUT - \"COMMAND:INPUT;PORT:{PORT_NAME};PIN:{PIN_NUMBER};\"\n       Example : COMMAND:INPUT;PORT:GPIO_PORT_A;PIN:GPIO_PIN_5;\n\n");
	strcat(resp, "   RESPONSE LIST\n");
	strcat(resp,
			"1. FOR CONFIG : On Successful Pin Configuration - CONFIGURATION_SUCCESSFUL, On Failed Pin Configuration - CONFIGURATION_FAILED:{Reason for failure}\n");
	strcat(resp,
			"2. FOR OUTPUT : On Successfully setting the Output - OUTPUT_STATE_SET_SUCCESSFUL, On Failure to set the Output - OUTPUT_STATE_SET_FAILED:{Reason for failure}\n");
	strcat(resp,
			"3. FOR INPUT  : On Successfully reading the Input state - INPUT_STATE_READ_SUCCESSFUL:{STATE} (STATE value can be HIGH or LOW), On Failure to read the Input state - INPUT_STATE_READ_FAILED:{Reason for failure}\n\n");

	return STATUS_EXECUTION_SUCCESSFUL;
}

System_status_code_t GCS_CommandParser(char *command, char *resp) {

	 gpio_pin_state_t gpio_state;
	 gpio_config_t config;

	if ((command == NULL) || (resp == NULL)) {
		return STATUS_EXECUTION_FAILED;
	}

	Status_code_t status;
	config.port = compute_gpio_port(command);
	config.pin = compute_gpio_pin(command);

	if (strstr(command, "CONFIG") != 0) /* Command is to configure a pin */
	{
		config.state = compute_gpio_mode(command);
		config.pull_up_or_down = compute_gpio_pull_up_or_down(command);
		if(config.state == GPIO_MODE_OUTPUT)
		{
			config.output_type = GPIO_OUTPUT_PUSH_PULL;
			config.output_speed = GPIO_LOW_SPEED;
		}
		status = GPIO_PinInit(&config);
		if (STATUS_OK != status) {
			sprintf(resp, "CONFIGURATION FAILED - Reason Unknown : %d\n\n",status);
			return STATUS_EXECUTION_FAILED;
		} else {
			strcpy(resp, "CONFIGURATION SUCCESSFUL\n\n");
		}
	} else if (strstr(command, "OUTPUT") != 0) /* Command is to set a pin as output */
	{
		gpio_state = compute_output_state(command);
		status = GPIO_PinWrite(config.port, config.pin, gpio_state);
		if (STATUS_OK != status) {
			sprintf(resp, "OUTPUT_STATE_SET_FAILED - Reason Unknown : %d\n\n",status);
			return STATUS_EXECUTION_FAILED;
		} else {
			strcpy(resp, "OUTPUT_STATE_SET_SUCCESSFUL\n\n");
		}

	} else if (strstr(command, "INPUT") != 0) /* Command is to read the input state of a pin */
	{
		status = GPIO_PinRead(config.port, config.pin, &gpio_state);
		if (STATUS_OK != status) {
			sprintf(resp, "INPUT_STATE_READ_FAILED - Reason Unknown : %d\n\n",status);
			return STATUS_EXECUTION_FAILED;
		} else {
			strcpy(resp, "");
			if (gpio_state == GPIO_PIN_SET) {
				sprintf(resp, "INPUT_STATE_READ_SUCCESSFUL:HIGH\n\n");
			} else {
				sprintf(resp, "INPUT_STATE_READ_SUCCESSFUL:LOW\n\n");
			}
		}

	}

	return STATUS_EXECUTION_SUCCESSFUL;

}
