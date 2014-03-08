#ifndef EASIPARSE_RUN_MODULE_HH
#define EASIPARSE_RUN_MODULE_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"

struct RunModule
{
    std::string module_name;

    static boost::spirit::qi::rule<common::iter, RunModule()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(RunModule,
                          (std::string,module_name));

#endif
