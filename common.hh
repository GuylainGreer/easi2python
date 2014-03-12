#ifndef EASI_PARSE_COMMON_HH
#define EASI_PARSE_COMMON_HH

#include <string>
#include <vector>
#include <boost/spirit/include/qi_rule.hpp>

namespace common
{
    typedef std::string::iterator iter;
    extern boost::spirit::qi::rule<iter, void()> newline;
    extern boost::spirit::qi::rule<iter, void()> end_statement;
    extern boost::spirit::qi::rule<iter, void()> end_quote;
    extern boost::spirit::qi::rule<iter, std::string()> identifier;
    extern boost::spirit::qi::rule<iter, std::string()> quoted_string;
    extern boost::spirit::qi::rule<iter, std::vector<std::string>()> id_list;
};

#endif
