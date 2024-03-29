# Deoxys-II

This is an implementation of the Deoxys-II and Deoxys-AE2 algorithms described in [the paper](https://thomaspeyrin.github.io/web/assets/docs/papers/Jean-etal-JoC2021.pdf).

More infos on [this Google Site](https://sites.google.com/view/deoxyscipher)

# Usage

The build step requires the GCC compiler.

- Run `./build` to build the tests
- Run `./test` to run the tests

To use AES-NI, run `./build aes`

To fully utilize vector instructions, run `./build vector`

For both, use the option `vector-aes`.

When compiling AE2 mode, use `AE2` before the optimization option \
e.g. `./build AE2 vector-aes`

# Goals

The goal of this exercise is a working implementation of the Deoxys-II-256-128 cipher. Since there is a current lack of test samples, successful en- and decryption would meet the goal.
The cipher might be coded modular enough to also allow the 128bit version of Deoxys-II to work.
Support for Deoxys-I is not planned.

# Disclaimers

This project is a side project of mine. There might not be development in months as other work gets priority.
The goal is solely a working implementation, not a secure one. There might be implementation specific exploits which may weaken the security of the encryption.
In particular, timing attacks and other side-channel attacks might be possible, severly limiting the actual usability of the implementation. Usability is not a priority.

For further disclaimers and the conditions of licensing, please refer to the `LICENSE`.
