//包含一些算法

#include <string.h>
#include "circularbuffer.h"
#include <arpa/inet.h>

inline uint16_t XHTONS(uint16_t i16)
{
    return ((i16 << 8) | (i16 >> 8));
}

inline uint32_t XHTONL(uint32_t i32)
{
    return ((uint32_t(XHTONS(i32)) << 16) | XHTONS(i32 >> 16));
}

//构造
CircularBuffer::CircularBuffer(size_t capacity)
  : beg_index_(0)
  , end_index_(0)
  , capacity_(capacity)
  , size_(0)
{
    Init();
}

bool CircularBuffer::Init()
{

    //当内存不足时并不抛出异常，而是返回NLL
    data_ = new (std::nothrow) char[capacity_];
    if(data_ == NULL)
        return false;
    return true;
}

//析构
CircularBuffer::~CircularBuffer()
{
    if(data_)
        delete [] data_;
}

//这里如果是socket接受的话，需要避免内存拷贝,怎样直接用当前空间接受
//写入,如果空间不足怎么处理
size_t CircularBuffer::Write(const char *data, size_t bytes)
{
    //如果没有数据需要写入直接返回
    if (bytes == 0 || data == NULL) return 0;

    size_t capacity = capacity_;
    size_t bytes_to_write = std::min(bytes, capacity - size_);

    // Write in a single steps
    // 首先，如果可以写完，把数据写在数据区的剩余位置
    if (bytes_to_write <= capacity - end_index_)
    {
        memcpy(data_ + end_index_, data, bytes_to_write);
        end_index_ += bytes_to_write;
        if (end_index_ == capacity)
        {
            end_index_ = 0;
        }
    }
    // Write in two steps
    // 如果写不完，就继续写入前面的空间
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


//读取
size_t CircularBuffer::Read(char *data, size_t bytes)
{
    if (bytes == 0||data == NULL) return 0;

    size_t capacity = capacity_;
    size_t bytes_to_read = std::min(bytes, size_);

    // Read in a single step
    if (bytes_to_read <= capacity - beg_index_)
    {
        memcpy(data, data_ + beg_index_, bytes_to_read);
        beg_index_ += bytes_to_read;
        if (beg_index_ == capacity) beg_index_ = 0;
    }
    // Read in two steps
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


//读取，读取之后不移动指针
size_t CircularBuffer::ReadOnly(char *data, size_t bytes ,size_t index)
{
    if (bytes == 0||size_ == 0) return 0;

    int old_beg = beg_index_;
    beg_index_ += index;
    if(beg_index_ > capacity_)
    {
        beg_index_ -= capacity_;
    }

    size_t capacity = capacity_;
    size_t bytes_to_read = std::min(bytes, size_ - index);

    // Read in a single step
    if (bytes_to_read <= capacity - beg_index_)
    {
        memcpy(data, data_ + beg_index_, bytes_to_read);
        //beg_index_ += bytes_to_read;
        //if (beg_index_ == capacity) beg_index_ = 0;
    }
    // Read in two steps
    else
    {
        size_t size_1 = capacity - beg_index_;
        memcpy(data, data_ + beg_index_ , size_1);
        size_t size_2 = bytes_to_read - size_1;
        memcpy(data + size_1, data_, size_2);
        //beg_index_ = size_2;
    }
    //还原beg_index_
    beg_index_ = old_beg;

    //不需要更新size_
    //size_ -= bytes_to_read;
    return bytes_to_read;
}

//读取一个int，返回值返回
uint32_t CircularBuffer::ReadUint32()
{
    uint32_t i;
    Read((char*)&i,sizeof(i));
    return XHTONL(i);
}

//读取一个int，返回值返回
uint32_t CircularBuffer::ReadUint32Only(size_t index)
{
    uint32_t i;
    //这里可能出现读取数据不足的情况，读取之前需要先判断
    ReadOnly((char*)&i,sizeof(i),index);
    //return XHTONL(i);
    return ntohl(i);
}

//可用的部分
size_t CircularBuffer::AvailableSize()
{
    return capacity_ - size_;
}

//可用于接受数据的大小
size_t CircularBuffer::RecvSize()
{
    //如果,是从0开始的，或者到最后结束，可以证明是连续的
    if(beg_index_ == 0 || end_index_ == capacity_)
    {
        return AvailableSize();
    }
    else
    {
        return capacity_ - end_index_;
    }
}

//重置buffer,这里不会清除数据
void CircularBuffer::Clear(bool rmdata)
{
    beg_index_ = 0;
    end_index_ = 0;
    size_ = 0;
    //把数据清0
    if(rmdata)
    {
        memset(data_, 0, capacity_);
    }
}

//增加容量,传入参数的容量为从容量的大小,如果传入参数为0，就默认增加两倍
bool CircularBuffer::IncreaseCapacity(size_t n)
{
    if(!n)
    {
        IncreaseCapacity(capacity_*2);
    }
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
            //拷贝原来的数据
            memmove(data_,olddata,oldcappacity);
            delete [] olddata;
            Clear(false);   //初始化原来的数据
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


bool CircularBuffer::SkipData(size_t bytes)
{
    if(bytes >= size_)
    {
        //既然是删除所有，直接清0
        Clear(false);
    }
    else
    {
        //第一种情况，调到末尾之前
        if(bytes < (capacity_  - beg_index_))
        {
            beg_index_ += bytes;
            size_ -= bytes;
        }
        else
        {
            bytes = bytes - (capacity_ - beg_index_);
            beg_index_ += bytes;
            size_ -= bytes;
        }
    }
}

bool CircularBuffer::UpdateSize(size_t addsize)
{
    if((size_ + addsize) > capacity_)
    {
        return false;
    }
    size_ += addsize;
    end_index_ += addsize;
    if(end_index_ > capacity_)
    {
        end_index_ = end_index_ - capacity_ ;
    }
}


