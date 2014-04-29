/* 
    File: frame_pool.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/05/13

    Implementation of the manager for the Free-Frame Pool.

    THIS IS A DUMMY IMPLEMENTATION ONLY! 
    IT HAS BEEN IMPLEMENTED AS POORLY AS HUMANLY POSSIBLE.
    DO NOT TRY TO USE OR IMITATE IN ANY WAY!!

    NOTE: THIS IMPLEMENTATION SUPPORTS THE CREATION OF ONLY ONE FRAME POOL!!

*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "fat.H"

/*--------------------------------------------------------------------------*/
/* LOCAL VARIABLES */
/*--------------------------------------------------------------------------*/

static unsigned long next_free_frame;

/*--------------------------------------------------------------------------*/
/* F r a m e   P o o l  */
/*--------------------------------------------------------------------------*/

FAT::FAT (int* _fat, int _no_blocks, bool init):no_blocks(_no_blocks), fat(_fat){
    //no_blocks = _no_blocks;
    
   if(init) // init only if set
   {
    
      for(i=0;i<no_blocks;i++)
      {
            fat[i]=-1; // - 1 means that it is free block; it points to nothing
      }
   }

}
    
void FAT::SetNextBlock(int CurrBlock, int NextBlock){
    
   fat[CurrBlock] = NextBlock;
}

int GetNextBlock(int CurrBlock){
   return fat[currBlock];
}
    
void SetLastBlock(int CurrBlock){
    fat[CurrBlock] = 0;
}
    
BOOLEAN isLastBlock(int CurrBlock){
    return (fat[CurrBlock] == 0);
}

