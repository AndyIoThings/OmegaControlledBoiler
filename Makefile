VERSION = $$(git rev-list --count HEAD)

#TODO 
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
	if [ -d $(BINARY_DIR)/ ]; then echo '$(BINARY_DIR)/ already exists'; else mkdir $(BINARY_DIR)/; fi
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
clean :
	-rm -r ./${BINARY_DIR}/${SERVER_NAME}
	-rm -r ./${BUILD_DIR}/*
	-rm *.o */*.o

zip :
	if [ -f ${HTTP_SERVER_ZIP}]; then rm ${HTTP_SERVER_ZIP}; fi
	zip -r ${HTTP_SERVER_ZIP} ./$(BINARY_DIR)/${SERVER_NAME}
