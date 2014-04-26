#define Block_Count 10*2*1024

class freeList{

private:
	bool vector[Block_Count] = {false};

public:
	freeList();
	int getBlock();
	void releaseBlock(int _block_no);
};
