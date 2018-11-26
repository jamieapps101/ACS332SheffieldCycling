#include "modelAgent.h"
#include "fl/Headers.h"
#include <vector>


modelAgent::modelAgent(repast::AgentId id)
{
  selfID = id;
  for(int a = 0; a < 3; a++)
  {
    homeLocation.push_back(0);
    workLocation.push_back(0);
  }
}

modelAgent::~modelAgent()
{

}
// insert actions/functions here

void modelAgent::didICrash()
{

}
void modelAgent::makeDecision()
{
  struct pathInfoStruct pathInfo = assessPath();
  std::cout << "homeX " << homeLocation.at(0) << std::endl;
  std::cout << "homeY " << homeLocation.at(1) << std::endl;
  std::cout << "workX " << workLocation.at(0) << std::endl;
  std::cout << "workY " << workLocation.at(1) << std::endl;
  std::cout << "Distance " << pathInfo.distance << std::endl;
  std::cout << "Delta height " << pathInfo.deltaHeight << std::endl;
  std::cout << "socioEconSum " << pathInfo.socioEconSum << std::endl;
  std::cout << std::endl;
  fuzzyEngine.fitnessInput->setValue(fitness);
  fuzzyEngine.pathLengthInput->setValue(pathInfo.distance);
  fuzzyEngine.SESPathInput->setValue(pathInfo.socioEconSum);
  fuzzyEngine.deltaHeightInput->setValue(pathInfo.deltaHeight);
  fuzzyEngine.temperatureInput->setValue(currentTemp);
  //fuzzyEngine.safetyMetricInput->setValue(fitness); // need to be implimented

  fuzzyEngine.engine->process();
  float output = fuzzyEngine.commuteChoiceOutput->getValue();
  std::cout << "Fuzzy out is " << output << std::endl;
}

struct pathInfoStruct modelAgent::assessPath()
{
  struct pathInfoStruct internalData;
  //std::cout << "flag 1" << std::endl;
  // look at path cell quality for partial safety metric
  // look at either common cell paths or vector intesection to see if paths collide with other agents
  //https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
  std::vector<int> travel;
  travel.push_back(workLocation.at(0) - homeLocation.at(0));
  travel.push_back(workLocation.at(1) - homeLocation.at(1));
  //std::cout << "flag 2" << std::endl;
  if(std::abs(travel.at(0)) > std::abs(travel.at(1)))
  {
    float gradient = (float)travel.at(1)/travel.at(0);
    for(int a = 0; a < std::abs(travel.at(0)); a++)
    {
      internalData.pathX.push_back(a + homeLocation.at(0));
      internalData.pathY.push_back((float)a*gradient + homeLocation.at(1));
    }
  }
  else
  {
    float gradient = (float)travel.at(0)/travel.at(1);
    for(int a = 0; a < std::abs(travel.at(1)); a++)
    {
      internalData.pathX.push_back((float)a*gradient + homeLocation.at(0));
      internalData.pathY.push_back(a + homeLocation.at(1));
    }
  }
  //std::cout << "flag 3, and pathX is size " << internalData.pathX.size() << std::endl;
  internalData.socioEconSum = 0;
  for(int a = 0; a < internalData.pathX.size(); a++)
  {
    //std::cout << "I want element at X" << internalData.pathX.at(a) << " and Y" << internalData.pathY.at(a) <<std::endl;
  //  SESLocal.printMap();
    internalData.socioEconSum += SESLocal.getElement(internalData.pathX.at(a),internalData.pathY.at(a));
    //int z = SESLocal.getElement(0,internalData.pathY.at(a));
    //std::cout << "z = " << internalData.socioEconSum << std::endl;
  }
  //std::cout << "flag 4" << std::endl;
  internalData.distance = (int)pow((pow((homeLocation.at(0) - workLocation.at(0)),2) + pow((homeLocation.at(1) - workLocation.at(1)),2)),0.5);
  internalData.deltaHeight = std::abs(homeLocation.at(2) - workLocation.at(2));
  return internalData;
}

void modelAgent::init(propertiesMap SESinput)
{
    // need to pick home/work locations here. need to export common work/living locations for this
    std::cout<<"My fitness is: " << fitness << std::endl;
    SESLocal = SESinput;
    std::cout<< "and my map got here ok" << std::endl;
    fuzzyEngine = buildEngine();
    //SESLocal.printMap();
}
void modelAgent::doSomething()
{
  unsigned long a = 0;
  while(a < 10)
  {
    a+=1;
  }
}
float modelAgent::block2Distance(int input)
{
  return (float)input*1.37; // as calculated from lat/long distance measurements from original map
}
// insert getters and setters for variables here
void modelAgent::setCurrentTravelMode(int *input)
{
  currentTravelMode = *input;
}
void modelAgent::setFitness(float input)
{
  fitness = input;
}
void modelAgent::setIllness(float input)
{
  illness = input;
}
void modelAgent::setHomeLocation(std::vector<int> input)
{
  homeLocation = input;
}
void modelAgent::setWorkLocation(std::vector<int> input)
{
  workLocation = input;
}
int modelAgent::getCurrentTravelMode()
{
  return currentTravelMode;
}
void modelAgent::getFitness(float *output)
{
  *output = fitness;
}
void modelAgent::getIllness(float *output)
{
  *output = illness;
}
void modelAgent::getHomeLocation(std::vector<int> *output)
{
  *output = homeLocation;
}
void modelAgent::getWorkLocation(std::vector<int> *output)
{
  *output = workLocation;
}
void modelAgent::setCurrentTemp(float input)
{
  currentTemp = input;
}
void modelAgent::getCurrentTemp(float *output)
{
  *output = currentTemp;
}

float getRouteTravelSafetyMetric()
{
  return float routeTravelSafetyMetric;
}

struct fuzzyLogicStruct modelAgent::buildEngine()
{
  struct fuzzyLogicStruct internal;
  internal.engine = new fl::Engine;
  internal.fitnessInput = new fl::InputVariable;
  {
    internal.fitnessInput->setName("fitness");
    internal.fitnessInput->setDescription("");
    internal.fitnessInput->setEnabled(true);
    internal.fitnessInput->setRange(0.000, 1.000);
    internal.fitnessInput->addTerm(new fl::Ramp("unfit", 1.000, 0.000));
    internal.fitnessInput->addTerm(new fl::Ramp("fit", 0.000, 1.000));
  }
  internal.engine->addInputVariable(internal.fitnessInput);
  internal.pathLengthInput = new fl::InputVariable;
  {
    internal.pathLengthInput->setName("pathLength");
    internal.pathLengthInput->setDescription("");
    internal.pathLengthInput->setEnabled(true);
    internal.pathLengthInput->setRange(0.000, 5.000); // up to 5k segmentation, see first report
    internal.pathLengthInput->addTerm(new fl::Ramp("vShort",     1.000, 0.000));
    internal.pathLengthInput->addTerm(new fl::Triangle("Short",  0.000, 1.000, 2.000));
    internal.pathLengthInput->addTerm(new fl::Triangle("Medium", 1.000, 2.000, 3.000));
    internal.pathLengthInput->addTerm(new fl::Triangle("Long",   2.000, 3.000, 4.000));
    internal.pathLengthInput->addTerm(new fl::Ramp("vLong", 3.000, 5.000));
  }
  internal.engine->addInputVariable(internal.pathLengthInput);
  internal.SESPathInput = new fl::InputVariable; //// Needs calibration based on data
  {
    internal.SESPathInput->setName("SESPath");
    internal.SESPathInput->setDescription("");
    internal.SESPathInput->setEnabled(true);
    internal.SESPathInput->setLockValueInRange(false);
    internal.SESPathInput->setRange(0.000, 1.000);
    internal.SESPathInput->addTerm(new fl::Ramp("notNice", 1.000, 0.000));
    internal.SESPathInput->addTerm(new fl::Ramp("nice", 0.000, 1.000));
  }
  internal.engine->addInputVariable(internal.SESPathInput);
  internal.deltaHeightInput = new fl::InputVariable;
  {
    internal.deltaHeightInput->setName("deltaHeight");
    internal.deltaHeightInput->setDescription("");
    internal.deltaHeightInput->setEnabled(true);
    internal.deltaHeightInput->setRange(0.000, 50.000);
    internal.deltaHeightInput->addTerm(new fl::Ramp("small", 50.000, 0.000));
    internal.deltaHeightInput->addTerm(new fl::Ramp("large", 0.000, 50.000));
  }
  internal.engine->addInputVariable(internal.deltaHeightInput);
  internal.temperatureInput = new fl::InputVariable;
  {
    internal.temperatureInput->setName("temperature");
    internal.temperatureInput->setDescription("");
    internal.temperatureInput->setEnabled(true);
    internal.temperatureInput->setRange(0.000, 30.000);
    internal.temperatureInput->addTerm(new fl::Ramp("cold", 5.000, 0.000));
    internal.temperatureInput->addTerm(new fl::Triangle("cool",  2.500, 7.500, 12.500));
    internal.temperatureInput->addTerm(new fl::Triangle("meh",  10.000, 15.000, 20.000));
    internal.temperatureInput->addTerm(new fl::Triangle("warm",  17.500, 22.500, 27.500));
    internal.temperatureInput->addTerm(new fl::Ramp("hot", 25.000, 30.000));
  }
  internal.engine->addInputVariable(internal.temperatureInput);
  internal.safetyMetricInput = new fl::InputVariable;
  {
    internal.safetyMetricInput->setName("safetyMetric");
    internal.safetyMetricInput->setDescription("");
    internal.safetyMetricInput->setEnabled(true);
    internal.safetyMetricInput->setRange(0.000, 1.000);
    internal.safetyMetricInput->addTerm(new fl::Ramp("unsafe", 1.000, 0.000));
    internal.safetyMetricInput->addTerm(new fl::Ramp("safe", 0.000, 1.000));
  }
  internal.engine->addInputVariable(internal.safetyMetricInput);

  internal.commuteChoiceOutput = new fl::OutputVariable; // look at this
  {
    internal.commuteChoiceOutput->setName("commuteChoice");
    internal.commuteChoiceOutput->setDescription("");
    internal.commuteChoiceOutput->setEnabled(true);
    internal.commuteChoiceOutput->setRange(0.000, 1.000);
    internal.commuteChoiceOutput->setLockValueInRange(true);
    //commuteChoiceOutput->setAggregation(new Maximum);
    internal.commuteChoiceOutput->setDefuzzifier(new fl::WeightedAverage());
    internal.commuteChoiceOutput->setDefaultValue(fl::nan);
    internal.commuteChoiceOutput->setLockPreviousValue(false);
    internal.commuteChoiceOutput->addTerm(new fl::Ramp("cycle", 1.000, 0.000));
    internal.commuteChoiceOutput->addTerm(new fl::Ramp("notCycle", 0.000, 1.000));
  }
  internal.engine->addOutputVariable(internal.commuteChoiceOutput);

  internal.mamdani = new fl::RuleBlock; //  Look at this
  {
    internal.mamdani->setName("mamdani");
    internal.mamdani->setDescription("");
    internal.mamdani->setEnabled(true);
    internal.mamdani->setConjunction(fl::null);
    internal.mamdani->setDisjunction(fl::null);
    internal.mamdani->setImplication(new fl::AlgebraicProduct);
    internal.mamdani->setActivation(new fl::General);
    //internal.mamdani->addRule(Rule::parse("if obstacle is left then mSteer is right", engine));
  }
  internal.engine->addRuleBlock(internal.mamdani);

  std::string status;
  if (not internal.engine->isReady(&status))
  {
      //return null;
  }
  else
  {
    return internal;
  }
}

struct exportAgentPathInfoStruct getAgentPathInfo()
{
  struct exportAgentPathInfoStruct internal;
  internal.homeLocation = homeLocation;
  internal.workLocation = workLocation;
  internal.selfID = selfID;
  return internal;
}
