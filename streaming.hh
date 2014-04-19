#ifndef EASIPARSE_STREAMING_HH
#define EASIPARSE_STREAMING_HH

#include <ostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <ostream>
#include <boost/ref.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/variant.hpp>

struct Statement;
struct Expression;

//std::ostream & operator<<(std::ostream & s, const Statement & st);

//Don't know if this is technically UB or not...
//I think it is, but too bad
namespace std {

    template <class T>
    ostream & operator<<(ostream & s, const vector<T> & v)
    {
        s << "vector( " << v.size() << ": ";
        s << std::accumulate(v.begin(), v.end(), std::string(),
                             [](const std::string & st, const T & t)
                             {
                                 std::stringstream s;
                                 if(!st.empty())
                                     s << ", ";
                                 if(std::is_same<T, std::string>::value)
                                     s << "\"" << t << "\"";
                                 else
                                     s << t;
                                 return st + s.str();
                             });
        return s << ")";
    }
}

struct tuple_print
{
    boost::reference_wrapper<std::ostream> stream;
    std::string inter;
    tuple_print(std::ostream & s, std::string inter = ", "):
        stream(s), inter(inter){}

    template <int N, class T>
    void print(const T & t)
    {
        if(N)
            print<N?N-1:N>(t);
        if(std::is_same<
               typename boost::fusion::result_of::value_at_c<T, N>::type,
               std::string
               >::value)
            stream.get() << "\"" << boost::fusion::at_c<N>(t) << "\"" << inter;
        else
            stream.get() << boost::fusion::at_c<N>(t) << inter;
    }

    template <class T>
    void operator()(const T & t)
    {
        print<boost::fusion::result_of::size<T>::value-1>(t);
    }
};

template <int variant_index>
struct tuple_with_variant_print : boost::static_visitor<void>
{
    boost::reference_wrapper<std::ostream> stream;
    std::string inter;
    tuple_with_variant_print(std::ostream & s,
                             std::string inter = ", "):
        stream(s), inter(inter){}

    template <class T>
    void operator()(const T & t)
    {
        stream.get() << t;
    }

    template <class T>
    void print(const T & t)
    {
        print(t, boost::mpl::int_<
              boost::fusion::result_of::size<T>::value-1>());
    }

    template <class T, int N>
    void print(const T & t, boost::mpl::int_<N>)
    {
        if(N)
            print(t, boost::mpl::int_<N?N-1:N>());
        if(std::is_same<
               typename boost::fusion::result_of::value_at_c<T, N>::type,
               std::string
               >::value)
            stream.get() << "\"" << boost::fusion::at_c<N>(t) << "\"" << inter;
        else
            stream.get() << boost::fusion::at_c<N>(t) << inter;
    }

    template <class T>
    void print(const T & t,  boost::mpl::int_<variant_index>)
    {
        if(variant_index)
            print(t,
                  boost::mpl::int_
                  <variant_index?variant_index-1:variant_index>());
        boost::apply_visitor(*this,
                             boost::fusion::at_c<variant_index>(t));
    }
};

template <class Derived>
struct TuplePrinter 
{
};

template <class Derived>
std::ostream & operator<<(std::ostream & s,
                          const TuplePrinter<Derived> & t)
{
    tuple_print tp(s);
    tp(static_cast<const Derived&>(t));
    return s;
}

template <class Derived, int variant_index>
struct TupleWithVariantPrinter
{
};

template <class Derived, int N>
std::ostream & operator<<(std::ostream & s,
                          const TupleWithVariantPrinter<Derived, N> & t)
{
    tuple_with_variant_print<N> tp(s);
    tp.print(static_cast<const Derived&>(t));
    return s;
}

#endif
