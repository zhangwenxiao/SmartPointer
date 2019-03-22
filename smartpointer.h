#ifndef __SMART_POINTER_H__
#define __SMART_POINTER_H__

#define COMPARE(__op__)                                         \
bool operator __op__ (const SmartPointer& o) const              \
{                                                               \
    return counter_ -> getPtr() __op__ o.counter_ -> getPtr();  \
}                                                               \
bool operator __op__ (const T* o) const                         \
{                                                               \
    return counter_ -> getPtr() __op__ o;                       \
}                                                               \

template<typename T>
class Counter {
public:
    Counter()
    {
        ptr_ = nullptr;
        cnt_ = 0;
    }

    Counter(T* p)
    {
        ptr_ = p;
        cnt_ = 1;
    }

    ~Counter() { delete ptr_; }

    void addCnt() { ++cnt_; }
    void decCnt() { --cnt_; }
    int getCnt() const { return cnt_; }

    T* getPtr() const { return ptr_; }

private:
    T* ptr_;
    int cnt_;
};

template<typename T>
class SmartPointer {
public:
    SmartPointer(T* p) { counter_ = new Counter<T>(p); }

    SmartPointer(const SmartPointer& sp)
    {
        counter_ = sp.counter_;
        counter_ -> addCnt();
    }

    SmartPointer& operator=(const SmartPointer& sp)
    {
        counter_ -> decCnt();
        if(counter_ -> getCnt() == 0)
            delete counter_;
        counter_ = sp.counter_;
        counter_ -> addCnt();
        return *this;
    }

    ~SmartPointer()
    {
        counter_ -> decCnt();
        if(counter_ -> getCnt() == 0)
            delete counter_;
    }

    T& operator*() const { return *(counter_ -> getPtr()); }
    T* operator->() const { return counter_ -> getPtr(); }

    COMPARE(==);
    COMPARE(!=);

private:
    Counter<T>* counter_;
};

#endif
