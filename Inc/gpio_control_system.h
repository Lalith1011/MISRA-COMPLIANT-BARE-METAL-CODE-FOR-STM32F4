/*
 * gpio_control_system.h
 *
 *  Created on: Jul 1, 2026
 *      Author: lalithk
 *      version : 1.0
 */

#ifndef GPIO_CONTROL_SYSTEM_H_
#define GPIO_CONTROL_SYSTEM_H_

#include "gpio_safe.h"
typedef enum
{
  STATUS_EXECUTION_SUCCESSFUL,
  STATUS_EXECUTION_FAILED
}System_status_code_t;

/*
 * @brief This function is used to initialize the 'GPIO_UART' system.
 * @return System_status_code_t
 * @retval STATUS_EXECUTION_SUCCESSFUL : If initialization is successful.
 * @retval STATUS_EXECUTION_FAILED : If initialization fails.
 */
System_status_code_t GCS_Init();

/*
 * @brief This function is used to display the 'Helper box' of the system.
 * @param[in] resp : This is a pointer to the response to be transmitted over UART.
 * @return System_status_code_t
 * @retval STATUS_EXECUTION_SUCCESSFUL : If initialization is successful.
 * @retval STATUS_EXECUTION_FAILED : If initialization fails.
 */
System_status_code_t GCS_Helper(char *resp);

/*
 * @brief This function is used to parse the command, execute it and provide the response to be transmitted over the terminal.
 * @param[in] command : This is a pointer to the input command that comes from the terminal.
 * @param[in] resp : This is a pointer to the response to be sent to the terminal.
 * @return System_status_code_t
 * @retval STATUS_EXECUTION_SUCCESSFUL : If initialization is successful.
 * @retval STATUS_EXECUTION_FAILED : If initialization fails.
 *
 */
System_status_code_t GCS_CommandParser(char *command, char *resp);

/*
 * @brief This function is used to fetch the version of the GCS module.
 * @param[in] major : Pointer for the major version of the GCS.
 * @param[in] minor : Pointer for the minor version of the GCS.
 * @param[in] patch : Pointer for the patch version of the GCS.
 * @return System_status_code_t
 * @retval STATUS_EXECUTION_SUCCESSFUL : If initialization is successful.
 * @retval STATUS_EXECUTION_FAILED : If initialization fails.
 */
System_status_code_t GCS_Version(uint8_t *major, uint8_t *minor, uint8_t *patch);

/*
 * @brief This function is used to de-initialize the GCS driver.
 * @return System_status_code_t
 * @retval STATUS_EXECUTION_SUCCESSFUL : If initialization is successful.
 * @retval STATUS_EXECUTION_FAILED : If initialization fails.
 */
System_status_code_t GCS_DeInit();
#endif /* GPIO_CONTROL_SYSTEM_H_ */
