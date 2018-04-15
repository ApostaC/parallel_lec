CC = gcc
CPP = g++
CCFLAGS= -fopenmp -g -DLOCAL -lpthread
CPPFLAGS = ${CCFLAGS} #-std=c++11

MPICC = mpicc
MPICPP = mpic++
MPIFLAGS = 

INSTALL_PREFIX = bin/

SRC_C = ${wildcard *.c}
SRC_CPP = ${wildcard *.cpp}
EXEC_CPP = ${patsubst %.cpp, %, ${SRC_CPP}}
EXEC_C = ${patsubst %.c, %, ${SRC_C}}
EXEC = ${EXEC_C} ${EXEC_CPP}

.PHONY: test

AA = 3-mpi.c
FIND = mpi

all: ${EXEC}

%: %.c 
ifeq ($(findstring mpi,$@),mpi)
	${MPICC} ${CCFLAGS} $^ -o ${INSTALL_PREFIX}$@;
else
	${MPICC} ${CCFLAGS} $^ -o ${INSTALL_PREFIX}$@; 	
endif

%: %.cpp
ifeq ($(findstring mpi,$^),$^)
	${MPICPP} ${CPPFLAGS} $^ -o ${INSTALL_PREFIX}$@;
else 
	${MPICPP} ${CPPFLAGS} $^ -o ${INSTALL_PREFIX}$@
endif

test: 3-mpi.c
	echo -$^--3-mpi.c-
ifeq ($(findstring mpi,3-mpi.c),mpi)
	echo find!
else
	echo $(findstring mpi,$^)
endif

clean:
	(cd ${INSTALL_PREFIX}; rm -rf *)
