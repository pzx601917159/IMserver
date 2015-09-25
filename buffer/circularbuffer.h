#ifndef _CIRCLEULARBUFFER_H
#define _CIRCLEULARBUFFER_H

#include <algorithm> // for std::min

class CircularBuffer
{
public:
    CircularBuffer(size_t capacity);

    ~CircularBuffer();

    bool Init();

    size_t Size() const { return size_; }

    size_t Capacity() const { return capacity_; }

    size_t Write(const char *data, size_t bytes);

    size_t Read(char *data, size_t bytes);

    size_t AvailableSize();

    void Clear(bool rmdata);

    char *Tail(){return data_ + end_index_;}

    size_t RecvSize();

    bool ReSize(size_t n,char c);

    bool IncreaseCapacity(size_t n);

private:
    size_t beg_index_;  
    size_t end_index_;  
    size_t size_;       
    size_t capacity_;   
    char *data_;    
};


#endif //_CIRCLEULARBUFFER_H

