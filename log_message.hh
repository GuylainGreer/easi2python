#ifndef EASIPARSE_LOG_MESSAGE_HH
#define EASIPARSE_LOG_MESSAGE_HH

#include <boost/spirit/include/qi_rule.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "common.hh"
#include "streaming.hh"

struct LogMessage : TuplePrinter<LogMessage>
{
    std::string message;
    static boost::spirit::qi::rule<common::iter, LogMessage()> get_rule();
};

BOOST_FUSION_ADAPT_STRUCT(LogMessage,
                          (std::string, message));

#endif
