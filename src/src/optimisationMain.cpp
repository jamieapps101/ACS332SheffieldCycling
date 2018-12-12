#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <time.h>
#define DEFAULT_STEPS 5

#define OUTPUTPATH ./output/


int main()
{
  system("make cleanData");
  usleep(1000000);
  std::string fileName;
  fileName = "output/outputReference.csv";
  std::ofstream myfile;
  srand(89793);
 std::vector<float> range;
 range.push_back(0.0);
 range.push_back(1.0);
 int steps = 4;
 int numberOfWeights = 12;
 double weights[numberOfWeights] = {};
 std::string argsStringInput;
 int numberOfSamples = 500;

 myfile.open (fileName,std::fstream::trunc);
 myfile << "sample" << ",";
 for(int d = 0; d < numberOfWeights; d++)
 {
   myfile << "weight" << std::to_string(d) << ",";
 }
 myfile << std::endl;
 myfile.close();

 for(int b = 0; b < numberOfSamples; b++)
 {
   std::cout << "<><><><><><><> Running Sample " << std::to_string(b) << " <><><><><><><>" << std::endl;
   for(int c = 0; c < numberOfWeights; c++)
   {
     double normRand = (double)std::rand()/RAND_MAX;
     int steppedRandom = (int)(normRand * steps);
     double normSteppedRandom = ((double)steppedRandom)/steps;
     double newWeight = (range.at(1)-range.at(0))*normSteppedRandom + range.at(0);
     weights[c] = newWeight;
   }
   for(int a = 0; a < numberOfWeights; a++)
   {
     if(a != 0)
     {
       argsStringInput += " ";
     }
     argsStringInput += " weight" + std::to_string(a) + "=" + std::to_string(weights[a]);
   }
   argsStringInput += " total.Weights=12";
   if(b == 0)
   {
     //std::cout << argsStringInput << std::endl;
   }

   myfile.open (fileName,std::fstream::app);
   myfile << std::to_string(b) << ",";
   for(int d = 0; d < numberOfWeights; d++)
   {
     myfile << weights[d] << ",";
   }
   myfile << std::endl;
   myfile.close();
   std::string command;
   command = "mpirun -n 10 ./bin/executable ./props/config.props ./props/model.props " + argsStringInput;
   system(command.c_str());
   //system("mkdir hello");
 }

 time_t theTime = time(NULL);
 struct tm *aTime = localtime(&theTime);
 int hour=aTime->tm_hour;
 int min=aTime->tm_min;
 std::string folderName = "output" + std::to_string(hour) + ":" + std::to_string(min);
 std::string command;
 command = "mkdir output/" + folderName;
 system(command.c_str());
 command = "mv output/*.txt output/*.csv output/" + folderName + "/";
 system(command.c_str());


  return 0;
}

// weight5=1
