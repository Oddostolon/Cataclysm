#OBJS specifies which files to compile as part of the project
OBJS = cataclysm.cpp wrappers/LTexture.cpp wrappers/LButton.cpp wrappers/LTimer.cpp Dot.cpp KeyboardInput.cpp wrappers/LWindow.cpp

#CC specifies which compiler we're using
CC = clang++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -g

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = cataclysm

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

# Clean target to remove object files and the executable
clean:
	rm -f $(OBJS) $(OBJ_NAME)