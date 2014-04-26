#include "freeList.H"

freeList::freeList()
{
}

int freeList::getBlock()
{
	int i;
	for(i=0;i<Block_Count;i++)
	{
		if(vector[i]==false){
			vector[i] = true;
			return i+1;
		}
	}
	return -1;
}

void freeList::releaseBlock(int _block_no)
{
	vector[_block_no - 1] = false;
	return;
}
