

#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <string>

namespace parfenov
{

struct DataStruct
{

    std::string key1;
    std::string key2;
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

}


#endif
