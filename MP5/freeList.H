//#define Block_Count 10*2*1024

class freeList{

private:
	char *vector;  //[Block_Count/8];
   int Block_Count;

public:
	freeList(char *, int _block_count, bool init);
	int getBlock();
	void releaseBlock(int _block_no);
   void allocBlock(int _block_no);
};
