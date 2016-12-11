##########################
# Makefile for testcase  #
##########################

TARGET = testcase.a

CC = g++ -g
AR = ar rcs    # create, modify, and extract from archives

INCDIR = -I inc/ -I ../Platform/inc/ -I ../Blocks/inc/
SRCDIR = src/
BUILDDIR = build/

OBJS = $(BUILDDIR)test_main.o \
	   $(BUILDDIR)test_whiteGaussChannel.o
	
$(BUILDDIR)$(TARGET) : $(OBJS)
	$(AR) $@ $(OBJS)
	
$(BUILDDIR)test_main.o : $(SRCDIR)test_main.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)test_whiteGaussChannel.o : $(SRCDIR)test_whiteGaussChannel.cpp
	$(CC) $(INCDIR) -c $^ -o $@
			
clean :
	rm -f $(OBJS) $(BUILDDIR)$(TARGET) -d $(BUILDDIR)	