/*Implementation of Queue for Thread Scheduler*/
#ifndef QUEUE_H
#define QUEUE_H

#include "utils.H"
#include "thread.H"
#include "console.H"

#define QUEUE_SIZE 100

template <typename T>
class Queue{

private:
	
	int head, tail;
   T list[QUEUE_SIZE];

public:
	Queue();
   int isEmpty();
   int isFull();
	void enqueue(T th);
	T dequeue();

};

#include "template_queue.C"
#endif
