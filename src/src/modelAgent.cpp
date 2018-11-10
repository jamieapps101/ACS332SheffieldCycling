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

    if(workLocation.size() ==2 && homeLocation.size() ==2)
    {
      distanceToWork = 0;
      /*
      distanceToWork = sqrt(pow(homeLocation.at(0)-workLocation.at(0),2) + pow(homeLocation.at(1)-workLocation.at(1),2));
      */
      std::cout << "Hey, my distance to work is: " << distanceToWork << std::endl;
    }
    else
    {
      std::cout << "Agent " << selfID.id() << " on process " << selfID.startingRank() << " doesn't know where they live or work" << std::endl;
    }

}

void modelAgent::doSomething()
{
  unsigned long a = 0;
  while(a < 10)
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
