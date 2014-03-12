#ifndef EASIPARSE_STREAMING_HH
#define EASIPARSE_STREAMING_HH

#include <ostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>

struct Statement;
struct Expression;

std::ostream & operator<<(std::ostream & s, const Statement & st);
std::ostream & operator<<(std::ostream & s, Expression e);

//Don't know if this is technically UB or not...
namespace std {

    template <class T>
    ostream & operator<<(ostream & s, const vector<T> & v)
    {
        s << "vector( " << v.size() << ": ";
        s << 
            std::accumulate(v.begin(), v.end(), std::string(),
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

#endif
