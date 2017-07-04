# ringbuf
A ring buffer template class in C++
## Introduction
The template class `Ringbuf` provides a simple ring buffer for an arbitrary data type. It uses copy semantics to insert and remove data items; this emulates the containers in the Standard Template Library.
## Contents
The definition for the class and associated helpers is in file `ringbuf.h`.

For readability, the implementation is in a separate file `ringbuf.tcc`.

There is an example derived class in file `posix_ringbuf.h`.

There is a simple test and demonstration program in file `testring.cc`.

There is a `Makefile` for the demonstration program, I tested it on a Debian Linux host.
## Thread Safety
A mutex or similar mechanism is almost certain to be needed. This can be accomplished by deriving a new class from `Ringbuf` with overrides for two virtual functions `increment_size()` and `decrement_size()`. The first function is guaranteed to be called only when inserting into the ring buffer, the second function is guaranteed to be called only when removing from the ring buffer.

An example of this derivation is included in file `posix_ringbuf.h`. This file is useful in its own right: it implements the mutex using the Posix pthread library. The derived template class is named `Posix_Ringbuf`.
## The Test Program
The file `testring.cc` contains a test program. An associated `Makefile` is also provided.

`testring.cc` has a few tuining parameters near its beginning. In particular, a `#define` determines whether the basic `Ringbuf` class or the thread-safe `Posix_Ringbuf` class is to be tested.

I verified the test program and its associated Makefile on a Debian Linux host. I found that the mutex feature was absolutely required to avoid incorrect behavior.
