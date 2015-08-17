CXX = $(shell root-config --cxx)
LD = $(shell root-config --ld)

INC = $(shell pwd)
ROOTINC = ~/Downloads/root

CPPFLAGS := $(shell root-config --cflags) -I$(INC)/include -I$(ROOTINC)/math/mathmore/inc
LDFLAGS := $(shell root-config --glibs) $(STDLIBDIR) -lMathMore

# Debugging Flag
#CPPFLAGS += -g


# make plot for time resolution
TARGET2 = makePlot
SRC2 = makePlot.cc
OBJ2 = $(SRC2:.cc=.o)

# make plot for calibrated resolution
TARGET1 = makePlot2
SRC1 = makePlot2.cc
OBJ1 = $(SRC1:.cc=.o)

# Resolution-Number Profile analysis 
TARGET3 = weighting
SRC3 = weighting.cc
OBJ3 = $(SRC3:.cc=.o)

all : $(TARGET1) $(TARGET2) $(TARGET2)


$(TARGET1) : $(OBJ1)
	$(LD) $(CPPFLAGS) -o $(TARGET1) $(OBJ1) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

$(TARGET2) : $(OBJ2)
	$(LD) $(CPPFLAGS) -o $(TARGET2) $(OBJ2) $(LDFLAGS)
	@echo $@
	@echo $<
	@echo $^

$(TARGET3) : $(OBJ3)
	$(LD) $(CPPFLAGS) -o $(TARGET3) $(OBJ3) $(LDFLAG3)
	@echo $@
	@echo $<
	@echo $^

%.o : %.cc
	$(CXX) $(CPPFLAGS) -o $@ -c $<
	@echo $@
	@echo $<

clean :
	rm -f *.o src/*.o $(TARGET1) $(TARGET2) $(TARGET3) *~
