TARGET   := wof_example

ARCHITECTURE=`/bin/uname -m`

# Choose a matching distribution below:
DISTRO :=../binaries/lib_ubuntu22.04_${ARCHITECTURE}

# Will be supported soon:
#DISTRO :=../lib_APPLE

CXX      := g++
ifeq ($(DISTRO),../lib_APPLE)
#   Will be supported soon
else
	CXXFLAGS := -g -Wextra -Wall -Wno-long-long -pedantic-errors -I../include
	LNKFLAGS  :=  -L${DISTRO} 
	LIBS     := -lwofPURE -lpthread -Wl,-rpath=../${DISTRO} 
endif




EXT      := cpp
BUILDDIR := build

override BUILDDIR := $(strip $(BUILDDIR))
SOURCES  := $(wildcard *.$(EXT))
OBJECTS  := $(patsubst %.$(EXT), $(BUILDDIR)/%.o, $(SOURCES))
DEPS     := $(patsubst %.$(EXT), $(BUILDDIR)/%.dep, $(SOURCES))

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS) $(DEPS)
	$(CXX) $(CXXFLAGS) $(LNKFLAGS) -o $(BUILDDIR)/$(TARGET) $(OBJECTS) $(LIBS)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif

$(OBJECTS): $(BUILDDIR)/%.o: %.$(EXT) $(BUILDDIR)/%.dep $(BUILDDIR)/.tag
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEPS): $(BUILDDIR)/%.dep: %.$(EXT) $(BUILDDIR)/.tag
	mkdir -p $(dir $(@))
	$(CXX) $(CXXFLAGS) -MM $< -MT $@ -MT $(<:.$(EXT)=.o) -o $@

%.tag:
	mkdir -p $(dir $(@))
	touch $@

.PHONY: clean
clean:
	pwd
	echo $(LIBS)
	rm -rf $(BUILDDIR) $(TARGET) *.ps *.list out0.txt out1.txt out2.txt


