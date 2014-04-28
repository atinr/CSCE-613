/*
     File        : blocking_disk.c

     Author      : Atin Ruia
     Modified    : 27/04/14

     Description : Block-level READ/WRITE operations on a simple LBA28 disk 
                   using Programmed I/O.
                   
                   The disk must be MASTER or SLAVE on the PRIMARY IDE controller.

                   The code is derived from the "LBA HDD Access via PIO" 
                   tutorial by Dragoniz3r. (google it for details.)
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "utils.H"
#include "console.H"
#include "blocking_disk.H"
#include "Scheduler.H"

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR */
/*--------------------------------------------------------------------------*/

BlockingDisk::BlockingDisk(DISK_ID _disk_id, unsigned int _size):SimpleDisk(_disk_id,_size),operating(false),disk_id(_disk_id)
{
//   operating = false;
}

/*--------------------------------------------------------------------------*/
/* BLOCKING_DISK FUNCTIONS */
/*--------------------------------------------------------------------------*/

void BlockingDisk::issue_operation(DISK_OPERATION _op, unsigned long _block_no) {

  outportb(0x1F1, 0x00); /* send NULL to port 0x1F1         */
  outportb(0x1F2, 0x01); /* send sector count to port 0X1F2 */
  outportb(0x1F3, (unsigned char)_block_no); 
                         /* send low 8 bits of block number */
  outportb(0x1F4, (unsigned char)(_block_no >> 8)); 
                         /* send next 8 bits of block number */
  outportb(0x1F5, (unsigned char)(_block_no >> 16)); 
                         /* send next 8 bits of block number */
  outportb(0x1F6, ((unsigned char)(_block_no >> 24)&0x0F) | 0xE0 | (disk_id << 4));
                         /* send drive indicator, some bits, 
                            highest 4 bits of block no */

  outportb(0x1F7, (_op == READ) ? 0x20 : 0x30);

}

void BlockingDisk::read(unsigned long _block_no, char * _buf) {
/* Reads 512 Bytes in the given block of the given disk drive and copies them 
   to the given buffer. No error check! */

   //Check if the current disk is performing an operation
   //Console::puts("entered read in blockdisk");
   //for(;;);  
   //int k = 0;
   //while(!is_ready())
   //{
   //   ++k;
   //}
   //Console::puti(k);
   //Console::puts("\nsome random value");   
   if(is_op_disk() || !diskQueueOp.isEmpty())
   {
     struct disk_operation new_op;
     new_op.current_op = READ;
     new_op.block_no = _block_no;
     diskQueueOp.enqueue(&new_op); //TODO : Add error checking later in both the enqueues
     diskQueue.enqueue(Thread::CurrentThread());
     //Console::puts("\nThread and op is queue");
       //  for(;;);
     SYSTEM_SCHEDULER->yield();
   }
   else
   {
      if(diskQueue.isEmpty() )
      {
         issue_operation( READ, _block_no);
         diskQueue.enqueue(Thread::CurrentThread());   // Enqueue the current thread in the device_queue while the disk move its head to the required sector
         set_op_disk();
       //  Console::puts("\nThread is queue");
         SYSTEM_SCHEDULER->yield();
      }
      else
      {
         Console::puts("\nError in queue setup");
         assert(false);
      }
   }
//  issue_operation(READ, _block_no);

//  wait_until_ready();

  /* read data from port */
  int i;
  unsigned short tmpw;
  for (i = 0; i < 256; i++) {
    tmpw = inportw(0x1F0);
    _buf[i*2]   = (unsigned char)tmpw;
    _buf[i*2+1] = (unsigned char)(tmpw >> 8);
  }
}

void BlockingDisk::write(unsigned long _block_no, char * _buf) {
/* Writes 512 Bytes from the buffer to the given block on the given disk drive. */

   //Check if the current disk is performing an operation
   if(is_op_disk() || !diskQueueOp.isEmpty()) 
   {
     struct disk_operation new_op;
     new_op.current_op = WRITE;
     new_op.block_no = _block_no;
     diskQueueOp.enqueue(&new_op); //TODO : Add error checking later in both the enqueues
     diskQueue.enqueue(Thread::CurrentThread());
     SYSTEM_SCHEDULER->yield();
   }
   else
   {

    if(diskQueue.isEmpty() )
      {
         issue_operation( WRITE, _block_no);
         diskQueue.enqueue(Thread::CurrentThread());   // Enqueue the current thread in the device_queue while the disk move its head to the required sector
         set_op_disk();
         SYSTEM_SCHEDULER->yield();
      }
      else
      {
         Console::puts("\nError in queue setup");
         assert(false);
      }

   }


//  issue_operation(WRITE, _block_no);

//  wait_until_ready();

  /* write data to port */
  int i; 
  unsigned short tmpw;
  for (i = 0; i < 256; i++) {
    tmpw = _buf[2*i] | (_buf[2*i+1] << 8);
    outportw(0x1F0, tmpw);
  }

}


void BlockingDisk::set_op_disk()
{
   operating = true;   
}

bool BlockingDisk::is_op_disk()
{
   return operating;
}

void BlockingDisk::reset_op_disk()
{
   operating = false;
}

void BlockingDisk::enqueue_thread(Thread* t)
{
   diskQueue.enqueue(t);
}

Thread* BlockingDisk::dequeue_thread()
{
   return diskQueue.dequeue();
}

void BlockingDisk::enqueue_operation(disk_op* new_op)
{
   diskQueueOp.enqueue(new_op);
}

disk_op* BlockingDisk::dequeue_operation()
{
   return diskQueueOp.dequeue();
      
}



