# Makefile for the vtapeutils virtual tape utilities package. See the
# file LICENSE for license terms.
#
# Copyright (c) 2005, 2007 James R. Maynard, III

VERSION=0.2
SOURCES=awstape.c ebcdic.c faketape.c stdlabel.c tap.c tpc.c vtape.c \
	vtapelist.c vtapecp.c \
	Makefile LICENSE 
HEADERS=awstape.h ebcdic.h faketape.h stdlabel.h tap.h tpc.h vtape.h
LIBOBJECTS=awstape.o ebcdic.o faketape.o stdlabel.o tap.o tpc.o vtape.o
OBJECTS=vtapelist.o vtapecp.o
PROGRAMS=vtapelist vtapecp
CCOPTS=-g

all: $(PROGRAMS)

vtapecp: vtapecp.o $(LIBOBJECTS)
	cc $(CCOPTS) -o vtapecp vtapecp.o $(LIBOBJECTS)

vtapecp.o: vtapecp.c vtape.h
	cc $(CCOPTS) -c vtapecp.c

vtapelist: vtapelist.o $(LIBOBJECTS)
	cc $(CCOPTS) -o vtapelist vtapelist.o $(LIBOBJECTS)

vtapelist.o: vtapelist.c vtape.h
	cc $(CCOPTS) -c vtapelist.c

vtape.o: vtape.c vtape.h awstape.h tap.h tpc.h
	cc $(CCOPTS) -c vtape.c

awstape.o: awstape.h awstape.c
	cc $(CCOPTS) -c awstape.c

ebcdic.o: ebcdic.h ebcdic.c
	cc $(CCOPTS) -c ebcdic.c

faketape.o: faketape.c faketape.h
	cc $(CCOPTS) -c faketape.c

stdlabel.o: stdlabel.h stdlabel.c
	cc $(CCOPTS) -c stdlabel.c

tap.o: tap.h tap.c
	cc $(CCOPTS) -c tap.c

tpc.o: tpc.h tpc.c
	cc $(CCOPTS) -c tpc.c

clean:
	rm $(OBJECTS) $(LIBOBJECTS) $(PROGRAMS)

tarball:
	mkdir vtapeutils-$(VERSION)
	cp $(SOURCES) $(HEADERS) vtapeutils-$(VERSION) 
	tar cvzf vtapeutils-$(VERSION).tar.gz vtapeutils-$(VERSION)
	rm -rf vtapeutils-$(VERSION)
