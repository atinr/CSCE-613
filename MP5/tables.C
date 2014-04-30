//
//  File.c
//  
//
//  Created by Anil Alagiya on 4/29/14.
//
//
#include "tables.H"
#include "file_system.H"

File* system_wide_table[MAX_OPEN_FILES];

void insert_system_wide_table(File* newFile)
{
    for(int i = 0; i<MAX_OPEN_FILES ;++i)
    {
        if (system_wide_table[i] == NULL)
        {
            system_wide_table[i] = newFile;
            return;
        }
    }
}

void delete_system_wide_table(File* file)
{
    for(int i = 0; i<MAX_OPEN_FILES ;++i)
    {
        if(system_wide_table[i] != NULL && system_wide_table[i] == file)
        {
            delete system_wide_table[i];
            system_wide_table[i] = NULL;
            return;
        }
    }
    
    Console::puts("\n No entry with the given file_id present in the table");
}

File* lookup_system_wide_table(int _file_id)
{
    for(int i = 0; i<MAX_OPEN_FILES ;++i)
    {
        if(system_wide_table[i] != NULL && system_wide_table[i]->getFileID() == _file_id)
        {
            return system_wide_table[i];
        }
    }
    
    Console::puts("\nNo file_id entry in the system table exists");
    return NULL;
}

void init_system_wide_table()
{
    for(int i = 0; i<MAX_OPEN_FILES ;++i)
        system_wide_table[i] = NULL;
}
