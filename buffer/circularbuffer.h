//包含一些算法
#ifndef _CIRCLEULARBUFFER_H
#define _CIRCLEULARBUFFER_H

#include <algorithm> // for std::min
#include <inttypes.h>

/////这部分明显还需要进行优化，但暂时就不优化了,还有很多事情要做

class CircularBuffer
{
public:
    CircularBuffer(size_t capacity);//容量

    ~CircularBuffer();

    bool Init();

    //每次调用这个函数的时候都更新一下_size
    size_t Size() const { return size_; }//大小

    size_t Capacity() const { return capacity_; }//容量

    // Return number of bytes written.
    //返回写入的字节数
    size_t Write(const char *data, size_t bytes);//读

    // Return number of bytes read.
    // 返回读取的字节数
    size_t Read(char *data, size_t bytes);//写

    //只读取，不移动指针
    size_t ReadOnly(char* data, size_t bytes, size_t index = 0);

    //读取一个uint32，读取后指针不动,默认从当前位置开始读，也可以从指定的位置读
    uint32_t ReadUint32Only(size_t index = 0);

    //读取一个uint32
    uint32_t ReadUint32();

    size_t AvailableSize();//可用的大小

    void Clear(bool rmdata);

    //返回尾指针，用来接受数据
    char *Tail(){return data_ + end_index_;}

    //接受数据可用的大小
    size_t RecvSize();

    //重新分配大小
    bool ReSize(size_t n,char c);

    //增加buffer的容量
    bool IncreaseCapacity(size_t n = 0);

    //是否为空
    bool Empty(){return size_ == 0;}

    //默认跳过所有
    bool SkipData(size_t bytes = 0XFFFFFFFF);

    bool UpdateSize(size_t addsize);

private:
    size_t beg_index_;  //开始
    size_t end_index_;  //结束
    size_t size_;       //大小
    size_t capacity_;   //容量
    char *data_;        //数据指针
};


#endif //_CIRCLEULARBUFFER_H

