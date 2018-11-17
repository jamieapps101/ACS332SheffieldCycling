#ifndef AGENT_HEADER
  #define AGENT_HEADER
  #define CYCLEMODE 1

  #include "repast_hpc/AgentId.h"
  #include "repast_hpc/SharedContext.h"
  #include "propertiesMap.h"
  #include <vector>
  #include <math.h>
  #include <cmath>

  struct pathInfoStruct
  {
    int deltaHeight;
    int distance;
    std::vector<int> pathX;
    std::vector<int> pathY;
    int socioEconSum;
  };

  class modelAgent
  {
  private:
    repast::AgentId selfID;
    // insert private variables
    propertiesMap SESLocal;
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
    struct pathInfoStruct assessPath();
    void init(propertiesMap SESinput);
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
    int getCurrentTravelMode();
    void getFitness(float *output);
    void getIllness(float*output);
    void getHomeLocation(std::vector<int> *output);
    void getWorkLocation(std::vector<int> *output);
    float block2Distance(int input);
    // insert actions/functions here

  };

#endif
