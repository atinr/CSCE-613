
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
    
    page_directory = (unsigned long *)(((process_mem_pool->get_frame()) << 12));

    Console::putui((unsigned long)page_directory); 
    /* The first entry i.e. page_directory[0] corresponds to 0-4 MB - common shared area which is the first entry in page directory and other 1023 entries are loaded with address 0 [does not matter as flag is set as "not present"] */
    
    page_directory[0] = (PageTable::process_mem_pool->get_frame());

    Console::putui(page_directory[0]);
//u might need to shift frame address to first 20bits

    page_directory[0]  = page_directory[0] << 12; 
    page_table_local = (unsigned long *)page_directory[0];
    page_directory[0] = page_directory[0] | 3;
    
    for (i=1; i<1024; i++)
    {
        page_directory[i] = 0 | 2;
    }
     page_directory[1023]=((unsigned long)page_directory) | 3; 

    /*Loading up the first page table which covers the 0-4 MB which will be obtained via page directory entry [0] */
    
    for(i=0; i<1024; i++)
    {
        page_table_local[i] = address | 3;
        address = address + 4096;
    }
   
    //page_directory |= 3;
    //for(;;);
    
   for(i=0; i<VM_REGISTER_SIZE; i++)
    {
        register_dir[i] = NULL;
    }
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

unsigned long LogicalPT (unsigned long LogicalAddress)
{
    unsigned long ThousandTwentyThree = 0x03FF; //1023 =  2^10-1
    ThousandTwentyThree = ThousandTwentyThree << 22;
    //Console::putui(ThousandTwentyThree);
    LogicalAddress = (LogicalAddress >> 12) << 2;
    //Console::putui(LogicalAddress);
    LogicalAddress = LogicalAddress | ThousandTwentyThree ;
    //Console::putui(LogicalAddress);
    
    return LogicalAddress;
}

unsigned long LogicalPD (unsigned long LogicalAddress)
{
    unsigned long ThousandTwentyThree = 0xFFFFF; //1048575 = 2^20-1
    ThousandTwentyThree = ThousandTwentyThree << 12;
    //Console::putui(ThousandTwentyThree);
    LogicalAddress = (LogicalAddress >> 22) << 2;
    //Console::putui(LogicalAddress);
    LogicalAddress = LogicalAddress | ThousandTwentyThree ;
    //Console::putui(LogicalAddress);
    return LogicalAddress;
}


void register_vmpool(VMPool *_pool){

    unsigned int i=0;
    
    while((register_dir[i] != NULL) && (i<VM_REGISTER_SIZE))
    {i++;
    }
    
    if(i<VM_REGISTER_SIZE)
        register_dir[i] = _pool;
    else
        Console::puts("VM Register Files in page Table Exceeded \n");
    
}

void free_page(unsigned long _page_no)
{
    unsigned long PhysicalFrameNo;
    unsigned long * PageTableEntryPtr;
    PageTableEntryPtr = (unsigned long *)LogicalPT (_page_no);
    PhysicalFrameNo = (*PageTableEntryPtr) >> 12;
    *PageTableEntryPtr = 0;
    FramePool::release(PhysicalFrameNo);
}



void PageTable::handle_fault(REGS *_x){

    //signed long *pg_dir = (unsigned long *)read_cr3();
    unsigned int temp;
    unsigned long address;
    //unsigned long *page_table_local;
    unsigned long *dir_addr, *page_addr, *temp_addr;

    temp = _x->err_code;
    temp &= 7;

    if((temp & 1) == 0)
    {
      address = read_cr2();
      //Console::puts("the above is the address\n");
      //Console::putui(address);
      dir_addr = (unsigned long *)LogicalPD(address);
      page_addr = (unsigned long *)LogicalPT(address);
      //Console::putui(LogicalPD(address));
      //Console::putui((unsigned long)dir_addr);
      //Console::putui(LogicalPT(address));
      //Console::putui((unsigned long)page_addr);

      //while(1){}
      if(*dir_addr & 1 == 1)
      {
         //page_table_local = (unsigned long *)(((pg_dir[address>>22])>>12)<<12);
         //page_table_local[(address>>12) & 0x03FF] =  PageTable::process_mem_pool->get_frame();
         //page_table_local[(address>>12) & 0x03FF] =  ((page_table_local[(address>>12) & 0x03FF]) << 12) | 5;
         *page_addr = PageTable::process_mem_pool->get_frame();
         *page_addr = (*page_addr << 12) | 5;
       
      }
      else
      {
         unsigned long t = PageTable::process_mem_pool->get_frame();
         /*pg_dir[address>>22] = t;
         pg_dir[address>>22] =( (pg_dir[address>>22]) << 12) | 3;
         page_table_local = (unsigned long *)(((pg_dir[address>>22])>>12)<<12);
      
         t = (t << 12) |3;
          
         for(int i=0; i<1024; i++)
         {
            page_table_local[i] = 4;
         }
         
         page_table_local[(address>>12) & 0x03FF] =  PageTable::process_mem_pool->get_frame();
         page_table_local[(address>>12) & 0x03FF] =  ((page_table_local[(address>>12) & 0x03FF]) << 12) | 3;*/
         *dir_addr = t;
	 *dir_addr =(( *dir_addr) << 12) | 3;

         address = (address>>22)<<22;
         for(int i=0; i<1024; i++)
         {
	    
            temp_addr = (unsigned long *)LogicalPT(address);
            *temp_addr = 4;
	    address = ((address>>12)+1)<<12;
         }

         *page_addr = PageTable::process_mem_pool->get_frame();
         *page_addr =  ((*page_addr) << 12) | 3;

      }
    }
}


