ARCH	= `uname -s`

OBJS	= arch/${ARCH}/getload.o arch/default/homebrews.o

# this is what I use most places...
CC=gcc

# for the things in the sub-directory:
INCLUDES	= -I${PWD} -I ${PWD}/arch/${ARCH} -I${PWD}/arch/default

# Hopefully you don't need this:
OTHER_FLAGS	= -DNEED_LOCAL_HEADERS

# Debugging compiles?
DEBUG	= -g

VERSION	= -DVERSION='"'`cat Version`'"'

CFLAGS	= $(INCLUDES) $(OTHER_FLAGS) $(DEBUG) $(VERSION)

default:	archbuild loader

test:		archbuild
	./ttyload -i 1

archbuild:
	make ttyload ARCH=`uname -s`

ttyload.c:	ttyload.h Version
	touch ttyload.c

ttyload: $(OBJS) ttyload.o

clean:
	rm -f *.o $(OBJS)
