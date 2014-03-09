#include "statement.hh"
#include "function_definition.hh"
#include "try_catch.hh"
#include "expression.hh"
#include "multiply_divide.hh"
#include "add_subtract.hh"
#include "function_call.hh"
#include "common.hh"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include "typename.hh"
#include <vector>
#include <iterator>
#include <fstream>

namespace std {
    template <class T>
    ostream & operator<<(ostream & s, const vector<T> & v)
    {
        s << "vector(";
        for(auto && i : v)
        {
            if(std::is_same<T, std::string>::value)
                s << "\"" << i << "\", ";
            else
                s << i << ", ";
        }
        return s << ")";
    }
}

using namespace boost::spirit;
using namespace boost::phoenix;

struct ExpressionStreamer
{
    std::reference_wrapper<std::ostream> stream;
    void operator()(double d)
    {
        stream << d;
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
        stream << get_cpp_name<T>() << ": ";
        static const int length =
            boost::fusion::result_of::size<T>::value;
        Print<length-1>(t);
    }

    template <int N,class T>
    void Print(T t)
    {
        if(N)
            Print<N?N-1:N>(t);
        stream << boost::fusion::at_c<N>(t) << inter;
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

std::string filter_name(std::string name)
{
    std::transform(name.begin(), name.end(), name.begin(),
                   [](char c)
                   {
                       if(!std::isalnum(c)) return '_';
                       else return c;
                   });
    return name;
}

template <class ... Things>
void dummy(Things ...){}

template <class T>
using get_attr =
   typename boost::mpl::at_c<typename T::template_params, 0>::type;

template <class ... Rules>
void init(Rules&...rules)
{
    dummy((rules.name(filter_name(get_cpp_name<get_attr<Rules> >())), 0)...);
    dummy((qi::debug((rules)), 0)...);
}

template <class Rule>
void init2(std::string name, Rule & rule)
{
    rule.name(name);
    qi::debug(rule);
}

#define INIT(n) init2(#n, n);

struct EASIRules :
    qi::grammar<common::iter,
                std::vector<Statement>()>
{
    EASIRules() : EASIRules::base_type(start)
    {
        statement = Statement::get_rule().copy();
        start = *(statement >> +common::end_statement);

        init(statement);
        INIT(common::identifier);
        INIT(common::quoted_string);
        INIT(common::end_statement);
        INIT(common::newline);
        INIT(common::end_quote);
        INIT(common::word_list);
    }

    qi::rule<common::iter,std::vector<Statement>()> start;
    qi::rule<common::iter,Statement()> statement;
};

int main()
{
    std::ifstream stream("EASI_testfile.txt");
    stream.unsetf(std::ios::skipws);
    std::string text(std::istream_iterator<char>(stream),
                     (std::istream_iterator<char>()));
    common::iter begin = text.begin(), end = text.end();
    std::vector<Statement> s;
    EASIRules easi;
    try
    {
        parse(begin, end, easi, s);
    } catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << std::boolalpha;
    std::cout << "Parsed " << s.size() << " statements\n";
    std::copy(s.begin(), s.end(),
              std::ostream_iterator<Statement>(std::cout,"\n"));
    std::cout << "Left over: \"";
    std::copy(begin, end, std::ostream_iterator<char>(std::cout));
    std::cout << "\"\n";
}
