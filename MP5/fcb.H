
struct fcb_data{
	int file_id;
	char owner[20];
	bool permissions[3];
	int fileSize;
    int first_block;
};

class fcb{

private:
	int file_id;
	char owner[20];
	bool permissions[3];
	int fileSize;
    int first_block;

public:
	fcb(int _file_id);
    fcb(struct fcb_data);
	int getId();
	void setId(int _file_id);
	char * getOwner();
        void setOwner(char *str);
	bool * getPermissions();
        void setPermissions(bool *str);
	int getFileSize();
        void setFileSize(int size);
    int GetFirstBlock();
    void SetFirstBlock(int block);
    static int getFCBSize();
    
    struct fcb_data getFCBData();
    void SetfcbData(struct fcb_data* new_data);
};
