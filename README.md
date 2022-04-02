# Deoxys-II

This is an implementation of the Deoxys-II algorithm described in `deoxys_v143.pdf`.

# Usage

The build step requires the GCC compiler.

- Run `./build` to build the tests
- Run `./test` to run the tests

To use AES-NI, append `-maes` to the command in `./build`

# Goals

The goal of this exercise is a working implementation of the Deoxys-II-256-128 cipher. Since there is a current lack of test samples, successful en- and decryption would meet the goal.
The cipher might be coded modular enough to also allow the 128bit version of Deoxys-II to work.
Support for Deoxys-I is not planned.

# Disclaimers

This project is a side project of mine. There might not be development in months as other work gets priority.
The goal is solely a working implementation, not a secure one. There might be implementation specific exploits which may weaken the security of the encryption.
In particular, timing attacks and other side-channel attacks might be possible, severly limiting the actual usability of the implementation. Usability is not a priority.

For further disclaimers and the conditions of licensing, please refer to the `LICENSE`.
