PKGCONFIG=pkg-config
OS=Linux
CXXFLAGS=-std=c++11 -g -Wall -pedantic-errors `$(PKGCONFIG) --cflags glew glfw3`
WINFLAGS=-Wl,-subsystem,windows -static-libgcc -static-libstdc++ -DWINDOWS
LINUXFLAGS=-DLINUX
LIBS=`$(PKGCONFIG) --libs glew glfw3`
WINLIBS=
LINUXLIBS=-Wl,-rpath -Wl,./lib
SRCPATH=src/
OBJPATH=obj/
SRCS:=$(wildcard $(SRCPATH)*.cpp)
OBJS:=$(patsubst $(SRCPATH)%.cpp,$(OBJPATH)%.o,$(SRCS))
DEPS:=$(patsubst $(SRCPATH)%.cpp,$(OBJPATH)%.depend,$(SRCS))
APP=duke-spookem-3d
ifeq ($(OS), Linux)
OSFLAGS=$(LINUXFLAGS)
OSLIBS=$(LINUXLIBS)
else ifeq ($(OS), Windows)
OSFLAGS=$(WINFLAGS)
OSLIBS=$(WINLIBS)
CXX=i686-w64-mingw32-g++
PKGCONFIG=i686-w64-mingw32-pkg-config
APP := $(APP).exe
else
$(error Unknown OS selected for output)
endif

$(OBJPATH)%.depend: $(SRCPATH)%.cpp
	@echo -e "Building dependecies for \e[1;35m$<\e[0m..."
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $(OSFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)%.o: $(SRCPATH)%.cpp
	@echo -e "Building \e[1;35m$<\e[0m..."
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(OSFLAGS)

all: $(DEPS) $(SRCS) $(OBJS)
	@echo -e "\e[0;33mBuilding main application for \e[1;35m$(OS)\e[0;33m systems...\e[0m"
	$(CXX) -o $(APP) $(OBJS) $(CXXFLAGS) $(OSFLAGS) $(LIBS) $(OSLIBS)
	@echo -e "\e[0;32mBuild completed.\e[0m"

release: all
	cp $(APP) bin

-include $(DEPS)
$(shell   mkdir -p obj)
$(shell   mkdir -p bin)

.PHONY: clean
clean:
	@echo -e "\e[0;31mCleaning up...\e[0m"
	@$(RM) $(OBJPATH)*
	@$(RM) $(APP)
	@$(RM) bin/$(APP)

