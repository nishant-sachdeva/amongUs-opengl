UNAME := $(shell uname -s)
CC = g++
OBJ = source/miro.o source/pathfinder.o source/coin.o source/fire.o source/vaporiser.o
LDFLAGS = -lGL -lGLU -lglut

# Mac OS
ifeq ($(UNAME), Darwin)
	LDFLAGS = -framework OpenGL -framework glut
endif

miro : $(OBJ)
	$(CC) -o miro $(OBJ) $(LDFLAGS)
	
clean :
	rm $(OBJ)
