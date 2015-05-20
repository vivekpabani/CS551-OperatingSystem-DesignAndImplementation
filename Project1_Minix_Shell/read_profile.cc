#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<syslog.h>

#define HOME_ENV_NAME "HOME"
#define PATH_ENV_NAME "PATH"
#define MAX_LINE_LEN 1024
#define DEBUG 0

char *gHOME = NULL;
char *gPATH = NULL;

void set_path_value(char* str)
{
	gPATH = strdup(str);
}

void set_name_value(char* str)
{
	gPATH = strdup(str);
}


int begins_with(const char* line, char *name, int *pos)
{
	const char *temp_line = line;
	int i=0;
	while((temp_line != NULL) && *temp_line == ' ')
	{
#if DEBUG
    printf("Entered firstWhile\n");
#endif
		temp_line++;
		i++;
	}
	int j=0;
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
		*pos = i + j - strlen(name);
		return 1;
	}
	*pos = 0;
	return 0;
}

int processEnvVars(char* line)
{
   char value[MAX_LINE_LEN];
   int pos;
   if(line[0] == '#')
   {
	printf("Ignoring comment line\n");
	return 0;
   }
   printf("%s\n", line);
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
   if(begins_with(line, PATH_ENV_NAME, &pos))
   {
	int i;
	pos = pos + strlen(PATH_ENV_NAME);
	if(line[pos] == '=')
	{
		pos++;
		for(i = 0; line[i + pos] != NULL; i++)
			value[i] = line[i+pos]; 	
		value[i] = NULL;
   		printf("Name=%s Value=%s",PATH_ENV_NAME, value);
		set_path_value(value);
	}
   }

   pos=0;
   if(begins_with(line, HOME_ENV_NAME, &pos))
   {
	int i;
	pos = pos + strlen(HOME_ENV_NAME);
	if(line[pos] == '=')
	{
		pos++;
		for(i = 0; line[i + pos] != NULL; i++)
			value[i] = line[i+pos]; 	
		value[i] = NULL;
   		printf("Name=%s Value=%s",HOME_ENV_NAME, value);
		set_name_value(value);
	}
   }

	
}

int readFile(const char *filename, size_t *lineCount){
	FILE *fp;
	char buff[4096];

	if(NULL==(fp=fopen(filename, "r"))){
    		perror("file can't open.");
		return -1;
	}
	int i = 0;
	while(NULL!=fgets(buff, sizeof(buff), fp)){
	    printf("Line %d:", i++);
	    processEnvVars(buff);
       	}
    	fclose(fp);
}

int main()
{
	char* file_name = "/root/.myprofile";
	size_t lines; 
	char **line;
	if(-1 == readFile(file_name, &lines))
	{
		printf("Error reading file: %s\n", file_name);
		return 1;
	}

	return 0;
}
