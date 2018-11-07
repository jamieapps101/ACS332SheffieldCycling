#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <boost/mpi.hpp>
#include "repast_hpc/Schedule.h"
#include "repast_hpc/Properties.h"

class cityModel
{
private:
	int stopAt;
	repast::Properties* props;
public:
	cityModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm);
	~cityModel();
	void init();
	void initSchedule(repast::ScheduleRunner& runner);
};

#endif
