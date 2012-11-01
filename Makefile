#
# Localthresh makefile
#
program_NAME := localthresh
program_C_SRCS := $(wildcard src/*.c)
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_OBJS := $(program_C_OBJS)

CPPFLAGS += -Iinclude/ `GraphicsMagick-config --cppflags`
LDFLAGS += `GraphicsMagick-config --ldflags`
LDLIBS += `GraphicsMagick-config --libs`

.PHONY : all clean distclean

all : $(program_NAME)

$(program_NAME) : $(program_OBJS)
	$(CC) $(program_OBJS) -o $(program_NAME) -O $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)

main.o : main.h
algorithms.o : algorithms.h
misc.o : misc.h

clean :
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean : clean
