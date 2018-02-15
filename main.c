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
	int p;
};


int errorCase(char);	
double checkGPA(char*);
//Add node to list. 
int add(struct student_records *current, struct student_records *cursor){
	int added;
	while (cursor->next != NULL) {
		if(current->id > cursor->next->id){
			added = 0;
			cursor = cursor->next;
			//printf("moved forward\n");
		}//find the first one greater than current id, if found:
		else if (current->id < cursor->next->id){	
			current->next = cursor->next;
			cursor->next = current;
			added = 1;
		}else errorCase('i');
	}	//
	if (cursor->next == NULL && added != 1 ){
	cursor->next = current;
	cursor = current;
	//printf("added in the end\n");
}
	return 0;
}	

//Remove nodes from linked list. 
int delete(int id, struct student_records *cursor){
	int deleted = -1;
	while (cursor->next != NULL && deleted != 1){
		if(cursor->next->id == id ){
			cursor->next = cursor->next->next;
			deleted = 1;
			return 0;
			//printf("deleted\n");
		} else {
			deleted = 0;
			cursor = cursor->next;
			//printf("move forward\n");
		}
		
	}
	if (cursor->id == id && cursor->next == NULL && deleted == 0){
		free(cursor);
		deleted = 1;
		return 0;
		//printf("deleted from tail\n");
	}
	if (deleted == 0)
		errorCase('s');
	else if (deleted == -1) 
		errorCase('n');
	else return 0;
}

int update(struct student_records *current, struct student_records *cursor){
	int updated = -1;
	if (cursor != NULL && cursor->id == current->id){
		current->next = cursor->next;
		cursor = current;
	}
	//from this line check one node ahead so I'll be able to just insert a new one, should work
	while (cursor->next != NULL && updated != 1){
		if(cursor->next->id != current->id){
			updated = 0;
			cursor = cursor->next;
		}else {
			updated = 1;
			current->next = cursor->next->next;	//doesn't matter if null
			cursor->next = current;
			//printf("updated %s", cursor->next->firstName);	
		}	//it should be doable since i can just add new node and rm old node
		// here i'm missing the check for cursor itself so if i check it then should be correct
	
	}
	if (updated == 0){
		errorCase('s');
	}else if (updated == -1) errorCase('n');
	
	return 0;
}

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

int convert(char* str, int flag){
	int i;
	for (i = 0; *(str + i); i++){
		if (isalpha(*(str + i)) == 0){
			if (!flag) errorCase('p');
			else errorCase('a');
		}
		if (i == 0)
			*(str) = toupper(*str);
		else *(str + i) = tolower(*(str + i));
	}
	if ( i < 3 || i > 10){
		if (!flag) errorCase('p');
		else errorCase('a');
	}
}

int convertMajor(char * str, int flag){
	//for (; *str; ++str) *str = toupper(*str);
	int i;
	for (i = 0; *(str + i); i++){
		if (isalpha(*(str + i)) == 0){
			if (!flag) errorCase('p');
			else errorCase('a');
		}
		*(str + i) = toupper(*(str + i));
	}
	if ( i != 3){
		if (!flag) errorCase('p');
		else errorCase('a');
	}
}

int readData(FILE *file, struct student_records *cursor){
	char *buffer = (char *)malloc(sizeof(char)*128);
	char *cmd = (char *)malloc(sizeof(char)* 8);
	char *gpa = (char *)malloc(sizeof(char)* 5);
	while (	fgets(buffer, 128, file) != NULL && !feof(file)){
		struct student_records *current =(struct student_records *)malloc(sizeof(struct student_records));
		current->firstName = (char *)malloc(sizeof(char)*20);
		current->lastName = (char *)malloc(sizeof(char)*20);
		current->major = (char *)malloc(sizeof(char)*5);
		current->next = NULL;
		sscanf(buffer, "%s %d %s %s %s %s ", cmd, &(current->id), current->firstName, current->lastName, gpa, current->major); 
		//printf("GPA: %s\n", gpa);
		current->gpa = (float)checkGPA(gpa);
		
		//printf("current cmd: %s, current id: %d, first name: %s, last name: %s, gpa: %.2f, major: %s\n", cmd, current->id, current->firstName, current->lastName, current->gpa, current->major);
		if(compareCommand (cmd, "ADD") == 0 )
			{
			convertMajor(current->major, 0);
			convert(current->firstName, 0);
			convert(current->lastName, 0);
			add(current, cursor);
			// printf("if add is correct should see this line at the end\n");
			}
		else if(compareCommand (cmd, "DELETE") == 0)
			delete(current->id, cursor);
		else if (compareCommand(cmd, "UPDATE") == 0){
				convertMajor(current->major, 0);
				convert(current->firstName, 0);
				convert(current->lastName, 0);
				update(current, cursor);
			}
		else {
			errorCase('p');
		}
		*cmd = 'x';
		//printf("end of while loop\n");
		
	}
	//printf("outside of while loop\n");	//Never get out of while loop, the last command can be processed though
	free(cmd);
	free(buffer);
	//printf("It's ok until the end of readData");
	return 0;	//how to?
}


int printAll(struct student_records *head, struct student_records *cursor){
	cursor = head->next;
	//printf("@after printAll, should show up if no seg fault here new id %d", cursor->id);
	//move the cursor to the second node b/c head is empty
	while ( cursor != NULL ){ 
		//printf("@printAll while loop\n");
		cursor->p = 1;
		cursor = cursor->next; 
	}
	return 0;
}

int print(FILE* output, struct student_records *head, struct student_records *cursor){
	cursor = head->next;
	while ( cursor != NULL ){
		if (cursor->p == 1)
			fprintf(output, "%d %s %s %.2f %s\n",
				    cursor->id, cursor->firstName, cursor->lastName, cursor->gpa, cursor->major);

	cursor = cursor->next;	
	}
	return 0;
}

int printID(struct student_records *head, struct student_records *cursor, int id){
	int found = 0;
	cursor = head->next;
	         while ( cursor != NULL ){
	                               //printf("@printAll while loop\n");
		                               if(cursor->id != id) cursor->p = 0;				       else{
					 found++;      } cursor = cursor->next;
					 			}
		 if (found == 0)
			 errorCase('n');
			 return 0;
}

int printLastName(struct student_records *head, struct student_records *cursor, char *ln){
	int found = 0;
	cursor = head->next;
	while (cursor != NULL){
		if (compareCommand(cursor->lastName, ln) != 0){
			cursor->p = 0;
			}else found++;
		cursor = cursor->next;
	}
	if (found == 0)
		errorCase('n');
	return 0;
}

int printMajor(struct student_records *head, struct student_records *cursor, char *mjr){
	int found = 0;
	cursor = head->next;
	        while (cursor != NULL){
			//printf("@printMajor\n");
			                if (compareCommand(cursor->major, mjr) != 0){
	                     cursor->p = 0;
	                    //printf("@printMajor, cursor->major is %s", cursor->major); 
	           }else found++;
					cursor = cursor->next;
		}
	if (found == 0)
		errorCase('n');	
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
			printf("FILE EXISTS\n");
			exit(0);
			break;
		case 'q':
			printf("NO QUERY PROVIDED\n");
			exit(0);
			break;
		case 'p':
			printf("FAILED TO PARSE FILE\n");
			exit(0);
			break;
		case 's':
			printf("STUDENT RECORD CANNOT BE DELETED NOR UPDATED\n");
			exit(0);
			break;
		case 'i':
			printf("ID NOT UNIQUE\n");
			exit(0);
			break;
		case 'n':
			printf("STUDENT RECORD NOT FOUND\n");
			exit(0);
			break;
		default: 
			printf("OTHER ERROR\n");
			exit(0);
			break;
	
	}
	return 0;
}

double checkGPA(char *str){
	double gpa;
	int i;
	for (i = 0; *(str + i); i++){
		//printf("%c ", *(str+i));
		if (i == 1 && *(str + i) != '.'){
		errorCase('p');	
		}else if (i != 1 && isdigit(*(str + i)) == 0 ){
			//printf("error thrown\n");
		errorCase('p');
		}
	}
	gpa = atof(str);
	return gpa;
}

int main(int argc, char** argv) {
  /*
  *   
  printf("%d %s %s %.2f %s\n", id, first_name, last_name, gpa, major);
  */
  struct student_records records = {
  	.next = NULL
  }; 
  struct student_records *head = &records;
  struct student_records *cursor = head;	
  int vflag = 0;	//unless -v is called
  int oflag = 0;	//unless -o is called
  int withv = 0;
  int c = 0;
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
	  readData(fin, cursor);
	  //printf("just check if my list is still alive: %d\n", cursor->next->id);
	  //printf("no seg fault so far");
	  head = &records;
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
			  convert(lastname, 1);
			  break;
		case 'm': 
			  withv = -1;
			  major = optarg;
			  convertMajor(major, 1);
			  break;
		case 'o': 
			  filename = optarg;
			  break;	  
		default: 
			  errorCase('a');
	}
  }
  if ( c == 0 || argc < 3)
	  errorCase('q');
  //Check the flags
  //Sorry for the stupid long if-else
 	printAll(head, cursor); //set all p = 1
	if ( vflag == 1 && withv == -1 ){//Don't filter 
		errorCase('a');
	}
	else if (!vflag){// Filter
		if (id != -1)
			printID(head, cursor, id);
		if (lastname != NULL)
			printLastName(head, cursor, lastname);
		if (major != NULL)
			printMajor(head, cursor, major);
		if (filename != NULL){
			 if (access(filename, F_OK) != -1 ){
			  
			  printf("File exists. Override? Y/N\n");
			  char c;
			  scanf(" %c", &c);
			  if (c == 'y' || c == 'Y'){
			  f = fopen(filename, "w+");	
			  printToFile(f, head, cursor);//it wants the database or the info I printed on the screen?
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
	print(f, head, cursor);//print filtered list
		 return 0;


}
