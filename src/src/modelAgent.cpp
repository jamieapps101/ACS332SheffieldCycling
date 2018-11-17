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
  //std::cout << "right, into the meat" << std::endl;
  struct pathInfoStruct pathInfo = assessPath();
  std::cout << "homeX " << homeLocation.at(0) << std::endl;
  std::cout << "homeY " << homeLocation.at(1) << std::endl;
  std::cout << "workX " << workLocation.at(0) << std::endl;
  std::cout << "workY " << workLocation.at(1) << std::endl;
  std::cout << "Distance " << pathInfo.distance << std::endl;
  std::cout << "Delta height " << pathInfo.deltaHeight << std::endl;
  std::cout << "socioEconSum " << pathInfo.socioEconSum << std::endl;
  std::cout << std::endl;
  // I have avaliable:
    // Fitness
    // path length
    // ses sum accross path
    // path delta height
    // temperature
    // safety travel metric (still to impliement, range of 0-1)

  //I want:
    // current Travel mode
}

struct pathInfoStruct modelAgent::assessPath()
{
  struct pathInfoStruct internalData;
  //std::cout << "flag 1" << std::endl;
  // look at path cell quality for partial safety metric
  // look at either common cell paths or vector intesection to see if paths collide with other agents
  //https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
  std::vector<int> travel;
  travel.push_back(workLocation.at(0) - homeLocation.at(0));
  travel.push_back(workLocation.at(1) - homeLocation.at(1));
  //std::cout << "flag 2" << std::endl;
  if(std::abs(travel.at(0)) > std::abs(travel.at(1)))
  {
    float gradient = (float)travel.at(1)/travel.at(0);
    for(int a = 0; a < std::abs(travel.at(0)); a++)
    {
      internalData.pathX.push_back(a + homeLocation.at(0));
      internalData.pathY.push_back((float)a*gradient + homeLocation.at(1));
    }
  }
  else
  {
    float gradient = (float)travel.at(0)/travel.at(1);
    for(int a = 0; a < std::abs(travel.at(1)); a++)
    {
      internalData.pathX.push_back((float)a*gradient + homeLocation.at(0));
      internalData.pathY.push_back(a + homeLocation.at(1));
    }
  }
  //std::cout << "flag 3, and pathX is size " << internalData.pathX.size() << std::endl;
  internalData.socioEconSum = 0;
  for(int a = 0; a < internalData.pathX.size(); a++)
  {
    //std::cout << "I want element at X" << internalData.pathX.at(a) << " and Y" << internalData.pathY.at(a) <<std::endl;
  //  SESLocal.printMap();
    internalData.socioEconSum += SESLocal.getElement(internalData.pathX.at(a),internalData.pathY.at(a));
    //int z = SESLocal.getElement(0,internalData.pathY.at(a));
    //std::cout << "z = " << internalData.socioEconSum << std::endl;
  }
  //std::cout << "flag 4" << std::endl;
  internalData.distance = (int)pow((pow((homeLocation.at(0) - workLocation.at(0)),2) + pow((homeLocation.at(1) - workLocation.at(1)),2)),0.5);
  internalData.deltaHeight = std::abs(homeLocation.at(2) - workLocation.at(2));
  return internalData;
}

void modelAgent::init(propertiesMap SESinput)
{
    // need to pick home/work locations here. need to export common work/living locations for this
    std::cout<<"My fitness is: " << fitness << std::endl;
    SESLocal = SESinput;
    std::cout<< "and my map got here ok" << std::endl;
    //SESLocal.printMap();
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
int modelAgent::getCurrentTravelMode()
{
  return currentTravelMode;
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
