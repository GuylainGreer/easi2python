#ifndef EASIPARSE_STREAMING_HH
#define EASIPARSE_STREAMING_HH

#include <ostream>
#include <vector>

struct Statement;
struct Expression;

std::ostream & operator<<(std::ostream & s, const Statement & st);
std::ostream & operator<<(std::ostream & s, Expression e);

//Don't know if this is technically UB or not...
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

#endif
