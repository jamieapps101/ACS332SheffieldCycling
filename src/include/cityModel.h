#ifndef MODEL_HEADER
	#define MODEL_HEADER

	#include <boost/mpi.hpp> // to import boost mpi
	#include "repast_hpc/Schedule.h" // to import the scheduler
	#include "repast_hpc/Properties.h" // to import the properties object
	#include "repast_hpc/SharedContext.h" // to import the context object
	#include "modelAgent.h" // to import the agent to be simulated
	#include "propertiesMap.h" // to store mapVariables
	#include <string>
	#include <vector>

	struct mapElementStruct{
		int altitude;
		float crimeLevels;
		float socioEconomicStatus;

	};

	class cityModel
	{
	private:
		int stopAt; // a variable to tell the process which tick to stop at
		int agentCount;
		repast::Properties* props; // to hold the input model properties
		repast::SharedContext<modelAgent> context; // a context (an area) to store all the agents in
		propertiesMap cityElevationMap;
		std::vector<std::vector <int>> commonWork;
		std::vector<std::vector <int>> commonLiving;
		int string2int(std::string input);
		//struct C * mapElement;
	public:
		cityModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm);
		~cityModel();
		void init();
		void initSchedule(repast::ScheduleRunner& runner);
		void initAgents();
		void executeAgents();
		void doSomething();
	};

#endif
