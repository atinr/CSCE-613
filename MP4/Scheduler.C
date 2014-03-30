#include "Scheduler.H"
#include "queue.H"

/*Costructor*/
Scheduler::Scheduler()
{
}

/* Called by the currently running thread in order to give up the CPU. 
      The scheduler selects the next thread from the ready queue to load onto 
      the CPU, and calls the dispatcher function defined in 'threads.h' to
      do the context switch. */
void Scheduler::yield()
{
   Thread *next = ReadyQueue.dequeue();
   if (next != NULL)
   {
         Thread::dispatch_to(next);
   }
}


   /* Add the given thread to the ready queue of the scheduler. This is called
      for threads that were waiting for an event to happen, or that have 
      to give up the CPU in response to a preemption. */
void Scheduler::resume(Thread * _thread)
{
   ReadyQueue.enqueue(_thread);
}


   /* Make the given thread runnable by the scheduler. This function is called
     typically after thread creation. Depending on the
      implementation, this may not entail more than simply adding the 
      thread to the ready queue (see scheduler_resume). */
void Scheduler::add(Thread * _thread);
{
   ReadyQueue.enqueue(_thread);
}
  
 /* Remove the given thread from the scheduler in preparation for destruction
      of the thread. */
void Scheduler::terminate(Thread * _thread);
{
   delete _thread;
}

