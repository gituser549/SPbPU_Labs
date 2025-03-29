
#include "DataStruct.h"
#include <regex>

namespace parfenov
{

const int MIN_SYMB_IN_HEX_ULL = 3;
const int NUM_OF_FIELDS = 3;

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    
    DataStruct input;
    
    bool allIsOk = true;
    
    char bracket = ' ';
    char separator = ' ';
    
    std::string keyNum = "";
    std::string key = "";
    
    std::string key1 = "";
    std::string key2 = "";
    std::string key3 = "";
    
    do
    {
        in >> bracket;
        if (bracket != '(')
        {
            allIsOk = false;
            //throw std::logic_error("");
        }
        
        for (int i = 0; i < NUM_OF_FIELDS; i++)
        {
            in >> separator;
            if (separator != ':')
            {
                allIsOk = false;
                //throw std::logic_error("");
            }
            in >> keyNum;
            if (!std::regex_match(keyNum, std::regex("key[1-3]")))
            {
                allIsOk = false;
                //throw std::logic_error("");
            }
            switch (keyNum[3])
            {
                case '1':
                    if (key1.length())
                    {
                        allIsOk = false;
                        //throw std::logic_error("");
                    }
                    in.ignore();
                    std::getline(in, key1, ':');
                    in.putback(':');
                    if (!std::regex_match(key1, std::regex("([0]|[1-9][0-9]*)(ULL|ull)")))
                    {
                        allIsOk = false;
                        //throw std::logic_error("");
                    }
                    input.key1 = key1.substr(0, key1.length() - 3) + "ull";
                    break;
                case '2':
                    if (key2.length())
                    {
                        allIsOk = false;
                        //throw std::logic_error("");
                    }
                    in.ignore();
                    std::getline(in, key2, ':');
                    in.putback(':');
                    if (!std::regex_match(key2, std::regex("0x([0]|[1-9A-F][0-9A-F]*)")) &&
                        !std::regex_match(key2, std::regex("0X([0]|[1-9a-f][0-9a-f]*)")))
                    {
                        allIsOk = false;
                        //throw std::logic_error("");
                    }
                    for (int i = 0; i < key2.length(); i++)
                    {
                        if ((key2[i] >= 'a' && key2[i] <= 'f'))
                        {
                            input.key2.push_back(toupper(key2[i]));
                        }
                        else
                        {
                            input.key2.push_back(key2[i]);
                        }
                    }
                    break;
                case '3':
                    if (key3.length())
                    {
                        allIsOk = false;
                        //throw std::logic_error("");
                    }
                    in.ignore();
                    std::getline(in, key3, ':');
                    in.putback(':');
                    if (!std::regex_match(key3, std::regex("^\".+\"$")))
                    {
                        allIsOk = false;
                        //throw std::logic_error("");
                    }
                    input.key3 = key3;
                    break;
                default:
                    allIsOk = false;
                    //throw std::logic_error("");
                    break;
            }
        }
        
        in >> separator;
        if (separator != ':')
        {
            allIsOk = false;
            //throw std::logic_error("");
        }
        in >> bracket;
        if (bracket != ')')
        {
            allIsOk = false;
            //throw std::logic_error("");
        }
        
        if (allIsOk)
        {
            dest.key1 = input.key1;
            dest.key2 = input.key2;
            dest.key3 = input.key3;
        }
        else
        {
            in.get();
        }
        
    }
    while (!allIsOk && !in.eof());
    
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    
    {
        ScopeGuard guard(out);
        
        out << "(:key1 "
            << dest.key1
            << ":key2 "
            << dest.key2
            << ":key3 "
            << dest.key3
            << ":)\n";
    }
    
    return out;
    
}

bool compareDataStructs(const DataStruct& first, const DataStruct& second)
{
    unsigned long long fKey1, sKey1;
    if (first.key1.length() >= MIN_SYMB_IN_HEX_ULL && first.key1[1] == 'x')
    {
        fKey1 = stoull(first.key1, nullptr, 16);
    }
    else
    {
        fKey1 = stoull(first.key1);
    }
    
    if (second.key1.length() >= MIN_SYMB_IN_HEX_ULL && second.key1[1] == 'x')
    {
        sKey1 = stoull(second.key1, nullptr, 16);
    }
    else
    {
        sKey1 = stoull(second.key1);
    }
    
    if (fKey1 < sKey1)
    {
        return true;
    }
    else if (fKey1 > sKey1)
    {
        return false;
    }
    else
    {
        
        unsigned long long fKey2, sKey2;
        
        if (first.key2.length() >= MIN_SYMB_IN_HEX_ULL && first.key2[1] == 'x')
        {
            fKey2 = stoull(first.key2, nullptr, 16);
        }
        else
        {
            fKey2 = stoull(first.key2);
        }
        
        if (second.key2.length() >= MIN_SYMB_IN_HEX_ULL && second.key2[1] == 'x')
        {
            sKey2 = stoull(second.key2, nullptr, 16);
        }
        else
        {
            sKey2 = stoull(second.key2);
        }
        
        
        if (fKey2 < sKey2)
        {
            return true;
        }
        else if (fKey2 > sKey2)
        {
            return false;
        }
        else
        {
            if (first.key3 < second.key3)
            {
                return true;
            }
            else if (first.key3 > second.key3)
            {
                return false;
            }
        }
    }
    return false;
}


ScopeGuard::ScopeGuard(std::basic_ios<char>& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{}

ScopeGuard::~ScopeGuard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}



}
