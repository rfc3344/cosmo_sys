##########################
# Makefile for platform  #
##########################

TARGET = platform.a

CC = g++ -g
AR = ar rcs    # create, modify, and extract from archives

INCDIR = -I inc/
SRCDIR = src/
BUILDDIR = build/

OBJS = $(BUILDDIR)avltree.o   \
	   $(BUILDDIR)BlockBase.o \
	   $(BUILDDIR)Complex.o   \
	   $(BUILDDIR)Connector.o \
	   $(BUILDDIR)ErrorHandling.o \
	   $(BUILDDIR)list.o \
	   $(BUILDDIR)MatrixClass.o \
	   $(BUILDDIR)MsgBase.o \
	   $(BUILDDIR)Runable.o \
	   $(BUILDDIR)Star.o \
	   $(BUILDDIR)SystemScheduler.o
	
$(BUILDDIR)$(TARGET) : $(OBJS)
	$(AR) $@ $(OBJS)

$(BUILDDIR)avltree.o : $(SRCDIR)avltree.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)BlockBase.o : $(SRCDIR)BlockBase.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)Complex.o : $(SRCDIR)Complex.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)Connector.o : $(SRCDIR)Connector.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)ErrorHandling.o : $(SRCDIR)ErrorHandling.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)list.o : $(SRCDIR)list.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)MatrixClass.o : $(SRCDIR)MatrixClass.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)MsgBase.o : $(SRCDIR)MsgBase.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)Runable.o : $(SRCDIR)Runable.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)Star.o : $(SRCDIR)Star.cpp
	$(CC) $(INCDIR) -c $^ -o $@

$(BUILDDIR)SystemScheduler.o : $(SRCDIR)SystemScheduler.cpp
	$(CC) $(INCDIR) -c $^ -o $@

clean :
	rm -f $(OBJS) $(BUILDDIR)$(TARGET) -d $(BUILDDIR)
	
		