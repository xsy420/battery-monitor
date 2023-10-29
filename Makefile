all: battery-monitor

# battery-monitor version
VERSION = 0.1

# paths
PREFIX = /usr/local

SRC = battery-monitor.c
OBJ = $(SRC:.c=.o)
CC = gcc

.c.o:
	$(CC) -c $<

battery-monitor.o: battery-monitor.c

battery-monitor: battery-monitor.o
	ld $< -lc -e main -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o $@

install: battery-monitor
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 $< $(DESTDIR)$(PREFIX)/bin/$<

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/battery-monitor

clean:
	rm -f battery-monitor battery-monitor.o

dist: clean
	mkdir -p battery-monitor-$(VERSION)
	cp -R Makefile README.md $(SRC) battery-monitor-$(VERSION)
	tar -cf - battery-monitor-$(VERSION) | gzip > battery-monitor-$(VERSION).tar.gz
	rm -rf battery-monitor-$(VERSION)

.PHONY: all install clean dist uninstall
