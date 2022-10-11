#include "input_validator.h"

bool validate(void *pdata, size_t dataLength, enum EValidatorType type)
{
	if (pdata) {
		switch (type) {
		case IP_ADDRESS_VALIDATOR_TYPE:
			return validateIP4Address(pdata, dataLength);
		case LISTENING_PORT_VALIDATOR_TYPE:
			return validateListenPort(pdata, dataLength);
		case ROOT_FOLDER_VALIDATOR_TYPE:
			return validateRootFolderPath(pdata, dataLength);
		}
	} else
		false;
}
