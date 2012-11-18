#===============================================================================
#
#     Filename: Makefile
#  Description:
#
#        Usage: make              (generate executable                      )
#               make clean        (remove objects, executable, prerequisits )
#               make tarball      (generate compressed archive              )
#               make zip          (generate compressed archive              )
#
#      Version: 1.0
#      Created:
#     Revision: ---
#
#       Author:
#      Company:
#        Email:
#
#        Notes: This is a GNU make (gmake) makefile.
#               C   extension   :  c
#               C++ extensions  :  cc cpp C
#               C and C++ sources can be mixed.
#               Prerequisites are generated automatically; makedepend is not
#               needed (see documentation for GNU make Version 3.81, April 2006,
#               section 4.13). The utility sed is used.
#========================================== makefile template version 1.9 ======

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := YES

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# ------------  name of the executable  ----------------------------------------
EXECUTABLE      := main
NETWORKTEST_EXECUTABLE := network_test
EDUCATIONTEST_EXECUTABLE := education_test
MBTUTOR_EXECUTABLE := mbtutor
MBTESTER_EXECUTABLE := mbtester

# ------------  CUDA paths -----------------------------------------------------
CUDA_HOME := /usr/cuda

# ------------  list of all source files  --------------------------------------
SOURCES         := src/Common/Ticker.cpp src/Formatter/Formatter.cpp src/Formatter/FormatterMain.cpp src/Network/Network.cpp src/Topology/TopoGen.cpp src/Topology/TopoGenMain.cpp src/Topology/TopologyDefinition.cpp src/Tutor/TestCase.cpp src/Tutor/Tutor.cpp src/Tutor/TutorMain.cpp

NETWORKTEST_SOURCES := src/network/network.cpp src/network/topology.cpp src/tests/network_test.cpp src/tools/topology_analyze.cpp src/emulation/emulator_frontend.cpp src/emulation/cpu_emulator.cpp src/emulation/emulator.cpp src/tools/topology_factory.cpp

EDUCATIONTEST_SOURCES := src/tests/education_test.cpp src/network/network.cpp src/network/topology.cpp src/education/testcase.cpp src/tools/topology_analyze.cpp src/education/tutor.cpp src/education/cpu_tutor_algorithms.cpp src/education/cpu_tutor_algorithms_alpha.cpp src/education/testcase_loader.cpp src/tools/topology_factory.cpp src/emulation/emulator_frontend.cpp src/emulation/cpu_emulator.cpp src/emulation/emulator.cpp

MBTUTOR_SOURCES := src/education/tutor_main.cpp src/network/network.cpp src/network/topology.cpp src/education/testcase.cpp src/tools/topology_analyze.cpp src/education/tutor.cpp src/education/cpu_tutor_algorithms.cpp src/education/cpu_tutor_algorithms_alpha.cpp src/education/testcase_loader.cpp src/tools/topology_factory.cpp src/emulation/emulator_frontend.cpp src/emulation/cpu_emulator.cpp src/emulation/emulator.cpp src/tools/network_test.cpp

MBTESTER_SOURCES := src/tools/tester_main.cpp src/network/network.cpp src/network/topology.cpp src/education/testcase.cpp src/tools/topology_analyze.cpp src/education/tutor.cpp src/education/cpu_tutor_algorithms.cpp src/education/cpu_tutor_algorithms_alpha.cpp src/education/testcase_loader.cpp src/tools/topology_factory.cpp src/emulation/emulator_frontend.cpp src/emulation/cpu_emulator.cpp src/emulation/emulator.cpp src/tools/network_test.cpp

ifeq (${CUDA}, on)
MBTUTOR_SOURCES += src/education/cuda_tutor_algorithms.cu
EDUCATIONTEST_SOURCES += src/education/cuda_tutor_algorithms.cu
MBTESTER_SOURCES += src/education/cuda_tutor_algorithms.cu
endif

# ------------  compiler  ------------------------------------------------------
CC              := gcc
ifeq (${CUDA}, on)
CXX							:= ${CUDA_HOME}/bin/nvcc
else
CXX             := g++
endif

# ------------  compiler flags  ------------------------------------------------
ifeq (${CUDA}, on)
DEBUG_CFLAGS    := -O0 -g -DCUDA_ENABLED
RELEASE_CFLAGS  := -O3 -DCUDA_ENABLED
else
DEBUG_CFLAGS    := -std=c++0x -Wall -ansi -pedantic -O0 -g
RELEASE_CFLAGS  := -std=c++0x -Wall -ansi -pedantic -O3
endif

# ------------  linker flags  --------------------------------------------------
DEBUG_LDFLAGS    := -g
RELEASE_LDFLAGS  :=

ifeq (YES, ${DEBUG})
  CFLAGS       := ${DEBUG_CFLAGS}
  CXXFLAGS     := ${DEBUG_CXXFLAGS}
  LDFLAGS      := ${DEBUG_LDFLAGS}
else
  CFLAGS       := ${RELEASE_CFLAGS}
  CXXFLAGS     := ${RELEASE_CXXFLAGS}
  LDFLAGS      := ${RELEASE_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
  CFLAGS       := ${CFLAGS}   -pg -O3
  CXXFLAGS     := ${CXXFLAGS} -pg -O3
  LDFLAGS      := ${LDFLAGS}  -pg
endif

# ------------  binary output folder  ------------------------------------------
BIN_DIR = bin

# ------------  additional system include directories  -------------------------
GLOBAL_INC_DIR  =

# ------------  private include directories  -----------------------------------
LOCAL_INC_DIR   = $(HOME)/include src

# ------------  system libraries  (e.g. -lm )  ---------------------------------
SYS_LIBS        = -lm -lglog -lboost_program_options -lboost_system -lboost_filesystem -lboost_regex
ifeq (${CUDA}, on)
		SYS_LIBS += -lcuda -lcudart
endif

# ------------  additional system library directories  -------------------------
GLOBAL_LIB_DIR  =

# ------------  additional system libraries  -----------------------------------
GLOBAL_LIBS     = 

# ------------  private library directories  -----------------------------------
LOCAL_LIB_DIR   = $(HOME)/lib

# ------------  private libraries  (e.g. libxyz.a )  ---------------------------
LOCAL_LIBS      =

# ------------  archive generation ---------------------------------------------
TARBALL_EXCLUDE = *.{o,gz,zip}
ZIP_EXCLUDE     = *.{o,gz,zip}

# ------------  run executable out of this Makefile  (yes/no)  -----------------
# ------------  cmd line parameters for this executable  -----------------------
EXE_START       = no
EXE_CMDLINE     =

#===============================================================================
# The following statements usually need not to be changed
#===============================================================================

C_SOURCES       = $(filter     %.c, $(SOURCES))
CPP_SOURCES     = $(filter-out %.c, $(SOURCES))
ALL_INC_DIR     = $(addprefix -I, $(LOCAL_INC_DIR) $(GLOBAL_INC_DIR))
ALL_LIB_DIR     = $(addprefix -L, $(LOCAL_LIB_DIR) $(GLOBAL_LIB_DIR))
GLOBAL_LIBSS    = $(addprefix $(GLOBAL_LIB_DIR)/, $(GLOBAL_LIBS))
LOCAL_LIBSS     = $(addprefix $(LOCAL_LIB_DIR)/, $(LOCAL_LIBS))
ALL_CFLAGS      = $(CFLAGS) $(ALL_INC_DIR)
ALL_LFLAGS      = $(LDFLAGS) $(ALL_LIB_DIR)
BASENAMES       = $(basename $(SOURCES))

# ------------  generate the names of the object files  ------------------------
OBJECTS         = $(addsuffix .o,$(BASENAMES))
NETWORKTEST_OBJECTS = $(addsuffix .o,$(basename $(NETWORKTEST_SOURCES)))
EDUCATIONTEST_OBJECTS = $(addsuffix .o,$(basename $(EDUCATIONTEST_SOURCES)))
MBTUTOR_OBJECTS = $(addsuffix .o,$(basename $(MBTUTOR_SOURCES)))
MBTESTER_OBJECTS = $(addsuffix .o,$(basename $(MBTESTER_SOURCES)))

# ------------  generate the names of the hidden prerequisite files  -----------
PREREQUISITES   = $(addprefix .,$(addsuffix .d,$(BASENAMES)))

# ------------  make the executable (the default goal)  ------------------------
$(EXECUTABLE):	$(OBJECTS)
ifeq ($(strip $(CPP_SOURCES)),)
								$(CC)  $(ALL_LFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS)
else
								$(CXX) $(ALL_LFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS)
endif
ifeq ($(EXE_START),yes)
								./$(EXECUTABLE) $(EXE_CMDLINE)
endif

$(NETWORKTEST_EXECUTABLE): $(NETWORKTEST_OBJECTS)
								$(CXX) $(ALL_LFLAGS) -o $(BIN_DIR)/$(NETWORKTEST_EXECUTABLE) $(NETWORKTEST_OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS)

$(EDUCATIONTEST_EXECUTABLE): $(EDUCATIONTEST_OBJECTS)
								$(CXX) $(ALL_LFLAGS) -o $(BIN_DIR)/$(EDUCATIONTEST_EXECUTABLE) $(EDUCATIONTEST_OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS)

$(MBTUTOR_EXECUTABLE): $(MBTUTOR_OBJECTS)
								$(CXX) $(ALL_LFLAGS) -o $(BIN_DIR)/$(MBTUTOR_EXECUTABLE) $(MBTUTOR_OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS)

$(MBTESTER_EXECUTABLE): $(MBTESTER_OBJECTS)
								$(CXX) $(ALL_LFLAGS) -o $(BIN_DIR)/$(MBTESTER_EXECUTABLE) $(MBTESTER_OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS)

# ------------  include the automatically generated prerequisites  -------------
# ------------  if target is not clean, tarball or zip             -------------
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),tarball)
ifneq ($(MAKECMDGOALS),zip)
-include         $(PREREQUISITES)
endif
endif
endif

# ------------  make the objects  ----------------------------------------------
%.o:		%.c
				$(CC)  -c $(ALL_CFLAGS) $<

%.o:		%.cc
				$(CXX) -c $(ALL_CFLAGS) $<

%.o:		%.cpp
				$(CXX) -c $(ALL_CFLAGS) -o $(addsuffix .o,$(basename $<)) $<

%.o:		%.cu
				$(CXX) -c $(ALL_CFLAGS) -o $(addsuffix .o,$(basename $<)) $<

%.o:		%.C
				$(CXX) -c $(ALL_CFLAGS) $<

# ------------  make the prerequisites  ----------------------------------------
#
.%.d:   %.c
				@$(make-prerequisite-c)

.%.d:		%.cc
				@$(make-prerequisite-cplusplus)

.%.d:		%.cpp
				@$(make-prerequisite-cplusplus)

.%.d:		%.cu
				@$(make-prerequisite-cplusplus)

.%.d:		%.C
				@$(make-prerequisite-cplusplus)

#  canned command sequences
#  echoing of the sed command is suppressed by the leading @

define	make-prerequisite-c
				@$(CC)   -MM $(ALL_CFLAGS) $< > $@.$$$$;            \
				sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
				rm -f $@.$$$$;
endef

define	make-prerequisite-cplusplus
				@$(CXX)  -MM $(ALL_CFLAGS) $< > $@.$$$$;            \
				sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
				rm -f $@.$$$$;
endef

# ------------  remove generated files  ----------------------------------------
# ------------  remove hidden backup files  ------------------------------------
clean:
								-rm  --force  $(EXECUTABLE) $(OBJECTS) $(PREREQUISITES) *~
								-rm  --force  $(NETWORKTEST_EXECUTABLE) $(NETWORKTEST_OBJECTS) $(PREREQUISITES) *~
								-rm  --force  $(EDUCATIONTEST_EXECUTABLE) $(EDUCATIONTEST_OBJECTS) $(PREREQUISITES) *~
								-rm  --force  $(MBTUTOR_EXECUTABLE) $(MBTUTOR_OBJECTS) $(PREREQUISITES) *~
								-rm  --force  $(MBTESTER_EXECUTABLE) $(MBTESTER_OBJECTS) $(PREREQUISITES) *~

# ------------ tarball generation ----------------------------------------------
tarball:
					@lokaldir=`pwd`; lokaldir=$${lokaldir##*/}; \
					rm --force $$lokaldir.tar.gz;               \
					tar --exclude=$(TARBALL_EXCLUDE)            \
					    --create                                \
					    --gzip                                  \
					    --verbose                               \
					    --file  $$lokaldir.tar.gz *

# ------------ zip -------------------------------------------------------------
zip:
					@lokaldir=`pwd`; lokaldir=$${lokaldir##*/}; \
					zip -r  $$lokaldir.zip * -x $(ZIP_EXCLUDE)

.PHONY: clean tarball zip

# ==============================================================================
# vim: set tabstop=2: set shiftwidth=2:
