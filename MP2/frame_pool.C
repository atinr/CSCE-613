#include "utils.H"
#include "frame_pool.H"
#include "console.H"

FramePool::FramePool(unsigned long _base_frame_no,
          unsigned long _nframes,
          unsigned long _info_frame_no):base_frame_no(_base_frame_no), 
          nframes(_nframes),info_frame_no(_info_frame_no)
{

   bytes_count = _nframes/4;
   if ( nframes % 4 != 0)
      ++bytes_count;
   if (_info_frame_no != 0)
   { 
      bitmap = (unsigned char *)((_info_frame_no)*4096);  
      for (unsigned long i = 0; i<bytes_count; ++i)
      {
          bitmap[i] = 0;    //initialising all pages as unused
      } 

   }
   else
   {
      bitmap = (unsigned char *)(base_frame_no*4096);
      memset(bitmap, 0, 4096);
      get_frame();
   }

}


unsigned long FramePool::get_frame()
{
   unsigned long i = 0, frame_number = 0;
   unsigned char empty_frame = 0, val = 1;
   while((bitmap[i] & 0x55) == 0x55)   //Checking if all the frames have been allocated
                                                   //2 bits for each frame bit 0-> allocated/non-allocated
                                                   //                          1-> inaccessible bit
   {
      ++i;
      if(i >= bytes_count)
      {
         //TODO
         //All frames allocated. Frames have to be overwritten
         Console::puts("All bytes allocated\n");
         return 0;
      }
   
   }
   
   empty_frame = ((bitmap[i] | 0xAA) ^ 0xFF);   //gets the empty_frame bits in the current bitmap

   if (empty_frame == 0)
      Console::puts("Error found\n");

   while((empty_frame & val) == 0)
   {
       ++frame_number;
       val = val << 2;
//       Console::puts("Hello World infinite\n");
   }
      
   bitmap[i] |= val;
      
   frame_number += (4 * i);

   return ((frame_number + base_frame_no));

}


void FramePool::mark_inaccessible(unsigned long _base_frame_no,
                       unsigned long _nframes)
{
   unsigned long base_frame = (_base_frame_no) - base_frame_no;
   if(base_frame < 0 || base_frame > (nframes))
   {
      //Invalid frame.Out of Scope
      return; 
   }
   
   unsigned long id = base_frame/4;
   unsigned char val = (3 << (base_frame - (id * 4)));
   unsigned long count = 0;
   while(count < _nframes)
   {
      bitmap[id] |= val;
      if(val == 0b11000000)
      {
         val = 3;
         ++id;
      }
      else
      {
         val = val << 2;
      }

      ++count;
   }
}


//_frame_no is the physical frame address being passed(22 bits)
void FramePool::release_frame(unsigned long _frame_no)
{
   unsigned long current_frame_no = ((_frame_no) - base_frame_no);
  
   if (current_frame_no > nframes || current_frame_no < 0)
   {
      //("Invalid frame number to be released");
      return;
   }

   unsigned long id = current_frame_no/4;
   unsigned char byte = bitmap[id];    
   unsigned char val = (3 << (current_frame_no - (id * 4)));
   if (byte & val == val)
   {
      //frame inaccessible. Frame cannot be released.
   }
   else
   {
      bitmap[id] &= ~val;  //Frame released
   }
}
