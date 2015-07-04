# global directory layout
ROOT       := ../..
BIN        := $(ROOT)/bin
INC        := $(ROOT)/inc
LIB        := $(ROOT)/lib
SRC        := $(ROOT)/src

# project directories and files
PROJECT    := libjcolor

PROJECTDIR := .
OBJDIR     := obj
OUTPUTDIR   = $(BIN)/$(CONFIG)

CFILES     := juliaColor.c
CCFILES    :=
DEPFILES   := $(INC)/juliaColor.h
OBJFILES   := $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(CFILES))) \
              $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(CCFILES)))
TARGETFILE  = $(OUTPUTDIR)/$(PROJECT).so

# compiler setup
CC         := gcc -fPIC
CFLAGS      = -DUNIX -std=c99 $(ARCH) $(DEBUG) $(INCLUDES)
CXX        := g++ -fPIC
CXXFLAGS    = -DUNIX $(ARCH) $(DEBUG) $(INCLUDES)

LINK       := g++ -fPIC
LDFLAGS     = -DUNIX -shared
#LDFLAGS     = -DUNIX -shared $(LIBRARIES)

AMD64 = $(shell uname -m | grep 64)
ifeq "$(strip $(AMD64))" ""
    ARCH   := -m32
else
    ARCH   := -m64
endif

ifeq ($(dbg),1)
    DEBUG  := -g -D_DEBUG
    CONFIG := debug
else
    DEBUG  := -O2 -fno-strict-aliasing
    CONFIG := release
endif

INCLUDES   := -I$(PROJECTDIR) -I$(INC)
LIBRARIES   = -L$(LIB) -L$(OUTPUTDIR) -lm

ifeq ($(omp),1)
    CFLAGS += -fopenmp
  CXXFLAGS += -fopenmp
      LINK += -fopenmp
endif

# misc.
ifeq ($(quiet), 1)
    QUIET  := @
else
    QUIET  :=
endif

# build rules
.PHONY: all
all: build

.PHONY: build
build: $(TARGETFILE)

$(OBJDIR)/%.o: %.c $(DEPFILES)
	$(QUIET)$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: %.cpp $(DEPFILES)
	$(QUIET)$(CXX) $(CXXFLAGS) -o $@ -c $<

$(TARGETFILE): $(OBJFILES) | $(OUTPUTDIR)
	$(QUIET)$(LINK) $(LDFLAGS) -o $@ $+ $(LIBRARIES)

$(OUTPUTDIR):
	mkdir -p $(BIN)/release
	mkdir -p $(BIN)/debug
	mkdir -p $(LIB)

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: run
run: build
	$(QUIET)$(TARGETFILE)

.PHONY: tidy
tidy:
	$(QUIET)find . | egrep "#" | xargs rm -f
	$(QUIET)find . | egrep "\~" | xargs rm -f

.PHONY: clean
clean: tidy
	$(QUIET)rm -f $(OBJFILES)
	$(QUIET)rm -f $(TARGETFILE)

.PHONY: clobber
clobber: clean
	$(QUIET)rm -rf $(OBJDIR)
