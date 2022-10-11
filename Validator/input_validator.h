#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H
#include "options_validator.h"

enum EValidatorType {
	IP_ADDRESS_VALIDATOR_TYPE,
	LISTENING_PORT_VALIDATOR_TYPE,
	ROOT_FOLDER_VALIDATOR_TYPE
};

bool validate(void *data, size_t dataLength, enum EValidatorType type);

#endif // INPUT_VALIDATOR_H
