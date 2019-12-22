all: forksrv.h forksrv.c hello.c 
	clang -g -Og -o forksrv forksrv.c 
	clang -g -Og -o hello hello.c 

clean: 
	rm forksrv hello

