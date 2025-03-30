

#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <string>

namespace parfenov
{

using ULL = unsigned long long;

struct DataStruct
{

    ULL key1;
    ULL key2;
    std::string key3;

};

class ScopeGuard
{
public:
    ScopeGuard(std::basic_ios<char>& s);
    ~ScopeGuard();
private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

bool compareDataStructs(const DataStruct& first, const DataStruct& second);

void ignoreTillBracket(std::istream& in);

}


#endif
