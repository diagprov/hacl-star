module Hacl.Ocaml.Argmax.Functest

open FStar.All
open FStar.IO
open FStar.Mul
open FStar.UInt

open Hacl.Argmax.Common

module GM = Hacl.Argmax.GM
module P = Hacl.Argmax.Paillier
module U64 = FStar.UInt64

val print_nat: c:nat{c < pow2 64} -> ML unit
let print_nat c =
  let c': uint_t 64 = c in
  print_uint64 (U64.uint_to_t c)

val print_bool: b:bool -> ML unit
let print_bool b = print_string (if b then "true" else "false")

val run_test_gm:
     p:prm
  -> q:prm{q <> p /\ p*q < pow2 64}
  -> y:fe (p * q){GM.is_nonsqr (to_fe #p y) /\ GM.is_nonsqr (to_fe #q y)}
  -> r:fe (p * q){sqr r > 0 /\ sqr r *% y > 0}
  -> ML unit
let run_test_gm p q y r =
  let n:comp = mkcomp p q in
  assert (n = p * q);
  let sec = GM.Secret p q y in
  let pub = GM.s2p sec in

  let encdec (m:bool) = begin
    print_string "\n* ";
    print_bool m;

    print_string " --enc-> ";
    let c = GM.encrypt pub r m in
    print_nat c;

    print_string " --dec-> ";
    let m' = GM.decrypt sec c in
    print_bool m'

    end in

  encdec false;
  encdec true


val test_gm: unit -> ML unit
let test_gm () =
  let to_prime (p:big): prm = (admit(); p) in

  let with_asserted_vals (p0:big) (q0:big{q0<>p0}) (y0:pos) (r0:pos) = begin
    let p:prm = to_prime p0 in
    let q:prm = to_prime q0 in
    assume (p * q < pow2 64);
    let y:fe (p*q) = (assume (y0 < p * q); y0) in
    assume (GM.is_nonsqr #p (to_fe #p y) /\ GM.is_nonsqr #q (to_fe #q y));
    let r:fe (p*q) = (assume (r0 < p * q); r0) in
    assume (sqr r > 0 /\ sqr r *% y > 0);

    run_test_gm p q y r
    end in

  print_string "GM params1";
  with_asserted_vals 7 11 6 6;
  print_string "\nGM params2";
  with_asserted_vals 2309 5651 6283665 1642087;
  print_string "\nGM params3";
  with_asserted_vals 2309 5651 6283665 11200984;
  print_string "\nGM test done\n\n"


val test_paillier: unit -> ML unit
let test_paillier () =
  let q:prm = (assume(isprm 7); 7) in
  let p:prm = (assume(isprm 11); 11) in
  let n:comp = mkcomp p q in
  assert (n = p * q);
  assert (n = 77);
  // filter (\(x,o) -> o `mod` 77 == 0) $ catMaybes $ map (\x -> (x,) <$> order x (77*77)) [1..77*77-1]
  // order of 617 is exactly 77, and gcd 617 (77*77) = 1
  assert (617 < 77 * 77);
  let g:P.fen2 n = 617 in
  assume (isunit g /\ mult_order g = 77);
  let sec = P.Secret p q g in
  let pub = P.s2p sec in
  let r:fe n = 40 in
  assert (gcd r n = 1);
  inv_as_gcd1 #n r;

  print_string "Paillier:";

  let enc_dec (m:fe n) = begin
    print_string "\n* ";
    print_nat m;

    print_string " --enc-> ";
    let c = P.encrypt pub r m in
    print_nat c;

    print_string " --dec-> ";
    let m' = P.decrypt sec c in
    print_nat m'
    end in

  enc_dec 1;
  enc_dec 20;
  enc_dec 35;
  enc_dec 47;
  enc_dec 58;
  enc_dec 76;

  print_string "\nPaillier test done\n"


val main: unit
let main =
  test_gm ();
  test_paillier ()