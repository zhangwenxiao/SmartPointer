#ifndef __SMARTPOINTER_H_
#define __SMARTPOINTER_H_

#define COMPARE(__op__)                            \
bool operator __op__ (const SmartPointer& o) const \
{                                                  \
    return mPointer __op__ o.mPointer;             \
}                                                  \
bool operator __op__ (const T* o) const            \
{                                                  \
    return mPointer __op__ o;                      \
}                                                  \

template <typename T>
class SmartPointer {
public:
    // 构造函数
    SmartPointer() : mPointer(NULL) // 默认构造函数
    {
        std::cout << "create unknown smart pointer." << std::endl;
    }

    SmartPointer(T* p) : mPointer(p) // 接收指定类型的指针参数
    {
        std::cout << "create smart pointer at " 
            << static_cast<const void*>(p) << std::endl;

        if(mPointer) mPointer -> incRefCount();
    }

    // 拷贝构造函数
    SmartPointer(const SmartPointer& other) 
        : mPointer(other.mPointer)
    {
        std::cout << "Copy smart pointer at " 
            << static_cast<const void*>(other.mPointer) 
            << std::endl;

        if(mPointer) mPointer -> incRefCount();
    }
    // 赋值操作符
    SmartPointer& operator = (const SmartPointer& other)
    {
        T* temp = other.mPointer;
        if(temp) temp -> incRefCount();

        if(mPointer && mPointer -> decRefCount() == 0) 
            delete mPointer;

        mPointer = temp;
        return *this;
    }

    // 析构函数
    ~SmartPointer()
    {
        std::cout << "release smart pointer at " 
            << static_cast<const void*>(mPointer) << std::endl;

        if(mPointer && mPointer -> decRefCount() == 0) 
            delete mPointer;
    }

    T& operator* () const { return *mPointer; }
    T* operator -> () const { return mPointer; }

    COMPARE(==);
    COMPARE(!=);

private:
    T* mPointer; // 指向智能指针实际对应的内存资源
};

class RefBase {
public:
    RefBase() : mCount(0) {}

    void incRefCount()
    {
        mCount++;
    }
    int decRefCount()
    {
        return --mCount;
    }
    //调试接口，返回对象当前引用计数
    int getRefCount()
    {
        return mCount;
    }

    virtual ~RefBase() {}

private:
    int mCount;
};

#endif // __SMARTPOINTER_H_
