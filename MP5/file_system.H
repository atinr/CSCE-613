/* 
    File: file_system.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 10/04/05

    Description: File System.
    

*/

#ifndef _FILE_SYSTEM_H_                   // include file only once
#define _FILE_SYSTEM_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

extern void * operator new (size_t size); 

//replace the operator "new[]"
extern void * operator new[] (size_t size);

//replace the operator "delete"
extern void operator delete (void * p);

//replace the operator "delete[]"
void operator delete[] (void * p);

#define SUPERBLOCK_SIZE 2048
#define MAX_FILES 256
#define MAX_BLOCKS 20480 

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "simple_disk.H"
#include "fcb.H"
#include "freeList.H"
#include "fat.H"
#include "tables.H"
/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef struct superblock{

   char type[20]; //specify the type of filesystem supported   20 bytes
   unsigned int block_size; //                                  4 bytes
   unsigned int no_of_blocks; //                                4 bytes
   unsigned int disk_size; //                                   4 bytes
   unsigned int sblock_size;  //                                4 bytes
//   unsigned int freeList_size; //                               4 bytes
//   unsigned int fat_size; //                                    4 bytes
//   unsigned int directory_size; //                              4 bytes
   char pad[28]; //                                            16 bytes
                                                      // total 64 bytes
   char freeList[MAX_BLOCKS / 8 ]; //                 //     2560 bytes -> 5 blocks                
   int fat[MAX_BLOCKS];                      //    20480 bytes -> 40 blocks
   struct fcb_data directory_list[MAX_FILES];                     //      512 bytes

//   fcb fcb_list[MAX_FILES];
}s_block;
/*--------------------------------------------------------------------------*/
/* FORWARD DECLARATIONS */ 
/*--------------------------------------------------------------------------*/

class FileSystem;

/*--------------------------------------------------------------------------*/
/* F i l e */
/*--------------------------------------------------------------------------*/

class File {
private: 
     /* -- DEFINE YOUR FILE DATA STRUCTURES HERE. */
     
     FileSystem   * file_system;

     unsigned int   file_id;
    
    int file_size;

     /* -- You may want to store other information, such as 
             .. position in the file
             .. cached block(s)
             .. etc.
      */
public:
    int curr_block; // To keep track of the current block
    int curr_position; // To keep track of the bytes within current block
    char cached_block[512];//keep the read block in a local variable -- cached

    File();
    /* Constructor for the file handle. Set the 'current
       position' to be at the beginning of the file. */

    unsigned int Read(unsigned int _n, char * _buf);
    /* Read _n characters from the file starting at the 
       current location and copy them in _buf.
       Return the number of characters read. */

    unsigned int Write(unsigned int _n, char * _buf);
    /* Write _n characters to the file starting at the current 
       location, if we run past the end of file, we increase 
       the size of the file as needed. 
     */

    void Reset();
    /* Set the 'current position' at the beginning of the file. */

    void Rewrite();
    /* Erase the content of the file. Return any freed blocks. 
       Note: This function does not delete the file! It just erases its
       content. */

    BOOLEAN EoF();
    /* Is the current location for the file at the end of the file? */
    
    int getFileID();


};

/*--------------------------------------------------------------------------*/
/* F i l e S y s t e m  */
/*--------------------------------------------------------------------------*/

class FileSystem {

friend class File;

private:
     /* -- DEFINE YOUR FILE SYSTEM DATA STRUCTURES HERE. */
     
     SimpleDisk * disk;
     unsigned int size;
     char* storage;
    struct fcb_data * file_list;
    FAT * fat_ptr;
    freeList * freelist_ptr;
 
public:

   FileSystem();   
   /* Just initializes local data structures. Does not connect to disk yet. */

   BOOLEAN Mount(SimpleDisk * _disk);
   /* Associates the file system with a disk. We limit ourselves to at most one
      file system per disk. Returns TRUE if 'Mount' operation successful (i.e. there
      is indeed a file system on the disk. */

   static BOOLEAN Format(SimpleDisk * _disk, unsigned int _size);
   /* Wipes any file system from the given disk and installs a new, empty, file
      system that supports up to _size Byte. */

   BOOLEAN LookupFile(int _file_id, File * _file);
   /* Find file with given id in file system. If found, initialize the file 
      object and return TRUE. Otherwise, return FALSE. */

   BOOLEAN CreateFile(int _file_id);
   /* Create file with given id in the file system. If file exists already,
      abort and return FALSE. Otherwise, return TRUE. */

   BOOLEAN DeleteFile(int _file_id);
   /* Delete file with given id in the file system and free any disk block
      occupied by the file. */

   
};
#endif
