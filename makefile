#This is a Makefile for HW2
CCC = g++
CXXFLAGS = -Wall -g -std=c++11 -lpthread
CXXLINK = $(CCC)
OBJS = Account.o ATM.o Bank.o common.o main.o PrintSafe.o

RM = rm -f

#Default target (usually "all")
all: Bank

#Creating the executables
Bank: $(OBJS)
	$(CXXLINK) -o Bank $(OBJS) $(CXXFLAGS)

#Creating object files using default rules
Account.o: Account.cpp Account.h common.h PrintSafe.h Bank.h
ATM.o: ATM.cpp ATM.h common.h PrintSafe.h Bank.h Account.h
Bank.o: Bank.cpp Bank.h Account.h common.h PrintSafe.h
common.o: common.cpp common.h PrintSafe.h
main.o: main.cpp common.h PrintSafe.h ATM.h Bank.h Account.h
PrintSafe.o: PrintSafe.cpp PrintSafe.h

#Cleaning old files before new make
clean:
	$(RM) Bank *.o *.bak *~ "#"* core
