module Hacl.Impl.Chacha20Poly1305.PolyCore

module ST = FStar.HyperStack.ST
open FStar.HyperStack
open FStar.HyperStack.All
open FStar.Mul

open Lib.IntTypes
open Lib.Buffer

open Hacl.Impl.Poly1305.Fields
open Hacl.Spec.Poly1305.Equiv

module SpecPoly = Spec.Poly1305
module Spec = Spec.Chacha20Poly1305
module Poly = Hacl.Impl.Poly1305

inline_for_extraction noextract
let poly1305_padded_st (w:field_spec) =
    ctx:Poly.poly1305_ctx w
  -> len:size_t
  -> text:lbuffer uint8 len ->
  Stack unit
    (requires fun h ->
      live h ctx /\ live h text /\ disjoint ctx text /\
      Poly.state_inv_t h ctx)
    (ensures fun h0 _ h1 ->
      modifies (loc ctx) h0 h1 /\
      Poly.state_inv_t h1 ctx /\
      // Additional framing for r_elem
      Poly.as_get_r h0 ctx == Poly.as_get_r h1 ctx /\
      // Functional spec
      Poly.as_get_acc h1 ctx ==
      Spec.poly1305_padded (Poly.as_get_r h0 ctx) (as_seq h0 text) (Poly.as_get_acc h0 ctx))

#set-options "--z3rlimit 50 --max_fuel 0 --max_ifuel 1"

inline_for_extraction noextract
val poly1305_padded_: #w:field_spec -> poly1305_padded_st w
let poly1305_padded_ #w ctx len text =
  let h0 = ST.get () in
  push_frame ();
  let h1 = ST.get () in
  Poly.reveal_ctx_inv ctx h0 h1;
  let n = len /. 16ul in
  let r = len %. 16ul in
  let blocks = sub text 0ul (n *! 16ul) in
  let rem = sub text (n *! 16ul) r in // the extra part of the input data
  Poly.poly1305_update #w ctx (n *! 16ul) blocks;
  let h2 = ST.get () in
  poly_eq_lemma #(width w) (as_seq h1 blocks) (Poly.as_get_acc h1 ctx) (Poly.as_get_r h1 ctx);
  let tmp = create 16ul (u8 0) in
  update_sub tmp 0ul r rem;
  let h3 = ST.get() in
  Poly.reveal_ctx_inv ctx h2 h3;
  if r >. 0ul then
    Poly.poly1305_update1 ctx tmp;
  let h4 = ST.get () in
  pop_frame();
  let h5 = ST.get () in
  Poly.reveal_ctx_inv ctx h4 h5

[@CInline]
let poly1305_padded_32 : poly1305_padded_st M32 = poly1305_padded_
[@CInline]
let poly1305_padded_128 : poly1305_padded_st M128 = poly1305_padded_
[@CInline]
let poly1305_padded_256 : poly1305_padded_st M256 = poly1305_padded_

inline_for_extraction noextract
val poly1305_padded: #w:field_spec -> poly1305_padded_st w
let poly1305_padded #w =
  match w with
  | M32 -> poly1305_padded_32
  | M128 -> poly1305_padded_128
  | M256 -> poly1305_padded_256
