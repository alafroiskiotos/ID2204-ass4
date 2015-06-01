CXX=g++
CXXFLAGS=-O1 -Wall
LIBS=-lgecodeflatzinc -lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel \
	-lgecodeset -lgecodefloat -lgecodeint -lgecodekernel -lgecodesupport
RM=rm
SRCS0=life.cpp
SRCS1=square.cpp
OBJS0=$(subst .cpp,.o,$(SRCS0))
OBJS1=$(subst .cpp,.o,$(SRCS1))
EXEC=life square

all: $(EXEC)

life: $(OBJS0)
	$(CXX) $(CXXFLAGS) -o life $(OBJS0) $(LIBS)

life.o: life.cpp

square: $(OBJS1)
	$(CXX) $(CXXFLAGS) -o square $(OBJS1) $(LIBS)

square.o: square.cpp

clean:
	$(RM) $(OBJS0) $(OBJS1)

dist-clean: clean
	$(RM) $(EXEC)
