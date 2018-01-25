# Makefile for HW1 test programs

# Here is a rule for building .o files from .c files
# the -Wall option tells gcc to print all warning messages

.c.o:
	gcc -c -Wall $<

# if you just type "make", this is the default target
#  (since it is the first one defined). You can also 
# explicitly tell make to do this by typing "make all"

#all: hw1test l3test
all: l3test

# here is the rule for building hw1test

#hw1test: hw1test.o l2.o l3.o l4.o l5.o
#	gcc -o hw1test hw1test.o l2.o l3.o l4.o l5.o

# here is the rule for building l3test

l3test: l3test.o l2.o l3.o 
	gcc -o l3test l3test.o l2.o l3.o 

