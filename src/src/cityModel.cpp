#include <string>
#include <stdio.h>
#include <iostream>

#include "cityModel.h"
#include "modelAgent.h"
#include "propertiesMap.h"

#include "DataSource_AgentDecisions.h"
#include "DataSource_AgentTSM.h"

#include <boost/mpi.hpp>
#include "repast_hpc/Random.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/Utilities.h"
#include "repast_hpc/SVDataSet.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/TDataSource.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/initialize_random.h"


cityModel::cityModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm)
{
  //std::cout << "making citymodel object" << std::endl;
  props = new repast::Properties(propsFile, argc, argv, comm); // import properties from model.props
  initializeRandom(*props, comm);
  stopAt = repast::strToInt(props->getProperty("stop.at"));
  agentCount = repast::strToInt(props->getProperty("count.of.agents"));
  std::string mapElevationsName = props->getProperty("elevations.map.name");
  std::string elevationsPath = "data/" + mapElevationsName;
  cityElevationMap.readMap(elevationsPath);
  std::string mapSEName = props->getProperty("SE.map.name");
  std::string mapSEConfName = props->getProperty("SE.map.conf.name");
  std::string SEPath = "data/" + mapSEName;
  std::string SEConfPath = "data/" + mapSEConfName;
  socioEconomicsMap.readMap(SEPath,SEConfPath);
  std::vector<int> collisionsMapDimensions = socioEconomicsMap.getDimensions();
  collisionsMap.setSize(collisionsMapDimensions.at(0), collisionsMapDimensions.at(1));
  initCyclistProportion = string2float(props->getProperty("initial.Cyclists.Proportion"));
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
      coordinates.push_back(cityElevationMap.getElement(coordinates.at(0),coordinates.at(1)));
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
  std::string outputPath = props->getProperty("output.path") + props->getProperty("output.name");
  // Data collection
  // Create the data set builder
  std::string fileOutputName(outputPath);
  repast::SVDataSetBuilder builder(fileOutputName.c_str(), ",", repast::RepastProcess::instance()->getScheduleRunner().schedule());

  // Create the individual data sets to be added to the builder
	DataSource_AgentDecisions* agentDecisions_DataSource = new DataSource_AgentDecisions(&context);
  builder.addDataSource(createSVDataSource("Total", agentDecisions_DataSource, std::plus<int>()));

  DataSource_AgentTSM* agentTSM_DataSource = new DataSource_AgentTSM(&context);
  builder.addDataSource(createSVDataSource("Ave TSM", agentTSM_DataSource, std::plus<double>()));

  // Use the builder to create the data set
  agentDecisions = builder.createDataSet();

  policy1Mode = repast::strToInt(props->getProperty("policy1.enable"));
  policy2Mode = repast::strToInt(props->getProperty("policy2.enable"));
  policy3Mode = repast::strToInt(props->getProperty("policy3.enable"));


  for(int a = 0; a < repast::strToInt(props->getProperty("total.Weights")); a++)
  {
    float input = string2float(props->getProperty("weight" + std::to_string(a+1)));
    //std::cout << input << std::endl;
    globalInternalRuleWeight.push_back(input);
  }

}

cityModel::~cityModel()
{
  delete props;
  delete agentDecisions;
}

void cityModel::init() // initialise the model with agents
{
  int rank = repast::RepastProcess::instance()->rank(); // get rank of this process' instance of the model
  //std::cout << "Starting process " << rank << " with " << agentCount << " agents." << std::endl;
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
    agent->setPolicies(policy1Mode,policy2Mode,policy3Mode);
    context.addAgent(agent); // add this agent to the process context
  }
  //std::cout << "Helloooo" << std::endl;
  if(repast::RepastProcess::instance()->rank() == 0)
  {
    std::cout << "Policy Mode 1: " << policy1Mode << std::endl;
    std::cout << "Policy Mode 2: " << policy2Mode << std::endl;
    std::cout << "Policy Mode 3: " << policy3Mode << std::endl;
  }
}

void cityModel::initSchedule(repast::ScheduleRunner& runner)
{
  runner.scheduleEvent(0, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::initAgents)));
  runner.scheduleEvent(0.8, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::temporalEvents)));// any temporalEvents
  runner.scheduleEvent(0.9, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::updateAgents)));//re-update local agent data
  runner.scheduleEvent(1, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::agentsDecide))); // let agents decide based on current information
  runner.scheduleEvent(1.3, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::assessAllProcessAgents)));// collect data of those who cycled
  runner.scheduleEvent(1.4, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::simulateColisions)));//  simulate collisions

  // Data collection
  runner.scheduleEvent(1.1, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentDecisions, &repast::DataSet::record)));
  runner.scheduleEvent(1.2, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentDecisions, &repast::DataSet::write)));
  runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentDecisions, &repast::DataSet::write)));

  runner.scheduleStop(stopAt);
}

void cityModel::initAgents() // this allows agents to do their own initialiseations here
{
  std::vector<modelAgent*> agents; // make a container for agents
  context.selectAgents(agentCount, agents); // get random ordered selection of agents and add them to container
  //std::cout << "init-int agents" << std::endl;
  for(int iterator = 0; iterator < agents.size(); iterator++)// iterate through all agents
  {
    (agents.at(iterator))->setInternalRuleWeight(globalInternalRuleWeight);
    (agents.at(iterator))->init(socioEconomicsMap); // for each agent, execute its init task;
    float randomDouble = repast::Random::instance()->nextDouble();
    (agents.at(iterator))->setTSM(0.7); // for each agent, set the global travelSafetyMetric value
    //std::cout << "RandomDouble:" << randomDouble << std::endl;
    //std::cout << "init prop" << initCyclistProportion << std::endl << std::endl;

    if(randomDouble < initCyclistProportion)
    {
      //std::cout << "Cycle" << std::endl;
      (agents.at(iterator))->setCurrentTravelMode(CYCLEMODE);
    }
    else
    {
      //std::cout << "or don't" << std::endl;
      (agents.at(iterator))->setCurrentTravelMode(DRIVEMODE);
    }
  }
}

void cityModel::agentsDecide() // this gets random order of all agents in context and runs them
{
  if(repast::RepastProcess::instance()->rank() == 0)
  {
    repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
    int ticker = runner.currentTick();
    if(ticker%200 == 0)
    {
      std::cout << "I'm on tick " << ticker << std::endl;
    }
  }
  std::vector<modelAgent*> agents; // make a container for agents
  context.selectAgents(agentCount, agents); // get random ordered selection of agents and add them to container
  for(int iterator = 0; iterator < agents.size(); iterator++)// iterate through all agents
  {
    (agents.at(iterator))->makeDecision(); // for each agent, execute its primary task;
  }
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
  //std::cout << "month Number " << monthNumber << std::endl;
  modelTemp = monthTemps.at(monthNumber).temperature;
  //std::cout << "model temp" << modelTemp << std::endl;
}

void cityModel::assessAllProcessAgents()
{
  int rank = repast::RepastProcess::instance()->rank();
  int worldSize= repast::RepastProcess::instance()->worldSize(); // get number of processes
  repast::AgentRequest req(rank);
  float totalSumTSM = 0;
  float totalAveragesSumTSM = 0;
  for(int i = 0; i < worldSize; i++)
  {
    float processSumTSM = 0;            // For each process
    std::vector<modelAgent*> agents;
    context.selectAgents(context.size(), agents);
    for(size_t j = 0; j < agents.size(); j++)
    {
      processSumTSM += agents.at(j)-> getTSM();
      struct exportAgentPathInfoStruct agentPath = agents.at(j)->getAgentPathInfo();
      for(int iterator = 0; iterator < agentPath.pathX.size(); iterator++)
      {
        int x = agentPath.pathX.at(iterator);
        int y = agentPath.pathY.at(iterator);
        int mode = agentPath.travelMode;
        collisionsMap.setElement(x,y,collisionsMap.getElement(x,y)+1+(5*(1-mode)*(1-policy2Mode))); // ie add 1 for cyclists and 5 for drivers
      }
    }
    float processAverageTSM = processSumTSM / (float)(agents.size()-1);
    totalAveragesSumTSM += processAverageTSM;
  }
  totalAveragesTSM = totalAveragesSumTSM/worldSize;
}

void cityModel::simulateColisions()
{
  std::vector<modelAgent*> agents; // make a container for agents
  context.selectAgents(agentCount, agents); // get random ordered selection of agents and add them to container
  for(int iterator = 0; iterator < agents.size(); iterator++)// iterate through all agents
  {
    struct exportAgentPathInfoStruct AgentPathInfo = (agents.at(iterator))->getAgentPathInfo(); // for each agent, execute its init task;

    for(int a = 0; a < AgentPathInfo.pathX.size(); a++)
    {
      int x = AgentPathInfo.pathX.at(a);
      int y = AgentPathInfo.pathY.at(a);
      int cellBusyness = collisionsMap.getElement(x,y);
      double cellcrashThreshold = (double)cellBusyness*0.0001; // multiply by a constant to set proportional crash likelyhood based on cell busyness, ie per bike crossing, chance of crash increases by 0.01%
      double randomDouble = repast::Random::instance()->nextDouble();
      if(randomDouble < cellcrashThreshold)
      {
        (agents.at(iterator))->setPreviousColision(true);
      }
      else
      {
        (agents.at(iterator))->setPreviousColision(false);
      }
    }
  }
}

void cityModel::updateAgents()
{
  std::vector<modelAgent*> agents; // make a container for agents
  context.selectAgents(agentCount, agents); // get random ordered selection of agents and add them to container
  for(int iterator = 0; iterator < agents.size(); iterator++)// iterate through all agents
  {
    (agents.at(iterator))->setCurrentTemp(modelTemp); // for each agent, set the global model temp
    //(agents.at(iterator))->setTSM(totalAveragesTSM); // for each agent, set the global travelSafetyMetric value
  }
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
