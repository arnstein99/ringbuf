// Specialization of Ringbuf class with Posix mutex added

#ifndef __POSIX_RINGBUF_H_
#define __POSIX_RINGBUF_H_

#include <stdexcept>
#include <pthread.h>
#include "ringbuf.h"

template<typename _T>
class Posix_Ringbuf : public Ringbuf<_T>
{
public:

    Posix_Ringbuf (size_t capacity)
        : Ringbuf<_T> (capacity)
    {
        if (pthread_mutex_init (&_mutex, NULL) != 0)
	    throw std::runtime_error("mutex init");
    }

    ~Posix_Ringbuf ()
    {
	// TODO: handle an error here?
        pthread_mutex_destroy (&_mutex);
    }

protected:

    void increment_size (void)
    {
        if (pthread_mutex_lock (&_mutex) != 0)
	    throw std::runtime_error("mutex lock");
	Ringbuf<_T>::increment_size();
        if (pthread_mutex_unlock (&_mutex) != 0)
	    throw std::runtime_error("mutex unlock");
    }

    void decrement_size (void)
    {
        if (pthread_mutex_lock (&_mutex) != 0)
	    throw std::runtime_error("mutex lock");
	Ringbuf<_T>::decrement_size();
        if (pthread_mutex_unlock (&_mutex) != 0)
	    throw std::runtime_error("mutex unlock");
    }

private:

    pthread_mutex_t _mutex;
};

#endif // __POSIX_RINGBUF_H_
