# SYSC4810_AccessControl
Access control and user authentication program for SYSC 4810 - Network and Software Security

Access control is implemented through Role Based Access Control (RBAC) to define distinct roles in the system and associate permissions with users. 
Privileges are assigned based on an Access Control Matrix, implemented in AccessControl.c and AccessControl.h.

Program utilizes SHA 256 hashing algorithm to encrypt user passwords, and authenticate users on login.

in order to execute the progam, compile the code using the MakeFile by entering the following command in the directory with the makefile and other source code:

```
make
```
Upon compiling the program, an executable `main` will be created. 

Run the program and follow instructions to first create a user, and then execute again and select the option to login. 
