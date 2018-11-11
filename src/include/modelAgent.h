#ifndef AGENT_HEADER
  #define AGENT_HEADER

  #include "repast_hpc/AgentId.h"
  #include "repast_hpc/SharedContext.h"
  #include "propertiesMap.h"
  #include <vector>
  #include <math.h>

  class modelAgent
  {
  private:
    repast::AgentId selfID;
    // insert private variables
    int currentTravelMode;
    float fitness;
    float illness;
    std::vector<int> homeLocation;
    std::vector<int> workLocation;
    float routeTravelSafetyMetric;
    float distanceToWork;
    float currentTemp;

  public:
    modelAgent(repast::AgentId id);
    ~modelAgent();

    void makeDecision();
    void assessPath();
    void init();
    void doSomething();

    /* Required Getters */
    virtual repast::AgentId & getId(){                   return selfID;    }
    virtual const repast::AgentId & getId() const {      return selfID;    }

    // insert getters and setters for variables here
    void setCurrentTravelMode(int *input);
    void setFitness(float input);
    void setIllness(float input);
    void setHomeLocation(std::vector<int> input);
    void setWorkLocation(std::vector<int> input);
    void setCurrentTemp(float input);
    void getCurrentTemp(float *output);
    void getCurrentTravelMode(int *output);
    void getFitness(float *output);
    void getIllness(float*output);
    void getHomeLocation(std::vector<int> *output);
    void getWorkLocation(std::vector<int> *output);
    float block2Distance(int input);
    // insert actions/functions here

  };

#endif
