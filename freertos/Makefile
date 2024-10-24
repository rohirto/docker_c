######## Build options ########

verbose = 0

######## Build setup ########

# SRCROOT should always be the current directory
SRCROOT         = $(CURDIR)

# .o directory
ODIR            = obj

# Source VPATHS
VPATH           += $(SRCROOT)/Source
VPATH	        += $(SRCROOT)/Source/portable/MemMang
VPATH	        += $(SRCROOT)/Source/portable/GCC/POSIX
VPATH           += $(SRCROOT)/Demo/Common
VPATH			+= $(SRCROOT)/Project/FileIO
VPATH			+= $(SRCROOT)/Project
VPATH			+= $(SRCROOT)/Project/example_queue
VPATH			+= $(SRCROOT)/Project/example_timers
VPATH			+= $(SRCROOT)/Project/example_semaphore
VPATH			+= $(SRCROOT)/Project/example_mutex
VPATH			+= $(SRCROOT)/Project/example_recursive_mutex
VPATH			+= $(SRCROOT)/Project/example_static_memory_allocation
VPATH			+= $(SRCROOT)/Project/example_cpp_task

# FreeRTOS Objects
C_FILES			+= croutine.c
C_FILES			+= event_groups.c
C_FILES			+= list.c
C_FILES			+= queue.c
C_FILES			+= tasks.c
C_FILES			+= timers.c

# portable Objects
C_FILES			+= heap_3.c
C_FILES			+= port.c

# Demo Objects
#C_FILES			+= Minimal/blocktim.c
#C_FILES			+= Minimal/countsem.c
#C_FILES			+= Minimal/GenQTest.c
#C_FILES			+= Minimal/QPeek.c
#C_FILES			+= Minimal/recmutex.c
#C_FILES			+= Full/BlockQ.c
#C_FILES			+= Full/death.c
#C_FILES			+= Full/dynamic.c
#C_FILES			+= Full/flop.c
#C_FILES			+= Full/integer.c
#C_FILES			+= Full/PollQ.c
#C_FILES			+= Full/semtest.c
#C_FILES			+= Full/print.c
#
#C_FILES			+= Minimal/AbortDelay.c
#C_FILES			+= Minimal/EventGroupsDemo.c
#C_FILES			+= Minimal/IntSemTest.c
#C_FILES			+= Minimal/QueueSet.c
#C_FILES			+= Minimal/QueueSetPolling.c
#C_FILES			+= Minimal/QueueOverwrite.c
#C_FILES			+= Minimal/TaskNotify.c
#C_FILES			+= Minimal/TimerDemo.c
#
# Main Object
#C_FILES			+= queue_rxtx.c
# C_FILES			+= main.c
C_FILES			+= example_queue/example_queue.c
C_FILES			+= example_timers/example_timers.c
C_FILES			+= example_semaphore/example_semaphore.c
C_FILES			+= example_mutex/example_mutex.c
C_FILES			+= example_task_notify/example_task_notify.c
C_FILES			+= example_event_group/example_event_group.c
C_FILES			+= example_recursive_mutex/example_recursive_mutex.c
C_FILES			+= example_static_memory_allocation/example_static_memory_allocation.c

CXX_FILES			+= example_cpp_task/example_cpp_task.cpp  # Added C++ source file
CXX_FILES			+= main.cpp  # Added C++ source file

# Include Paths
INCLUDES        += -I$(SRCROOT)/Source/include
INCLUDES        += -I$(SRCROOT)/Source/portable/GCC/POSIX/
INCLUDES        += -I$(SRCROOT)/Project
INCLUDES        += -I/usr/include/x86_64-linux-gnu/
INCLUDES		+= -I$(SRCROOT)/Project/example_queue
INCLUDES		+= -I$(SRCROOT)/Project/example_timers
INCLUDES		+= -I$(SRCROOT)/Project/example_semaphore
INCLUDES		+= -I$(SRCROOT)/Project/example_mutex
INCLUDES		+= -I$(SRCROOT)/Project/example_task_notify
INCLUDES		+= -I$(SRCROOT)/Project/example_event_group
INCLUDES		+= -I$(SRCROOT)/Project/example_recursive_mutex
INCLUDES		+= -I$(SRCROOT)/Project/example_static_memory_allocation
INCLUDES		+= -I$(SRCROOT)/Project/example_cpp_task

# Generate OBJS names
OBJS = $(patsubst %.c,%.o,$(C_FILES))
OBJS += $(patsubst %.cpp,%.o,$(CXX_FILES))  # Handle C++ files

######## C/C++ Flags ########

# C Compiler Warnings
CWARNS += -W
CWARNS += -Wall
CWARNS += -Wextra
CWARNS += -Wformat
CWARNS += -Wmissing-braces
CWARNS += -Wno-cast-align
CWARNS += -Wparentheses
CWARNS += -Wshadow
CWARNS += -Wno-sign-compare
CWARNS += -Wswitch
CWARNS += -Wuninitialized
CWARNS += -Wunknown-pragmas
CWARNS += -Wunused-function
CWARNS += -Wunused-label
CWARNS += -Wunused-parameter
CWARNS += -Wunused-value
CWARNS += -Wunused-variable
CWARNS += -Wmissing-prototypes

# C Compiler Flags
CFLAGS += -m32
CFLAGS += -DDEBUG=1
CFLAGS += -g -UUSE_STDIO -D__GCC_POSIX__=1
ifneq ($(shell uname), Darwin)
CFLAGS += -pthread
endif
CFLAGS += -DMAX_NUMBER_OF_TASKS=300

CFLAGS += $(INCLUDES) $(CWARNS) -O2

# C++ Compiler Flags
CXXFLAGS += -std=c++11 -fno-exceptions -fno-rtti  # Added here
CXXFLAGS += $(CFLAGS)  # Inherit CFLAGS

######## Makefile targets ########

# Fix to place .o files in ODIR
_OBJS = $(patsubst %,$(ODIR)/%,$(OBJS))

# Compile C files
$(ODIR)/%.o: %.c
	@mkdir -p $(dir $@)
ifeq ($(verbose),1)
	@echo ">> Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<
else
	@echo ">> Compiling $(notdir $<)"
	@$(CC) $(CFLAGS) -c -o $@ $<
endif

# Compile C++ files
$(ODIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
ifeq ($(verbose),1)
	@echo ">> Compiling C++ file: $<"
	$(CXX) $(CXXFLAGS) -c -o $@ $<
else
	@echo ">> Compiling C++ file: $(notdir $<)"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
endif

# Linking C and C++ objects
FreeRTOS-Sim: $(_OBJS)
	@echo ">> Linking $@..."
ifeq ($(verbose),1)
	$(CXX) $(CXXFLAGS) $^ $(LINKFLAGS) $(LIBS) -lstdc++ -o $@
else
	@$(CXX) $(CXXFLAGS) $^ $(LINKFLAGS) $(LIBS) -lstdc++ -o $@
endif

	@echo "-------------------------"
	@echo "BUILD COMPLETE: $@"
	@echo "-------------------------"

.PHONY : clean
clean:
	@-rm -rf $(ODIR) FreeRTOS-Sim
	@echo "--------------"
	@echo "CLEAN COMPLETE"
	@echo "--------------"


.PHONY: valgrind
valgrind: FreeRTOS-Sim
	valgrind.bin --tool=memcheck --leak-check=full --show-reachable=yes --track-fds=yes ./FreeRTOS-Sim
