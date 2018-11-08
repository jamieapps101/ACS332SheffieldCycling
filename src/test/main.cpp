#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

int function(std::vector<int> *input);
void printVector(std::vector<int> input);

int main(void)
{
  std::vector<int> * vec;
  vec = (std::vector<int> *)calloc(1,sizeof(std::vector<int>));
  printVector(*vec);
  function(vec);
  printVector(*vec);
  //printf("A = %d\n", *a);
}

int function(std::vector<int> *input)
{
  int array[] = {1,2,3};
  for(int a = 0; a < 3; a++)
  {
    (*input).push_back(array[a]);
  }
}

void printVector(std::vector<int> input)
{
  if(input.size() > 0)
  {
    for(int a = 0; a < input.size(); a++)
    {
      std::cout << "Element " << a << " = " << input.at(a) << std::endl;
    }
  }
  else
  {
    std::cout << "No Elements to display" << std::endl;
  }
}
