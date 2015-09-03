/*Laura Stalker
LS451494
lstalker@albany.edu
*/

#include <stdio.h>  /*Include the standard input/output library*/
#include <stdlib.h>  /*To use exit function*/
#include <string.h>  /*String library for string manipulation functions*/
#include "constants.h"  /*Header file with constants*/
#include "prototypes.h"  /*Header file with prototypes*/


/*A function that takes a pointer to a text file and a string as arguments 
and returns an int.  The text file is assumed to be a config file whose 
first line consists of an int which corresponds to the number of relations 
in the database. Each successive line contains a string which corresponds 
to the name of a relation in the db. (It is assumed that the number of strings
present in the file will match the number that appeared on the first line of 
the file.) The function checks the entire file for a relation name that 
matches the string passed as a param. If a match is found, the function 
returns 1, otherwise it returns 0.
*/

int checkRelation( FILE *configFile, char *relationName){

	int numRelations;  /*Number of relations in the file*/
	char newRelation[MAX_STRING];  /*A temp string to hold the next relation*/

	rewind( configFile);  /*Start at beginning to check all relations*/
	fscanf( configFile, "%d", &numRelations);  /*Get numRelations*/

	/*While there are still more relation names in the file, compare each one 
	to the string parameter. If a match is found return 1, else return 0.
	*/
	while( numRelations > 0){
		fscanf( configFile, "%s", newRelation);
		if( strcmp( newRelation, relationName) == 0){
			return 1;
		}
		numRelations--;
	}
	return 0;
}