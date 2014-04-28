//#include "template_queue.H"
//#include "queue_using_array.H"
/*Implementation of Queue for Thread Scheduler*/

/*	
	typedef struct list{
        	Thread *thread;
        	struct list *next;
	};

	struct list **Q;
*/
template<typename T>	
Queue<T>::Queue():head(-1),tail(-1)
{}

template<typename T>
int Queue<T>::isEmpty()
{
   return (tail == -1);
}

template<typename T>
int Queue<T>::isFull()
{
   return (((tail+1)%QUEUE_SIZE) == head);
}

template<typename T>
void Queue<T>::enqueue(T t)
{
   
   if (!this->isFull())
   {
      if (this->isEmpty())
         ++head;

      tail = (tail + 1)%QUEUE_SIZE;
     list[tail] = t;
   }
   else
   {
     Console::puts("\n ReadyQueue Full!!");
     for(;;);
   }
}

template<typename T>
T Queue<T>::dequeue()
{
   if (!this->isEmpty())
   {
      T t = list[head];
      
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

