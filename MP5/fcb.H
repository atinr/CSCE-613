class fcb{

private:
	char *id;
	char *owner;
	bool *permissions;
	int fileSize;

public:
	fcb();
	char * getId();
	void setId(char *str);
	char * getOwner();
        void setOwner(char *str);
	bool * getPermissions();
        void setPermissions(bool *str);
	int getFileSize();
        void setFileSize(int size);
};