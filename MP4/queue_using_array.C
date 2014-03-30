#include "queue_using_array.H"

/*Implementation of Queue for Thread Scheduler*/

/*	
	typedef struct list{
        	Thread *thread;
        	struct list *next;
	};

	struct list **Q;
*/
	
Queue::Queue():head(-1),tail(-1)
{}


int Queue::isEmpty()
{
   return (tail == -1);
}


int Queue::isFull()
{
   return (((tail+1)%QUEUE_SIZE) == head);
}

void Queue::enqueue(Thread *th)
{
   
   if (!this->isFull())
   {
      if (this->isEmpty())
         ++head;

      tail = (tail + 1)%QUEUE_SIZE;
     list[tail] = th;
   }
   else
     Console::puts("\n ReadyQueue Full!!");
}


Thread* Queue::dequeue()
{
   if (!this->isEmpty())
   {
      Thread *t = list[head];
      
      if (head == tail)
      {
         tail = -1;
         head = -1;
      }
      else
         head = (head + 1)%QUEUE_SIZE;
      
      return t;
   }
   else
   {
      Console::puts("\nReadyQueue Empty!!");
      return NULL;
   }
}

