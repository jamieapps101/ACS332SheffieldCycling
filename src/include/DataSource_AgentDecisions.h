#ifndef AGENT_DECISIONS
  #define AGENT_DECISIONS
  #include "modelAgent.h"
  #include "repast_hpc/TDataSource.h"
  #include "repast_hpc/SVDataSet.h"
	#include "repast_hpc/SharedContext.h"

  class DataSource_AgentDecisions : public repast::TDataSource<int>
  {
  private:
    repast::SharedContext<modelAgent>* context;
  public:
    DataSource_AgentDecisions(repast::SharedContext<modelAgent>* c);
    int getData();
  };



#endif
