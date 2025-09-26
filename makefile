#COMPILER
COMPILER = g++

#FLAGS
COMPILER_FLAGS = -Wall -std=c++11 -Isrc -Iinclude

#OUTPUT
TARGET = main

#DIRECTORIES
SRC_DIR = src
OBJ_DIR = build

#CPP FILES
SOURCES = ${wildcard ${SRC_DIR}/*.cpp}

# OBJECTS
OBJECTS = ${patsubst ${SRC_DIR}/%.cpp,${OBJ_DIR}/%.o,${SOURCES}}

#Target
all: ${TARGET}

#Link object files into executable
${TARGET}: ${OBJECTS}
	${COMPILER} ${COMPILER_FLAGS} ${OBJECTS} -o ${TARGET}

#Compile .cpp into .o
${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p ${OBJ_DIR}
	${COMPILER} ${COMPILER_FLAGS} -c $< -o $@

clean:
	rm -rf ${OBJ_DIR} ${TARGET}
