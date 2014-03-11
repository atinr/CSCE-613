
#include "vm_pool.H"
#include "page_table.H"
//   char* page_bitmap;
//   struct alloc_list{
//      unsigned long address;
//      int size;
//   };


VMPool::VMPool(unsigned long _base_address,
               unsigned long _size,
               FramePool *_frame_pool,
               PageTable *_page_table):base_address(_base_address),
                                       page_table(_page_table),frame_pool(_frame_pool)
{

     unsigned long* alloc_frame_addr = (unsigned long *)(PageTable::PAGE_SIZE * (frame_pool->get_frame()));
     unsigned long* free_frame_addr = (unsigned long *)(PageTable::PAGE_SIZE * (frame_pool->get_frame()));

     //Zero the frame obtained
     memset(alloc_frame_addr, 0, PageTable::PAGE_SIZE);
     memset(free_frame_addr, 0, PageTable::PAGE_SIZE);

     alloc_list = (struct alloc_elem *)alloc_frame_addr;
     free_list  = (struct free_elem *)free_frame_addr;

     //Size in number of pages
     size = _size / PageTable::PAGE_SIZE;

     //Entering the first entry in free list 
     free_list[0].size = size;
     free_list[0].address = base_address;
     free_list[0].next = NULL;

     _page_table->register_vmpool(this);
}
     
      
unsigned long VMPool::allocate(unsigned long _size)
{

   _size = _size/PageTable::PAGE_SIZE;
   if (_size % PageTable::PAGE_SIZE != 0)
      ++_size;

   unsigned long return_addr;
   struct free_elem *t = free_list;
   struct alloc_elem *p = alloc_list;
   unsigned long new_frame;

   //Search for free_elem in free_list for the specified size
   while(t != NULL)
   {
      if (t->size < _size)
      {
         t = t->next;
      }
      else if (t->size == _size)//exact match
      {
         return_addr = t->address;
         t->size = -1;  //elem is set to -1 to be used later
         break;
      }
      else  //make the free space available smaller in size
      {
         return_addr = t->address;
         t->address += _size * PageTable::PAGE_SIZE;
         t->size    -= _size;
         break;
      }
   }

   if (t == NULL) //no free page found
      Console::puts("No free space available!! Please free space before allocation.");
   else  // insert the new elem in alloc_list
   {
      while(p->next != NULL && p->size != -1)
      {
         p = p->next;
      }
   
      if(p->size != -1 && p->next == NULL) //No free alloc_elem found. Create one
      {
         if (p + sizeof(struct alloc_elem) > (alloc_list + PageTable::PAGE_SIZE))  //overflow.
         {
            new_frame = frame_pool->get_frame();
            p->next = (struct alloc_elem *)(PageTable::PAGE_SIZE * new_frame);
         }
         else
         {
            p->next = p + sizeof(struct alloc_elem);
         }
   
         p = p->next;
      }

      p->size = _size;
      p->address = return_addr;
      p->next = NULL;

      return return_addr;
   }

}


void VMPool::release(unsigned long _start_address)
{
   
   struct free_elem *t = free_list;
   struct alloc_elem *p = alloc_list;
   int size;
   unsigned long new_frame;

   while(p != NULL)
   {
      //find start address
      if(p->address == _start_address && p->size != -1)
      {  
         size = p->size;
         p->size = -1;  
         break;
      }

      p = p->next;
   }

   if(p == NULL)
      Console::puts("Invalid start address!");
   else
   {
      while(t->next != NULL && t->size != -1)
      {
         t = t->next;
      }

      if(t->size != -1 && t->next == NULL)   //alloc a new free_elem
      {
         if(t + sizeof(struct free_elem) > (free_list + PageTable::PAGE_SIZE))  //overflow
         {
            new_frame = frame_pool->get_frame();
            t->next = reinterpret_cast<struct free_elem *>(new_frame*PageTable::PAGE_SIZE);
         }
         else
         {
            t->next = t + sizeof(struct free_elem);
         }

         t = t->next;
      }

      t->size = size;
      t->address = _start_address;
      t->next = NULL;     
   }
}

BOOLEAN VMPool::is_legitimate(unsigned long _address)
{

   struct alloc_elem *p = alloc_list;

   while(p != NULL)
   {
      if (_address >= p->address && _address <= (p->address + PageTable::PAGE_SIZE * p->size)) //automatically takes care of the condition when p->size is -1 
         return true;
   
      p = p->next;
   }

   return false;
}
         


