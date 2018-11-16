#include "propertiesMap.h"
#include <iostream>
#include <fstream>
#define STANDARDELEVATIONFILENAME "elevations.txt"
#define STANDARDRESOLUTIONFILENAME "resolutions.txt"

propertiesMap::propertiesMap()
{

}


propertiesMap::~propertiesMap()
{

}

void propertiesMap::readMap(std::string inputFile,std::string keyFile)
{

}

void propertiesMap::readMap(std::string inputFile)
{
    std::ifstream file;
    file.open(inputFile);
    if(file.is_open())
    {
      //std::cout << "File opened" << std::endl;
      std::string line;
      int lineNo = 0;
      while(getline(file, line))
      {
        if(lineNo != 0)
        {
          std::vector<int> currentLine;
          while(line.size() > 0)
          {
            int nextCommaPos = line.find_first_of(",");
            if(nextCommaPos ==0 )
            {
              break;
            }
            currentLine.push_back(string2int(line.substr(0,nextCommaPos)));
            line = line.substr(nextCommaPos+1, line.size());
          }
          mapElementsMatrix.push_back(currentLine);
        }
        else
        {
        }
        lineNo++;
      }
      //std::cout << "Read " << lineNo << " lines" << std::endl;
      file.close();
    }
    else
    {
      std::cout << "Cannot open map file" << std::endl;
    }
    //std::cout << "Map read from file" << std::endl;
}

int propertiesMap::string2int(std::string input)
{
  int value = 0;
  for(int a = 0; a < input.size(); a++)
  {
    char character = input.at(a);
    if(character >= 48 && character <= 57)
    {
      value *= 10;
      value += (int)character - 48;
    }
    else
    {
      return (-1);
    }
  }
  return value;
}

int propertiesMap::getElement(int x, int y)
{
  return mapElementsMatrix.at(y).at(x);
}

std::vector<int> propertiesMap::getDimensions()
{
  std::vector<int> output;
  std::vector<int> temp = mapElementsMatrix.at(0);
  output.push_back(temp.size());
  output.push_back(mapElementsMatrix.size());
  return output;
}

void propertiesMap::printMap()
{
  std::cout << "Printing Map" << std::endl;
  for(int a = 0; a < mapElementsMatrix.size(); a++)
  {
    for(int b = 0; b < mapElementsMatrix.at(0).size(); b++)
    {
      std::cout << getElement(b,a) << ", ";
    }
    std::cout<<std::endl;
  }
}
