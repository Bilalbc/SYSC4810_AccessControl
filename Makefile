main: main.o passwordHashing.o Enrollment.o Login.o accessControl.o
	gcc -o main main.o Enrollment.o passwordHashing.o Login.o accessControl.o -lssl -lcrypto

passwordHashing.o: passwordHashing.c passwordHashing.h
	gcc -c passwordHashing.c -o passwordHashing.o -lssl -lcrypto

main.o: main.c passwordHashing.h Enrollment.h
	gcc -c main.c -o main.o

Enrollment.o: Enrollment.c Enrollment.h
	gcc -c Enrollment.c -o Enrollment.o

Login.o: Login.c Login.h
	gcc -c Login.c -o Login.o

accessControl.o: accessControl.c accessControl.h
	gcc -c accessControl.c accessControl.o