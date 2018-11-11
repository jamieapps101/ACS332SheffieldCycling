#include <stdio.h>
#include <iostream>
#include "cityModel.h"
#include "modelAgent.h"
#include <boost/mpi.hpp>
#include "repast_hpc/AgentId.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/initialize_random.h"
#include "propertiesMap.h"
#include <string>
#include "repast_hpc/Random.h"
//#include <Random.h>

cityModel::cityModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm)
{
  props = new repast::Properties(propsFile, argc, argv, comm); // import properties from model.props
	stopAt = repast::strToInt(props->getProperty("stop.at"));
  agentCount = repast::strToInt(props->getProperty("count.of.agents"));
  std::string mapElevationsName = props->getProperty("elevations.map.name");
  initializeRandom(*props, comm);
  std::string filename = "data/" + mapElevationsName;
  std::cout << "map name : " << filename << std::endl;
  cityElevationMap.readMap(filename);
  std::vector<string> locationOptions;
  locationOptions.push_back("Work");
  locationOptions.push_back("Living");

  for(int a = 0; a < 2; a++)
  {
    for (int b = 0; b < 5; b++)
    {
      std::string propertyString = "common." + locationOptions.at(a) + "." + std::to_string(b+1);
      std::string inputString = props->getProperty(propertyString);
      int commaLocation = inputString.find_first_of(",");
      std::vector<int> coordinates;
      std::string firstVal = inputString.substr(0, commaLocation);
      coordinates.push_back(string2int(firstVal));
      std::string secondVal = inputString.substr(1 + commaLocation, inputString.size());
      coordinates.push_back(string2int(secondVal));
      //std::cout << "coordinates are: " << coordinates.at(0) << ", " << coordinates.at(1) << std::endl;
      coordinates.push_back(cityElevationMap.getElement(coordinates.at(0),coordinates.at(1)));
      //std::cout << "Z coordinate was " << coordinates.at(2) << std::endl;
      if(a==0) // aka work
      {
        commonWork.push_back(coordinates);
      }
      else // aka living
      {
        commonLiving.push_back(coordinates);
      }
    }
  }
  time.week = 0;
  time.year = 2010;
  std::string monthNames[]={"jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"};
  for(int a = 0; a < 12; a++)
  {
    struct monthStruct temp;
    temp.name = monthNames[a];
    std::string currentParameterName = "temp." + monthNames[a];
    temp.temperature = string2float(props->getProperty(currentParameterName));
    monthTemps.push_back(temp);
  }
}

cityModel::~cityModel()
{
  delete props;
}

void cityModel::init() // initialise the model with agents
{
  int rank = repast::RepastProcess::instance()->rank(); // get rank of this process' instance of the model
  std::cout << "Starting process " << rank << " with " << agentCount << " agents." << std::endl;
  repast::TriangleGenerator gen = repast::Random::instance()->createTriangleGenerator(0,0.5,1);
  for(int i = 0; i < agentCount; i++) // a counter to count upto the desired number of agents
  {
    repast::AgentId id(i, rank, 0); // create an agentID object, with its ID, starting process number and type (left as 0)
    id.currentRank(rank); //this just seems to reset the rank value of the object
    modelAgent* agent = new modelAgent(id); // create a new agent with the ID set to the agentID object
    //std::cout << agent->
    int workLocationChoice = round((int)(repast::Random::instance()->nextDouble()*4.0));
    int homeLocationChoice = round((int)(repast::Random::instance()->nextDouble()*4.0));
    std::vector<int> chosenLocation;
    chosenLocation = commonWork.at(homeLocationChoice);
    agent->setHomeLocation(chosenLocation);
    chosenLocation = commonLiving.at(workLocationChoice);
    agent->setWorkLocation(chosenLocation);
    agent->setFitness(gen.next());
    context.addAgent(agent); // add this agent to the process context
  }
}

void cityModel::initAgents() // this allows agents to do their own initialiseations here
{
  std::vector<modelAgent*> agents; // make a container for agents
  context.selectAgents(agentCount, agents); // get random ordered selection of agents and add them to container
  for(int iterator = 0; iterator < agents.size(); iterator++)// iterate through all agents
  {
    (agents.at(iterator))->init(); // for each agent, execute its init task;
  }
}

void cityModel::executeAgents() // this gets random order of all agents in context and runs them
{
  std::vector<modelAgent*> agents; // make a container for agents
  context.selectAgents(agentCount, agents); // get random ordered selection of agents and add them to container
  for(int iterator = 0; iterator < agents.size(); iterator++)// iterate through all agents
  {
    (agents.at(iterator))->makeDecision(); // for each agent, execute its primary task;
  }
}

void cityModel::initSchedule(repast::ScheduleRunner& runner)
{
  //runner.scheduleEvent(1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::doSomething)));
  runner.scheduleEvent(0, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::initAgents)));
  runner.scheduleEvent(0.5, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::temporalEvents)));
  //runner.scheduleEvent(2, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::executeAgents)));
  //runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::recordResults)));
	runner.scheduleStop(stopAt);

}

void cityModel::temporalEvents() // to be executed with every tick, manages temporal events;
{
  time.week++;
  if(time.week == 52)
  {
    weekOffset = 0;
    time.week = 0;
    time.year++;
  }
  int tempWeek=time.week;
  if(time.week == 13 || time.week == 25 || time.week == 37 || time.week == 49)
  {
    weekOffset--;
  }
  int monthNumber = (int)(time.week+weekOffset)/4;
  modelTemp = monthTemps.at(monthNumber).temperature;
}

int cityModel::string2int(std::string input)
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


float cityModel::string2float(std::string input)
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
