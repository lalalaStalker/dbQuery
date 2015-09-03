/*Laura Stalker
LS451494
lstalker@albany.edu
*/


#include <stdio.h>  /*Include the standard input/output library*/
#include <stdlib.h>  /*To use exit function*/
#include <string.h>  /*String library for string manipulation functions*/
#include "constants.h"  /*Header file with constants*/
#include "prototypes.h"  /*Header file with prototypes*/


/*A function that takes two strings and an int as param and returns an int.
The two strings are assumed to be the name of a relation and the name
of an attribute, respectively. This function adds ".sch\0" to the end 
of the given relation name, to create the name of the schema file for 
this relation. The schema file is then opened, and attribute after 
attribute is compared to the attribute name given as a parameter. If the
attribute is found for this relation, its type and number of bytes are
printed to stdout. If it is not found, an error message prints. The flag 
param is assumed to be 1 if the user would like this printing to happen,
and something else if not. The int returned tells if the attribute belongs 
to the given relation (1 if yes, otherwise something else)
*/

int infattr( char *relationName, char *attrName, int flag){

	FILE *schemaFile;  /*Ptr to schema file we will name from relation*/
	int numAttr;  /*For storing number of attributes*/
	char name[MAX_STRING];  /*For storing string name in schema file*/
	char type[CHAR_LENGTH];  /*For storing char type in schema file*/
	int bytes;  /*For storing number of bytes for this attribute*/
	int attrFound;  /*int to tell if attr is in this relation*/

	/*Create an array large enough for a string that is the attribute
	name plus ".sch\0". This will be the name of the schema file. 
	*/
	char fileName[ strlen( relationName) + strlen( ".sch\0")];
	strcpy( fileName, relationName);
	strcat( fileName, ".sch\0");

	/*Open the schema file for reading. If unable to open, print an
	error message and exit.
	*/
	if( (schemaFile = ( fopen( fileName, "r"))) == NULL ){
		printf("There was an error trying to open schema file.\n");
		exit(1);
	}

	fscanf( schemaFile, "%d", &numAttr);  /*Get first int in schema file*/
	attrFound = 0;  /*Set attr not found*/

	/*While there are more attributes in the schema file, get their 
	names, types, and number of bytes. Then check if any of them match
	the attribute name passed as a param (specified in the query). If 
	one of them is a match, print the type and number of bytes for the
	attribute and return.
	*/
	while( numAttr > 0){
		fscanf( schemaFile, "%s%s%d", name, type, &bytes);
		if( strcmp( attrName, name) == 0){
			if( flag == 1){  /*If printing wanted*/
				printf("%s is an %s type, occupying %d bytes.\n\n", 
						name, type, bytes);
				fflush( stdout);
			}
			attrFound = 1;  /*Attribute is in this relation*/
			return attrFound;
		}
		numAttr--;
	}

	/*If the attribute was not found for this relation, print a message
	*/
	printf(" Error: %s attribute is not part of %s relation.\n\n", 
			attrName, relationName);
	fflush( stdout);

	/*Close schema file
	*/
	if( fclose( schemaFile) == EOF){
		fprintf( stderr, "Error closing schema file.\n");
		exit(1);
	}

	return attrFound;
}