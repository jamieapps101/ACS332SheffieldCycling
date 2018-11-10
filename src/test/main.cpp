#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "map.h"

int main(void)
{
  printf("Getting elevations\n");
  map localMap("elevations.txt");
  printf("Making dims vector\n");
  std::vector<int> dims;
  printf("Getting dims\n");
  dims = localMap.getDimentsions();
  printf("Printing\n");
  if(dims.size() == 2)
  {
    printf("Dims are %d and", dims.at(0));
    printf("%d\n", dims.at(1));
  }
  int x = 0;
  int y = 3;
  std::cout << "Element at (" << x << ", " << y << ") was " << localMap.getElevation(x,y) << std::endl;
  localMap.printMap();
}
