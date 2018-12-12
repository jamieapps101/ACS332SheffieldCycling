#include "DataSource_AgentTSM.h"
#include "repast_hpc/RepastProcess.h"

DataSource_AgentTSM::DataSource_AgentTSM(repast::SharedContext<modelAgent>* c) : context(c)
{

}

/*
DataSource_AgentTSM::~DataSource_AgentTSM()
{
//  *context = NULL;
//  delete context;
}
*/

double DataSource_AgentTSM::getData()
{
  int TravelModeSum = 0;
  repast::SharedContext<modelAgent>::const_local_iterator iter    = context->localBegin();
  repast::SharedContext<modelAgent>::const_local_iterator iterEnd = context->localEnd();
  int agentCount = 0;
  double TSMValue = 0;
  while(iter != iterEnd)
  {
    TSMValue += (*iter)->getTSM();
    iter++;
    agentCount++;
  }
  int worldSize= repast::RepastProcess::instance()->worldSize();
  return (double)(TSMValue/(agentCount*worldSize));
}
