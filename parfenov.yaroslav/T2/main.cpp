
#include <iostream>
#include "DataStruct.h"
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

int main()
{
    std::vector<parfenov::DataStruct> inputVector;
    while (!std::cin.eof())
    {
        std::copy(
                  std::istream_iterator<parfenov::DataStruct>(std::cin),
                  std::istream_iterator<parfenov::DataStruct>(),
                  std::back_inserter(inputVector)
                  );
    }

    std::sort(
              inputVector.begin(),
              inputVector.end(),
              parfenov::compareDataStructs
              );
    std::copy(
              inputVector.begin(),
              inputVector.end(),
              std::ostream_iterator<parfenov::DataStruct>(std::cout, "")
              );

    return EXIT_SUCCESS;
}
