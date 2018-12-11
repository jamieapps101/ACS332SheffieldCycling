# Get the includes done
include ./env
# How about some variables?

Processes = 6
arguments = weight1=0,weight2=0,weight3=0,weight4=0,weight5=0



# Right, now for some recipes.PHONY
.PHONY : init all
all: bin/executable

.PHONY : count
count:
	@cloc --exclude-dir=fuzzylite,output --exclude-list-file=makefile *

.PHONY : clearData
clearData:
	rm output/*

.PHONY : build
build: # This is where all the crazy linking occurs, see other makefile for ref
	@echo nothing to see here

.PHONY : init
init:
	-source ~/.profile

.PHONY : optimiser
optimiser: bin/optimiser


bin/optimiser: src/optimisationMain.cpp
	@echo Building optimiser
	g++ -o ./bin/optimiser ./src/optimisationMain.cpp


.PHONY : runOptimiser
runOptimiser:  #*.props
	@echo Running Simulation
	@./bin/optimiser

MODEL_OBJECTS = objects/modelAgent.o objects/DataSource_AgentTSM.o objects/cityModel.o objects/modelMain.o objects/propertiesMap.o objects/DataSource_AgentDecisions.o objects/DataSource_AgentTSM.o
MODEL_OBJECTS_2_LINK = ./objects/modelAgent.o ./objects/cityModel.o ./objects/modelMain.o ./objects/propertiesMap.o ./objects/DataSource_AgentDecisions.o objects/DataSource_AgentTSM.o

bin/executable: $(MODEL_OBJECTS)
	@echo Building Executable
	@$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) $(FUZZYLITE_LIB_DIR) -o ./bin/executable $(MODEL_OBJECTS_2_LINK) $(REPAST_HPC_LIB) $(BOOST_LIBS) $(FUZZYLITE_LIBS) -w

objects/propertiesMap.o: src/propertiesMap.cpp
	@echo Creating propertiesMap.o
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) $(FUZZYLITE_INCLUDE) -I ./include/ -c ./src/propertiesMap.cpp -o ./objects/propertiesMap.o  -w

objects/cityModel.o: src/cityModel.cpp
	@echo Creating cityModel.o
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) $(FUZZYLITE_INCLUDE) -I ./include/ -c ./src/cityModel.cpp -o ./objects/cityModel.o  -w

objects/modelAgent.o: src/modelAgent.cpp
	@echo Creating modelAgent.o
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) $(FUZZYLITE_INCLUDE) -I ./include/ -c ./src/modelAgent.cpp -o ./objects/modelAgent.o  -w

objects/DataSource_AgentDecisions.o: src/DataSource_AgentDecisions.cpp
		@echo Creating DataSource_AgentDecisions.o
		@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) $(FUZZYLITE_INCLUDE) -I ./include/ -c ./src/DataSource_AgentDecisions.cpp -o ./objects/DataSource_AgentDecisions.o  -w

objects/DataSource_AgentTSM.o: src/DataSource_AgentTSM.cpp
		@echo Creating DataSource_AgentTSM.o
		@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) $(FUZZYLITE_INCLUDE) -I ./include/ -c ./src/DataSource_AgentTSM.cpp -o ./objects/DataSource_AgentTSM.o  -w

objects/modelMain.o: src/modelMain.cpp
	@echo Creating main.o
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) $(FUZZYLITE_INCLUDE) -I ./include/ -c ./src/modelMain.cpp -o ./objects/modelMain.o  -w

.PHONY : run
run:  #*.props
	@echo Running Simulation
	rm output/*.txt
	@mpirun -n $(Processes) ./bin/executable ./props/config.props ./props/model.props $(arguments)


.PHONY : clean
clean:
	@echo Cleaning!
	-rm objects/*.o

.PHONY : clear
clear: clean
	@echo Clearing!
	-rm bin/*
