/* 
    File: simple_timer.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/19

    Simple control of the 
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
#include "interrupts.H"
#include "simple_timer.H"
#include "thread.H"

BOOLEAN generated_by_slave_PIC(unsigned int int_no) {
  return int_no > 7;
}
/*--------------------------------------------------------------------------*/
/* VARIABLES */
/*--------------------------------------------------------------------------*/

extern Scheduler * SYSTEM_SCHEDULER;
unsigned long SimpleTimer::seconds; 
int           SimpleTimer::ticks;   /* ticks since last "seconds" update.    */

  /* At what frequency do we update the ticks counter? */
int           SimpleTimer::hz;      /* Actually, by defaults it is 18.22Hz.
                                       In this way, a 16-bit counter wraps
                                       around every hour.                    */

/*--------------------------------------------------------------------------*/
/* INITIALIZATION OF STATIC VARIABLES */
/*--------------------------------------------------------------------------*/

void SimpleTimer::init(int _hz) {
  /* How long has the system been running? */
  seconds =  0; 
  ticks   =  0; /* ticks since last "seconds" update.    */

  /* At what frequency do we update the ticks counter? */
  /* hz      = 18; */
                /* Actually, by defaults it is 18.22Hz.
                   In this way, a 16-bit counter wraps
                   around every hour.                    */
  set_frequency(_hz);

}

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   S i m p l e T i m e r */
/*--------------------------------------------------------------------------*/


void SimpleTimer::handler(REGS *_r) {
/* What to do when timer interrupt occurs? In this case, we update "ticks",
   and maybe update "seconds".
   This must be installed as the interrupt handler for the timer in the 
   when the system gets initialized. (e.g. in "kernel.C") */

    /* Increment our "ticks" count */
    ticks++;

    /* Whenever a second is over, we update counter accordingly. */
    /*if (ticks >= hz )
    {
        seconds++;
        ticks = 0;
        Console::puts("One second has passed\n");
    }
    */

  unsigned int int_no = _r->int_no - 32;

  if (generated_by_slave_PIC(int_no)) {
       outportb(0xA0, 0x20);
  }

  // Send an EOI message to the master interrupt controller. 
  outportb(0x20, 0x20);


    if (ticks >= 5)
    {
        SYSTEM_SCHEDULER->resume(Thread::CurrentThread());
        SYSTEM_SCHEDULER->yield();      
    }
  
}


void SimpleTimer::set_frequency(int _hz) {
/* Set the interrupt frequency for the simple timer.
   Preferably set this before installing the timer handler!                 */

  hz = _hz;                            /* Remember the frequency.           */
  int divisor = 1193180 / _hz;         /* The input clock runs at 1.19MHz   */
  outportb(0x43, 0x34);                /* Set command byte to be 0x36.      */
  outportb(0x40, divisor & 0xFF);      /* Set low byte of divisor.          */
  outportb(0x40, divisor >> 8);        /* Set high byte of divisor.         */
}

void SimpleTimer::current(unsigned long * _seconds, int * _ticks) {
/* Return the current "time" since the system started. */

  *_seconds = seconds;
  *_ticks   = ticks;
}

void SimpleTimer::wait(unsigned long _seconds) {
/* Wait for a particular time to be passed. This is based on busy looping! */

    unsigned long now_seconds;
    int           now_ticks;
    current(&now_seconds, &now_ticks);

    unsigned long then_seconds = now_seconds + _seconds;

    while((seconds <= then_seconds) && (ticks < now_ticks));
}


