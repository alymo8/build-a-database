/*
 ============================================================================
 Name        : dbReader.c
 Author      : alymo

 Description : A simple program to manage a small database of student
			   records using B-Trees for storage.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



#define MAXLEN 20
#define false 0
#define true !false


typedef struct SR {				// The student record object
	char Last[MAXLEN];
	char First[MAXLEN];
	int ID;
	int marks;
} SRecord;

typedef struct bN {				
	struct SR* Srec;
	struct bN* left;
	struct bN* right;
} bNode;


// Function Prototypes
// Notice that there are two versions of add_node, inorder, and search.
// This is to accommodate sorting the database by last name OR student ID


bNode* addNode_Name(bNode* root, SRecord* Record);
bNode* addNode_ID(bNode* root, SRecord* Record);
bNode* makeNode(SRecord* data);

void inorder(bNode* root);
void search_Name(bNode* root, char* data);
void search_ID(bNode* root, int ID);
void str2upper(char* string);
void help();


bNode* match;



int main(int argc, char* argv[]) {

	// Internal declarations

	FILE* NAMESIDS;        // File descriptor (an object)!
	FILE* MARKS;           // Will have two files open

	bNode* root_N;   		// Pointer to names B-Tree
	bNode* root_I;   		// Pointer to IDs B-Tree
	SRecord* Record;   		// Pointer to current record read in

	int NumRecords;
	char cmd[MAXLEN], sName[MAXLEN];
	int sID;

	// Argument check
	if (argc != 3) {
		printf("Usage: sdb [Names+IDs] [marks] \n\n");
		return -1;
	}

	// Attempt to open the user-specified file.  If no file with
	// the supplied name is found, exit the program with an error
	// message.

	if ((NAMESIDS = fopen(argv[1], "r")) == NULL) {
		printf("Can't read from file %s\n", argv[1]);
		return -2;
	}

	if ((MARKS = fopen(argv[2], "r")) == NULL) {
		printf("Can't read from file %s\n", argv[2]);
		fclose(NAMESIDS);
		return -2;
	}

	// Initialize B-Trees by creating the root pointers;

	root_N = NULL;
	root_I = NULL;

	//  Read through the NamesIDs and marks files, record by record.

	NumRecords = 0;

	printf("Building database...\n");

	while (true) {

		// 	Allocate an object to hold the current data

		Record = (SRecord*)malloc(sizeof(SRecord));
		if (Record == NULL) {
			printf("Failed to allocate object for data in main\n");
			return -1;
		}

		//  Read in the data.  If the files are not the same length, the shortest one
		//  terminates reading.

		int status = fscanf(NAMESIDS, "%s%s%d", Record->First, Record->Last, &Record->ID);
		if (status == EOF) break;
		status = fscanf(MARKS, "%d", &Record->marks);
		if (status == EOF) break;
		NumRecords++;

		//	Add the record just read in to 2 B-Trees - one ordered
		//  by name and the other ordered by student ID.

		root_N = addNode_Name(root_N, Record);
		root_I = addNode_ID(root_I, Record);

		//	For this demo we'll simply list each record as we receive it

	}

	// Close files once we're done

	fclose(NAMESIDS);
	fclose(MARKS);

	printf("Finished, %d records found...\n", NumRecords);


	//
	//  Simple Command Interpreter: - This is commented out until you implement the functions listed above
	//

	while (1) {
		printf("sdb> ");
		scanf("%s", cmd);					  // read command
		str2upper(cmd);

		if (0) {						// This is a temporary stub
			continue;
		}

		// This directive hides the subsequent text from
					 // the compiler until it sees #endif
		// List by Name

		if (strncmp(cmd, "LN", 2) == 0) {         // List all records sorted by name
			printf("Student Record Database sorted by Last Name\n\n");
			inorder(root_N);
			printf("\n");
		}

		// List by ID

		else if (strncmp(cmd, "LI", 2) == 0) {    // List all records sorted by ID
			printf("Student Record Database sorted by Student ID\n\n");
			inorder(root_I);
			printf("\n");
		}

		// Find record that matches Name

		else if (strncmp(cmd, "FN", 2) == 0) {    // List record that matches name
			printf("Enter name to search: ");
			scanf("%s", sName);
			match = NULL;
			search_Name(root_N, sName);
			if (match == NULL)
				printf("There is no student with that name.\n\n");
			else {
				if (strlen(match->Srec->First) + strlen(match->Srec->Last) > 15) {
					printf("\nStudent Name:\t%s %s\n", match->Srec->First, match->Srec->Last);
				}
				else {
					printf("\nStudent Name:\t\t%s %s\n", match->Srec->First, match->Srec->Last);
				}
				printf("Student ID:\t\t%d\n", match->Srec->ID);
				printf("Total Grade:\t\t%d\n\n", match->Srec->marks);
			}
		}

		// Find record that matches ID


		else if (strncmp(cmd, "FI", 2) == 0) {    // List record that matches ID
			printf("Enter ID to search: ");
			scanf("%d", &sID);
			match = NULL;
			search_ID(root_I, sID);
			if (match == NULL)
				printf("There is no student with that ID.\n\n");
			else {
				if (strlen(match->Srec->First) + strlen(match->Srec->Last) > 15) {
					printf("\nStudent Name:\t%s %s\n", match->Srec->First, match->Srec->Last);
				}
				else {
					printf("\nStudent Name:\t\t%s %s\n", match->Srec->First, match->Srec->Last);
				}
				printf("Student ID:\t\t%d\n", match->Srec->ID);
				printf("Total Grade:\t\t%d\n\n", match->Srec->marks);
			}
		}



		// Help

		else if (strncmp(cmd, "H", 1) == 0) {  // Help
			help();
		}

		else if (strncmp(cmd, "?", 2) == 0) {     // Help
			help();
		}

		// Quit

		else if (strncmp(cmd, "Q", 1) == 0) {  // Help
			printf("Program terminated...\n");
			return 0;
		}

		// Command not understood

		else {
			printf("Command not understood.\n\n");
		}
	}

}

bNode* addNode_ID(bNode* root, SRecord* data) {
	bNode* current;
	// Empty tree

	if (root == NULL) {
		root = makeNode(data);
	}

	// If not empty, descend to the leaf node according to
	// the input data.

	else {
		current = root;

		while (1) {

			if (data->ID < current->Srec->ID) {

				// New data < data at node, branch left

				if (current->left == NULL) {				// leaf node
					current->left = makeNode(data);		// attach new node here
					break;
				}
				else {									// otherwise
					current = current->left;				// keep traversing
				}
			}
			else {
				// New data >= data at node, branch right

				if (current->right == NULL) {			// leaf node
					current->right = makeNode(data);		// attach
					break;
				}
				else {									// otherwise
					current = current->right;			// keep traversing
				}
			}
		}
	}
	return root;
}

bNode* addNode_Name(bNode* root, SRecord* data) {
	bNode* current;
	// Empty tree

	if (root == NULL) {
		root = makeNode(data);
	}

	// If not empty, descend to the leaf node according to
	// the input data.

	else {
		current = root;

		while (1) {

			if (strcmp(data->Last, current->Srec->Last)<0) {

				// New data < data at node, branch left

				if (current->left == NULL) {				// leaf node
					current->left = makeNode(data);		// attach new node here
					break;
				}
				else {									// otherwise
					current = current->left;				// keep traversing
				}
			}
			else {
				// New data >= data at node, branch right

				if (current->right == NULL) {			// leaf node
					current->right = makeNode(data);		// attach
					break;
				}
				else {									// otherwise
					current = current->right;			// keep traversing
				}
			}
		}
	}
	return root;
}
/**
 * makeNode
 *
 * Creates a single instance of a bNode
 *
 * @param	int data   Data to be added
 * @return  bNode node Node created
 */

bNode* makeNode(SRecord* data) {
	bNode* node = (bNode*)malloc(sizeof(bNode));							// create new object
	node->Srec = data;									// initialize data field
	node->left = NULL;									// set both successors
	node->right = NULL;									// to null

	return node;										// return handle to new object

}

/**
 * traverse_inorder method - recursively traverses tree in order (LEFT-Root-RIGHT) and prints each node.
 */

void inorder(bNode* root) {
	if (root->left != NULL) inorder(root->left);
	if (strlen(root->Srec->First)+strlen(root->Srec->Last)>15){
			printf("%s %s\t\t%d\t%d\n", root->Srec->First,root->Srec->Last,root->Srec->ID,root->Srec->marks);
		} else if(strlen(root->Srec->First)+strlen(root->Srec->Last)==15){
			printf("%s %s\t\t%d\t%d\n", root->Srec->First,root->Srec->Last,root->Srec->ID,root->Srec->marks);
		}
		else {
			printf("%s %s\t\t\t%d\t%d\n", root->Srec->First,root->Srec->Last,root->Srec->ID,root->Srec->marks);
		}
	if (root->right != NULL) inorder(root->right);
}

void search_ID(bNode* root, int ID) {

	if (root != NULL) {
		if (ID == root->Srec->ID) {
			match = root;
			return;
		}
		else if (ID < root->Srec->ID) {
			if (root->right != NULL) search_ID(root->left, ID);
		}
		else if(ID > root->Srec->ID) {
			if (root->right!=NULL) search_ID(root->right, ID);
		}
		else {
			match = NULL;
			return;
		}
		//current = current->right;

		//printf("%d", current->Srec->ID);
	//		         else printf("couldn't find");
	}
}

void search_Name(bNode* root, char*data) {

	if (root != NULL) {

		if (strcasecmp(data, root->Srec->Last) == 0) {
			match = root;
			return;
		}
		else if (strcasecmp(data, root->Srec->Last) < 0) {
			if (root->left != NULL) search_Name(root->left, data);
		}
		else if (strcasecmp(data, root->Srec->Last) > 0) {
			if (root->right != NULL) search_Name(root->right, data);
		}
		else {
			match = NULL;
			return;
		}
	}
}



//  Convert a string to upper case


void str2upper(char* string) {
	int i;
	for (i = 0; i < strlen(string); i++)
		string[i] = toupper(string[i]);
	return;
}

// Help
// prints command list

void help() {
	printf("LN List all the records in the database ordered by last name.\n");
	printf("LI List all the records in the database ordered by student ID.\n");
	printf("FN Prompts for a name and lists the record of the student with the corresponding name.\n");
	printf("FI Prompts for a name and lists the record of the student with the Corresponding ID.\n");
	printf("HELP Prints this list.\n");
	printf("? Prints this list.\n");
	printf("Q Exits the program.\n\n");

	return;
}
