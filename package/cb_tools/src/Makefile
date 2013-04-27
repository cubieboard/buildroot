
#CROSS_COMPILE?=arm-linux-gnueabihf-
#CC=$(CROSS_COMPILE)gcc
#LD=$(CROSS_COMPILE)ld

#ifneq "CROSS_SYSROOT" ""
#CROSS_SYSROOT=$(shell cd ../../../../out/br/staging; pwd)
#endif

#CFLAGS+=--sysroot=$(CROSS_SYSROOT)

ir-daemon:ir-daemon.c
	$(CC) $(CFLAGS) -o ir-daemon -lsysfs ir-daemon.c


all: ir-daemon

install:
	install ir-daemon  $(DESTDIR)/bin

clean:
	rm -rf *.o ir-daemon

.PHONY: all clean


