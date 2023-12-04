main: main.o PasswordHashing.o Enrollment.o Login.o AccessControl.o
	gcc -o main main.o Enrollment.o PasswordHashing.o Login.o AccessControl.o -lssl -lcrypto

PasswordHashing.o: PasswordHashing.c PasswordHashing.h
	gcc -c PasswordHashing.c -o PasswordHashing.o -lssl -lcrypto

main.o: main.c PasswordHashing.h Enrollment.h
	gcc -c main.c -o main.o

Enrollment.o: Enrollment.c Enrollment.h
	gcc -c Enrollment.c -o Enrollment.o

Login.o: Login.c Login.h
	gcc -c Login.c -o Login.o

AccessControl.o: AccessControl.c AccessControl.h
	gcc -c AccessControl.c AccessControl.o