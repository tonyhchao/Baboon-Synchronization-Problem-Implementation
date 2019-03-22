# How to Compile Program

To compile Synchronization.c program, use the following command:
```
gcc -o main *.c -lpthread -lrt
```
# Problem Overview

Synchronization: There is a deep canyon somewhere in Kruger National Park, South Africa, and a single rope that spans the canyon. Baboons can cross the canyon by swinging hand- over-hand on the rope, but if two baboons going in opposite directions meet in the middle, they will fight and drop to their deaths. Furthermore, the rope is only strong enough to hold three baboons. If there are more baboons on the rope at the same time, it will break. Assuming that we can teach the baboons to use semaphores, we would like to design a synchronization scheme with the following properties.
1.	Once a baboon has begun to cross, it is guaranteed to get to the other side without running into a baboon going the other way.
2.	There are never more than three baboons on the rope. The order of the baboons crossing the rope should be preserved; i.e., the order in which they enter the rope should be the order in which they exit the rope.
3.	A continuing stream of baboons crossing in one direction should not bar baboons going the other way indefinitely (no starvation). Solve this requirement such that the FIFO order is preserved. That is, a baboon trying to cross to the left/right that arrives earlier than a baboon trying to cross in the opposite direction gets on the rope first.

Implementation Specifications
•	Implement the behavior of the baboons in C using the Pthreads library.
•	Make use of an input parameter that indicates the time required for a baboon to cross the canyon. Assume, all the baboons require the same time to cross the canyon.
•	Overall, this problem deals with the access to a common resource, the rope. Access to the rope here means access for the threads to the critical section of your program, and how long a baboon takes to cross the canyon is indicated by the amount of sleep time (in seconds), which is the amount of thread sleep time in the critical section of your program.

Input
The input to the program should be provided in an input _le. The input _le name and the time (in seconds) required for a baboon to cross the canyon have to be command-line parameters.
The format of the input file is given below.
•	Line 1: L,R,R,R,R,R,L,L,R (a sequence of alphabets L (meaning left) and R (meaning right) separated by a comma, that indicate the side of the rope a baboon is trying to cross the rope from). Please note that, the delimiter here is a ',' (comma), and it does not appear after the last symbol in the first line of the file.
