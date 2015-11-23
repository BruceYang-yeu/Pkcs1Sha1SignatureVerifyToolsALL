NAME                  := verify
VERSION               := 1.0.0
PROFILE               ?= default
ARCH                  ?= $(shell uname -m | sed 's/i.86/x86/;s/x86_64/x64/;s/arm.*/arm/;s/mips.*/mips/')
CC_ARCH               ?= $(shell echo $(ARCH) | sed 's/x86/i686/;s/x64/x86_64/')
OS                    ?= linux
CC                     = gcc
CONFIG                ?= $(OS)-$(ARCH)-$(PROFILE)
BUILD                 ?= $(shell pwd)
LBIN                  ?= $(BUILD)/bin
PATH                  := $(LBIN):$(PATH)

ME_COM_COMPILER       ?= 1
OPENSSL_LIB_PATH      ?= 
OPENSSL_INCLUDE_PATH  ?= 


CFLAGS                += -fPIC  -Wall
IFLAGS                += -I./inc
LIBPATHS              += -L$(OPENSSL_LIB_PATH)
LIBS                  +=  

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

TARGETS += $(BUILD)/obj/rsa.o
TARGETS += $(BUILD)/obj/sha.o
TARGETS += $(BUILD)/obj/verifier.o
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
	rm -f $(BUILD)/obj/sha.o
	rm -f $(BUILD)/obj/rsa.o
	rm -f $(BUILD)/obj/verifier.o
	rm -f $(BUILD)/obj/main.o
	rm -f $(BUILD)/out/verify

#
#   sha.o
#
DEPS_1 += inc/sha.h
$(BUILD)/obj/sha.o: \
    rsalib/sha.c $(DEPS_1)
	@echo '   [Compile] $(BUILD)/obj/sha.o'
	$(CC) -c -o $(BUILD)/obj/sha.o $(CFLAGS) $(DFLAGS) -D_FILE_OFFSET_BITS=64   $(IFLAGS) rsalib/sha.c

#
#   rsa.o
#
DEPS_2 += inc/rsa.h
DEPS_2 += inc/sha.h
DEPS_2 += inc/common.h

$(BUILD)/obj/rsa.o: \
    rsalib/rsa.c $(DEPS_2)
	@echo '   [Compile] $(BUILD)/obj/rsa.o'
	$(CC) -c -o $(BUILD)/obj/rsa.o $(CFLAGS) $(DFLAGS) -D_FILE_OFFSET_BITS=64   $(IFLAGS) rsalib/rsa.c

#
#   verifier.o
#
DEPS_5 += inc/verifier.h

$(BUILD)/obj/verifier.o: \
    rsalib/verifier.c $(DEPS_5)
	@echo '   [Compile] $(BUILD)/obj/verifier.o'
	$(CC) -c -o $(BUILD)/obj/verifier.o $(CFLAGS) $(DFLAGS) -D_FILE_OFFSET_BITS=64   $(IFLAGS) rsalib/verifier.c
	
#
#   main.o
#
DEPS_3 += inc/osdep.h
DEPS_3 += inc/rsa.h
DEPS_3 += inc/verifier.h
DEPS_3 += inc/sha.h

$(BUILD)/obj/verify_test.o: \
    test/verify_test.c $(DEPS_3)
	@echo '   [Compile] $(BUILD)/obj/verify_test.o'
	$(CC) -c -o $(BUILD)/obj/verify_test.o $(CFLAGS) $(DFLAGS) -D_FILE_OFFSET_BITS=64   $(IFLAGS) test/verify_test.c
	
	
#
#   verify
#
DEPS_5 += inc/osdep.h
DEPS_5 += inc/rsa.h
DEPS_5 += inc/verifier.h
DEPS_5 += inc/sha.h

DEPS_5 += $(BUILD)/obj/verify_test.o

LIBS_5 += -lgo
LIBPATHS_5 += -L./out
$(BUILD)/out/verify: $(DEPS_5)
	@echo '      [Link] $(BUILD)/out/verify'
	$(CC) -o $(BUILD)/out/verify $(LDFLAGS)  "$(BUILD)/obj/verify_test.o" "$(BUILD)/obj/rsa.o" "$(BUILD)/obj/verifier.o" "$(BUILD)/obj/sha.o" 
	
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
	
