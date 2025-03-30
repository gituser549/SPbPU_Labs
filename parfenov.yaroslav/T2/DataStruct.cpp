
#include "DataStruct.h"
#include <regex>

namespace parfenov
{

const int NUM_OF_FIELDS = 3;

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    bool allIsOk = false;

    char bracket = ' ';
    char separator = ' ';

    std::string keyNum = "";

    std::string key1 = "";
    std::string key2 = "";
    std::string key3 = "";

    while (!allIsOk && !in.eof())
    {
        allIsOk = true;
        in >> bracket;

        for (int i = 0; i < NUM_OF_FIELDS && allIsOk; i++)
        {
            in >> separator;
            in >> keyNum;
            switch (keyNum[3])
            {
                case '1':
                {
                    in.ignore();
                    std::getline(in, key1, ':');
                    in.putback(':');
                    if (!std::regex_match(key1, std::regex("([0]|[1-9][0-9]*)(ULL|ull)")))
                    {
                        allIsOk = false;
                        ignoreTillBracket(in);
                    }
                    else
                    {
                        key1 = key1.substr(0, key1.length() - 3) + "ull";
                    }
                    break;
                }
                case '2':
                {
                    in.ignore();
                    std::getline(in, key2, ':');
                    in.putback(':');
                    if (!std::regex_match(key2, std::regex("0x([0]|[1-9A-F][0-9A-F]*)"))
                    &&  !std::regex_match(key2, std::regex("0X([0]|[1-9a-f][0-9a-f]*)")))
                    {
                        allIsOk = false;
                        ignoreTillBracket(in);
                    }
                    else
                    {
                        key2 = key2.substr(2, key2.length() - 2);
                        makeUpperCase(key2);
                        key2 = "0x" + key2;
                    }
                    break;
                }
                case '3':
                {
                    in.ignore();
                    in.ignore();
                    std::getline(in, key3, '\"');
                    key3 = '\"' + key3 + '\"';
                    break;
                }
            }
        }

        if (allIsOk)
        {
            in >> separator;
            in >> bracket;

            dest.key1 = key1;
            dest.key2 = key2;
            dest.key3 = key3;
        }
        else
        {
            char testEnd;
            in >> testEnd;
            if (testEnd == '(')
            {
                in.putback('(');
            }
        }

    }


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
    unsigned long int fKey1 = stoull(first.key1),
        fKey2 = stoull(first.key2, nullptr, 16),
        sKey1 = stoull(second.key1),
        sKey2 = stoull(second.key2, nullptr, 16);

    if (fKey1 < sKey1)
    {
        return true;
    }
    else if (fKey1 == sKey1)
    {
        if (fKey2 < sKey2)
        {
            return true;
        }
        else if (fKey2 == sKey2)
        {
            if (first.key3.length() < second.key3.length())
            {
                return true;
            }
        }
    }
    return false;
}

void ignoreTillBracket(std::istream& in)
{
    char prev, cur;
    cur = in.get();
    do
    {
        prev = cur;
        cur = in.get();
    }
    while (!(cur == ')' && prev == ':'));
}

void makeUpperCase(std::string& str)
{
    for (unsigned long int i = 2; i < str.length(); i++)
    {
        if (str[i] >= 'a' && str[i] <= 'f')
        {
            str[i] = toupper(str[i]);
        }
    }
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
