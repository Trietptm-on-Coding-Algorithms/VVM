STD=-std=c99
override CFLAGS+=-Werror -Wall -g -fPIC -O2 -DNDEBUG -ftrapv -Wfloat-equal -Wundef -Wwrite-strings -Wconversion -Wuninitialized -pedantic $(STD)
DEBUG+=-DDEBUG_ON
PREFIX=/usr/bin/
BUILDDIR=bin/

CC=cc

TARGET=vvm
SOURCES=$(wildcard src/*.c)

#Makes everything
all:
	mkdir $(BUILDDIR)  2> /dev/null || true
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILDDIR)$(TARGET)

#Uses picky extensions and makes everything(Extensions may break compiling)
dev:
	make all CFLAGS+="-Wshadow -Wunreachable-code -Wswitch-enum -Wswitch-default -Wcast-align -Winit-self -Wpointer-arith -fsanitize=address"

#Makes a debug build
debug:
	mkdir $(BUILDDIR)  2> /dev/null || true
	$(CC) $(CFLAGS) $(DEBUG) $(SOURCES) -o $(BUILDDIR)$(TARGET)

#Runs all tests
runtests:
	./test.sh

#Cleans directory(no uninstall!)
clean:
	rm -rf bin

#Installs into specified(or default) directory
install:
	install -d $(PREFIX)$(TARGET)
	install $(BUILDDIR)$(TARGET) $(PREFIX)$(TARGET)

#Uninstalls from specified(or default)directory
uninstall:
	rm -rf $(PREFIX)$(TARGET)

#Checks for bad functions
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@grep $(BADFUNCS) $(SOURCES) || echo None
