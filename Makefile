#OBJS specifies which files to compile as part of the project
OBJS = SDL/main.cpp

SOURCES := $(wildcard SDL/*.cpp) $(wildcard SDL/Scripts/*.cpp) 

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#INCLUDES
INCLUDES = -I./SDL/Sounds -I./SDL/Art -I./SDL/Scripts -I./SDL/Fonts

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = play

#This is the target that compiles our executable
all : $(SOURCES)
	$(CC) $(SOURCES) $(COMPILER_FLAGS) $(INCLUDES) $(LINKER_FLAGS) -o $(OBJ_NAME)
