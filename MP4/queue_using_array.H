/*Implementation of Queue for Thread Scheduler*/
#ifndef QUEUE_H
#define QUEUE_H

#include "utils.H"
#include "thread.H"
#include "console.H"

#define QUEUE_SIZE 100

class Queue{

private:
	
	int head, tail;
   Thread* list[QUEUE_SIZE];

public:
	Queue();
   int isEmpty();
   int isFull();
	void enqueue(Thread *th);
	Thread * dequeue();

};

#endif
