#ifndef CLOGERR_H
#define CLOGERR_H

#include <iostream>
#include <string>
using std::string;
using std::clog;

namespace CLogErr{

// 使用 NDEBUG 预编译标志区分编译与未编译使得测试输出失效
class CLogErr
{
    public:
        CLogErr(){}
        virtual ~CLogErr(){}
        // 作为成员函数只能写一个参数，另一个是this
        // 对象版
        template <typename T>
        inline CLogErr &operator<< ( const T &object )
        {
            #ifndef NDEBUG
                clog << object;
            #endif
            return *this;
        }

        // 给 endl 和其他的函数指针版本
        template <typename T>
        inline CLogErr &operator<< ( T& (*fpObject)(T&))
        {
            #ifndef NDEBUG
                clog << fpObject;
            #endif
            return *this;
        }

        // from : http://stackoverflow.com/questions/1134388/stdendl-is-of-unknown-type-when-overloading-operator

        // function that takes a custom stream, and returns it
        typedef CLogErr& (*MyStreamManipulator)(CLogErr&);

        // take in a function with the custom signature
        inline CLogErr& operator<<(MyStreamManipulator manip)
        {
            // call the function, and return it's value
            return manip(*this);
        }

        // define the custom endl for this stream.
        // note how it matches the `MyStreamManipulator`
        // function signature
        static inline CLogErr& endl(CLogErr& stream)
        {
            // print a new line

            #ifndef NDEBUG
            std::clog << std::endl;
            #endif

            // do other stuff with the stream
            // std::cout, for example, will flush the stream
//            stream << "Called MyStream::endl!" << std::endl;

            return stream;
        }


        // this is the type of std::cout
        typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

        // this is the function signature of std::endl
        typedef CoutType& (*StandardEndLine)(CoutType&);

        // define an operator<< to take in std::endl
        CLogErr& operator<<(StandardEndLine manip)
        {
            #ifndef NDEBUG
            // call the function, but we cannot return it's value
            manip(std::clog);
            #endif

            return *this;
        }
    protected:
    private:
};

namespace clog{
static CLogErr clogerr;
}

}   // namespace CLogErr

#endif // CLOGERR_H
