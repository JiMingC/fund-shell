LIBS = -lm -lcurl -lsqlite3 -lxml2 -lncurses
CC = gcc
LD = gcc

SUBDIRS = . ./src
SRCS = $(wildcard $(foreach S,$(SUBDIRS), $S/*.c $S/*.cpp))
OBJS = $(SRCS:.c = .o)

INCLUDES = $(foreach S,$(SUBDIRS),-I$S)
INCLUDES += /lib/sqlite 
CCFAGS = -g -Wall -O0 $(INCLUDES)

TARGET = fund_test


.PHONY:all clean
all:$(TARGET)
$(TARGET):$(OBJS) $(M_OBJS)
	$(LD) $(CCFLAGS) -o $@ $^ $(LIBS)


/%o:/%c
	$(CC) $(CCFLAGS) -c $^
 
clean:
	rm -rf *.o $(foreach D,$(SUBDIRS),$D*.o) $(TARGET)
