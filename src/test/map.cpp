#include "map.h"
#include <iostream>
#include <fstream>
#define STANDARDELEVATIONFILENAME "elevations.txt"
#define STANDARDRESOLUTIONFILENAME "resolutions.txt"

map::map(std::string inputFile)
{
  readMap(inputFile);
}

map::~map()
{

}

void map::readMap(std::string inputFile)
{
    //printf("Reading the map\n");
    std::ifstream file;
    file.open(inputFile);
    if(file.is_open())
    {
      //printf("File is open\n");
      std::string line;
      int lineNo = 0;
      while(getline(file, line))
      {
        if(lineNo != 0)
        {
          //printf("reading another line\n");
          std::vector<int> currentLine;
          //printf("Line is %d long\n", line.size());
          while(line.size() > 0)
          {
            int nextCommaPos = line.find_first_of(",");
            if(nextCommaPos ==0 )
            {
              break;
            }
            currentLine.push_back(string2int(line.substr(0,nextCommaPos)));
            if(lineNo ==1)
            {
              //printf("Comma pos: %d\n", nextCommaPos);
              //printf("Val = %d\n",string2int(line.substr(0,nextCommaPos)) );
              //std::cout << "Sub string was:" << line.substr(0,nextCommaPos) << std::endl;
              //std::cout << "String was:" << line << std::endl;
            }
            line = line.substr(nextCommaPos+1, line.size());
          }
          if(inputFile.compare(STANDARDELEVATIONFILENAME) == 0)
          {
            mapElevations.push_back(currentLine);
          }
          else if(inputFile.compare(STANDARDRESOLUTIONFILENAME) == 0)
          {
            mapResolutions.push_back(currentLine);
          }
          else
          {
            printf("Check map filenames please\n");
          }
        }
        else
        {
        }
        lineNo++;
      }
      file.close();
    }
    else
    {
      std::cout << "Cannot open map file" << std::endl;
    }
}

int map::string2int(std::string input)
{
  //bool valid = true;
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

int map::getElevation(int x, int y)
{
  return mapElevations.at(y).at(x);
}

/*
int map::getElevationInv(int x, int y)
{
  return mapElevations.at(x).at(y);
}
*/

std::vector<int> map::getDimentsions()
{
  std::vector<int> output;
  std::vector<int> temp = mapElevations.at(0);
  output.push_back(temp.size());
  output.push_back(mapElevations.size());
  return output;
}

void map::printMap()
{
  for(int a = 0; a < mapElevations.size(); a++)
  {
    for(int b = 0; b < mapElevations.at(0).size(); b++)
    {
      std::cout << getElevation(b,a) << ", ";
    }
    std::cout<<std::endl;
  }
}
