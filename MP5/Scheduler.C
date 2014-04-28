#include "Scheduler.H"
#include "blocking_disk.H"
#include "assert.H"

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

   if(SYSTEM_BLOCK_DISK->is_op_disk() && SYSTEM_BLOCK_DISK->is_disk_ready())    //is operating on disk
   {
      //Console::puts("\ncame inside 1");
      //for(;;);
      Thread *next = SYSTEM_BLOCK_DISK->dequeue_thread();
      if(next == NULL)
      {
         Console::puts("\n Error! DiskQueue empty!!");
         assert(false);
      }
      else
      {
         SYSTEM_BLOCK_DISK->reset_op_disk();
         Thread::dispatch_to(next);
         return;
      }
   }
   else if (!SYSTEM_BLOCK_DISK->is_op_disk()) //disk is free
   {
      //Console::puts("\nCame inside 2");
      //for(;;);
      struct disk_operation* new_op = SYSTEM_BLOCK_DISK->dequeue_operation();
      if(new_op != NULL)
      {
         SYSTEM_BLOCK_DISK->issue_operation(new_op->current_op, new_op->block_no);
         SYSTEM_BLOCK_DISK->set_op_disk();
      }
      else
      {//TODO : remove for loop
         Console::puts("\n The disk queue is empty");         
         for(int j=0; j<2000000 * 5; ++j);
      }
   }

   Thread *next = ReadyQueue.dequeue();
   if (next != NULL)
   {
         Thread::dispatch_to(next);
   }
   else
   {
      Console::puts("Error encountered!!");
      assert(false);
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
void Scheduler::add(Thread * _thread)
{
   ReadyQueue.enqueue(_thread);
}
  
 /* Remove the given thread from the scheduler in preparation for destruction
      of the thread. */
void Scheduler::terminate(Thread * _thread)
{
   delete _thread;
}

