CXXFLAGS =	-std=c++0x -O2 -g -Wall -fmessage-length=0 -Wreorder -Wwrite-strings -Wsign-compare

OBJS =		GSAT.o ArrayFiller.o CFormula.o

LIBS =

TARGET =	GSAT

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
