#include "DataSource_AgentDecisions.h"

DataSource_AgentDecisions::DataSource_AgentDecisions(repast::SharedContext<modelAgent>* c) : context(c)
{

}

/*
DataSource_AgentDecisions::~DataSource_AgentDecisions()
{
  //*context = NULL;
  //delete context;
}
*/

int DataSource_AgentDecisions::getData()
{
  int TravelModeSum = 0;
  repast::SharedContext<modelAgent>::const_local_iterator iter    = context->localBegin();
  repast::SharedContext<modelAgent>::const_local_iterator iterEnd = context->localEnd();
  while(iter != iterEnd) {
    int decision = (*iter)->getCurrentTravelMode();
    if(decision == CYCLEMODE)
    {
      TravelModeSum++;
    }
    iter++;
  }
  return TravelModeSum;
}
