# Makefile for ttyload
# Copyright 2001 by David Lindes, All Rights Reserved
# see the "LICENSE" file for licensing info.

# change this if you want to use 'make install' and have it go
# somewhere else:
INSTALLDIR	= /usr/local/bin

ARCH	= `uname -s`

OBJS	= arch/${ARCH}/getload.o arch/default/homebrews.o

# this is what I use most places...
CC=gcc

# for the things in the sub-directory:
INCLUDES	=	-I$${PWD:-.} \
			-I$${PWD:-.}/arch/${ARCH} \
			-I$${PWD:-.}/arch/default

# Hopefully you don't need this... on Solaris (SunOS), though,
# you probably do.  Uncomment it if things don't build:
# OTHER_FLAGS	= -DNEED_LOCAL_HEADERS

# Debugging compiles?
DEBUG	= -g

VERSION	= -DVERSION='"'`cat Version`'"'

CFLAGS	= $(INCLUDES) $(OTHER_FLAGS) $(DEBUG) $(VERSION)

# most people won't want loader, so don't bother building it:
# PROGRAMS =	archbuild loader
PROGRAMS =	archbuild

default:	$(PROGRAMS)

test:		archbuild
	./ttyload -i 1

archbuild:
	make ttyload ARCH=`uname -s`

ttyload.c:	ttyload.h Version
	touch ttyload.c

ttyload: $(OBJS) ttyload.o

clean:
	rm -f *.o $(OBJS)

clobber:	clean
	rm -f loader ttyload

# install, gently.  not much to it:
install:	archbuild
	/bin/cp -i ttyload ${INSTALLDIR}/ttyload
