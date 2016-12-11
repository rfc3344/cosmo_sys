 
##########################
# Makefile for Cosmo_sys #
##########################

TARGET = cosmo

CPPFLAGS = -I Platform/inc/ -I TestCase/inc/ -I Blocks/inc/

BUILDDIR = build/
OBJS = $(BUILDDIR)cosmo_main.o

Target :  all

all : buildfolder $(OBJS)
	(cd Platform; test -d $(BUILDDIR) || mkdir -p $(BUILDDIR); make -f platform.mk)
	(cd TestCase; test -d $(BUILDDIR) || mkdir -p $(BUILDDIR); make -f testcase.mk)
	(cd Blocks;   test -d $(BUILDDIR) || mkdir -p $(BUILDDIR); make -f blocks.mk)
	g++ -g -o $(TARGET) $(OBJS) TestCase/$(BUILDDIR)testcase.a Platform/$(BUILDDIR)platform.a Blocks/$(BUILDDIR)blocks.a

buildfolder : 
	test -d $(BUILDDIR) || mkdir -p $(BUILDDIR);

$(BUILDDIR)cosmo_main.o : cosmo_main.cpp
	g++ -g -o $(BUILDDIR)cosmo_main.o -c cosmo_main.cpp

clean :
	(cd Platform; make -f platform.mk clean)
	(cd TestCase; make -f testcase.mk clean)
	(cd Blocks;   make -f blocks.mk   clean)
	rm -f $(TARGET) $(OBJS) -d $(BUILDDIR)