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

File::File(int _file_id, FileSystem *_fileSystem)
{
    file_id = _file_id;
    curr_position = 0;
    file_system = _fileSystem;
    file_size = file_system->file_list[_file_id].fileSize;
    
    // TODO we have to set the file_system pointer
    
//    if(!file_system->LookupFile(file_id, this)){
//        if(file_system->CreateFile(file_id));
//    }
    
    // Add into system wide OFT
    //Set the curr block and position to first block and 0
    
}

int File::getFileID()
{
    return file_id;
}

unsigned int File::Read(unsigned int _n, char * _buf){
    unsigned int characters_read = 0;
    unsigned int no_char;
    bool end_of_block=false;
    
    while((_n-characters_read)>0){
        file_system->disk->read(curr_block, cached_block);
        
       // no of char to read in this block
        if((curr_position+(_n-characters_read))>512){
            no_char= 512-curr_position;
            end_of_block = true;
        }
        else{
            no_char = _n-characters_read;
        }
        for(int i=0; i< no_char ; i++){
            _buf[i]=cached_block[curr_position+i];
            characters_read = characters_read + 1;
        }
        
        // now current block is read --- fetch a new block if still something is to be read
        if(end_of_block){
            curr_block = file_system->fat_ptr->GetNextBlock(curr_block);
            curr_position = 0;
        }
        else{
            curr_block = curr_block;
            curr_position = curr_position + no_char;
        }
        
    }
    
    return characters_read;
    
}


unsigned int File::Write(unsigned int _n, char * _buf){
    unsigned int characters_write = 0;
    unsigned int no_char;
    bool end_of_block=false;
    
    while((_n-characters_write)>0){
        file_system->disk->write(curr_block, cached_block);
        
        // no of char to write in this block
        if((curr_position+(_n-characters_write))>512){
            no_char= 512-curr_position;
            end_of_block = true;
        }
        else{
            no_char = _n-characters_write;
        }
        for(int i=0; i< no_char ; i++){
            _buf[i]=cached_block[curr_position+i];
            characters_write = characters_write + 1;
        }
        
        // now current block is write --- fetch a new block if still something is to be write
        if(end_of_block){
            if(file_system->fat_ptr->isLastBlock(curr_block)){
                int next_block= file_system->freelist_ptr->getBlock();
                file_system->fat_ptr->SetNextBlock(curr_block, next_block);
                curr_block = next_block;

            }
            else{
                curr_block = file_system->fat_ptr->GetNextBlock(curr_block);
                }
            
            curr_position = 0;
        }
        else{
            curr_position = curr_position + no_char;
        }
        
    }
    
    return characters_write;
    
}

void File::Reset(){
    curr_block = file_system->file_list[file_id].first_block;
    curr_position=0;
}

void File::Rewrite(){
    curr_block = file_system->file_list[file_id].first_block;
    while(!file_system->fat_ptr->isLastBlock(curr_block))
    {
        int next_block = file_system->fat_ptr->GetNextBlock(curr_block);
        file_system->fat_ptr->freeBlock(curr_block);
        file_system->freelist_ptr->releaseBlock(curr_block);
        curr_block = next_block;
    }
    file_system->fat_ptr->freeBlock(curr_block);
    file_system->freelist_ptr->releaseBlock(curr_block);
    file_system->file_list[file_id].first_block = file_system->freelist_ptr->getBlock();
    curr_block = file_system->file_list[file_id].first_block;
    file_system->fat_ptr->SetLastBlock(curr_block);
    curr_position = 0;
}

BOOLEAN File::EoF(){
    if(file_system->fat_ptr->isLastBlock(curr_block)){
        if((file_size % 512 ) == curr_position){
            return true;
        }
    }
    return false;
}




/*--------------------------------------------------------------------------*/
/* F i l e S y s t e m  */
/*--------------------------------------------------------------------------*/

FileSystem::FileSystem(){
    int i;

    storage = new char[ 512 ];        
    //storage = new char[SUPERBLOCK_SIZE];
    


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
   struct superblock *sblock = (struct superblock *)buffer;
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
   for(int read_block = 0; read_block < superblock_size; ++read_block)   
   { 
      buffer +=  block_size;
      _disk->read(read_block, buffer);    
   }
   
   sblock = (struct superblock *)storage;

   freelist_ptr = new freeList(sblock->freeList, no_of_blocks, false);
   fat_ptr   = new FAT(sblock->fat, no_of_blocks, false);
   file_list  = sblock->directory_list;
   //TODO; Check the name of the directory_list pointer
}

BOOLEAN FileSystem::Format(SimpleDisk * _disk, unsigned int _size)
{
   /* Wipes any file system from the given disk and installs a new, empty, file
      system that supports up to _size Byte. */

   s_block *sblock = new s_block; //TODO:check this definition
   char *c = "spec_file_system";
   memcpy(sblock->type, c, 17);
   sblock->block_size = 512;
   sblock->no_of_blocks = _size/512;
   if (_size % 512 > 0)
      (sblock->no_of_blocks)++; //                                
   sblock->disk_size = _size; //                                
                                                      
   freeList* freelist_ptr = new freeList(sblock->freeList, sblock->no_of_blocks, true);
   FAT* fat_ptr = new FAT(sblock->fat, sblock->no_of_blocks, true);
   //fcb directory_list[MAX_FILES];                     //TODO: Check with Anil on how to initialize the directory
    struct fcb_data directory_list[MAX_FILES];
    for(int i=0; i<MAX_FILES; i++){
        directory_list[i].file_id = i;
        directory_list[i].first_block = -1;
    }
   
   int no_of_blocks = sblock->no_of_blocks;
    sblock->sblock_size = sizeof(struct superblock)/512;//1 + (no_of_blocks/sizeof(char))/512 + (no_of_blocks*sizeof(int))/512 + ((fcb::size()) * MAX_FILES)/512;
   for(int i = 0; i < sblock->sblock_size; ++i)
   {
      freelist_ptr->allocBlock(i);
       fat_ptr->SetLastBlock(i);
   }

   char *buf = (char *)sblock;
   for(int i = 0; i < sblock->sblock_size; ++i)
   {
      _disk->write(i, buf);
      buf += 512;
   }

   Console::puts("\nDisk Formatted");

}

BOOLEAN FileSystem::LookupFile(int _file_id, File **_file){
    BOOLEAN found=0;
    
// TODO -- lookup first in system wide table;
    File* f;
    if((f = lookup_system_wide_table(_file_id)) != NULL)
    {
        *_file = f;
        return 1;
    }
    if(file_list[_file_id].first_block == -1){
        found = false;
    }
    else{
        found = true;
    }
    
    if(found){
        (*_file)->curr_block = file_list[_file_id].first_block;
        (*_file)->curr_position = 0;
        insert_system_wide_table(*_file);
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
    
    //Once you got the first block then give it to the new file
    if(file_list[_file_id].first_block == -1){
        first_block = freelist_ptr->getBlock();
        file_list[_file_id].first_block = first_block;
    
    // Set the first block of the new file as the last block
    fat_ptr->SetLastBlock(first_block);
    
    // Add this file to system wide OFT
        file_list[_file_id].fileSize = 0;
        File * newFile = new File(_file_id, this);
        newFile->curr_block = first_block;
        insert_system_wide_table(newFile);
        
        return true;
    }
    else{
        return false;
    }
    
}
