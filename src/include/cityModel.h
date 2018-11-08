#ifndef MODEL_HEADER
	#define MODEL_HEADER

	#include <boost/mpi.hpp> // to import boost mpi
	#include "repast_hpc/Schedule.h" // to import the scheduler
	#include "repast_hpc/Properties.h" // to import the properties object
	#include "repast_hpc/SharedContext.h" // to import the context object
	#include "modelAgent.h" // to import the agent to be simulated

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
		struct mapElementStruct * mapElement;
	public:
		cityModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm);
		~cityModel();
		void init();
		void initSchedule(repast::ScheduleRunner& runner);
		void executeAgents();
		void doSomething();
	};

#endif
