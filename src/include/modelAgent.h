#ifndef AGENT_HEADER
  #define AGENT_HEADER
  #define CYCLEMODE 1
  #define DRIVEMODE 0

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
    long socioEconSum;
  };

  struct exportAgentPathInfoStruct
  {
    std::vector<int> pathX;
    std::vector<int> pathY;
    int travelMode;
    //repast::AgentId selfID;
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
    float travelSafetyMetric;
    float distanceToWork;
    float currentTemp;
    struct exportAgentPathInfoStruct internalAgentPathInfo;
    bool previousCollision;
    struct fuzzyLogicStruct fuzzyEngine;
    struct fuzzyLogicStruct buildEngine();
    std::vector<float> ruleWeight;
    bool setRuleWeightsCheck;
    struct pathInfoStruct pathInfo;
    struct pathInfoStruct assessPathInternalData;
    //void createRuleBase(std::vector<std::string> * rules,std::vector<std::string> rulesToBeUsed, fl::Engine engine);
    int policy1Mode;
    int policy2Mode;
    int policy3Mode;
    std::vector<float> internalRuleWeight;

  public:
    modelAgent(repast::AgentId id);
    ~modelAgent();
    void didICrash(propertiesMap internalCollisionsMap);
    void makeDecision();
    struct pathInfoStruct assessPath();
    void init(propertiesMap SESinput);
    void doSomething();

    /* Required Getters */
    virtual repast::AgentId & getId(){                   return selfID;    }
    virtual const repast::AgentId & getId() const {      return selfID;    }

    // insert getters and setters for variables here
    //void setRuleWeights(std::vector<float> input);
    void setCurrentTravelMode(int input);
    void setFitness(float input);
    void setIllness(float input);
    void setTSM(float input);
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
    float getTSM();
    void setPolicies(int mode1Input, int mode2Input, int mode3Input);
    void setInternalRuleWeight(std::vector<float> input);
    //void setTSM(float input);
    struct exportAgentPathInfoStruct getAgentPathInfo();
    void setPreviousColision(bool input);
    // insert actions/functions here

  };

#endif
