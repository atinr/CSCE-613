#define Block_Count 10*2*1024

class freeList{

private:
	char vector[Block_Count/8];

public:
	freeList();
	int getBlock();
	void releaseBlock(int _block_no);
};
