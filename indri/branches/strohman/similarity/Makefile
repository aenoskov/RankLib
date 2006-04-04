
include ../MakeDefns
SHARED=
INCPATH=-I../include $(patsubst %, -I../contrib/%/include, $(DEPENDENCIES))
LIBPATH=-L../obj  $(patsubst %, -L../contrib/%/obj, $(DEPENDENCIES))
LIBS=-lindri $(patsubst %, -l%, $(DEPENDENCIES))
APP=similarity

all:
	$(CXX) $(CXXFLAGS) -c ClusterEnvironment.cpp -o ClusterEnvironment.o
	$(CXX) $(CXXFLAGS) -c $(APP).cpp -o $(APP).o
	$(CXX) $(CXXFLAGS) *.o -o $(APP) $(LIBPATH) $(LIBS) $(CPPLDFLAGS)

install:
	$(INSTALL_PROGRAM) $(APP) $(bindir)

clean:
	rm -f $(APP)
