/* MIT License
 *
 * Copyright (c) 2016-2020 INRIA, CMU and Microsoft Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "Hacl_Curve25519_64.h"

static u8
Hacl_Curve25519_64_g25519[32U] =
  {
    (u8)9U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U,
    (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U,
    (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U, (u8)0U
  };

static void Hacl_Curve25519_64_point_add_and_double(u64 *q, u64 *p01_tmp1, u64 *tmp2)
{
  u64 *nq = p01_tmp1;
  u64 *nq_p1 = p01_tmp1 + (u32)8U;
  u64 *tmp1 = p01_tmp1 + (u32)16U;
  u64 *x1 = q;
  u64 *x2 = nq;
  u64 *z2 = nq + (u32)4U;
  u64 *z3 = nq_p1 + (u32)4U;
  u64 *a = tmp1;
  u64 *b = tmp1 + (u32)4U;
  u64 *ab = tmp1;
  u64 *dc = tmp1 + (u32)8U;
  u64 *x3;
  u64 *z31;
  u64 *d0;
  u64 *c0;
  u64 *a1;
  u64 *b1;
  u64 *d;
  u64 *c;
  u64 *ab1;
  u64 *dc1;
  fadd(a, x2, z2);
  fsub(b, x2, z2);
  x3 = nq_p1;
  z31 = nq_p1 + (u32)4U;
  d0 = dc;
  c0 = dc + (u32)4U;
  fadd(c0, x3, z31);
  fsub(d0, x3, z31);
  fmul2(tmp2, dc, dc, ab);
  fadd(x3, d0, c0);
  fsub(z31, d0, c0);
  a1 = tmp1;
  b1 = tmp1 + (u32)4U;
  d = tmp1 + (u32)8U;
  c = tmp1 + (u32)12U;
  ab1 = tmp1;
  dc1 = tmp1 + (u32)8U;
  fsqr2(tmp2, ab1, dc1);
  fsqr2(tmp2, nq_p1, nq_p1);
  a1[0U] = c[0U];
  a1[1U] = c[1U];
  a1[2U] = c[2U];
  a1[3U] = c[3U];
  fsub(c, d, c);
  fmul1(b1, c, (u64)121665U);
  fadd(b1, b1, d);
  fmul2(tmp2, dc1, nq, ab1);
  fmul(tmp2, z3, z3, x1);
}

static void Hacl_Curve25519_64_point_double(u64 *nq, u64 *tmp1, u64 *tmp2)
{
  u64 *x2 = nq;
  u64 *z2 = nq + (u32)4U;
  u64 *a = tmp1;
  u64 *b = tmp1 + (u32)4U;
  u64 *d = tmp1 + (u32)8U;
  u64 *c = tmp1 + (u32)12U;
  u64 *ab = tmp1;
  u64 *dc = tmp1 + (u32)8U;
  fadd(a, x2, z2);
  fsub(b, x2, z2);
  fsqr2(tmp2, ab, dc);
  a[0U] = c[0U];
  a[1U] = c[1U];
  a[2U] = c[2U];
  a[3U] = c[3U];
  fsub(c, d, c);
  fmul1(b, c, (u64)121665U);
  fadd(b, b, d);
  fmul2(tmp2, dc, nq, ab);
}

static void Hacl_Curve25519_64_montgomery_ladder(u64 *out, u8 *key, u64 *init1)
{
  u64 tmp2[16U] = { 0U };
  u64 p01_tmp1_swap[33U] = { 0U };
  u64 *p0 = p01_tmp1_swap;
  u64 *p01 = p01_tmp1_swap;
  u64 *p03 = p01;
  u64 *p11 = p01 + (u32)8U;
  u64 *x0;
  u64 *z0;
  u64 *p01_tmp1;
  u64 *p01_tmp11;
  u64 *nq10;
  u64 *nq_p11;
  u64 *swap1;
  u64 sw0;
  u64 *nq1;
  u64 *tmp1;
  memcpy(p11, init1, (u32)8U * sizeof init1[0U]);
  x0 = p03;
  z0 = p03 + (u32)4U;
  x0[0U] = (u64)1U;
  x0[1U] = (u64)0U;
  x0[2U] = (u64)0U;
  x0[3U] = (u64)0U;
  z0[0U] = (u64)0U;
  z0[1U] = (u64)0U;
  z0[2U] = (u64)0U;
  z0[3U] = (u64)0U;
  p01_tmp1 = p01_tmp1_swap;
  p01_tmp11 = p01_tmp1_swap;
  nq10 = p01_tmp1_swap;
  nq_p11 = p01_tmp1_swap + (u32)8U;
  swap1 = p01_tmp1_swap + (u32)32U;
  cswap2((u64)1U, nq10, nq_p11);
  Hacl_Curve25519_64_point_add_and_double(init1, p01_tmp11, tmp2);
  swap1[0U] = (u64)1U;
  {
    u32 i;
    for (i = (u32)0U; i < (u32)251U; i = i + (u32)1U)
    {
      u64 *p01_tmp12 = p01_tmp1_swap;
      u64 *swap2 = p01_tmp1_swap + (u32)32U;
      u64 *nq2 = p01_tmp12;
      u64 *nq_p12 = p01_tmp12 + (u32)8U;
      u64 bit = (u64)(key[((u32)253U - i) / (u32)8U] >> ((u32)253U - i) % (u32)8U & (u8)1U);
      u64 sw = swap2[0U] ^ bit;
      cswap2(sw, nq2, nq_p12);
      Hacl_Curve25519_64_point_add_and_double(init1, p01_tmp12, tmp2);
      swap2[0U] = bit;
    }
  }
  sw0 = swap1[0U];
  cswap2(sw0, nq10, nq_p11);
  nq1 = p01_tmp1;
  tmp1 = p01_tmp1 + (u32)16U;
  Hacl_Curve25519_64_point_double(nq1, tmp1, tmp2);
  Hacl_Curve25519_64_point_double(nq1, tmp1, tmp2);
  Hacl_Curve25519_64_point_double(nq1, tmp1, tmp2);
  memcpy(out, p0, (u32)8U * sizeof p0[0U]);
}

static void Hacl_Curve25519_64_fsquare_times(u64 *o, u64 *inp, u64 *tmp, u32 n1)
{
  u32 i;
  fsqr(tmp, inp, o);
  for (i = (u32)0U; i < n1 - (u32)1U; i = i + (u32)1U)
    fsqr(tmp, o, o);
}

static void Hacl_Curve25519_64_finv(u64 *o, u64 *i, u64 *tmp)
{
  u64 t1[16U] = { 0U };
  u64 *a0 = t1;
  u64 *b = t1 + (u32)4U;
  u64 *c = t1 + (u32)8U;
  u64 *t00 = t1 + (u32)12U;
  u64 *tmp1 = tmp;
  u64 *a;
  u64 *t0;
  Hacl_Curve25519_64_fsquare_times(a0, i, tmp1, (u32)1U);
  Hacl_Curve25519_64_fsquare_times(t00, a0, tmp1, (u32)2U);
  fmul(tmp, t00, b, i);
  fmul(tmp, b, a0, a0);
  Hacl_Curve25519_64_fsquare_times(t00, a0, tmp1, (u32)1U);
  fmul(tmp, t00, b, b);
  Hacl_Curve25519_64_fsquare_times(t00, b, tmp1, (u32)5U);
  fmul(tmp, t00, b, b);
  Hacl_Curve25519_64_fsquare_times(t00, b, tmp1, (u32)10U);
  fmul(tmp, t00, c, b);
  Hacl_Curve25519_64_fsquare_times(t00, c, tmp1, (u32)20U);
  fmul(tmp, t00, t00, c);
  Hacl_Curve25519_64_fsquare_times(t00, t00, tmp1, (u32)10U);
  fmul(tmp, t00, b, b);
  Hacl_Curve25519_64_fsquare_times(t00, b, tmp1, (u32)50U);
  fmul(tmp, t00, c, b);
  Hacl_Curve25519_64_fsquare_times(t00, c, tmp1, (u32)100U);
  fmul(tmp, t00, t00, c);
  Hacl_Curve25519_64_fsquare_times(t00, t00, tmp1, (u32)50U);
  fmul(tmp, t00, t00, b);
  Hacl_Curve25519_64_fsquare_times(t00, t00, tmp1, (u32)5U);
  a = t1;
  t0 = t1 + (u32)12U;
  fmul(tmp, t0, o, a);
}

static void Hacl_Curve25519_64_store_felem(u64 *b, u64 *f)
{
  u64 f30 = f[3U];
  u64 top_bit0 = f30 >> (u32)63U;
  u64 carry0;
  u64 f31;
  u64 top_bit;
  u64 carry;
  u64 f0;
  u64 f1;
  u64 f2;
  u64 f3;
  u64 m0;
  u64 m1;
  u64 m2;
  u64 m3;
  u64 mask;
  u64 f0_;
  u64 f1_;
  u64 f2_;
  u64 f3_;
  u64 o0;
  u64 o1;
  u64 o2;
  u64 o3;
  f[3U] = f30 & (u64)0x7fffffffffffffffU;
  carry0 = add1(f, f, (u64)19U * top_bit0);
  f31 = f[3U];
  top_bit = f31 >> (u32)63U;
  f[3U] = f31 & (u64)0x7fffffffffffffffU;
  carry = add1(f, f, (u64)19U * top_bit);
  f0 = f[0U];
  f1 = f[1U];
  f2 = f[2U];
  f3 = f[3U];
  m0 = FStar_UInt64_gte_mask(f0, (u64)0xffffffffffffffedU);
  m1 = FStar_UInt64_eq_mask(f1, (u64)0xffffffffffffffffU);
  m2 = FStar_UInt64_eq_mask(f2, (u64)0xffffffffffffffffU);
  m3 = FStar_UInt64_eq_mask(f3, (u64)0x7fffffffffffffffU);
  mask = ((m0 & m1) & m2) & m3;
  f0_ = f0 - (mask & (u64)0xffffffffffffffedU);
  f1_ = f1 - (mask & (u64)0xffffffffffffffffU);
  f2_ = f2 - (mask & (u64)0xffffffffffffffffU);
  f3_ = f3 - (mask & (u64)0x7fffffffffffffffU);
  o0 = f0_;
  o1 = f1_;
  o2 = f2_;
  o3 = f3_;
  b[0U] = o0;
  b[1U] = o1;
  b[2U] = o2;
  b[3U] = o3;
}

static void Hacl_Curve25519_64_encode_point(u8 *o, u64 *i)
{
  u64 *x = i;
  u64 *z = i + (u32)4U;
  u64 tmp[4U] = { 0U };
  u64 u64s[4U] = { 0U };
  u64 tmp_w[16U] = { 0U };
  Hacl_Curve25519_64_finv(tmp, z, tmp_w);
  fmul(tmp_w, tmp, tmp, x);
  Hacl_Curve25519_64_store_felem(u64s, tmp);
  {
    u32 i0;
    for (i0 = (u32)0U; i0 < (u32)4U; i0 = i0 + (u32)1U)
      store64_le(o + i0 * (u32)8U, u64s[i0]);
  }
}

void Hacl_Curve25519_64_scalarmult(u8 *out, u8 *priv, u8 *pub)
{
  u64 init1[8U] = { 0U };
  u64 tmp[4U] = { 0U };
  u64 tmp3;
  u64 *x;
  u64 *z;
  {
    u32 i;
    for (i = (u32)0U; i < (u32)4U; i = i + (u32)1U)
    {
      u64 *os = tmp;
      u8 *bj = pub + i * (u32)8U;
      u64 u = load64_le(bj);
      u64 r = u;
      u64 x0 = r;
      os[i] = x0;
    }
  }
  tmp3 = tmp[3U];
  tmp[3U] = tmp3 & (u64)0x7fffffffffffffffU;
  x = init1;
  z = init1 + (u32)4U;
  z[0U] = (u64)1U;
  z[1U] = (u64)0U;
  z[2U] = (u64)0U;
  z[3U] = (u64)0U;
  x[0U] = tmp[0U];
  x[1U] = tmp[1U];
  x[2U] = tmp[2U];
  x[3U] = tmp[3U];
  Hacl_Curve25519_64_montgomery_ladder(init1, priv, init1);
  Hacl_Curve25519_64_encode_point(out, init1);
}

void Hacl_Curve25519_64_secret_to_public(u8 *pub, u8 *priv)
{
  u8 basepoint[32U] = { 0U };
  {
    u32 i;
    for (i = (u32)0U; i < (u32)32U; i = i + (u32)1U)
    {
      u8 *os = basepoint;
      u8 x = Hacl_Curve25519_64_g25519[i];
      os[i] = x;
    }
  }
  Hacl_Curve25519_64_scalarmult(pub, priv, basepoint);
}

bool Hacl_Curve25519_64_ecdh(u8 *out, u8 *priv, u8 *pub)
{
  u8 zeros1[32U] = { 0U };
  Hacl_Curve25519_64_scalarmult(out, priv, pub);
  {
    u8 res = (u8)255U;
    u8 z;
    bool r;
    {
      u32 i;
      for (i = (u32)0U; i < (u32)32U; i = i + (u32)1U)
      {
        u8 uu____0 = FStar_UInt8_eq_mask(out[i], zeros1[i]);
        res = uu____0 & res;
      }
    }
    z = res;
    r = z == (u8)255U;
    return !r;
  }
}
