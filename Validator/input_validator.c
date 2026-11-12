#include "input_validator.h"

bool validate(void *pdata, size_t dataLength, enum EValidatorType type)
{
	if (pdata)
	{
		switch (type)
		{
		case IP_ADDRESS_VALIDATOR_TYPE:
			return validateIP4Address((char *)pdata, dataLength);
		case LISTENING_PORT_VALIDATOR_TYPE:
			return validateListenPort((char *)pdata, dataLength);
		case ROOT_FOLDER_VALIDATOR_TYPE:
			return validateRootFolderPath((char *)pdata, dataLength);
		}
	}
	else
	{
		false;
	}
	return false;
}
