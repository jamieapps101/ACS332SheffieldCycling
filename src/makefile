# Get the includes done
include ./env
# How about some variables?

Processes = 8

# Right, now for some recipes.PHONY
.PHONY : all
all: bin/executable


.PHONY : build
build: # This is where all the crazy linking occurs, see other makefile for ref
	@echo nothing to see here

bin/executable: objects/modelAgent.o objects/cityModel.o objects/main.o
	@echo Building Executable
	@$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./bin/executable ./objects/*.o $(REPAST_HPC_LIB) $(BOOST_LIBS) -w

objects/cityModel.o: src/cityModel.cpp
	@echo Creating cityModel.o
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I ./include/ -c ./src/cityModel.cpp -o ./objects/cityModel.o  -w

objects/modelAgent.o: src/modelAgent.cpp
	@echo Creating modelAgent.o
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I ./include/ -c ./src/modelAgent.cpp -o ./objects/modelAgent.o  -w

objects/main.o: src/main.cpp
	@echo Creating main.o
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I ./include/ -c ./src/main.cpp -o ./objects/main.o  -w

.PHONY : run
run:  #*.props
	#source ~/.profile
	@echo Running Simulation
	@mpirun -n $(Processes) ./bin/executable ./props/config.props ./props/model.props

.PHONY : clean
clean:
	@echo Cleaning!
	-rm objects/*.o

.PHONY : clear
clear: clean
	@echo Clearing!
	-rm bin/*
