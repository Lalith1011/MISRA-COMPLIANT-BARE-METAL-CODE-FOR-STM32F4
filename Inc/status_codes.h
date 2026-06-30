/*
 * error_codes.h
 *
 *  Created on: May 15, 2026
 *      Author: lalithk
 */

#ifndef STATUS_CODES_H_
#define STATUS_CODES_H_


/*
 * @brief The various kinds of errors that can arise and their respective codes along with all_ok code.
 */

typedef enum
{
	STATUS_OK = 0U,            //STATUS OK
	STATUS_GENERIC_ERROR = 1U, //Generic error
	STATUS_PARAM_ERROR = 2U,   //Invalid Parameter
	STATUS_RANGE_ERROR = 3U,   //Out of Range
	STATUS_NULL_ERROR = 4U,    //Null pointer error
	STATUS_BUSY_ERROR = 5U,     //Busy Error
	STATUS_TIMEOUT_ERROR = 6U, //Timeout Error
	STATUS_RESOURCE_ERROR = 7U, //Resource not available
	STATUS_INIT_ERROR = 8U, //Initialisation Error
	STATUS_STATE_ERROR = 9U, //Invalid State
	STATUS_ERROR_NOT_IMPLEMENTED = 10U, //Intended operation did not go well
    STATUS_PENDING = 11U //Operation is in pending state
}Status_code_t;


#endif /* ERROR_CODES_H_ */
