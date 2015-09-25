#include <string.h>
#include "circularbuffer.h"

CircularBuffer::CircularBuffer(size_t capacity)
  : beg_index_(0)
  , end_index_(0)
  , size_(0)
  , capacity_(capacity)
{
}

bool CircularBuffer::Init()
{
    data_ = new (std::nothrow) char[capacity_];
    if(data_ == NULL)
        return false;
    return true;
}

CircularBuffer::~CircularBuffer()
{
    if(data_)
        delete [] data_;
}

size_t CircularBuffer::Write(const char *data, size_t bytes)
{
    if (bytes == 0) return 0;

    size_t capacity = capacity_;
    size_t bytes_to_write = std::min(bytes, capacity - size_);

    if (bytes_to_write <= capacity - end_index_)
    {
        memcpy(data_ + end_index_, data, bytes_to_write);
        end_index_ += bytes_to_write;
        if (end_index_ == capacity) end_index_ = 0;
    }
    else
    {
        size_t size_1 = capacity - end_index_;
        memcpy(data_ + end_index_, data, size_1);
        size_t size_2 = bytes_to_write - size_1;
        memcpy(data_, data + size_1, size_2);
        end_index_ = size_2;
    }

    size_ += bytes_to_write;
    return bytes_to_write;
}

size_t CircularBuffer::Read(char *data, size_t bytes)
{
    if (bytes == 0||size_ == 0) return 0;

    size_t capacity = capacity_;
    size_t bytes_to_read = std::min(bytes, size_);

    if (bytes_to_read <= capacity - beg_index_)
    {
        memcpy(data, data_ + beg_index_, bytes_to_read);
        beg_index_ += bytes_to_read;
        if (beg_index_ == capacity) beg_index_ = 0;
    }
    else
    {
        size_t size_1 = capacity - beg_index_;
        memcpy(data, data_ + beg_index_, size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy(data + size_1, data_, size_2);
        beg_index_ = size_2;
    }

    size_ -= bytes_to_read;
    return bytes_to_read;
}

size_t CircularBuffer::AvailableSize()
{
    return capacity_ - size_;
}

size_t CircularBuffer::RecvSize()
{
    if(beg_index_ == 0 || end_index_ > beg_index_)
    {
        return AvailableSize();
    }
    else
    {
        return capacity_ - end_index_;
    }
}

void CircularBuffer::Clear(bool rmdata)
{
    beg_index_ = 0;
    end_index_ = 0;
    size_ = 0;
    if(rmdata)
    {
        memset(data_, 0, capacity_);
    }
}

bool CircularBuffer::IncreaseCapacity(size_t n)
{
    if(n <= capacity_)
    {
        return true;
    }
    else
    {
        char *olddata = data_;
        size_t oldcappacity = capacity_;
        capacity_ = n;
        data_ = NULL;
        data_ = new (std::nothrow) char[capacity_];
        if(data_)
        {
            delete [] olddata;
            Clear(false);
            return true;
        }
        else
        {
            data_ = olddata;
            capacity_ = oldcappacity;
            return false;
        }
    }
}





