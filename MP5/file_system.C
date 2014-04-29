/*/\r//g
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

File::File(int _file_id)
{
    file_id = _file_id;
    
    // TODO we have to set the file_system pointer
    
    if(!file_system->LookupFile(file_id, this)){
        if(file_system->CreateFile(file_id));
    }
    
    // Add into system wide OFT
    //Set the curr block and position to first block and 0
    
    
    
}

unsigned int File::Read(unsigned int _n, char * _buf){
    unsigned int characters_read = 0;
    unsigned int no_char;
    
    while((_n-characters_read)>0){
        file_system->disk->read(curr_block, &cached_block);
        
       // no of char to read in this block
        if((curr_position+(_n-characters_read))>511){
            no_char= 512-curr_position;
        }
        else{
            no_char = _n-characters_read;
        }
        for(i=0; i< no_char ; i++){
            _buf[i]=cached_block[curr_position+i];
            characters_read = characters_read + 1;
        }
        
        // now current block is read --- fetch a new block if still something is to be read
        curr_block = curr_block + 1;
        curr_position = (curr_position + no_char) % 512;
        
    }
    
    return characters_read;
    
}


unsigned int File::Write(unsigned int _n, char * _buf){
    unsigned int characters_read = 0;
    
    while((_n-characters_read)>0){
        file_system->disk->write(curr_block, &cached_block);
        
        unsigned int no_char; // no of char to read in this block
        if((curr_position+(_n-characters_read))>511){
            no_char= 512-curr_position;
        }
        else{
            no_char = _n-characters_read;
        }
        for(i=0; i< no_char ; i++){
            _buf[i]=cached_block[curr_position+i];
            characters_read = characters_read + 1;
        }
        
        // now current block is read --- fetch a new block if still something is to be read
        curr_block = curr_block + 1;
        curr_position = (curr_position + no_char) % 512;
        
    }
    
}

void File::Reset(){
    curr_block = file_list[_file_id]->GetFirstBlock();
}







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

<<<<<<< HEAD
    storage = new char[ 512 ];        
=======
    storage = new char[SUPERBLOCK_SIZE];
    
    FAT * fat_ptr;
    freeList * freelist_ptr;
>>>>>>> e61ce38666f676ed4b1b89615432aeb16c24e6ab

//    for(i=0; i<512; i=i+1){
//     file_list[i] = -1; // Initialize it to -1 ; file has not created.
//    }

}


BOOLEAN FileSystem::Mount(SimpleDisk * _disk)
{ 
  /* Associates the file system with a disk. We limit ourselves to at most one
      file system per disk. Returns TRUE if 'Mount' operation successful (i.e. there
      is indeed a file system on the disk. */
   char *buffer = storage;
   //superblock_size = SUPERBLOCK_SIZE;
   _disk->read(0, buffer);
   struct superblock *sblock = buffer;
   if(sblock->type != "spec_file_system")
   {
      Console::puts("\nUnknown File System!!Not supported!!");
      return false;
   }
   unsigned int superblock_size = sblock->sblock_size;
   unsigned int block_size      = sblock->block_size;
   unsigned int no_of_blocks    = sblock->no_of_blocks;
   delete storage;

   storage = new char[superblock_size * block_size];
   buffer = storage;
   for(int read_block = 0; i < superblock_size; ++i)   
   { 
      buffer +=  block_size;
      _disk->read(read_block, buffer);    
   }
   struct superblock *sblock = buffer;

   freeList* flist = new freeList(no_of_blocks, sblock->freeList, false);
   FAT*      fat   = new FAT(no_of_blocks, sblock->fat, false);
   directory_list  = sblock->directory_list;
   //TODO; Check the name of the directory_list pointer
}

BOOLEAN FileSystem::Format(SimpleDisk * _disk, unsigned int _size)
{
   /* Wipes any file system from the given disk and installs a new, empty, file
      system that supports up to _size Byte. */

   s_block *sblock = new s_block; //TODO:check this definition
   sblock->type = "spec_file_system";
   sblock->block_size = 512;
   sblock->no_of_blocks = _size/512;
   if (no_of_blocks % 512 > 0)
      (sblock->no_of_blocks)++; //                                
   sblock->disk_size = _size; //                                
                                                      
   freeList* flist = new freeList(sblock->freeList, sblock->no_of_blocks, true)                
   FAT* fat = new FAT(sblock->fat, sblock->no_of_blocks, true)
   fcb directory_list[MAX_FILES];                     //TODO: Check with Anil on how to initialize the directory
   
   int no_of_blocks = sblock->no_of_blocks;
   sblock->sblock_size = 1 + (no_of_blocks/sizeof(char))/512 + (no_of_blocks*sizeof(int))/512 + ((fcb::size()) * MAX_FILES)/512; 
   for(int i = 0; i < sblock->sblock_size; ++i)
   {
      flist->allocBlock(i);
   }

   char *buf = sblock;
   for(int i = 0; i < sblock->sblock_size; ++i)
   {
      _disk->write(i, buf);
      buf += 512;
   }

   Console::puts("\nDisk Formatted");

}

BOOLEAN FileSystem::LookupFile(int _file_id, File *_file){
    BOOLEAN found=0;
    
// TODO -- lookup first in system wide table;
    if(file_list[_file_id]->GetFirstBlock() == -1){
        found = false;
    }
    else{
        found = true;
    }
    
    if(found){
        _file->curr_block = file_list[_file_id].GetFirstBlock();
        _file->curr_position = 0;
    }
    
    return found;
}

BOOLEAN FileSystem::CreateFile(int _file_id){
    
//    int i;
//    File * _file;
//    BOOLEAN Exist = LookupFile(_file_id, _file);
//    if(Exist){
//        return false;
//    }
//    else{
//        for(i=0; i<511; i=i+1){
//            if(file_list[i].file_id == -1);
//            break;
//        }
//        
//        // Found the next empty spot in directory and i is the index for that
//        file_list[i].file_id = _file_id;
//        // Can not acces the file_id variable... have to define two funtions..set fid and get fid
//        return true;
//    }
    int first_block;
    // get a free block from free list
    first_block = freelist_ptr->getblock();
    //Once you got the first block then give it to the new file
    file_list[_file_id]->SetFirstBlock(first_block);
    
    // Set the first block of the new file as the last block
    fat_ptr->SetLastBlock(first_block);
}
