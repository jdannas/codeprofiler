# This makefile compiles ....

INCLUDE = -I/usr/X11R6/include

CC=g++ 
CFLAGS=-w -D LINUX -O3 
ODIR=obj
LDFLAGS=-lrt
SOURCES_RAW=main.cpp codeprofiler.cpp

TARGET:= test_profile
TARGETD:= test_profiled

OBJECTS:=$(SOURCES_RAW:.cpp=.o)
SOURCES:=$(SOURCES_RAW:%=src/%)

.PHONY: all clean debug

all: $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) -w $(CFLAGS) $(INCLUDE) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS) -o $@ 

debug: $(TARGETD)
	
$(TARGETD): $(OBJECTS)
	$(CC) -w $(CFLAGS) $(INCLUDE) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) -c -g $< $(CFLAGS) -o $@ 

clean:
	rm *.o
