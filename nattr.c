/*Laura Stalker
LS451494
lstalker@albany.edu
*/


#include <stdio.h>  /*Include the standard input/output library*/
#include <stdlib.h>  /*To use exit function*/
#include <string.h>  /*String library for string manipulation functions*/
#include "constants.h"  /*Header file with constants*/
#include "prototypes.h"  /*Header file with prototypes*/


/*A function that takes a string as a parameter, and returns nothing. The
string param is assumed to be the name of a relation in a database. Adds 
the suffix ".sch\0" to the attribute name to get the name of the schema 
file. Opens the schema file, and reads the first int from that file, which
we assume represents the number of attributes for this relation. Prints 
that number to stdout.
*/

void nattr( char *relationName){

	FILE *schemaFile;  /*Ptr to schema file we will name from relation*/
	int numAttributes;  /*Number of attributes: found in schema file*/

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

	/*Get the first integer from the schema file, which tells the 
	number of attributes for this relation. Print out that number.
	*/
	fscanf( schemaFile, "%d", &numAttributes);
	printf("There are %d attributes for the %s relation.\n\n", 
			numAttributes, relationName);
	fflush( stdout);

	/*Close schema file
	*/
	if( fclose( schemaFile) == EOF){
		fprintf( stderr, "Error closing schema file.\n");
		exit(1);
	}

	return;
}
