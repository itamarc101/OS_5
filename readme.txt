Itamar Cohen 209133826
Naor Ladani 318664190

Operating System Course Assignment 5
Computer Science Ariel University

Description about the assignment:

Practice and demonstrate understanding of multi-threading, synchronization, and active object design patterns. It involves implementing a multi-threaded pipeline of active objects that perform various tasks on a sequence of numbers, such as checking if a number is prime, manipulating the numbers, and passing them between the active objects. 

TASK A:
Implement a function that takes an unsigned int as input and checks if the number is prime. The function should return 0 if the number is not prime. We check wether the number is prime without using Miller-Rabin method.

Task B:
Implement a thread-safe queue in a multi-threaded environment. The queue should have mutex and allow waiting for an item in the queue without busy waiting. You can use a conditional variable (cond) to achieve this. The queue should hold elements of type void*.
- cond: initialized everytime one thread at a time from the threads that await by the signal, and also be responsible for the threads conditions.
- mutex: At most one thread uses a particular resouce at any given time in critical time. This requirement is concurrency control and the purpose is to prevent thread race situation.

Task C:
Implement an active object that supports the following functions:
a. CreateActiveObject: This function should create and run a thread for the ActiveObject.  the function should enqueue a task and receive a pointer to a function to be called for each item in the queue.
b. getQueue: This function return a pointer to the queue of the ActiveObject passed as a parameter (this). It can be used to add an item to the queue.
c. stop: This function should stop the ActiveObject passed as a parameter (this). Additionally, it should release all memory resources of the object.
- Queue: this is normal queue that uses mutex and cond as explained above, that represents multi-thread environment.
-Active Object: creating an Active Object that posesses function and AO for continuation, it runs untill it gets NULL.

Task D:
Use the functions implemented in the previous tasks to create a program called st_pipeline. The program takes one or two arguments from the command line: N, representing the number of tasks, and an optional random seed. If no random seed is provided, it can be generated using null or time function.

A pipeline is built using Active Objects (AO), forming a collection of Active Objects. The pipeline should be constructed as follows:

The first AO:

Initialize the random number generator with the given seed and generate N six-digit random numbers (e.g., 3 + 3 using .rand).
Pass each number one by one to the next AO with a time delay of one millisecond (1ms).
The second AO:

Print the number received.
Check if the number is prime and print "true" or "false" accordingly.
Add 11 to the number and pass it to the next AO.
The third AO:

Print the number received.
Check if the number is prime and print "true" or "false" accordingly.
Subtract 13 from the number and pass it to the next AO.
The fourth AO:

Print the received number.
Add 2 to the number and print the new number.
If implemented correctly, the last number printed should be the same as the first number.


HOW TO RUN??
make all 
./st_pipeline 3

   


   

