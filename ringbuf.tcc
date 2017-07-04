// Implementation of Ringbuf

template<typename _T>
Ringbuf<_T>::Ringbuf (size_t capacity)
    : _size(0), _capacity(capacity), _push_next(0), _pop_next(0)
{
    _ring = new _T[capacity];
}

template<typename _T>
Ringbuf<_T>::~Ringbuf()
{
    delete[] _ring;
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
    _ring[_push_next] = value;
    if (++_push_next == _capacity) _push_next = 0;
    increment_size();
    return 0;
}

template<typename _T>
int Ringbuf<_T>::ipop (_T& value)
{
    if (_size == 0) return 1;
    value = _ring[_pop_next];
    if (++_pop_next == _capacity) _pop_next = 0;
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

