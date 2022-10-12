CC = gcc
CXX = g++
CFLAGS = -g -Wall -Wextra -std=c99 -pedantic
CXXFLAGS = -g -Wall -std=c++11
CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS += $(shell pkg-config --libs gtk+-3.0)
CFILES = $(wildcard *.c)
OBJFILES = $(CFILES:.c=.o) 
LDLIBS = -lGL \
         -lglfw \
         -lGLEW \
         -lSOIL \
         -lassimp \
         -lBulletDynamics \
         -lBulletCollision \
         -lBullet3Common \
         -lLinearMath
OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))	
OBJECTS += osdialog.o osdialog_gtk3.o
TARGET = sample

${TARGET} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^  -o $@ ${LOADLIBES} ${LDLIBS} ${LDFLAGS} $(shell pkg-config --cflags gtk+-3.0)

.PHONY : clean
clean :
	-${RM} ${TARGET} ${OBJECTS} *~ .*~ core