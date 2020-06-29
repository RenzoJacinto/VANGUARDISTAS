OBJ = $(shell find . -name '*.cpp')

CC = g++ 

CXXFLAGS = -g -O2 -Wall
 
LINKER_FLAGS = -std=c++11 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lpthread

OBJCLI_NAME = cliente
OBJSERV_NAME = servidor

cliente: $(OBJSCLI)
	$(CC) $(OBJ) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJCLI_NAME)

servidor: $(OBJSERV)
	$(CC) $(OBJ) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJSERV_NAME)

clean:
	rm -rf *.o $(OBJCLI_NAME) $(OBJSERV_NAME)
