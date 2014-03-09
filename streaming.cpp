#include "streaming.hh"
#include "statement.hh"
#include "expression.hh"
#include "typename.hh"
#include <functional>

struct ExpressionStreamer
{
    std::reference_wrapper<std::ostream> stream;
    void operator()(double d)
    {
        stream.get() << d;
    }
};

std::ostream & operator<<(std::ostream & s, Expression e)
{
    return s;
}

struct streamer : public boost::static_visitor<>
{
    std::string inter;
    std::reference_wrapper<std::ostream> stream;
    streamer(std::ostream & stream, std::string inter):
        inter(inter), stream(stream){}
    template <class T>
    void operator()(T t)
    {
        stream.get() << get_cpp_name<T>() << ": ";
        static const int length =
            boost::fusion::result_of::size<T>::value;
        Print<length-1>(t);
    }

    template <int N,class T>
    void Print(T t)
    {
        if(N)
            Print<N?N-1:N>(t);
        stream.get() << boost::fusion::at_c<N>(t) << inter;
    }
};

std::ostream & operator<<(std::ostream & s, const Statement & st)
{
    if(st.line_number != -1)
        s << "line " << st.line_number << ": ";
    streamer str(s, " ");
    boost::apply_visitor(str, st.info);
    return s;
}
