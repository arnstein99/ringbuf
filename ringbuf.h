// Ringbuf classes
// A ring buffer and associated classes that are mostly in the style
// of the C++ Standard Template Library
//
// This class has a provision to install a mutex. To do so, override the
// provided methods increment_size() and decrement_size(). These functions
// are the only places where a "thread unsafe" condition can occur.

#ifndef __RINGBUF_H_
#define __RINGBUF_H_

// Some exceptions for use with this class
class RingbufException
{
};
class RingbufEmptyException
    : public RingbufException
{
};
class RingbufFullException
    : public RingbufException
{
};

// Class Ringbuf
template<typename _T>
class Ringbuf
{
public:

    Ringbuf (size_t capacity);
    virtual ~Ringbuf ();
    int ipushback (const _T&); // 0 for success, 1 for failure.
    int ipop (_T&);            // 0 for success, 1 for failure.

    size_t size(void) const;

protected:

    // Called when putting something into the ring buffer. To install eg a
    // mutex, override this and include your mutex logic.
    virtual void increment_size (void)
        { ++_size; }

    // Called when taking something out of the ring buffer. To install eg a
    // mutex, override this and include your mutex logic.
    virtual void decrement_size (void)
        { --_size; }
private:

    Ringbuf(); // No default constructor
    Ringbuf(const Ringbuf<_T>&); // No copy constructor

    size_t _size;
    size_t _capacity;
    _T* _push_next;
    _T* _pop_next;
    _T* _ring_start;
    _T* _ring_end;
};

// Helper functions
template<typename _T>
Ringbuf<_T>& operator>> (Ringbuf<_T>&, _T&) throw (RingbufEmptyException);
template<typename _T>
Ringbuf<_T>& operator<< (Ringbuf<_T>&, const _T&) throw (RingbufFullException);

// Implementation
#include "ringbuf.tcc"

#endif // __RINGBUF_H_
