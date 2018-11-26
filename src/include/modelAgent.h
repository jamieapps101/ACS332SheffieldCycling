#ifndef AGENT_HEADER
  #define AGENT_HEADER
  #define CYCLEMODE 1

  #include "repast_hpc/AgentId.h"
  #include "repast_hpc/SharedContext.h"
  #include "propertiesMap.h"
  #include <vector>
  #include <math.h>
  #include <cmath>
  #include "fl/Headers.h"


  struct pathInfoStruct
  {
    int deltaHeight;
    int distance;
    std::vector<int> pathX;
    std::vector<int> pathY;
    int socioEconSum;
  };

  struct exportAgentPathInfoStruct
  {
    std::vector<int> homeLocation;
    std::vector<int> workLocation;
    repast::AgentId selfID;
  };

  struct fuzzyLogicStruct
  {
    fl::Engine* engine;
    fl::InputVariable* fitnessInput;
    fl::InputVariable* pathLengthInput;
    fl::InputVariable* SESPathInput;
    fl::InputVariable* deltaHeightInput;
    fl::InputVariable* temperatureInput;
    fl::InputVariable* safetyMetricInput;
    fl::OutputVariable* commuteChoiceOutput;
    fl::RuleBlock* mamdani;
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
    //fl::Engine* buildEngine();
    struct fuzzyLogicStruct fuzzyEngine;
    struct fuzzyLogicStruct buildEngine();

  public:
    modelAgent(repast::AgentId id);
    ~modelAgent();
    void didICrash();
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
    void setEngine(fl::Engine* inputEngine);
    float getRouteTravelSafetyMetric();
    struct exportAgentPathInfoStruct getAgentPathInfo()
    // insert actions/functions here

  };

#endif
