#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<syslog.h>
#include<errno.h>

#define HOME_ENV_NAME "HOME"
#define PATH_ENV_NAME "PATH"
#define MAX_LINE_LEN 1024
#define DEBUG 0

char *gHOME = NULL;
char *gPATH = NULL;

/*
 * Replaces 'rep' with 'with' in the string 'orig'
 * Returns modified string obtained after replacement
 * Returns NULL if 'rep' is not found in 'orig'
 *
*/
char *str_replace(char *orig, char *rep, char *with) 
{
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
	return NULL;
    if (!rep)
    	rep = "";
    len_rep = strlen(rep);
    if (!with)
    	with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
	ins = tmp + len_rep;
    }

// first time through the loop, all the variable are set correctly
// from here on,
//    tmp points to the end of the result string
//    orig points to the remainder of orig after "end of rep"
tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

if (!result)
	return NULL;

while (count--) {
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep; // move to next "end of rep"
}
strcpy(tmp, orig);
if(result[strlen(result)] == '\n') 
	result[strlen(result)] == NULL; 
return result;
}

/*  
 * Emits current working directory as the default prompt
 */
void emit_my_prompt()
{
	char curr_working_dir[100];
	if(getcwd(curr_working_dir, 100) == NULL)
	{
		printf("Warning: Error setting prompt. errno:%d\n", errno);
	}
	else
	{
		printf("%s>", curr_working_dir);
	}
}

/*  
 * Sets HOME directory to what is set in HOME environment variable in .profile
 */
void set_home_dir()
{
	if(gHOME == NULL)
		printf("Warning: HOME not SET\n");
	else
	{	
		int retVal = chdir(gHOME);
		if(retVal == 0)
			printf("Changed home directory to %s \n", gHOME);
		else
		{
			printf("Error while setting home directory to %s errno:%s PATH_MAX=%d\n", gHOME, strerror(errno), PATH_MAX);
		}
	}

}

/*  
 * This function sets path value defined in .profile file to global variable gPATH
 */
void set_path_value(char* str)
{
	gPATH = strdup(str);
	char *replaced_str;
	if(strstr(gPATH, "$PATH") != NULL)
	{
		char  env_path[100];
		if(getenv_r("PATH", env_path, 100) == -1)
			str_replace(gPATH, "$PATH", ""); 
		else
			str_replace(gPATH, "$PATH", getenv_r("PATH", env_path, 100)); 
	}
#if DEBUG
	printf("gPATH = %s\n", gPATH);
#endif
	if(setenv("PATH", gPATH, 1) == -1)
		printf("Warning: Error setting PATH value");
	else
		printf("Setting PATH variable to %s successful\n", gPATH);
		
}

/*  
 * This function sets home directory path defined in .profile file to global variable gHOME
 */
void set_home_value(char* str)
{
	gHOME = strdup(str);
}


/*  
 * Checks if param:line begins with param:name
 * Ignores spaces in the beginning of line
 * Sets position to position from where name begins with
 * Return Value:
 * 	0 if the param:line does not begin with param:name
 * 	1 if the param:line begins with param:name
 */
int begins_with(const char* line, char *name, int *pos)
{
	const char *temp_line = line;
	int i=0;
	//Ignore any spaces in the beginning
	while((temp_line != NULL) && *temp_line == ' ')
	{
#if DEBUG
    printf("Entered firstWhile\n");
#endif
		temp_line++;
		i++;
	}
	int j=0;
	//Compare two strings character by character
        while(temp_line[j] == name[j] && temp_line[j] != NULL && name[j] != NULL)
	{
#if DEBUG
    printf("Entered secondWhile\n");
#endif
		j++;
	}
#if DEBUG
    printf("i= %d\n", i);
#endif
	if(name[j] == NULL)
	{
#if DEBUG
    printf("i= %d\n", i);
#endif
		//A match found
		*pos = i + j - strlen(name);
		return 1;
	}
	//No match found
	*pos = 0;
	return 0;
}

/*
 * Processes one line at a time
 * Ignores if it is comment line
 * Extracts value of path varaible when the line begins with either "export" or "export PATH="
 * Extracts value of home varaible when the line begins with either "export" or "export HOME="
 * Ignores all the other lines
 */
int processEnvVars(char* line)
{
   char value[MAX_LINE_LEN];
   int pos;
   //Return if comment line
   if(line[0] == '#')
   {
#if DEBUG
	printf("Ignoring comment line\n");
#endif
	return 0;
   }
#if DEBUG
   printf("%s\n", line);
#endif
   //If line begins with "export" consider processing this line further after export
   if(begins_with(line, "export", &pos))
   {
	int i;
	pos = pos + strlen("export");
	while(line != NULL && line[pos] == ' ')
		pos++;
	for(i = 0; line[i + pos] != NULL; i++)
		line[i] = line[i+pos]; 	
	line[i] = NULL;
   }

   pos=0;
   //Line begins with PATH. Extract PATH value if valid
   if(begins_with(line, PATH_ENV_NAME, &pos))
   {
	int i;
	pos = pos + strlen(PATH_ENV_NAME);
	if(line[pos] == '=')
	{
		pos++;
		for(i = 0; line[i + pos] != NULL; i++)
			value[i] = line[i+pos]; 	
		if(value[i-1] == '\n')
			value[i-1] = NULL;
		else
			value[i] = NULL;
#if DEBUG
   		printf("Name=%s Value=%s",PATH_ENV_NAME, value);
#endif
		set_path_value(value);
	}
   }

   pos=0;
   //Line begins with HOME. Extract PATH value if valid
   if(begins_with(line, HOME_ENV_NAME, &pos))
   {
	int i;
	pos = pos + strlen(HOME_ENV_NAME);
	if(line[pos] == '=')
	{
		pos++;
		for(i = 0; line[i + pos] != NULL; i++)
			value[i] = line[i+pos]; 	
		if(value[i-1] == '\n')
			value[i-1] = NULL;
		else
			value[i] = NULL;
#if DEBUG
   		printf("Name=%s Value=%s",HOME_ENV_NAME, value);
#endif
		set_home_value(value);
	}
   }

	
}

int readFile(const char *filename){
	FILE *fp;
	char buff[4096];

	if(NULL==(fp=fopen(filename, "r"))){
    		perror("file can't open.");
		return -1;
	}
	int i = 0;
	while(NULL!=fgets(buff, sizeof(buff), fp)){
#if DEBUG
	    printf("Line %d:", i++);
#endif
	    //Process one line at a time for environament variables
	    processEnvVars(buff);
       	}
    	fclose(fp);
}

void read_profile_file()
{
	char* file_name = "/root/.myprofile";
	if(-1 == readFile(file_name))
	{
#if DEBUG
		printf("Error reading file: %s\n", file_name);
#endif
//		return 1;
	}

//	return 0;
}
