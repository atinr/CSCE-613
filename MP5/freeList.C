#include "freeList.H"

freeList::freeList(char* _vector, int _block_count, bool init):vector(_vector), Block_Count(_block_count)
//freeList::freeList()
{
	int i;
   if (init)
   {
	   for( i = 0; i < Block_Count / 8 ;i++)
	   {
		   vector[i]= 0 ;
	   }
   }
}

int freeList::getBlock()
{
	int i,j;
	for( i= 0 ; i < Block_Count / 8 ;i++)
	{
		if(vector[i]!= 0xFF ){
			for( j = 0 ; j < 8 ; j++ )
			{
				if(((vector[i]>>( 7 - j )) & 1) == 0 )
				{
					return 8 * i  +  j + 1;
				}
			}
		}
	}
	return -1;
}

void freeList::releaseBlock(int _block_no)
{
	vector[(_block_no - 1)/8] = vector[(_block_no - 1)/8]|(1<<(7-((_block_no-1)%8)));
	return;
}

bool freeList::allocBlock(int _block_no)
{
   
}
