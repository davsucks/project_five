CC = g++
LD = g++
CFLAGS = -c -g -std=c++11 -pedantic-errors -Wall
LFLAGS = -g

OBJS = p5_main.o Model.o View.o Controller.o 
OBJS += Sim_object.o Structure.o Moving_object.o Agent.o
OBJS += Farm.o Town_Hall.o
OBJS += Peasant.o Soldier.o
OBJS += Agent_factory.o Structure_factory.o
OBJS += Geometry.o Utility.o
PROG = proj5exe

TEST_OBJS = Test.o Model.o View.o Controller.o
TEST_OBJS += Sim_object.o Structure.o Moving_object.o Agent.o
TEST_OBJS += Farm.o Town_Hall.o
TEST_OBJS += Peasant.o Soldier.o
TEST_OBJS += Agent_factory.o Structure_factory.o
TEST_OBJS += Geometry.o Utility.o
TEST_EXE = testexe


default: $(PROG)

test: CFLAGS += -gdwarf-3
test: $(TEST_EXE)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

$(TEST_EXE): $(TEST_OBJS)
	$(LD) $(LFLAGS) $(TEST_OBJS) -o $(TEST_EXE)

p5_main.o: p5_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p5_main.cpp

Test.o: Test.cpp
	$(CC) $(CFLAGS) Test.cpp

Model.o: Model.cpp Model.h View.h Sim_object.h Structure.h Agent.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Model.cpp

View.o: View.cpp View.h Geometry.h Utility.h
	$(CC) $(CFLAGS) View.cpp

Controller.o: Controller.cpp Controller.h Model.h View.h Sim_object.h Structure.h Agent.h Agent_factory.h Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Controller.cpp

Sim_object.o: Sim_object.cpp Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Sim_object.cpp

Structure.o: Structure.cpp Structure.h Model.h Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Structure.cpp

Farm.o: Farm.cpp Farm.h Structure.h Sim_object.h Geometry.h
	$(CC) $(CFLAGS) Farm.cpp

Town_Hall.o: Town_Hall.cpp Town_Hall.h Structure.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Town_Hall.cpp

Agent.o: Agent.cpp Agent.h Model.h Moving_object.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent.cpp

Peasant.o: Peasant.cpp Peasant.h Agent.h Moving_object.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Peasant.cpp

Soldier.o: Soldier.cpp Soldier.h Agent.h Moving_object.h Sim_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Soldier.cpp

Moving_object.o: Moving_object.cpp Moving_object.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Moving_object.cpp

Agent_factory.o: Agent_factory.cpp Agent_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Agent_factory.cpp

Structure_factory.o: Structure_factory.cpp Structure_factory.h Geometry.h Utility.h
	$(CC) $(CFLAGS) Structure_factory.cpp

Geometry.o: Geometry.cpp Geometry.h
	$(CC) $(CFLAGS) Geometry.cpp

Utility.o: Utility.cpp Utility.h
	$(CC) $(CFLAGS) Utility.cpp

clean:
	rm -f *.o
real_clean:
	rm -f $(PROG)
	rm -f $(TEST_EXE)
	rm -f *.o
