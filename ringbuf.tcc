// Implementation of Ringbuf

template<typename _T>
Ringbuf<_T>::Ringbuf (size_t capacity)
    : _size(0), _capacity(capacity)
{
    _ring_start = new _T[capacity];
    _ring_end  = _ring_start + capacity;
    _push_next = _ring_start;
    _pop_next  = _ring_start;
}

template<typename _T>
Ringbuf<_T>::~Ringbuf()
{
    delete[] _ring_start;
}

template<typename _T>
size_t Ringbuf<_T>::size (void) const
{
    return _size;
}

template<typename _T>
int Ringbuf<_T>::ipushback (const _T& value)
{
    if (_size == _capacity) return 1;
    *_push_next = value;
    if (++_push_next == _ring_end) _push_next = _ring_start;
    increment_size();
    return 0;
}

template<typename _T>
int Ringbuf<_T>::ipop (_T& value)
{
    if (_size == 0) return 1;
    value = *_pop_next;
    if (++_pop_next == _ring_end) _pop_next = _ring_start;
    decrement_size();
    return 0;
}

template<typename _T>
Ringbuf<_T>& operator>> (Ringbuf<_T>& rbuf, _T& value)
    throw (RingbufEmptyException)
{
    if (rbuf.ipop (value) != 0)
        throw RingbufEmptyException();
     return rbuf;
}

template<typename _T>
Ringbuf<_T>& operator<< (Ringbuf<_T>& rbuf, const _T& value)
    throw (RingbufFullException)
{
    if (rbuf.ipushback (value) != 0)
        throw RingbufFullException();
     return rbuf;
}

