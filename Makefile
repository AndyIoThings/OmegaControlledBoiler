VERSION = $$(git rev-list --count HEAD)


CFLAGS = -O0 -DSERVER_VERSION=${VERSION} -I${STAGING_DIR}/target-mipsel_24kc_musl/include -I${STAGING_DIR}/target-mipsel_24kc_musl/usr/include
LDFLAGS = -L${STAGING_DIR}/target-mipsel_24kc_musl/lib -L${STAGING_DIR}/target-mipsel_24kc_musl/usr/lib -lssl -lcrypto
BINARY_DIR = bin
BUILD_DIR = build
SERVER_NAME = http_server
BINARY = ${BINARY_DIR}/${SERVER_NAME}
HTTP_SERVER_ZIP = ~/Desktop/${SERVER_NAME}_${VERSION}.zip
TARGET = all

OBJS = \
	pre_header.o \
	main.o \
	Logger/logger.o\
	Server/server.o\
	RequestHandler/requestParser.o\
	OptionsParser/OptionsParser.o\
	Validator/input_validator.o\
	Validator/options_validator.o\
	Configuration/server_config.o\

.DEFAULT_GOAL := $(TARGET)

.PHONY: clean

all : ${BINARY}

$(BINARY) : $(OBJS)
	if [ -d bin/ ]; then echo 'bin/ already exists'; else mkdir bin/; fi
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean :
	-rm -rf ${BINARY_DIR}/${SERVER_NAME}
	-rm -rf ${BUILD_DIR}/
	-rm *.o */*.o

zip :
	if [ -f ${HTTP_SERVER_ZIP}]; then rm ${HTTP_SERVER_ZIP}; fi
	zip -r ${HTTP_SERVER_ZIP} ./bin/${SERVER_NAME}
