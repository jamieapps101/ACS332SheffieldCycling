#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
/*
#include <ctime>
#include <ratio>
#include <chrono>
#include <time.h>
*/


int string2int(std::string input);
float string2float(std::string input);

struct dataStruct
{
  int folder;
  int file;
  double RMSE;
  double weights[12];
};

int main(int argc, char *argv[])
{
  //system("make cleanData");
  //usleep(1000000); // time to delete data
  std::string inputCommands;

  if(argc > 1)
  {
    for(int a = 1; a < argc; a++)
    {
      inputCommands += argv[a];
      inputCommands += " ";
    }
  }
  //std::cout << "input was " << inputCommands << std::endl;
  srand(time(NULL));

  int numberOfWeights = 12;
  double weights[numberOfWeights] = {};
  int numberOfSamples = 70;

  std::vector<struct dataStruct> inputData;
  std::ifstream resultsFile("data/topResultsRmseData1.csv");
  std::string line;
  //std::cout << "Lets read some data" << std::endl;
  int lineNumber = 0;
  while (std::getline(resultsFile, line))
  {
    //std::cout << "On line " << lineNumber << std::endl;
    lineNumber++;
    struct dataStruct temp;
    int valueNumber = 0;
    while(line.length() > 2 && valueNumber < 20)
    {
      size_t nextComma = line.find_first_of(","); // get next comma position
      //std::cout << "comma at " << nextComma << std::endl;
      if(nextComma != std::string::npos)
      {
        std::string txtVal = line.substr(0, nextComma - 1); // get text string of number
        line = line.substr(nextComma + 1); // remove number from line
        //std::cout << "Well, I made it here" << std::endl;
        switch(valueNumber)
        {
          case 0:
            temp.folder = string2int(txtVal);
          break;
          case 1:
            temp.file = string2int(txtVal);
          break;
          case 2:
            temp.RMSE = string2float(txtVal);
          break;
          default:
            temp.weights[valueNumber - 3] = string2float(txtVal);
          break;
        }
      }
      else
      {
        break;
      }
      valueNumber++;
    }
    inputData.push_back(temp);
  }
  resultsFile.close();

  int iterations = inputData.size();
 for(int b = 0; b <  iterations; b++)
 {
   std::string argsStringInput;
   std::cout << "<><><><><><><> Running Sample " << std::to_string(b) << " <><><><><><><>" << std::endl;
   for(int a = 0; a < numberOfWeights; a++) // for each weight
   {
     if(a != 0)
     {
       argsStringInput += " ";
     }
     argsStringInput += " weight" + std::to_string(a) + "=" + std::to_string(inputData.at(b).weights[a]); // add the weight in the form of a string to the args variable
   }
   argsStringInput += " total.Weights=" + std::to_string(numberOfWeights);
   argsStringInput += " random.seed=" + std::to_string(rand());
   argsStringInput += " " + inputCommands;
   std::string command;
   //std::cout << "command:" << argsStringInput << std::endl;
   command = "mpirun -n 10 ./bin/executable ./props/config.props ./props/model.props " + argsStringInput;
   system(command.c_str());
 }

 std::string command = "";
 std::string folderName = "\"output" + inputCommands + "\"";
 command = "mkdir output/" + folderName;
 system(command.c_str());
 command = "mv output/*.txt output/" + folderName + "/";
 system(command.c_str());
 usleep(10000000);
 return 0;
}

int string2int(std::string input)
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

float string2float(std::string input)
{
  int prePointValue = 0;
  int postPointValue = 0;
  bool postPoint = false;
  int divideBy = 0;
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
        divideBy++;
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
  for(int a = 0; a < divideBy; a++)
  {
    decimal = decimal/10;
  }
  value += decimal;
  return value;
}
