##########################
# Makefile for blocks    #
##########################

TARGET = blocks.a

CC = g++ -g
AR = ar rcs    # create, modify, and extract from archives

INCDIR = -I inc/ -I ../Platform/inc/
SRCDIR = src/
BUILDDIR = build/

OBJS = $(BUILDDIR)RandBitGen.o \
       $(BUILDDIR)Gauss_Rnd.o  \
       $(BUILDDIR)Uniform_Rnd.o
       	
$(BUILDDIR)$(TARGET) : $(OBJS)
	$(AR) $@ $(OBJS)

$(BUILDDIR)RandBitGen.o : $(SRCDIR)RandBitGen.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)Gauss_Rnd.o : $(SRCDIR)Gauss_Rnd.cpp
	$(CC) $(INCDIR) -c $^ -o $@	
	
$(BUILDDIR)Uniform_Rnd.o : $(SRCDIR)Uniform_Rnd.cpp
	$(CC) $(INCDIR) -c $^ -o $@	
	
clean :
	rm -f $(OBJS) $(BUILDDIR)$(TARGET) -d $(BUILDDIR)
	
		