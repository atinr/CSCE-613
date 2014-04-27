#include<stdlib.h>
#include "fcb.H"

fcb::fcb()
{
	id = (char *)malloc(20*sizeof(char));
	owner = (char *)malloc(30*sizeof(char));
	permissions = (bool *)malloc(10*sizeof(bool));
}

char * fcb::getId()
{
	return id;
}

void fcb::setId(char *str)
{
	id = str;
}

char * fcb::getOwner()
{
	return owner;
}

void fcb::setOwner(char *str)
{
	owner = str;
}

bool * fcb::getPermissions()
{
	return permissions;
}

void fcb::setPermissions(bool *str)
{
	permissions = str;
}

int fcb::getFileSize()
{
	return fileSize;
}

void fcb::setFileSize(int size)
{
	fileSize = size;
}
