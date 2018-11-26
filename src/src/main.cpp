#include <stdio.h>
#include <boost/mpi.hpp>
#include "repast_hpc/RepastProcess.h"
//#include <string>
#include "cityModel.h"
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */


int main(int argc, char** argv)
{

  clock_t ticks;
  ticks = clock();
  printf("Hello World, I'm starting\n"); // Say hi to the world!!!!
  std::string configFile = argv[1]; // The name of the configuration file is config.props (Arg 1)
  std::string propsFile = argv[2]; // The name of the properties file is model.props (Arg 2)
  boost::mpi::environment env(argc, argv); // initialise the MPI invironment
  boost::mpi::communicator world; // make an MPI communicator onbject to talk between processes
  repast::RepastProcess::init(configFile); // initialise the repast processes based on the config file
  cityModel* model = new cityModel(propsFile, argc, argv, &world); // initialise the model object that runs the model
  repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner(); // initialise a scheduler that shedules the model processes
  model->init(); // de-reference the model and run the init function
  model->initSchedule(runner); // pass areference to the schedule object to the model instance

  runner.run(); // instruct the sheduler to run the model

  delete model; // after running it, release the memory from the pointer to the model

  repast::RepastProcess::instance()->done(); // instruct the repase process that its done with
  ticks = clock() - ticks;
  double timeTaken = (double)ticks / CLOCKS_PER_SEC;
  std::cout << "That took " << timeTaken << " seconds or " << ticks << " clock ticks"<< std::endl;
  //printf("Ok,I'm done here\n");
}

/*
To Do:
  Finish Fuzzy logic engine tuning:: agent
  Did I crash method::agent // consider cars high risk and bikes low risk per unit of the map decision
  Update local agent datas including TSM::model
  Add initialisation to set initial travel modes proportionately::model
  turn main into function and set up a real main with a few input arguments
  Clean up a little.............
*/
