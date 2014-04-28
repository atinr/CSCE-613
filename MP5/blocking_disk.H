/*
     File        : simple_disk.H

     Author      : Riccardo Bettati
     Modified    : 10/04/01

     Description : Block-level READ/WRITE operations on a simple LBA28 disk 
                   using Programmed I/O.
                   
                   The disk must be MASTER or SLAVE on the PRIMARY IDE controller.

                   The code is derived from the "LBA HDD Access via PIO" tutorial
                   by Dragoniz3r. (google it for details.)
*/

#ifndef _BLOCKING_DISK_H_
#define _BLOCKING_DISK_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

class Scheduler;
extern Scheduler* SYSTEM_SCHEDULER;
/*--------------------------------------------------------------------------n
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "template_queue.H"
#include "simple_disk.H"
#include "Scheduler.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/


//   typedef enum {MASTER = 0, SLAVE = 1} DISK_ID; 
//   typedef enum {READ = 0, WRITE = 1} DISK_OPERATION;

   typedef struct disk_operation{
      unsigned long block_no;
      DISK_OPERATION current_op;
   }disk_op;

/*--------------------------------------------------------------------------*/
/* B l o c k i n g D i s k  */
/*--------------------------------------------------------------------------*/

class BlockingDisk : public SimpleDisk  {
private:
     /* -- FUNCTIONALITY OF THE IDE LBA28 CONTROLLER */

     DISK_ID disk_id;

     bool operating;                  // checks if the disk is current operating

     Queue<Thread*> diskQueue;
     Queue<disk_op*> diskQueueOp;    
 
public:


   BlockingDisk(DISK_ID _disk_id, unsigned int _size); 
   /* Creates a SimpleDisk device with the given size connected to the MASTER or 
      SLAVE slot of the primary ATA controller.
      NOTE: We are passing the _size argument out of laziness. In a real system, we would
      infer this information from the disk controller. */

   void issue_operation(DISK_OPERATION _op, unsigned long _block_no);
   /* Send a sequence of commands to the controller to initialize the READ/WRITE 
        operation. This operation is called by read() and write(). */

   
   /* DISK OPERATIONS */

   virtual void read(unsigned long _block_no, char * _buf);
   /* Reads 512 Bytes from the given block of the disk and copies them 
      to the given buffer. No error check! */

   virtual void write(unsigned long _block_no, char * _buf);
   /* Writes 512 Bytes from the buffer to the given block on the disk. */

   //Set current operation on disk true. Time required to move the head to the current sector
   void set_op_disk();

   //Returns true if the current operation on disk is set
   bool is_op_disk();

   //No current operation is being done on the disk
   void reset_op_disk();

   void enqueue_thread(Thread* t);

   Thread* dequeue_thread();

   void enqueue_operation(struct disk_operation* new_op);

   struct disk_operation* dequeue_operation();

   BOOLEAN is_disk_ready() { return is_ready(); }
};

#endif
