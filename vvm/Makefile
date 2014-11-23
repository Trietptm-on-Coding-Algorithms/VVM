override CFLAGS+=-Werror -Wall -g -fPIC -O2 -DNDEBUG -ftrapv -Wfloat-equal -Wundef -Wwrite-strings -Wconversion -Wuninitialized -pedantic
DEBUG+=-DDEBUG_ON
PREFIX=/usr/bin
BUILDDIR=bin/

TARGET=vvm
SOURCES=$(wildcard src/*.c)

#Makes everything
all:
	mkdir $(BUILDDIR)  2> /dev/null || true
	cc $(CFLAGS) $(SOURCES) -o $(BUILDDIR)$(TARGET)

#Uses picky extensions and makes everything(Extensions may break compiling)
dev:
	make all CFLAGS+=-Wshadow -Wunreachable-code -Wswitch-enum -Wswitch-default -Wcast-align -Winit-self -Wpointer-arith

#Makes a debug build
debug:
	mkdir $(BUILDDIR)  2> /dev/null || true
	cc $(CFLAGS) $(DEBUG) $(SOURCES) -o $(BUILDDIR)$(TARGET)

#Cleans directory(no uninstall!)
clean:
	rm -rf bin

#Installs into specified(or default) directory
install:
	install -d $(PREFIX)/vvm
	install $(BUILDDIR)$(TARGET) $(PREFIX)/vvm

#Uninstalls from specified(or default)directory
uninstall:
	rm -rf $(PREFIX)/vvm

#Checks for bad functions
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@grep $(BADFUNCS) $(SOURCES) || echo None
