/* 
    File: fat.H

    Author: Anil Alagiya :)
            Electrical and Computer Engineering Department
            Texas A&M University
    Date  : 4/26/14

    Description: File Allocaiton Table - FAT
    

*/


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "simple_disk.H"
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARD DECLARATIONS */ 
/*--------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------*/
/* F i l e */
/*--------------------------------------------------------------------------*/

class FAT {
    

private:
    
    int no_blocks;
    int fat[no_blocks];
    
public:
    
    void SetNextBlock(int CurrBlock, int NextBlock);
    int GetNextBlock(int CurrBlock);
    void SetLastBlock(int CurrBlock);
    BOOLEAN isLastBlock(int CurrBlock);
    
    FAT(int _no_blocks);

};

