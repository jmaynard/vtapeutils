# Makefile for the vtapeutils virtual tape utilities package. See the
# file LICENSE for license terms.
#
# Copyright (c) 2005 James R. Maynard, III

VERSION=0.1
SOURCES=awstape.c tap.c tpc.c vtape.c vtapelist.c vtapecp.c Makefile LICENSE
HEADERS=awstape.h tap.h tpc.h vtape.h
LIBOBJECTS=awstape.o tap.o tpc.o vtape.o
OBJECTS=vtapelist.o vtapecp.o
PROGRAMS=vtapelist vtapecp

all: $(PROGRAMS)

vtapecp: vtapecp.o $(LIBOBJECTS)
	cc -o vtapecp vtapecp.o $(LIBOBJECTS)

vtapecp.o: vtapecp.c vtape.h
	cc -c vtapecp.c

vtapelist: vtapelist.o $(LIBOBJECTS)
	cc -o vtapelist vtapelist.o $(LIBOBJECTS)

vtapelist.o: vtapelist.c vtape.h
	cc -c vtapelist.c

vtape.o: vtape.c vtape.h awstape.h tap.h tpc.h
	cc -c vtape.c

awstape.o: awstape.h awstape.c
	cc -c awstape.c

tap.o: tap.h tap.c
	cc -c tap.c

tpc.o: tpc.h tpc.c
	cc -c tpc.c

clean:
	rm $(OBJECTS) $(LIBOBJECTS) $(PROGRAMS)

tarball:
	mkdir vtapeutils-$(VERSION)
	cp $(SOURCES) $(HEADERS) vtapeutils-$(VERSION) 
	tar cvzf vtapeutils-$(VERSION).tar.gz vtapeutils-$(VERSION)
	rm -rf vtapeutils-$(VERSION)
