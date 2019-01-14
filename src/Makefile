#Copyright Notice:
#The files within this zip file are copyrighted by Lazy Foo' Productions (2004-2014)
#and may not be redistributed without written permission.

#OBJS specifies which files to compile as part of the project
OBJS = Main.cpp

#CC specifies which compiler we're using
CC = g++
CC_WIN = x86_64-w64-mingw32-g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 
LINKER_FLAGS_WIN = -static-libgcc -static-libstdc++ -lmingw32 -lSDL2main -mwindows -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ../linux/Minesweeper
OBJ_NAME_WIN = ../win64/Minesweeper.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
windows : $(OBJS)
	$(CC_WIN) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS_WIN) -o $(OBJ_NAME_WIN)

