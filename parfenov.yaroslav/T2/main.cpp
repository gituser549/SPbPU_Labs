
#include <iostream>
#include "DataStruct.h"
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

int main()
{
    std::vector<parfenov::DataStruct> test;
    while (!std::cin.eof())
    {
        std::copy(
                  std::istream_iterator<parfenov::DataStruct>(std::cin),
                  std::istream_iterator<parfenov::DataStruct>(),
                  std::back_inserter(test)
                  );
    }

    std::sort(
              test.begin(),
              test.end(),
              parfenov::compareDataStructs
              );
    std::copy(
              test.begin(),
              test.end(),
              std::ostream_iterator<parfenov::DataStruct>(std::cout, "")
              );

    return 0;
}
