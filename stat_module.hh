#ifndef EASIPARSE_STAT_MODULE_HH
#define EASIPARSE_STAT_MODULE_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"

struct StatModule
{
    std::string module_name;
    static boost::spirit::qi::rule<common::iter,StatModule()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(StatModule,
                          (std::string,module_name));


#endif
