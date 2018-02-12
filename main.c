#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

//Linked list
struct student_records{
	int id;
	char *firstName;
	char *lastName;
	float gpa;
	char *major;
	struct student_records *next;
};

//Add node to list. 
int add(struct student_records *p, struct student_records *ptr){
	ptr->next = p;
	ptr = p;
	return 0;
}	//Move the cursor forward to the current node

//Remove nodes from linked list. 
//int remove(){}

//After all need to sort the list.
//int sort(){}

//Then write back from the list to the original file. 
//int writeBack(){}

//Read all the input data, create database.
int readData(FILE *file, struct student_records *cursor){
	//Read from file, line by line loop.
	char *buffer;
	while (fgets(buffer, sizeof(buffer), file) != NULL){
		struct student_records *current =(struct student_records *)malloc(sizeof(struct student_records));
		sscanf(buffer, "%d %s %s %f %s",
		&(current->id), current->firstName, current->lastName, &(current->gpa), current->major);
		 add(current, cursor);
	}

	return 0;
}


int printAll(struct student_records *head, struct student_records *cursor){
	cursor = head->next;	//move the cursor to the second node b/c head is empty
	while (cursor != NULL){
		printf("%d %s %s %.2f %s",
				cursor->id, cursor->firstName, cursor->lastName, cursor->gpa, cursor->major);
		cursor = cursor->next;
	}
	return 0;
}

int printID(){
	return 0;
}
int printLastName(){
	return 0;
}
int printMajor(){
	return 0;
}
int printToFile(){
	printf("reach printToFile");
	return 0;
}

int main(int argc, char** argv) {
  /*
  *   
  printf("%d %s %s %.2f %s\n", id, first_name, last_name, gpa, major);
  */
  struct student_records records = {
  	.next = NULL
  };
  
  struct student_records *head;
  head = &records;
  struct student_records *cursor = head;
  int vflag = 0;	//unless -v is called
  int oflag = 0;	//unless -o is called
  char *optarg;
  int optind;
  int c;
  char *filename;
  FILE *f;

  opterr = 0;
  while ((c = getopt (argc, argv, "vi:f:m:o:" )) != -1 ){
  	switch(c){
		case 'v':{ 
			vflag = 1;
			  printAll(head, cursor);}
			  break;
		case 'i': if (vflag == 0){
			  	printID();
			  }else{
			  
			  }
			  break;
		case 'f': if(vflag == 0){
				  printLastName();
			  }else{
			  
			  }
			  break;
		case 'm': if (vflag == 0){
				  printMajor();
			  }else{
			  
			  }
			  break;
		case 'o': {
			  oflag = 1;
			  printf("before filename\n");
			  filename = optarg;
			  printf("after filename\n");
			  f = fopen(filename, "r+");	
			  printf("file name is %s", filename);
			  readData(f, cursor);
			  printf("after readData");
			  printToFile();
			  }
			  break;
		case '?': printf("error occured when reading opts");
			  
			  break;

	}
  }
  
  return 0;
}
