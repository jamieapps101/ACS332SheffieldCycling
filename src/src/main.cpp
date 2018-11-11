#include <stdio.h>
#include <boost/mpi.hpp>
#include "repast_hpc/RepastProcess.h"
//#include <string>
#include "cityModel.h"


int main(int argc, char** argv)
{

  //printf("Hello World, I'm starting\n"); // Say hi to the world!!!!
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
  //printf("Ok,I'm done here\n");
}

// to do
  // source all data listed below
    // socio-economic status per area
      // crime stats
      // house prices
      // street lighting
    // elevation - done
    // global temp - done
  // need to read in all data and comminicate to agents
  // assign random variables to agents eg fitness  - done
  // add random temporal events to support illness
  // Create agent decision making and creat output data
  // work out how to output data
