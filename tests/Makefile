# A minimalistic Makefile for all testing options.
# Note: on OSX, this necessitates a recent OpenSSL, possibly installed by brew,
# so I had to do:
# export CFLAGS="-I/usr/local/Cellar/openssl@1.1/1.1.1d/include/"
# export LDFLAGS="-L/usr/local/Cellar/openssl@1.1/1.1.1d/lib"

KREMLIN_HOME?=../dist/kremlin

# Add GF128 tests once code/experimental/gf128 is promoted to code
TARGETS = $(filter-out gf128-%, $(patsubst %.c,%.exe,$(wildcard *.c)))
CFLAGS := -I$(KREMLIN_HOME)/include -I../dist/gcc64-only \
  -I$(KREMLIN_HOME)/kremlib/dist/minimal \
  -I../secure_api/merkle_tree \
  -O3 -march=native -mtune=native $(CFLAGS)

all: $(TARGETS)

test: $(patsubst %.exe,%.test,$(TARGETS))

arm: chacha20-arm-test.test poly1305-arm-test.test blake2-arm-test.test

# Dependency

%.d: %.c
	@set -e; rm -f $@; \
	  $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	  sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $@)\1.o $@ : ,g' < $@.$$$$ > $@; \
	  rm -f $@.$$$$

# Compiling against individual files rather than a whole .a
# Side-effect: running with -B ensures *ALL* files get recompiled with optimized
# flags.

curve64-rfc.exe: $(patsubst %.c,%.o,$(wildcard rfc7748_src/*.c))

%.exe: %.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ ../dist/gcc64-only/libevercrypt.a -o $@ -lcrypto

# Running tests

%.test: %.exe
	./$<
