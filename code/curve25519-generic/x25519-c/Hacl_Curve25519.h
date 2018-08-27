/* This file was generated by KreMLin <https://github.com/FStarLang/kremlin>
 * KreMLin invocation: /Users/bhargava/Desktop/repositories/kremlin/krml -I ../bignum -tmpdir x25519-c -bundle Hacl.Curve25519=* -minimal -add-include "kremlib.h" -skip-compilation x25519-c/out.krml -o x25519-c/Hacl_Curve25519.c
 * F* version: 2441c5c2
 * KreMLin version: 7661d65b
 */


#ifndef __Hacl_Curve25519_H
#define __Hacl_Curve25519_H


#include "kremlib.h"

extern FStar_UInt128_uint128 FStar_Int_Cast_Full_uint64_to_uint128(uint64_t x0);

void Hacl_Curve25519_crypto_scalarmult(uint8_t *mypublic, uint8_t *secret, uint8_t *basepoint);

#define __Hacl_Curve25519_H_DEFINED
#endif
