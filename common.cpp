
#include "common.hh"
#include <boost/spirit/include/qi.hpp> 

using namespace boost::spirit;

qi::rule<common::iter, void()> common::newline =
    '\r' || qi::char_('\n');

qi::rule<common::iter, void()> common::end_statement =
    *qi::blank >> (common::newline | '\\');

qi::rule<common::iter, void()> common::end_quote =
    '"' | common::newline | qi::eoi;

qi::rule<common::iter, std::string()> common::identifier =
    (qi::alpha | '$' | '#' | '_') >> *(qi::alnum | '$' | '#' | '_');

qi::rule<common::iter, std::string()> common::quoted_string =
    '"' >> *(qi::char_ - common::end_quote) >> common::end_quote;

qi::rule<common::iter, std::vector<std::string>()> common::word_list =
    ((+(qi::char_ - (qi::blank | common::end_statement | ','))) %
     (*qi::blank >> ',' >> *qi::blank));
