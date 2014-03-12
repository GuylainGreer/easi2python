#include "statement.hh"
#include "expression.hh"
#include "multiply_divide.hh"
#include "add_subtract.hh"
#include "function_call.hh"
#include "common.hh"
#include "streaming.hh"

#include <boost/spirit/include/qi.hpp>
#include "typename.hh"
#include <vector>
#include <iterator>
#include <fstream>

using namespace boost::spirit;
using namespace boost::phoenix;

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
#ifdef ENABLE_SPIRIT_DEBUGGING
    dummy((qi::debug((rules)), 0)...);
#endif
}

template <class Rule>
void init2(std::string name, Rule & rule)
{
    rule.name(name);
#ifdef ENABLE_SPIRIT_DEBUGGING
    qi::debug(rule);
#endif
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
        INIT(common::id_list);
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
