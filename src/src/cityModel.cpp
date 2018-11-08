#include <stdio.h>
#include <iostream>
#include "cityModel.h"
#include "modelAgent.h"
#include <boost/mpi.hpp>
#include "repast_hpc/AgentId.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/initialize_random.h"
#include <string>

cityModel::cityModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm)
{
  props = new repast::Properties(propsFile, argc, argv, comm); // import properties from model.props
	stopAt = repast::strToInt(props->getProperty("stop.at"));
  agentCount = repast::strToInt(props->getProperty("count.of.agents"));
  std::string mapDataName = props->getProperty("map.data.name");
  std::cout << "map name " << mapDataName << std::endl; 
  initializeRandom(*props, comm);
    // need to read in data files here, prehaps make use of comm and read in main?
  // allocate struct array containing map data from map element pointer. One struct element per grid square

}

cityModel::~cityModel()
{
  delete props;
}

void cityModel::init() // initialise the model with agents
{
  int rank = repast::RepastProcess::instance()->rank(); // get rank of this process' instance of the model
  std::cout << "Starting process " << rank << " with " << agentCount << " agents." << std::endl;
  for(int i = 0; i < agentCount; i++) // a counter to count upto the desired number of agents
  {
    repast::AgentId id(i, rank, 0); // create an agentID object, with its ID, starting process number and type (left as 0)
    id.currentRank(rank); //this just seems to reset the rank value of the object
    modelAgent* agent = new modelAgent(id); // create a new agent with the ID set to the agentID object
    context.addAgent(agent); // add this agent to the process context
  }
}


void cityModel::executeAgents() // this gets random order of all agents in context and runs them
{
  std::vector<modelAgent*> agents; // make a container for agents
  context.selectAgents(agentCount, agents); // get random ordered selection of agents and add them to container
  for(int iterator = 0; iterator < agents.size(); iterator++)// iterate through all agents
  {
    (agents.at(iterator))->doSomething(); // for each agent, execute its primary task;
  }
  std::cout << std::endl;
}

void cityModel::initSchedule(repast::ScheduleRunner& runner)
{

  runner.scheduleEvent(1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<cityModel> (this, &cityModel::executeAgents)));
  //runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::recordResults)));
	runner.scheduleStop(stopAt);

}
