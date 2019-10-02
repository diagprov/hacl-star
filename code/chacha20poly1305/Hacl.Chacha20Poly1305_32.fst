module Hacl.Chacha20Poly1305_32

open Hacl.Meta.Chacha20Poly1305
open Hacl.Impl.Chacha20Poly1305
open Hacl.Impl.Poly1305.Fields

#set-options "--z3rlimit 50 --max_fuel 0 --max_ifuel 0"

private
let poly1305_padded_32 = poly1305_padded_higher #M32
private
let poly1305_do_32 = poly1305_do_higher #M32 poly1305_padded_32

let aead_encrypt : aead_encrypt_st M32 =
  aead_encrypt_higher #M32 poly1305_do_32

let aead_decrypt : aead_decrypt_st M32 =
  aead_decrypt_higher #M32 poly1305_do_32