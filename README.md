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
## Public Members
### Constructor
`Ringbuf (size_t capacity)` This is the only provided constructor. The argument is the number of elements in the ring.
### Destructor
`virtual ~Ringbuf ()` Virtual, due to existence of virtual protected member functions `increment_size()` and `decrement_size()`. These are described in the subsequent section **Thread Safety**.
### Insert Function, With Return Value
`int ipushback (const _T&)` returns 0 for success, 1 if ring is full. Copy semantics. Entry in ring is overwritten if necessary.
### Insert Operator, With Exception
`template<typename _T>
Ringbuf<_T>& operator<< (Ringbuf<_T>&, const _T&) throw (RingbufFullException)`. Copy semantics. Entry in ring is overwritten if necessary.
### Removal Function, With Return Value
`int ipop (_T&)` Returns 0 for success, 1 if ring is empty. Copy semantics. Entry in ring is not destroyed.
### Removal Operator, With Exception
`template<typename _T>
Ringbuf<_T>& operator>> (Ringbuf<_T>&, _T&) throw (RingbufEmptyException)` Copy semantics. Entry in ring is not destroyed.
## Thread Safety
This design can be rendered safe to use in an environment where there is a single thread devoted to buffer insertion and a single thread devoted to buffer removal. **The design of this class is not suitable for use if there are multiple threads that insert or multiple threads that remove. Data corruption is almost certain to occur in these situations.**

A mutex or similar mechanism is almost certain to be needed. This can be accomplished by deriving a new class from `Ringbuf` with overrides for two virtual functions `increment_size()` and `decrement_size()`. The first function is guaranteed to be called only when inserting into the ring buffer, the second function is guaranteed to be called only when removing from the ring buffer.

An example of this derivation is included in file `posix_ringbuf.h`. This file is useful in its own right: it implements the mutex using the Posix pthread library. The derived template class is named `Posix_Ringbuf`.
## The Test Program
The file `testring.cc` contains a test program. An associated `Makefile` is also provided.

`testring.cc` has a few tuning parameters near its beginning. In particular, a `#define` determines whether the basic `Ringbuf` class or the thread-safe `Posix_Ringbuf` class is to be tested.

I verified the test program and its associated Makefile on a Debian Linux host. I found that the mutex feature was absolutely required to avoid incorrect behavior.
