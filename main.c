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
int add(struct student_records *current, struct student_records *cursor, struct student_records *prev){
	cursor->next = current;
	printf("@add, cursor is %d\n", cursor->id);
	*cursor = *current;
	printf("@add, current is %d, after adding, cursor is %d\n", current->id, cursor->id);
	return 0;
}	//Move the cursor forward to the current node

//Remove nodes from linked list. 
int delete(struct student_records *current, struct student_records *cursor, struct student_records *prev){
	return 0;
}

int update(struct student_records *current, struct student_records *cursor, struct student_records *prev){
	return 0;
}
//After all need to sort the list.
//int sort(){}

//Then write back from the list to the original file. 
//int writeBack(){}

//read all the input data, create database.
int compareCommand(char *cmd, char *str){
	int i;
	for ( i = 0; *(cmd + i) != '\0'; i++ )
		if (*(cmd + i) != *(str + i))
			return -1;
	return 0;
}

int readData(FILE *file, struct student_records *cursor, struct student_records *prev){
	char *buffer;
	char *cmd = (char *)malloc(sizeof(char)* 8);
	while (fgets(buffer, 128, file) != NULL){
		//printf("buffer is: %s, end here\n", buffer);
		struct student_records *current =(struct student_records *)malloc(sizeof(struct student_records));
		current->firstName = (char *)malloc(sizeof(char)*20);
		current->lastName = (char *)malloc(sizeof(char)*20);
		current->major = (char *)malloc(sizeof(char)*4);
		sscanf(buffer, "%s %d %s %s %f %s ", cmd, &(current->id), current->firstName, current->lastName, 
				&(current->gpa), current->major); 
		//printf("current cmd: %s, current id: %d, first name: %s, last name: %s, gpa: %.2f, major: %s\n", cmd, current->id, current->firstName, current->lastName, current->gpa, current->major);
		if (compareCommand (cmd, "ADD") == 0)
			add(current, cursor, prev);
		else if(compareCommand (cmd, "DELETE") == 0)
			delete(current, cursor, prev);
		else if (compareCommand(cmd, "UPDATE") == 0)
			update(current, cursor, prev);

	}

	return 0;
}


int printAll(struct student_records *head, struct student_records *cursor){
	head->next = cursor;
	printf("@Before printAll while loop, cursor is %d\n", cursor->id);
	//move the cursor to the second node b/c head is empty
	while ( cursor->firstName != NULL ){
		printf("@printAll while loop\n");
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
int printToFile( FILE *f, struct student_records *head, 
		struct student_records *cursor){
	cursor = head->next;
	while (cursor != NULL){
		fprintf(f, "%d %s %s %.2f %s", 
				cursor->id, cursor->firstName, cursor->lastName, cursor->gpa, cursor->major);
		
		cursor = cursor->next;
	}
	fclose(f);
	return 0;
}

int errorCase(char c){
	switch(c){
		case 'f':
			printf("FILE EXISTS");
			break;
		case 'q':
			printf("NO QUERY PROVIDED");
			break;
		case 'p':
			printf("FAILED TO PARSE FILE");
			break;
		case 's':
			printf("STUDENT RECORD CANNOT BE DELETED NOR UPDATED");
			break;
		case 'i':
			printf("ID NOT UNIQUE");
			break;
		case 'n':
			printf("STUDENT RECORD NOT FOUND");
			break;
		default: 
			printf("OTHER ERROR");
			break;
	
	}
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
  struct student_records *prev = head;
  int vflag = 0;	//unless -v is called
  int oflag = 0;	//unless -o is called
  int withv = 0;
  int c;
  int id = -1;
  char *lastname;
  char *major;
  char *input = *(argv + 1);
  char *filename;
  FILE *f = stdout;	//unless specified by -o that there's an output file
  FILE *fin;
  
  if (access(input, F_OK) != -1){  //File exists
  	  //Tested, input file name is parsed
	  fin = fopen(input, "r");
	  readData(fin, cursor, prev);
  }else {
  	errorCase('p');
  }

  opterr = 0;
  while ((c = getopt (argc, argv, "vi:f:m:o:" )) != -1 ){
  	switch(c){
		case 'v': 
			  vflag = 1;
			  break;
		case 'i': 
			  withv = -1;
			  id = atoi(optarg);
			  break;
		case 'f': 
			  withv = -1;
			  lastname = optarg;
			  break;
		case 'm': 
			  withv = -1;
			  major = optarg;
			  break;
		case 'o': 
			  filename = optarg;
			  break;
			  
		case '?': 
			  errorCase('a');
			  break;
			  
		default: 
			  abort();
	}
  }
  //Check the flags
  //Sorry for the stupid long if-else
 
	if ( vflag == 1 && withv != -1 ){
		//Tested, program reached here
		printAll(head, cursor);
		}
	else {
		if (id != -1)
			printID();
		if (lastname != NULL)
			printLastName();
		if (major != NULL)
			printMajor();
		if (filename != NULL){
			 if (access(filename, F_OK) != -1 ){
			  
			  printf("File exists. Override? Y/N\n");
			  char c;
			  scanf(" %c", &c);
			  if (c == 'y' || c == 'Y'){
			  f = fopen(filename, "w+");	
			  printToFile(f, head, cursor);
			  }else {
			  	errorCase('f');
			  }
		}
			  else {
			  f = fopen(filename, "w+");
			  printf("created new file %s\n", filename);
			  }  
		}
	}
 
		 return 0;


}
