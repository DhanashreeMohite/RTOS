/*
1. Write a program which will create three threads
2. There will be three global variables in the main program int a,b,c
3. Set the Value of the Global Variables in the mail program
4. Now Write Three functions Thread1Proc(), Thread2Proc(), Thread3Proc(), each of them should have local variables
      int i,j,k
5. Create three thread and each of the thread will execute the corresponding ThreadxProc() eg Thread1 will execute Thread1Proc()
6. What each ThreadProc() should do is print the address of both global and local variables then enter into a infinite loop and print the value of global and local variables
7. Change the values of the global variables in  each thread 
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int a=5,b=6,c=7;

void *Thread1Proc(void *x) 
{
	int i=1,j=11,k=111;
	int y = (int)x;
	a=10,b=100,c=1000;
	printf("thread %d created\n",y);
	printf("thread %d id: %ld\n", y, pthread_self());
	printf( "address of i : %p\n", (void*) &i);
	printf( "address of j : %p\n", (void*) &j);
	printf( "address of k : %p\n", (void*) &k);
	printf( "address of a : %p\n", (void*) &a);
	printf( "address of b : %p\n", (void*) &b);
	printf( "address of c : %p\n", (void*) &c);
	while(1)
	{
		printf( "value of i : %d\n", i);
		printf( "value of j : %d\n", j);
		printf( "value of k : %d\n", k);
		printf( "value of a : %d\n", a);
		printf( "value of b : %d\n", b);
		printf( "value of c : %d\n", c);
	}
}

void *Thread2Proc(void *x)
{
	int i=2,j=22,k=222;
	int y = (int)x;
	a=20,b=200,c=2000;
	printf("thread %d created\n",y);
	printf("thread %d id: %ld\n", y, pthread_self());
	printf( "address of i : %p\n", (void*) &i);
	printf( "address of j : %p\n", (void*) &j);
	printf( "address of k : %p\n", (void*) &k);
	printf( "address of a : %p\n", (void*) &a);
	printf( "address of b : %p\n", (void*) &b);
	printf( "address of c : %p\n", (void*) &c);
	while(1)
	{
		printf( "value of i : %d\n", i);
		printf( "value of j : %d\n", j);
		printf( "value of k : %d\n", k);
		printf( "value of a : %d\n", a);
		printf( "value of b : %d\n", b);
		printf( "value of c : %d\n", c);
	}
}

void *Thread3Proc(void *x)
{
	int i=3,j=33,k=333;
	int y = (int)x;
	a=30,b=300,c=3000;
	printf("thread %d created\n",y);
	printf("thread %d id: %ld\n", y, pthread_self());
	printf( "address of i : %p\n", (void*) &i);
	printf( "address of j : %p\n", (void*) &j);
	printf( "address of k : %p\n", (void*) &k);
	printf( "address of a : %p\n", (void*) &a);
	printf( "address of b : %p\n", (void*) &b);
	printf( "address of c : %p\n", (void*) &c);
	while(1)
	{
		printf( "value of i : %d\n", i);
		printf( "value of j : %d\n", j);
		printf( "value of k : %d\n", k);
		printf( "value of a : %d\n", a);
		printf( "value of b : %d\n", b);
		printf( "value of c : %d\n", c);
		printf("thread %d id: %ld\n", y, pthread_self());
	}
}

int main(void)
{
	pthread_t tid1,tid2,tid3;
	int d=1;	
	pthread_create(&tid1,NULL,Thread1Proc, (void *)(d));	
	pthread_create(&tid2,NULL,Thread2Proc, (void *)(d+1));
	pthread_create(&tid3,NULL,Thread3Proc, (void *)(d+2));
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	pthread_exit(NULL);
	return(0);	
}

/*

observations: 
thread 2 created
thread 2 id: -1218876608
address of i : 0xb759634c
thread 3 created
thread 3 id: -1227269312
address of i : 0xb6d9534c
address of j : 0xb6d95350
address of k : 0xb6d95354
address of a : 0x804a030
address of b : 0x804a034
address of c : 0x804a038
address of j : 0xb7596350
address of k : 0xb7596354
address of a : 0x804a030
address of b : 0x804a034
address of c : 0x804a038
thread 1 created
thread 1 id: -1210483904
address of i : 0xb7d9734c
address of j : 0xb7d97350
address of k : 0xb7d97354
address of a : 0x804a030
address of b : 0x804a034
address of c : 0x804a038

As threads runs in parallel manner, observed printing sequence of variables is different.
  
Since global variables are stored in the data section and it is shared among threads, all threads will have same global varibles and all threads will be able to modify the values of global variables a,b,c.
Now, as all threads have its own stak section in order to run their own execution sequence/code and local variables are allocated on stack, each thread local variable addresses will be different.
Hence threads will be able to perform their own sequence without modifying other thread's parameters with the help of local variables and global varibles can be used for communication among threads.

-->Observations regarding pthread_join and pthread_exit system calls:

If we don't use pthread_join and pthread_exit both in main: then main thread doesn't come to know about existance of other threads and it exits. Due to which other threads will not be able execute its sequence. And program terminates.

If we use only pthread_exit and pthread_join in main: Main thread will come to know about existance of other threads but due to pthread_exit main thread terminates. And other threads enter to zombie state. In order to avoid this, exit(3) will be used to terminate other threads along with main.

Use of pthread_join and pthread_exit both in main: pthread_join allows main thread to wait unless the mentioned thread returns the value. So other threads will enter to infinite loop and main thread will be in waiting state.

*/
