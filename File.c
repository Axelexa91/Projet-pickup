#include "File.h"

File *NewFile() {
	File *tmp;
	tmp = (File*)malloc(sizeof(File));
	if (tmp != NULL) {
		tmp->head = NULL;
		tmp->tail = NULL;
		tmp->size = 0;
	}
	return tmp;
}

ElemFile  *NewElemFile(int data) {
	ElemFile *tmp;
	tmp = (ElemFile *)malloc(sizeof(ElemFile));
	if (tmp != NULL) {
		tmp->data = data;
		tmp->next = NULL;
	}
	return(tmp);
}

void Emfiler(File *LaFile, int data)
{
	if (LaFile == NULL) return;
	ElemFile *tmp = NewElemFile(data);
	LaFile->tail->next = tmp;
	LaFile->tail = tmp;
} 

int Defiler(File *LaFile) {
	if (LaFile == NULL) return; 
	int temp;
	ElemFile *tmp = LaFile->head;
	LaFile->head = tmp->next;
	temp = tmp->data;
	free(tmp);
	return temp;
}