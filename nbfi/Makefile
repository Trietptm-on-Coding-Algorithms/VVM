override CFLAGS+=-Werror -Wall -g -fPIC -O2 -DNDEBUG -std=c99 -ftrapv -Wfloat-equal -Wundef -Wwrite-strings -Wconversion -Wuninitialized
PREFIX=/usr/bin
BUILDDIR=bin/
CC=gcc-mp-4.9

TARGET=nbfi
SOURCES=$(wildcard src/*.c)

#Makes everything
all:
	mkdir $(BUILDDIR)  2> /dev/null || true
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILDDIR)$(TARGET)
	@echo Running tests
	./test.sh

#Uses picky extensions and makes everything(Extensions may break compiling)
dev:
	make all CFLAGS+=-Wshadow -Wunreachable-code -Wswitch-enum -Wswitch-default -Wcast-align -Winit-self -Wpointer-arith -Weffc++

#Cleans directory(no uninstall!)
clean:
	rm -rf bin

#Installs into specified(or default) directory
install:
	install -d $(PREFIX)/NBFI
	install $(BUILDDIR)$(TARGET) $(PREFIX)/NBFI

#Uninstalls from specified(or default)directory
uninstall:
	rm -rf $(PREFIX)/NBFI

#Checks for bad functions
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@grep $(BADFUNCS) $(SOURCES) || echo None
