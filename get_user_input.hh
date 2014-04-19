#ifndef EASIPARSE_GET_USER_INPUT_HH
#define EASIPARSE_GET_USER_INPUT_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "streaming.hh"

struct GetUserInput : TuplePrinter<GetUserInput>
{
    std::string output_message;
    std::string input_into;
    static boost::spirit::qi::rule<common::iter, GetUserInput()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(GetUserInput,
                          (std::string, output_message)
                          (std::string, input_into));

#endif
