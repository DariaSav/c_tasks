
CC            = g++
CXXFLAGS      = -W -Wall -std=c++17
LDFLAGS       = -Wall

relative_source_dirs := ./
SRC := $(patsubst ../../%, %, $(wildcard $(addsuffix /*.c*, $(relative_source_dirs))))
#SRC += $(SRC) main.cpp

#SRC := src/linalg.cpp \
#	src/ap.cpp \
#	magma5.cpp

OBJ := $(SRC:.cpp=.o)


all: task2

clean:
	-rm *.o

.SUFFIXES:


# ----------------------------------------
#magma.o: magma5.cpp
#	$(CC) $(CFLAGS) $(MAGMA_CFLAGS) $(MAGMA_F90FLAGS) -c -o $@ $<

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $<

task2: $(OBJ)
	$(CC) $(LFLAGS) -o $@ $^ 
