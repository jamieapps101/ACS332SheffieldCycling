#include "DataSource_AgentDecisions.h"

DataSource_AgentDecisions::DataSource_AgentDecisions(repast::SharedContext<modelAgent>* c) : context(c)
{

}

int DataSource_AgentDecisions::getData()
{
  int sum = 0;
  repast::SharedContext<modelAgent>::const_local_iterator iter    = context->localBegin();
  repast::SharedContext<modelAgent>::const_local_iterator iterEnd = context->localEnd();
  while(iter != iterEnd) {
    int decision = (*iter)->getCurrentTravelMode();
    if(decision == CYCLEMODE)
    {
      sum++;
    }
    iter++;
  }
  return sum;
}
