/*Laura Stalker
LS451494
lstalker@albany.edu
*/


/*Fucntion prototypes
*/

/*Function that checks if a relation is in database
*/
int checkRelation( FILE *configFile, char *relationName);

/*Function that gets the number of attributes for a relation
*/
void nattr( char *relationName);

/*Function that gets the length in bytes of a tuple for a relation
*/
int tuplen( char *relationName, int flag);

/*Function that gets type and number of bytes for an attribute
*/
int infattr( char *relationName, char *attrName, int flag);

/*Function that gets the number of tuples for a relation in db
*/
int count( char *relName, int flag);

/*Function that gets all distinct values for a given attribute
*/
void project( char *relName, char *attrName);

/*Function that gets the type of an attribute
*/
int whatType( char *relName, char *attrName, int wantBytes);

/*Function that gets all tuples that satisfy a given condition
*/
void select( char *relName, char *attrName, char *relop, int value, 
				char *string, char *type);