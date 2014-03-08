
#include "page_table.H"
#include "frame_pool.H"
#include "paging_low.H"

  unsigned int       PageTable::paging_enabled;     /* is paging turned on (i.e. are addresses logical)? */
  FramePool*         PageTable::kernel_mem_pool;    /* Frame pool for the kernel memory */
  FramePool*         PageTable::process_mem_pool;   /* Frame pool for the process memory */
  unsigned long      PageTable::shared_size;        /* size of shared address space */
  PageTable*         PageTable::current_page_table;

PageTable::PageTable () {
    /*local variables to this constructor */
    unsigned long address =0;
    unsigned int i;
    unsigned long * page_table_local;

    page_directory = (unsigned long *)(((kernel_mem_pool->get_frame()) << 12));

    Console::putui((unsigned long)page_directory); 
    /* The first entry i.e. page_directory[0] corresponds to 0-4 MB - common shared area which is the first entry in page directory and other 1023 entries are loaded with address 0 [does not matter as flag is set as "not present"] */
    
    page_directory[0] = (PageTable::kernel_mem_pool->get_frame());

    Console::putui(page_directory[0]);
//u might need to shift frame address to first 20bits

    page_directory[0]  = page_directory[0] << 12; 
    page_table_local = (unsigned long *)page_directory[0];
    page_directory[0] = page_directory[0] | 3;
    
    for (i=1; i<1024; i++)
    {
        page_directory[i] = 0 | 2;
    }

    /*Loading up the first page table which covers the 0-4 MB which will be obtained via page directory entry [0] */
    
    for(i=0; i<1024; i++)
    {
        page_table_local[i] = address | 3;
        address = address + 4096;
    }
   
    //page_directory |= 3;
    //for(;;); 
}


void PageTable::init_paging(FramePool * _kernel_mem_pool,
                            FramePool * _process_mem_pool,
                            const unsigned long _shared_size)
{
    PageTable::kernel_mem_pool = _kernel_mem_pool;
    PageTable::process_mem_pool = _process_mem_pool;
    PageTable::shared_size = _shared_size;
}

void PageTable::enable_paging(){
    paging_enabled = 1;
    write_cr0(read_cr0() | 0x80000000);    
}


void PageTable::load(){

    current_page_table = this;
    write_cr3((unsigned long)page_directory);

}

void PageTable::handle_fault(REGS *_x){

    unsigned long *pg_dir = (unsigned long *)read_cr3();
    unsigned int temp;
    unsigned long address;
    unsigned long *page_table_local;

    temp = _x->err_code;
    temp &= 7;

    if((temp & 1) == 0)
    {
      address = read_cr2();
      //Console::puts("the above is the address\n");
      if(pg_dir[address>>22] & 1 == 1)
      {
         page_table_local = (unsigned long *)(((pg_dir[address>>22])>>12)<<12);
         page_table_local[(address>>12) & 0x03FF] =  PageTable::process_mem_pool->get_frame();
         page_table_local[(address>>12) & 0x03FF] =  ((page_table_local[(address>>12) & 0x03FF]) << 12) | 5;

       
      }
      else
      {
         unsigned long t = PageTable::kernel_mem_pool->get_frame();
         pg_dir[address>>22] = t;
         pg_dir[address>>22] =( (pg_dir[address>>22]) << 12) | 3;
         page_table_local = (unsigned long *)(((pg_dir[address>>22])>>12)<<12);
      
         t = (t << 12) |3;
          
         for(int i=0; i<1024; i++)
         {
            page_table_local[i] = 4;
         }
         
         page_table_local[(address>>12) & 0x03FF] =  PageTable::process_mem_pool->get_frame();
         page_table_local[(address>>12) & 0x03FF] =  ((page_table_local[(address>>12) & 0x03FF]) << 12) | 3;

      }
    }
}


