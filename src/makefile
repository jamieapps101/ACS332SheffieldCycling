# Get the includes done
include ./env

# How about some variables?


# Right, now for some recipes.PHONY
.PHONY : all
all:
	echo "Please specify an option when using this make file"

.PHONY : build
build: # This is where all the crazy linking occurs, see other makefile for ref
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/main.cpp -o ./objects/main.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./bin/executable ./objects/main.o $(REPAST_HPC_LIB) $(BOOST_LIBS)

.PHONY : run
run:  #*.props
	mpirun -n 4 ./bin/executable

.PHONY : clean
clean:
	rm objects/*.o

.PHONY : clear
clear:
	rm objects/*
	rm bin/*
