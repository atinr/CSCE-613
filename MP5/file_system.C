/* 
    File: file_system.H

    Author: Atin Ruia
    Date  : 28/04/14

    Description: File System.
    

*/



/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "file_system.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

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

     /* -- You may want to store other information, such as 
             .. position in the file
             .. cached block(s)
             .. etc.
      */

public:

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


};

/*--------------------------------------------------------------------------*/
/* F i l e S y s t e m  */
/*--------------------------------------------------------------------------*/

FileSystem::FileSystem(){
    int i;

    storage = new char[SUPERBLOCK_SIZE];        

    for(i=0; i<512; i=i+1){
        file_list[i].file_id = -1; // Initialize it to -1 ; file has not created.
    }

}


BOOLEAN FileSystem::Mount(SimpleDisk * _disk)
{ 
  /* Associates the file system with a disk. We limit ourselves to at most one
      file system per disk. Returns TRUE if 'Mount' operation successful (i.e. there
      is indeed a file system on the disk. */
   char *buffer = storage;
   //superblock_size = SUPERBLOCK_SIZE;
   _disk->read(read_block, buffer);
   struct superblock *sblock = buffer;
   if(sblock->type != "spec_file_system")
   {
      Console::puts("\nUnknown File System!!Not supported!!");
      return false;
   }
   superblock_size = sblock->sblock_size;
   
   for(int read_block = 1; i < superblock_size; ++i)   
   { 
      buffer +=  sblock-> block_size;
      _disk->read(read_block, buffer);    
   }
}

BOOLEAN FileSystem::Format(SimpleDisk * _disk, unsigned int _size)
{
   /* Wipes any file system from the given disk and installs a new, empty, file
      system that supports up to _size Byte. */
}

BOOLEAN FileSystem::LookupFile(int _file_id, File *_file){
    BOOLEAN found=0;
    int i;
    
    for(i=0; i<511; i=i+1){
        if(file_list[i].file_id == _file_id){
            found = 1;
            break;
        }
    }
    
    if(found){
        _file = &file_list[i];
        return found;
        }
    else{
        return found;
    }
}

BOOLEAN FileSystem::CreateFile(int _file_id){
    
    int i;
    File * _file;
    BOOLEAN Exist = LookupFile(_file_id, _file);
    if(Exist){
        return false;
    }
    else{
        for(i=0; i<511; i=i+1){
            if(file_list[i].file_id == -1);
            break;
        }
        
        // Found the next empty spot in directory and i is the index for that
        file_list[i].file_id = _file_id;
        // Can not acces the file_id variable... have to define two funtions..set fid and get fid
        return true;
    }
}
