#include <iostream>
#include <stdio.h>

#define DEFAULT_STEPS 5

#define OUTPUTPATH ./output/


int main()
{
  std::cout << "Hello There" << std::endl;
  std::vector<std::string> parameters;
  std::vector<std::vector<int>> ranges;
  std::vector<int> steps;
  std::vector<int> defaultRange;
  for(int a = 0; a < 5; a++)
  {
    parameters.push_back("weight" + std::to_string(a));
    steps.push_back(DEFAULT_STEPS);
  }
  defaultRange.push_back(0);
  defaultRange.push_back(1);
  for(int a = 0; a < parameters.size(); a++)
  {
    ranges.pushBack(defaultRange);
  }


  return 0;
}
