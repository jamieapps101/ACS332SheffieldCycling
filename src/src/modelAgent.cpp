#include "modelAgent.h"
#include "fl/Headers.h"
#include <vector>
#include "repast_hpc/RepastProcess.h"

modelAgent::modelAgent(repast::AgentId id)
{
  selfID = id;
  for(int a = 0; a < 3; a++)
  {
    homeLocation.push_back(0);
    workLocation.push_back(0);
  }
  travelSafetyMetric = 0.0;
  setRuleWeightsCheck = false;
  previousCollision = false;
}

modelAgent::~modelAgent()
{

}
// insert actions/functions here

void modelAgent::setPreviousColision(bool input)
{
  previousCollision = input;
}

void modelAgent::didICrash(propertiesMap internalCollisionsMap)
{
  for(int a = 0; a < internalAgentPathInfo.pathX.size(); a++)
  {
    int x = internalAgentPathInfo.pathX.at(a);
    int y = internalAgentPathInfo.pathY.at(a);
    int cellBusyness = internalCollisionsMap.getElement(x,y);
    double cellcrashThreshold = (double)cellBusyness*0.0001; // multiply by a constant to set proportional crash likelyhood based on cell busyness, ie per bike crossing, chance of crash increases by 0.01%
    double randomDouble = repast::Random::instance()->nextDouble();
    if(randomDouble < cellcrashThreshold)
    {
      previousCollision = true;
      break;
    }
  }
}

void modelAgent::makeDecision()
{
  std::cout << "hello" << std::endl;
  if(repast::RepastProcess::instance()->rank() == 0)
  {
    repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();
    std::cout << "I'm on tick " << runner.currentTick() << std::endl;
  }
  /*
  internalAgentPathInfo.pathX = pathInfo.pathX;
  internalAgentPathInfo.pathY = pathInfo.pathY;
  internalAgentPathInfo.travelMode = currentTravelMode;
  */
  /*
  std::cout << "homeX " << homeLocation.at(0) << std::endl;
  std::cout << "homeY " << homeLocation.at(1) << std::endl;
  std::cout << "workX " << workLocation.at(0) << std::endl;
  std::cout << "workY " << workLocation.at(1) << std::endl;
  std::cout << "Distance " << pathInfo.distance << std::endl;
  std::cout << "Delta height " << pathInfo.deltaHeight << std::endl;
  std::cout << "socioEconSum " << pathInfo.socioEconSum << std::endl;
  std::cout << std::endl;
  */
  /*
  fuzzyEngine.fitnessInput->setValue(fitness);
  //std::cout << "Fitness " << fitness << std::endl;
  fuzzyEngine.pathLengthInput->setValue(pathInfo.distance);
  //std::cout << "PathLength " << pathInfo.distance << std::endl;
  fuzzyEngine.SESPathInput->setValue(pathInfo.socioEconSum);
  //std::cout << "socioEconSum " << pathInfo.socioEconSum << std::endl;
  fuzzyEngine.deltaHeightInput->setValue(pathInfo.deltaHeight);
  //std::cout << "deltaHeight " << pathInfo.deltaHeight << std::endl;
  fuzzyEngine.temperatureInput->setValue(currentTemp);
  //std::cout << "Temp " << currentTemp << std::endl;
  fuzzyEngine.safetyMetricInput->setValue(travelSafetyMetric);
  //std::cout << "safetyMetricInput " << travelSafetyMetric << std::endl;
  //std::cout << "Abt to infer" << std::endl;
  fuzzyEngine.engine->process();
  float output = fuzzyEngine.commuteChoiceOutput->getValue();
  */
  //std::cout << "Fuzzy out is " << output << std::endl;
  /*
  if(output > 0.5)
  {
    currentTravelMode = CYCLEMODE;
  }
  else
  {
    currentTravelMode = DRIVEMODE;
  }
  */

}

struct pathInfoStruct modelAgent::assessPath()
{
  //assessPathInternalData;
  std::vector<int> travel;
  travel.push_back(workLocation.at(0) - homeLocation.at(0));
  travel.push_back(workLocation.at(1) - homeLocation.at(1));
  if(std::abs(travel.at(0)) > std::abs(travel.at(1)))
  {
    float gradient = (float)travel.at(1)/travel.at(0);
    for(int a = 0; a < std::abs(travel.at(0)); a++)
    {
      assessPathInternalData.pathX.push_back(a + homeLocation.at(0));
      assessPathInternalData.pathY.push_back((float)a*gradient + homeLocation.at(1));
    }
  }
  else
  {
    float gradient = (float)travel.at(0)/travel.at(1);
    for(int a = 0; a < std::abs(travel.at(1)); a++)
    {
      assessPathInternalData.pathX.push_back((float)a*gradient + homeLocation.at(0));
      assessPathInternalData.pathY.push_back(a + homeLocation.at(1));
    }
  }
  assessPathInternalData.socioEconSum = 0;
  //std::cout << "Path Size " << assessPathInternalData.pathX.size() << std::endl;
  std::cout << "My rank is " << selfID.currentRank() << " Size " << assessPathInternalData.pathX.size() << std::endl;
  for(int a = 0; a < assessPathInternalData.pathX.size(); a++)
  {
    assessPathInternalData.socioEconSum += SESLocal.getElement(assessPathInternalData.pathX.at(a),assessPathInternalData.pathY.at(a));
    std::cout << "My rank is " << selfID.currentRank() << " X " << assessPathInternalData.pathX.at(a) << " Y " << assessPathInternalData.pathY.at(a) << std::endl;
  }
  /*
  assessPathInternalData.distance = (int)pow((pow((homeLocation.at(0) - workLocation.at(0)),2) + pow((homeLocation.at(1) - workLocation.at(1)),2)),0.5);
  assessPathInternalData.deltaHeight = std::abs(homeLocation.at(2) - workLocation.at(2));
  travel.clear();
  */
    //std::cout << "My rank is " << selfID.currentRank() << " and I'm returning" << std::endl;
  return assessPathInternalData;
}

void modelAgent::init(propertiesMap SESinput)
{
    SESLocal = SESinput;
    fuzzyEngine = buildEngine();
    pathInfo = assessPath(); // mem error here
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
void modelAgent::setCurrentTravelMode(int input)
{
  currentTravelMode = input;
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
float modelAgent::getTSM()
{
  return travelSafetyMetric;
}
struct fuzzyLogicStruct modelAgent::buildEngine()
{
  struct fuzzyLogicStruct internal;

  internal.engine = new fl::Engine;
  internal.engine->setName("toCycleorNotTocycle");
  internal.engine->setDescription("");

  internal.fitnessInput = new fl::InputVariable;
  {
    internal.fitnessInput->setName("fitness");
    internal.fitnessInput->setDescription("");
    internal.fitnessInput->setEnabled(true);
    internal.fitnessInput->setRange(0.000, 1.000);
    internal.fitnessInput->setLockValueInRange(true);
    internal.fitnessInput->setValue(0.5);
    internal.fitnessInput->addTerm(new fl::Ramp("unfit", 1.000, 0.000));
    internal.fitnessInput->addTerm(new fl::Ramp("fit", 0.000, 1.000));
  }
  internal.engine->addInputVariable(internal.fitnessInput);

  internal.pathLengthInput = new fl::InputVariable;
  {
    internal.pathLengthInput->setName("pathLength");
    internal.pathLengthInput->setDescription("");
    internal.pathLengthInput->setEnabled(true);
    internal.pathLengthInput->setRange(0.000, 45.000); // up to 5k segmentation, see first report
    internal.pathLengthInput->setLockValueInRange(true);
    internal.pathLengthInput->setValue(5);
    internal.pathLengthInput->addTerm(new fl::Ramp("short", 5.000, 0.000));
    internal.pathLengthInput->addTerm(new fl::Ramp("long", 1.000,45.000));
  }
  internal.engine->addInputVariable(internal.pathLengthInput);

  internal.SESPathInput = new fl::InputVariable; //// Needs calibration based on data
  {
    internal.SESPathInput->setName("SESPath");
    internal.SESPathInput->setDescription("");
    internal.SESPathInput->setEnabled(true);
    internal.SESPathInput->setRange(0.000, 1.000);
    internal.SESPathInput->setLockValueInRange(true);
    internal.SESPathInput->setValue(0.5);
    internal.SESPathInput->addTerm(new fl::Ramp("notNice", 1.000, 0.000));
    internal.SESPathInput->addTerm(new fl::Ramp("nice", 0.000, 1.000));
  }
  internal.engine->addInputVariable(internal.SESPathInput);
  internal.deltaHeightInput = new fl::InputVariable;
  {
    internal.deltaHeightInput->setName("deltaHeight");
    internal.deltaHeightInput->setDescription("");
    internal.deltaHeightInput->setEnabled(true);
    internal.deltaHeightInput->setLockValueInRange(true);
    internal.deltaHeightInput->setRange(0.000, 50.000);
    internal.deltaHeightInput->setValue(10);
    internal.deltaHeightInput->addTerm(new fl::Ramp("small", 50.000, 0.000));
    internal.deltaHeightInput->addTerm(new fl::Ramp("large", 1.000, 49.000));
  }
  internal.engine->addInputVariable(internal.deltaHeightInput);
  internal.temperatureInput = new fl::InputVariable;
  {
    internal.temperatureInput->setName("temperature");
    internal.temperatureInput->setDescription("");
    internal.temperatureInput->setEnabled(true);
    internal.temperatureInput->setLockValueInRange(true);
    internal.temperatureInput->setRange(0.000, 30.000);
    internal.temperatureInput->setValue(15);
    internal.temperatureInput->addTerm(new fl::Ramp("cool",  30.00, 0.00));
    internal.temperatureInput->addTerm(new fl::Ramp("warm",  0.00, 30.00));
  }
  internal.engine->addInputVariable(internal.temperatureInput);
  internal.safetyMetricInput = new fl::InputVariable;
  {
    internal.safetyMetricInput->setName("safetyMetric");
    internal.safetyMetricInput->setDescription("");
    internal.safetyMetricInput->setEnabled(true);
    internal.safetyMetricInput->setRange(0.000, 1.000);
    internal.safetyMetricInput->setLockValueInRange(true);
    internal.safetyMetricInput->setValue(0.5);
    internal.safetyMetricInput->addTerm(new fl::Ramp("unsafe", 1.000, 0.000));
    internal.safetyMetricInput->addTerm(new fl::Ramp("safe", 0.000, 1.000));
  }
  internal.engine->addInputVariable(internal.safetyMetricInput);

  internal.commuteChoiceOutput = new fl::OutputVariable;
  {
    internal.commuteChoiceOutput->setName("commuteChoice");
    internal.commuteChoiceOutput->setDescription("");
    internal.commuteChoiceOutput->setEnabled(true);
    internal.commuteChoiceOutput->setRange(0.000, 1.000);
    internal.commuteChoiceOutput->setLockValueInRange(true);
    internal.commuteChoiceOutput->setAggregation(new fl::Maximum);
    internal.commuteChoiceOutput->setDefuzzifier(new fl::WeightedAverage());
    //internal.commuteChoiceOutput->setDefaultValue(0.123456);
    internal.commuteChoiceOutput->setLockPreviousValue(false);
    internal.commuteChoiceOutput->addTerm(new fl::Ramp("cycle", 1.000, 0.000));
    internal.commuteChoiceOutput->addTerm(new fl::Ramp("notCycle", 0.000, 1.000));
  }
  internal.engine->addOutputVariable(internal.commuteChoiceOutput);

  internal.mamdani = new fl::RuleBlock;
  {
    internal.mamdani->setName("mamdani");
    internal.mamdani->setDescription("");
    internal.mamdani->setEnabled(true);
    internal.mamdani->setConjunction(new fl::Minimum); // How do we want AND to work
    internal.mamdani->setDisjunction(new fl::Maximum); // how do we want OR to work
    internal.mamdani->setImplication(new fl::AlgebraicProduct);
    internal.mamdani->setActivation(new fl::General);
  }

    ////////////////////////////////////////////// RULES
    std::vector<std::string> rules;
    rules.push_back("if fitness is fit and temperature is warm then commuteChoice is cycle");
    rules.push_back("if fitness is unfit and temperature is cool then commuteChoice is notCycle");
    /*
    rules.push_back("if fitness is fit and pathLength is short and SESPath is nice and deltaHeight is small and temperature is warm and safetyMetric is safe then commuteChoice is cycle");
    rules.push_back("if fitness is fit and pathLength is long and SESPath is nice and deltaHeight is small and temperature is warm and safetyMetric is safe then commuteChoice is cycle");
    rules.push_back("if fitness is fit and pathLength is short and SESPath is nice and deltaHeight is large and temperature is warm and safetyMetric is safe then commuteChoice is cycle");
    rules.push_back("if fitness is fit and pathLength is short and SESPath is nice and deltaHeight is large and temperature is warm and safetyMetric is safe then commuteChoice is cycle");

    rules.push_back("if fitness is fit and pathLength is long and SESPath is notNice and deltaHeight is small and temperature is warm and safetyMetric is unsafe then commuteChoice is notCycle");
    rules.push_back("if fitness is unfit and pathLength is short and SESPath is nice and deltaHeight is small and temperature is warm and safetyMetric is safe then commuteChoice is notCycle");
    rules.push_back("if fitness is unfit and pathLength is long and SESPath is notNice and deltaHeight is large and temperature is cool and safetyMetric is unsafe then commuteChoice is notCycle");
    */
    /////////////////////////////////////////// WEIGHTS
    std::vector<float> internalRuleWeight;
    internalRuleWeight.push_back(1);
    internalRuleWeight.push_back(1);
    internalRuleWeight.push_back(1);
    internalRuleWeight.push_back(1);
    internalRuleWeight.push_back(1);
    internalRuleWeight.push_back(1);
    internalRuleWeight.push_back(1);

    ///////////////////////////////////////// INPUT RULES AND WEIGHTS
    if(setRuleWeightsCheck == true)
    {
      internalRuleWeight = ruleWeight;
    }
    for(int iterator = 0; iterator < rules.size(); iterator++)
    {
      std::string argsString = rules.at(iterator);
      //argstring += " with " + std::to_string(internalRuleWeight.at(iterator));
      //std::cout << "argsString " << argsString << std::endl;
      internal.mamdani->addRule(fl::Rule::parse(argsString, internal.engine));
    }
    /*
    */
    //std::cout << "I got here!" << std::endl;
    /*
    internal.mamdani->addRule(fl::Rule::parse("if fitness is fit and pathLength is short then commuteChoice is cycle", internal.engine));
    std::cout << "I also got here!" << std::endl;
    internal.mamdani->addRule(fl::Rule::parse("if fitness is fit   and pathLength is long  and SESPath is nice    and deltaHeight is small and temperature is warm and safetyMetric is safe   then commuteChoice is cycle", internal.engine));
    internal.mamdani->addRule(fl::Rule::parse("if fitness is fit   and pathLength is short and SESPath is nice    and deltaHeight is small and temperature is warm and safetyMetric is safe   then commuteChoice is cycle", internal.engine));
    internal.mamdani->addRule(fl::Rule::parse("if fitness is fit   and pathLength is short and SESPath is nice    and deltaHeight is large and temperature is warm and safetyMetric is safe   then commuteChoice is cycle", internal.engine));
    internal.mamdani->addRule(fl::Rule::parse("if fitness is fit   and pathLength is long  and SESPath is notNice and deltaHeight is small and temperature is warm and safetyMetric is unsafe then commuteChoice is notCycle", internal.engine));
    internal.mamdani->addRule(fl::Rule::parse("if fitness is unfit and pathLength is short and SESPath is nice    and deltaHeight is small and temperature is warm and safetyMetric is safe   then commuteChoice is notCycle", internal.engine));
    internal.mamdani->addRule(fl::Rule::parse("if fitness is unfit and pathLength is long then commuteChoice is notCycle", internal.engine));
    std::cout << "and here! " << std::endl;
    */

  internal.engine->addRuleBlock(internal.mamdani);
  std::string status;
  if (not internal.engine->isReady(&status))
  {
      //return null;
      //std::cout << "Bad Fuzzy Engine" << std::endl;
      //std::cout << "Status:" << status << std::endl;
  }
  else
  {
    //std::cout << "Fuzzy Engine Ok!" << std::endl;
  }
  return internal;
  /*
  */
}
void modelAgent::setRuleWeights(std::vector<float> input)
{
  ruleWeight = input;
  setRuleWeightsCheck = true;
}
struct exportAgentPathInfoStruct modelAgent::getAgentPathInfo()
{
  //struct exportAgentPathInfoStruct internal;
  //internal.homeLocation = homeLocation;
  //internal.workLocation = workLocation;
  //internal.selfID = selfID;
  return internalAgentPathInfo;
}
void modelAgent::setTSM(float input)
{
  travelSafetyMetric = input;
}
