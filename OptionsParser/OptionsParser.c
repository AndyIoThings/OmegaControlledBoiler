#include "OptionsParser.h"
#include "../Configuration/server_config.h"
#include "../Logger/logger.h"
#include "../Validator/input_validator.h"
#include "../pre_header.h"
#include <unistd.h>

void ParseOptions(int argc, char *argv[])
{
	int opt = -1;
	char *ipaddress_val;
	char *port_val;
	char *root_folder;
	opterr = 0;

	while ((opt = getopt(argc, argv, "a:p:r:vh")) != -1) {
		switch (opt) {
		case 'a': {
			ipaddress_val = optarg;
			if (validate(ipaddress_val, strlen(ipaddress_val),
									 IP_ADDRESS_VALIDATOR_TYPE)) {
				logData(SERVER_LOG, "Server address was set to %s.\n", ipaddress_val);
				SetServerConfigIPAddress(ipaddress_val);
			} else {
				logData(SERVER_LOG, "Server address is not valid.\n");
			}
			break;
		}
		case 'p': {
			port_val = optarg;
			if (validate(port_val, strlen(port_val), LISTENING_PORT_VALIDATOR_TYPE)) {
				logData(SERVER_LOG, "Server port was set to %s.\n", port_val);
				SetServerConfigPort(atoi(port_val));
			}
			break;
		}
		case 'r': {
			root_folder = optarg;
			if (validate(
							root_folder, strlen(root_folder),
							ROOT_FOLDER_VALIDATOR_TYPE)) { // change the way to detect length
				logData(SERVER_LOG, "Server port was set to %s", root_folder);
				SetServerConfigRootDir(root_folder);
			}
			break;
		}
		case 'v': {
			logData(SERVER_LOG, "Verbose mode was set \n");
			break;
		}
		case 'h': {
			printf("HELP  Usage: -a [IP ADDRESS] -p [PORT ] -r[ROOT_FOLDER] -vh \n");
			break;
		}
		default:
			fprintf(stderr, "Usage: %s [-apvh]  \n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
}
