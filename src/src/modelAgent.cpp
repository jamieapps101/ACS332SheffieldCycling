#include "modelAgent.h"
#include <vector>


modelAgent::modelAgent(repast::AgentId id)
{
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

}
void modelAgent::doSomething()
{
  unsigned long a = 0;
  while(a < 4000000000)
  {
    a+=1;
  }
}

// insert getters and setters for variables here
void modelAgent::setCurrentTravelMode(int *input)
{
  currentTravelMode = *input;
}
void modelAgent::setFitness(float *input)
{
  fitness = *input;
}
void modelAgent::setIllness(float*input)
{
  illness = *input;
}
void modelAgent::setHomeLocation(std::vector<int> *input)
{
  homeLocation = *input;
}
void modelAgent::setWorkLocation(std::vector<int> *input)
{
  workLocation = *input;
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
