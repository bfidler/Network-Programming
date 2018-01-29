.c.o:
	gcc -c -Wall $<

tests: test2 test3 test4 test5

OBJT5= test5.o l5.o l1.o

test5: ${OBJT5}
	gcc -o test5 ${OBJT5}

OBJT4= test4.o l4.o l1.o

test4: ${OBJT4}
	gcc -o test4 ${OBJT4}


OBJT3= test3.o l3.o l1.o

test3: ${OBJT3}
	gcc -o test3 ${OBJT3}

OBJT2= test2.o l2.o l1.o

test2: ${OBJT2}
	gcc -o test2 ${OBJT2}

clean:
	rm *.o test5 test4 test3 test2 hw1test *~
