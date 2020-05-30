##=====================================================
CC = gcc
CXX = g++
AR = ar
RM = rm
ECHO = echo
MV = mv
MAKE = make
CD = cd
MKDIR = mkdir
CP = cp
LN = ln -f

EXENM = ../bin/client

LIBDIR = ~/lib
LIBPLA = 
PLGDIR = 
BINDIR = ~/bin
MYSQL_LIB = /usr/lib/x86_64-linux-gnu

##======================================================
COSFLAGS = 
ROSFLAGS = 
##======================================================
USERLIB = -lpthread
USERINC = -I./ -I/home/chenyc/inc -I/home/chenyc/inc/comm -I/home/chenyc/inc/db -I../inc -I../../inc -I./inc
USERLD = -L./ -L$(LIBDIR) -L$(MYSQL_LIB)

##======================================================
CFLAGS = -w -g -rdynamic $(COSFLAGS) $(OFLAG)
CXXFLAGS = -w -g -rdynamic $(COSFLAGS) $(OFLAG)
DLLFLAGS = -g
ARFLAGS = $(AROSFLAGS) -ruc
##======================================================
INCPATH = -I/usr/include ${USERINC} 
LDPATH = ${USERLD}
LDFLAGS = -L/usr/lib
LIBS = $(USERLIB) -lm -lc -lpthread -lm 
##=====================================================
LIB_OBJS = 
LIB_TARG = 
##=====================================================
BIN_OBJS = client.o
BIN_TARG = $(EXENM)
##=====================================================

##=====================================================
$(BIN_TARG): $(LIB_TARG) $(BIN_OBJS) $(DLL_TARG)
	@${ECHO} "Linking   [bin  ] file:[$@] ..."
	@$(CXX) $(CXXFLAGS) $(INCPATH) $(LDPATH) $(LDFLAGS) $(LDLIBS) $(DLLFLAGS) $(BIN_OBJS) $(LIBS) -o $@

##=====================================================
$(LIB_TARG): $(LIB_OBJS)
	@${ECHO} "Archiving [lib  ] file:[$@] ..."
	@$(AR) $(ARFLAGS) $@ $(LIB_OBJS)
##=====================================================
.SUFFIXES: .c .cpp .C .o
.c.o	:
	@$(ECHO) "Compiling [c    ] file:[$@] ..."
	@$(CC) $(CFLAGS) $(INCPATH) -g -O0 -c $*.c -o $@
.cpp.o	:
	@$(ECHO) "Compiling [cpp  ] file:[$@] ..."
	@$(CXX) $(CXXFLAGS) $(INCPATH) -c $*.cpp -o $@

all:
bin		: $(BIN_TARG)
lib		: $(LIB_TARG)

clean:
	@$(RM) -fr $(LIB_TARG) $(LIB_OBJS) $(BIN_TARG)$(EXENM) $(BIN_OBJS)
##======================================================

