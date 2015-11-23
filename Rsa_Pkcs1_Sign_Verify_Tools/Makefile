NAME                  := sigtool
VERSION               := 1.0.0
PROFILE               ?= default
ARCH                  ?= $(shell uname -m | sed 's/i.86/x86/;s/x86_64/x64/;s/arm.*/arm/;s/mips.*/mips/')
CC_ARCH               ?= $(shell echo $(ARCH) | sed 's/x86/i686/;s/x64/x86_64/')
OS                    ?= linux
CC                     = g++
CONFIG                ?= $(OS)-$(ARCH)-$(PROFILE)
BUILD                 ?= $(shell pwd)
LBIN                  ?= $(BUILD)/bin
PATH                  := $(LBIN):$(PATH)

ME_COM_COMPILER       ?= 1
OPENSSL_LIB_PATH      ?= /home/yangweiming/ProgramFiles/lib/
OPENSSL_INCLUDE_PATH  ?= /home/yangweiming/ProgramFiles/include/


CFLAGS                += -fPIC  -Wall
IFLAGS                += -I./inc -I$(OPENSSL_INCLUDE_PATH)
LIBPATHS              += -L$(OPENSSL_LIB_PATH)
LIBS                  +=  -lcrypto -ldl

DEBUG                 ?= debug
CFLAGS-debug          ?= -g
DFLAGS-debug          ?= -DME_DEBUG
LDFLAGS-debug         ?= -g
DFLAGS-release        ?= 
CFLAGS-release        ?= -O2
LDFLAGS-release       ?= 
CFLAGS                += $(CFLAGS-$(DEBUG))
DFLAGS                += $(DFLAGS-$(DEBUG))
LDFLAGS               += $(LDFLAGS-$(DEBUG)) 

TARGETS += $(BUILD)/obj/UtilsInspurBase.o
TARGETS += $(BUILD)/obj/KeyBase.o
TARGETS += $(BUILD)/obj/RsaBase.o
TARGETS += $(BUILD)/out/libgo.so
TARGETS += $(BUILD)/out/$(NAME)

ifndef SHOW
.SILENT:
endif

all build compile: prep $(TARGETS) compiler 

.PHONY: prep clean help compiler

prep:
	@echo "      [Info] Use "make SHOW=1" to trace executed commands."
	@[ ! -x $(BUILD)/out ] && mkdir -p $(BUILD)/out; true
	@[ ! -x $(BUILD)/obj ] && mkdir -p $(BUILD)/obj; true
	

clean:
	@echo "    [clean] delete $(BUILD)/obj/*.o"
	@echo "    [clean] delete $(BUILD)/out/$(NAME)"
	rm -f $(BUILD)/obj/UtilsInspurBase.o
	rm -f $(BUILD)/obj/RsaBase.o
	rm -f $(BUILD)/obj/KeyBase.o
	rm -f $(BUILD)/obj/main.o
	rm -f $(BUILD)/out/sigtool

#
#   UtilsInspurBase.o
#
DEPS_1 += inc/UtilsInspurBase.h
$(BUILD)/obj/UtilsInspurBase.o: \
    src/UtilsInspurBase.cpp $(DEPS_1)
	@echo '   [Compile] $(BUILD)/obj/UtilsInspurBase.o'
	$(CC) -c -o $(BUILD)/obj/UtilsInspurBase.o $(CFLAGS) $(DFLAGS) -D_FILE_OFFSET_BITS=64   $(IFLAGS) src/UtilsInspurBase.cpp

#
#   KeyBase.o
#
DEPS_2 += inc/KeyBase.h

$(BUILD)/obj/KeyBase.o: \
    src/KeyBase.cpp $(DEPS_2)
	@echo '   [Compile] $(BUILD)/obj/KeyBase.o'
	$(CC) -c -o $(BUILD)/obj/KeyBase.o $(CFLAGS) $(DFLAGS) -D_FILE_OFFSET_BITS=64   $(IFLAGS) src/KeyBase.cpp 

#
#   RsaBase.o
#
DEPS_5 += inc/RsaBase.h

$(BUILD)/obj/RsaBase.o: \
    src/RsaBase.cpp $(DEPS_5)
	@echo '   [Compile] $(BUILD)/obj/RsaBase.o'
	$(CC) -c -o $(BUILD)/obj/RsaBase.o $(CFLAGS) $(DFLAGS) -D_FILE_OFFSET_BITS=64   $(IFLAGS) src/RsaBase.cpp
	
#
#   main.o
#
DEPS_3 += inc/osdep.h
DEPS_3 += inc/UtilsInspurInterface.h
DEPS_3 += inc/UtilsInspurBase.h
DEPS_3 += inc/RsaInterface.h
DEPS_3 += inc/RsaBase.h
DEPS_3 += inc/KeyInterface.h
DEPS_3 += inc/KeyBase.h

$(BUILD)/obj/main.o: \
    src/main.cpp $(DEPS_3)
	@echo '   [Compile] $(BUILD)/obj/main.o'
	$(CC) -c -o $(BUILD)/obj/main.o $(CFLAGS) $(DFLAGS) -D_FILE_OFFSET_BITS=64   $(IFLAGS) src/main.cpp
	

#
#   libgo
#
DEPS_4 += inc/osdep.h
DEPS_4 += inc/UtilsInspurInterface.h
DEPS_4 += inc/UtilsInspurBase.h
DEPS_4 += inc/RsaInterface.h
DEPS_4 += inc/RsaBase.h
DEPS_4 += inc/KeyInterface.h
DEPS_4 += inc/KeyBase.h

DEPS_4 += $(BUILD)/obj/KeyBase.o
DEPS_4 += $(BUILD)/obj/UtilsInspurBase.o
DEPS_4 += $(BUILD)/obj/RsaBase.o

$(BUILD)/out/libgo.so: $(DEPS_4)
	@echo '      [Link] $(BUILD)/out/libgo.so'
	$(CC) -shared -o $(BUILD)/out/libgo.so $(LDFLAGS) $(LIBPATHS) "$(BUILD)/obj/KeyBase.o" "$(BUILD)/obj/UtilsInspurBase.o" "$(BUILD)/obj/RsaBase.o"  
	
#
#   sigtool
#
DEPS_5 += inc/osdep.h
DEPS_5 += inc/UtilsInspurInterface.h
DEPS_5 += inc/UtilsInspurBase.h
DEPS_5 += inc/RsaInterface.h
DEPS_5 += inc/RsaBase.h
DEPS_5 += inc/KeyInterface.h
DEPS_5 += inc/KeyBase.h

DEPS_5 += $(BUILD)/obj/main.o

LIBS_5 += -lgo
LIBPATHS_5 += -L./out
$(BUILD)/out/sigtool: $(DEPS_5)
	@echo '      [Link] $(BUILD)/out/sigtool'
	$(CC) -o $(BUILD)/out/sigtool $(LDFLAGS)  "$(BUILD)/obj/main.o" "$(BUILD)/obj/KeyBase.o" "$(BUILD)/obj/UtilsInspurBase.o" "$(BUILD)/obj/RsaBase.o"    $(LIBPATHS) $(LIBS)
	
help:
	@echo '' >&2
	@echo 'usage: make [compile, clean]' >&2
	@echo '' >&2
	@echo '  OPENSSL_LIB_PATH                    # The OpenSSL LIB PATH' >&2
	@echo '  OPENSSL_INCLUDE_PATH                # The OpenSSL INCLUDE PATH' >&2
	@echo '' >&2
	@echo '  ARCH               # CPU architecture (x86, x64, ppc, ...)' >&2
	@echo '  OS                 # Operating system (linux, macosx, windows, vxworks, ...)' >&2
	@echo '  CC                 # Compiler to use ' >&2
	@echo '  LD                 # Linker to use' >&2
	@echo '  CFLAGS             # Add compiler options. For example: -Wall' >&2
	@echo '  DEBUG              # Set to "debug" for symbols, "release" for optimized builds' >&2
	@echo '  DFLAGS             # Add compiler defines. For example: -DCOLOR=blue' >&2
	@echo '  IFLAGS             # Add compiler include directories. For example: -I/extra/includes' >&2
	@echo '  LDFLAGS            # Add linker options' >&2
	@echo '  LIBPATHS           # Add linker library search directories. For example: -L/libraries' >&2
	@echo '  LIBS               # Add linker libraries. For example: -lpthreads' >&2
	@echo '' >&2
	@echo 'Use "SHOW=1 make" to show executed commands.' >&2
	@echo '' >&2
	
