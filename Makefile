TARGET	= volrender
SOURCES	= $(wildcard *.cpp)
HEADERS	= $(wildcard *.h)
OBJECTS	= $(patsubst %.cpp,%.o,$(SOURCES))
LIBOVR  = /usr/local/LibOVR
CXXFLAGS        = --std=c++0x -g -Wall -DX11 -I$(LIBOVR)/Include -Dnullptr=NULL -D_DEBUG
LDLIBS  = -lGL -lglfw3 -lXi -lXinerama -lXrandr -lXxf86vm -lX11 -lrt -lpthread -ludev -lm -L$(LIBOVR)/Lib/Linux/Debug/i386 -lovr

.PHONY: clean

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(TARGET).dep: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > $@

clean:
	-$(RM) $(TARGET) *.o *~ .*~ a.out core

-include $(TARGET).dep
