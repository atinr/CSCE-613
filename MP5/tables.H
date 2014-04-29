#include "Console.H"
#include "utils.H"

#define MAX_OPEN_FILES 20

struct open_inode{
   
   int file_id;
   fcb* curr_fcb;
   char* curr_ptr;
   int open_counter;
   int curr_disk_block;
};

struct open_inode* system_wide_table[MAX_OPEN_FILES];

void init_system_wide_table()
{
   for(int i = 0; i<MAX_OPEN_FILES ;++i)
      system_wide_table[i] = NULL;
}

struct open_inode* lookup_system_wide_table(int _file_id)
{
   for(int i = 0; i<MAX_OPEN_FILES ;++i)
   {
      if(system_wide_table[i] != NULL && system_wide_table[i]->file_id == _file_id)
      {
         return system_wide_table[i];
      }
   }

   Console::puts("\nNo file_id entry in the system table exists");
   return NULL;
         
}

void insert_system_wide_table(struct open_inode* new_node)
{
   for(int i = 0; i<MAX_OPEN_FILES ;++i)
   {
      if (system_wide_table[i] == NULL)
      {
         system_wide_table[i] = new_node;
         return;
      }
   }

   Console::puts("\n No free space in system_wide_table");
}

void delete_system_wide_table(int _file_id)
{
   for(int i = 0; i<MAX_OPEN_FILES ;++i)
   {
      if(system_wide_table[i] != NULL && system_wide_table[i]->file_id == _file_id)
      {
         delete system_wide_table[i];
         system_wide_table[i] = NULL;  
         return;
      }
   }

   Console::puts("\n No entry with the given file_id present in the table");
}



