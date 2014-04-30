#include "fcb.H"

fcb::fcb(int _file_id)
{
//	id = (char *)malloc(20*sizeof(char));
//	owner = (char *)malloc(30*sizeof(char));
//	permissions = (bool *)malloc(10*sizeof(bool));
    file_id = _file_id;
    first_block = -1;
}

/*fcb::fcb(struct fcb_data* data)
{
    file_id = data->file_id;
    owner = data->owner;
    permissions = data->permissions;
    fileSize = data->fileSize;
    first_block = data->first_block;
}*/

int fcb::getId()
{
	return file_id;
}

void fcb::setId(int _file_id)
{
	file_id = _file_id;
}

char * fcb::getOwner()
{
	return owner;
}

void fcb::setOwner(char *str)
{
	memcpy(owner, str, 20);
}

bool * fcb::getPermissions()
{
	return permissions;
}

void fcb::setPermissions(bool *str)
{
	memcpy(permissions, str, 3);
}

int fcb::getFileSize()
{
	return fileSize;
}

void fcb::setFileSize(int size)
{
	fileSize = size;
}

int fcb::GetFirstBlock(){
    return first_block;
}

void fcb::SetFirstBlock(int block){
    first_block = block;
}

int fcb::getFCBSize(){
    return 0;
    //TO DO ;
}

/*struct fcb_data getFCBData(){
    struct fcb_data data;
    data.file_id = file_id;
    memcpy(data.owner, owner, 20);
    memcpy(data.permissions, permissions, 3);
    data.fileSize = fileSize;
    data.first_block = first_block;
    
}
void SetfcbData(struct fcb_data* new_data);
*/
