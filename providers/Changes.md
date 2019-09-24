## EverCrypt v0.1 alpha 2

### August 9th, 2019

- Addition of the box API under `Hacl_Nacl.h`. There is no multiplexing between
  implementations and as such, there will be no `EverCrypt.Nacl`.
- Addition of Salsa20 under `Hacl_Salsa20.h`. Salsa20 *may* be added to
  `EverCrypt.CTR` if there is demand for it (please speak up).

### August 5th, 2019

- OCaml bindings for EverCrypt (alpha, work in progress).

### July 13th, 2019

- Fully-verified implementation of Ed25519. Not currently multiplexing, but
  eventually will be. `EverCrypt_Ed25519.h` will perform multiplexing once
  multiple implementations of Ed25519 are available.

### July 9th, 2019

- `EverCrypt_CTR.h`, an agile, multiplexing API that exposes one block of the
  counter-mode construction (will eventually support complete encryption).

### June 27th, 2019

- (possibly) **Breaking change**: KreMLin now compiles deprecation warnings in
  F\* into C deprecation warnings. This may break clients who compile with -Werror.
  - Functions in EverCrypt.h for which a replacement exists have been marked
    with the attribute.
  - The public headers now take an additional header dependency on
    `kremlin/internal/target.h`

### June 25th, 2019

- **Breaking change** for F\* (`EverCrypt.AEAD.fst`) clients.  
  The state in EverCrypt.AEAD now contains scratch space to use in
  encrypt/decrypt. It is therefore modified at each encryption/decryption.
  (Aymeric Fromherz)

### June 20th, 2019

- **Breaking change** for C (`EverCrypt_AEAD.h`) and F\* (`EverCrypt.AEAD.fst`) clients.  
  EverCrypt now supports arbitrary length IVs for AES-GCM. A new
  parameter `iv_len` needs to be passed during encryption and decryption. In C, a
  new error InvalidIVLength is returned if the length of the iv does not satisfy
  an algorithm's requirements. (Aymeric Fromherz)

### Before then

- **Breaking change** for C (`EverCrypt_Hash.h`) and F\*
  (`EverCrypt.Hash.Incremental.fst`) clients.  
  The API now takes erased algorithm parameters; the style is now in line with
  other modules, relying on abstract states and framing lemmas (now with
  patterns). (Jonathan Protzenko)

- **New feature**: WASM implementation, found in dist/wasm. There are only
  tests, no proper JavaScript wrappers yet. (Jonathan Protzenko)

- **New feature**: fully-verified AVX and AVX2 implementations of Poly1305
  (Marina Polubelova, Karthikeyan Bhargavan)

- **New feature**: fully-verified ASM implementation of SHA2-256 using the
  SHA-EXT dedicated Intel instructions (Chris Hawblitzel, Bryan Parno)