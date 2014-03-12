#ifndef EASIPARSE_EXPRESSION_HH
#define EASIPARSE_EXPRESSION_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"

struct FunctionCall;
struct AddSubtract;
struct MultiplyDivide;

struct Expression
{
    typedef boost::variant<
        double,
        std::string,
        boost::recursive_wrapper<FunctionCall>,
        boost::recursive_wrapper<AddSubtract>
        > type;
    bool quoted_string = false;
    type expression;
    static boost::spirit::qi::rule<common::iter, Expression()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(Expression,
                          (Expression::type, expression)
                          (bool, quoted_string));

#include "add_subtract.hh"
#include "multiply_divide.hh"

#endif
