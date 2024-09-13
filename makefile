PCH = pch.h
PCH_OBJ = pch.o

#OBJS specifies which files to compile as part of the project
#OBJS = cataclysm.cpp wrappers/LTexture.cpp wrappers/LButton.cpp wrappers/LTimer.cpp Dot.cpp KeyboardInput.cpp wrappers/LWindow.cpp
OBJS = cataclysm.o wrappers/LTexture.o wrappers/LButton.o wrappers/LTimer.o Dot.o KeyboardInput.o wrappers/LWindow.o

#CC specifies which compiler we're using
CC = ccache clang++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -g
DEBUG_COMPILER_FLAGS = -g

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = cataclysm
DEBUG_OBJ_NAME = cataclysm_debug

#This is the target that compiles our executable
all : $(OBJ_NAME)

$(OBJ_NAME): $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

debug: $(OBJS)
	$(CC) $(OBJS) $(DEBUG_COMPILER_FLAGS) $(LINKER_FLAGS) -o $(DEBUG_OBJ_NAME)

$(PCH_OBJ): $(PCH)
	$(CC) $(COMPILER_FLAGS) -x c++-header $(PCH) -o $(PCH_OBJ)

%.o: %.cpp $(PCH_OBJ)
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

# Clean target to remove object files and the executable
clean:
	rm -f $(OBJS) $(OBJ_NAME) $(DEBUG_OBJ_NAME) $(PCH_OBJ)

