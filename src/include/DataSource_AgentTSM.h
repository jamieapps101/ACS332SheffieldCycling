#ifndef AGENT_TSM
  #define AGENT_TSM
  #include "modelAgent.h"
  #include "repast_hpc/TDataSource.h"
  #include "repast_hpc/SVDataSet.h"
	#include "repast_hpc/SharedContext.h"

  class DataSource_AgentTSM : public repast::TDataSource<double>
  {
  private:
    repast::SharedContext<modelAgent>* context;
  public:
    DataSource_AgentTSM(repast::SharedContext<modelAgent>* c);
    ~DataSource_AgentTSM();
    double getData();
  };
#endif
