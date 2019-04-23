#!/bin/bash

OPENSSL=openssl-1.1.1b
PAR=-j20
CONFIGS="gcc-7,g++-7 gcc-8,g++-8 clang-7,clang++-7 clang-8,clang++-8"
OPENSSL_CONFIGS="openssl-default, openssl-no-asm,no-asm"
SAMPLES=10

for c in $CONFIGS; do
  IFS=","
  set -- $c
  CC=$1
  CXX=$2
  unset IFS
  if hash $CC 2>/dev/null && hash $CXX 2>/dev/null; then
    for o in $OPENSSL_CONFIGS; do
      IFS=","
      set -- $o
      OCONF=$1
      OFLAGS=$2
      unset IFS
      if [ ! -d $OCONF-$CC ]; then
        if [ ! -f $OPENSSL.tar.gz ]; then
          wget https://www.openssl.org/source/$OPENSSL.tar.gz --no-check-certificate
        fi
        echo "Building $OCONF-$CC"
        (mkdir -p $OCONF-$CC; tar xfz $OPENSSL.tar.gz -C $OCONF-$CC; pushd $OCONF-$CC/$OPENSSL; CC=$CC CXX=$CXX ./config $OFLAGS; make $PAR; popd) > build.log 2>&1
      fi
      if [ ! -d evercrypt-$OCONF-$CC ]; then
        echo "Configuring $CC with $OCONF-$CC"
        mkdir -p evercrypt-$OCONF-$CC
        (pushd evercrypt-$OCONF-$CC; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX -DUSE_OPENSSL=ON -DOPENSSL_LIB=$OCONF-$CC/$OPENSSL/libcrypto.a -DOPENSSL_INC=$OCONF-$CC/$OPENSSL/include .. 2>&1; popd) > evercrypt-$OCONF-$CC/configure.log
      fi
      pushd evercrypt-$OCONF-$CC > /dev/null
      echo "(Re-)building EverCrypt with $CC and $OCONF-$CC"
      make $PAR > build.log 2>&1
      echo "Running benchmarks for $CC with $OCONF-$CC"
      (./runbenchmark -n $SAMPLES) > run.log 2>&1
      popd > /dev/null
    done
  fi
done