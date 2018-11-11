#include "modelAgent.h"
#include <vector>


modelAgent::modelAgent(repast::AgentId id)
{
  selfID = id;
  for(int a = 0; a < 3; a++)
  {
    homeLocation.push_back(0);
    workLocation.push_back(0);
  }
}

modelAgent::~modelAgent()
{

}
// insert actions/functions here
void modelAgent::makeDecision()
{

}
void modelAgent::assessPath()
{
  // look at path cell quality for partial safety metric
  // look at either common cell paths or vector intesection to see if paths collide with other agents
  //https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
}
void modelAgent::init()
{
    // need to pick home/work locations here. need to export common work/living locations for this
    std::cout<<"My fitness is: " << fitness << std::endl;
}
void modelAgent::doSomething()
{
  unsigned long a = 0;
  while(a < 10)
  {
    a+=1;
  }
}
float modelAgent::block2Distance(int input)
{
  return (float)input*1.37; // as calculated from lat/long distance measurements from original map
}
// insert getters and setters for variables here
void modelAgent::setCurrentTravelMode(int *input)
{
  currentTravelMode = *input;
}
void modelAgent::setFitness(float input)
{
  fitness = input;
}
void modelAgent::setIllness(float input)
{
  illness = input;
}
void modelAgent::setHomeLocation(std::vector<int> input)
{
  homeLocation = input;
}
void modelAgent::setWorkLocation(std::vector<int> input)
{
  workLocation = input;
}
void modelAgent::getCurrentTravelMode(int *output)
{
  *output = currentTravelMode;
}
void modelAgent::getFitness(float *output)
{
  *output = fitness;
}
void modelAgent::getIllness(float *output)
{
  *output = illness;
}
void modelAgent::getHomeLocation(std::vector<int> *output)
{
  *output = homeLocation;
}
void modelAgent::getWorkLocation(std::vector<int> *output)
{
  *output = workLocation;
}
void modelAgent::setCurrentTemp(float input)
{
  currentTemp = input;
}
void modelAgent::getCurrentTemp(float *output)
{
  *output = currentTemp;
}
