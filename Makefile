# Makefile for ttyload
# Copyright 2001 by David Lindes, All Rights Reserved
# see the "LICENSE" file for licensing info.

# change this if you want to use 'make install' and have it go
# somewhere else:
INSTALLDIR	= /usr/local/bin

ARCH	= `uname -s`
LDFLAGS	= `./ldflags`

OBJS	=	arch/${ARCH}/getload.o	\
		arch/${ARCH}/terminfo.o	\
		${NULL}

# this is what I use most places...
CC=gcc -pedantic -Wall

# for the things in the sub-directory:
INCLUDES	=	-I$${PWD:-.} \
			-I$${PWD:-.}/arch/${ARCH} \
			-I$${PWD:-.}/arch/default

# Debugging compiles?
DEBUG	= -g

VERSION	= -DVERSION='"'`cat Version`'"'

CFLAGS	= $(INCLUDES) $(OTHER_FLAGS) $(DEBUG) $(VERSION)

# most people won't want loader, so don't bother building it:
# PROGRAMS =	archbuild loader
PROGRAMS =	archbuild

default:	$(PROGRAMS)

test:		$(PROGRAMS)
	./ttyload -i 1

# which architechtures are just symlinks?
ARCHLINKS	=	\
	arch/Darwin	\
	${NULL}

# Some architechtures mimic eachother:
archlinks:
	ln -fs FreeBSD arch/Darwin

archbuild: archlinks
	make archtest ttyload ARCH=`uname -s`

ttyload.c:	ttyload.h Version
	touch ttyload.c

# have to be explicit here, for some make systems, like .c.o below:
ttyload: $(OBJS) ttyload.o
	$(CC) $(LDFLAGS) -o $@ $(OBJS) ttyload.o

archtest: $(OBJS) archtest.o
	$(CC) $(LDFLAGS) -o $@ $(OBJS) archtest.o

clean:
	rm -f *.o $(OBJS) core a.out

clobber:	clean
	rm -f loader ttyload archtest $(ARCHLINKS)

# install, gently.  not much to it:
install:	$(PROGRAMS)
	/bin/cp ttyload ${INSTALLDIR}/ttyload

# because different systems' make have different behaviors on how they
# deal with building stuff in subdirectories, and because I don't feel
# like descending into the subdirectories to make a single object file,
# I'll force the .c.o rule to what I'm expecting:
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
