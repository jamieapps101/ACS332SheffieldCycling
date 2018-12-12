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

void propertiesMap::setSize(int x, int y)
{
  std::vector<std::vector<int>> temp;
  std::vector<int> line;
  for(int a = 0; a < x; a++)
  {
    line.push_back(0);
  }
  for(int a = 0; a < y; a++)
  {
    temp.push_back(line);
  }
  mapElementsMatrix = temp;
}

void propertiesMap::readMap(std::string inputFileName,std::string keyFileName)
{
  std::vector<float> valueLookup;
  std::vector<char> letterLookup;
  std::ifstream keyFile;
  keyFile.open(keyFileName);
  if(keyFile.is_open())
  {
    //std::cout <<"Reading key file" << std::endl;
    std::string line;
    int lineNo = 0;
    while(getline(keyFile, line))
    {
      letterLookup.push_back(line.at(0));
      int equalsPos = line.find_first_of("=");
      int endlPos = line.find_first_of('\n');
      float value = string2float(line.substr(equalsPos+1,line.length()));
      valueLookup.push_back(value);
    }
    keyFile.close();
  }
  else
  {
    std::cout << "Cannot open key file \n";
  }
  std::ifstream inputFile;
  inputFile.open(inputFileName);
  if( inputFile.is_open())
  {
    //std::cout <<"Reading map file" << std::endl;
    //std::cout << "File opened" << std::endl;
    std::string line;
    int lineNo = 0;
    while(getline(inputFile, line))
    {
      if(lineNo != 0)
      {
        //std::cout << "Got line: " << line << std::endl;
        std::vector<int> currentLine;
        while(line.size() > 0)
        {
          int nextCommaPos = line.find_first_of(",");
          if(nextCommaPos ==0 )
          {
            break;
          }
          char letter = (line.substr(0,nextCommaPos)).at(0);
          int index = 0;
          for(int a = 0; a < letterLookup.size(); a++)
          {
            if(letter == letterLookup.at(a))
            {
              index = a;
              break;
            }
          }
          //std::cout << "index: " << index << std::endl;
          currentLine.push_back(valueLookup.at(index));
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
    inputFile.close();
  }
  else
  {
    std::cout << "Cannot open map file" << std::endl;
  }
  //std::cout << "Map read from file" << std::endl;
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
      file.close();
    }
    else
    {
      std::cout << "Cannot open map file" << std::endl;
    }
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
  //std::cout << "X " << x << " y " << y << std::endl;
  //std::cout << "Map Size " << mapElementsMatrix.at(y).size() << std::endl;
  if(y > mapElementsMatrix.size() || y < 0)
  {
    std::cout << "fuck" << std::endl;
  }
  if(x > mapElementsMatrix.at(0).size() || x < 0)
  {
    std::cout << "fuck mk2" << std::endl;
  }
  
  return mapElementsMatrix.at(y).at(x);
}

int propertiesMap::setElement(int x, int y, int val)
{

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

float propertiesMap::string2float(std::string input)
{
  int prePointValue = 0;
  int postPointValue = 0;
  bool postPoint = false;
  for(int a = 0; a < input.size(); a++)
  {
    char character = input.at(a);
    if(character >= 48 && character <= 57)
    {
      if(postPoint == false)
      {
        prePointValue *= 10;

  prePointValue += (int)character - 48;
      }
      else
      {
        postPointValue *= 10;
        postPointValue += (int)character - 48;
      }
    }
    else
    {
      if(character == '.') // aka are we at the decimal point.
      {
        postPoint = true;
      }
      else
      {
        return (-1); // else this must be an error
      }
    }
  }
  float value = (float)prePointValue;
  float decimal = (float)postPointValue;
  while(decimal > 1)
  {
    decimal = decimal/10;
  }
  value += decimal;
  return value;
}
