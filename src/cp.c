#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sys/stat.h>

int isFile(const char *source_path);
void pathDispose(const char *source_path, char *target_path);
int check(char *path);


int main(int argc,char *argv[])
{
	char cmd[2048] = { 0 };
	char source_path[1024] = { 0 };
	char target_path[1024] = { 0 };
	char temp_path[1024] = { 0 };
	FILE *fp_source_path;
	FILE *fp_target_path;
	
	if(argc < 2)
	{
		printf("Parameter error\n");
		printf("folder -> cp c:\\1 c:\\2 \n");
		printf("file -> cp c:\\1.c c:\\2.c \n");
		printf("file -> cp \"c:\\2 34\\1.c\" c:\\2.c \n");
		return 1;
	}
	
	if(strcmp(argv[1], argv[2]) == 0)
	{
		printf("Error: Same file or directory\n");
		printf("folder -> cp c:\1 c:\2 \n");
		printf("file -> cp c:\1.c c:\2.c \n");
		printf("file -> cp \"c:\2 34\1.c\" c:\2.c \n");
		return 1;
	}
	
	printf("argv[2] = %s\n", argv[2]);
	if(check(argv[2]) <= 1)
	{
		printf("It cannot be the root directory\n");
		printf("folder -> cp c:\1 c:\2 \n");
		printf("file -> cp c:\1.c c:\2.c \n");
		printf("file -> cp \"c:\2 34\1.c\" c:\2.c \n");
		return 1;
	}
	
	pathDispose(argv[1], source_path);
	pathDispose(argv[2], target_path);
	
	switch(isFile(argv[1]))
	{
	case 0: //copy file
		sprintf(cmd, "copy /Y %s %s", source_path, target_path);
		printf("cmd = %s\n", cmd);
		system(cmd);
		break;
	case 1: //copy folder
		fp_source_path = fopen("./source_path.dat", "w");
		fp_target_path = fopen("./target_path.dat", "w");
		fclose(fp_source_path);
		fclose(fp_target_path);
	
		/* 1 */
		sprintf(cmd, "dir /s /b %s > ./source_path.dat", source_path);
		printf("cmd = %s\n", cmd);
		system(cmd);
		
		/* 2 */
		sprintf(cmd, "xcopy /Y /E %s %s\\", source_path, target_path);
		printf("cmd = %s\n", cmd);
		system(cmd);
		
		/* 3 */
		sprintf(cmd, "dir /s /b %s > ./target_path.dat", target_path);
		printf("cmd = %s\n", cmd);
		system(cmd);
		
		/* 4 */
		fp_target_path = fopen("./target_path.dat", "r");
		fp_source_path = fopen("./source_path.dat", "r");
		
		printf("-----------------\n");
		do
		{
			if( fscanf(fp_source_path, "%[^\n]%*c", temp_path) != EOF )
			{
				pathDispose(temp_path, source_path);
			}
			else
			{
				break;
			}
			
			if( fscanf(fp_target_path, "%[^\n]%*c", temp_path) != EOF )
			{
				pathDispose(temp_path, target_path);
			}
			else
			{
				break;
			}
			sprintf(cmd, "copy /Y %s %s", source_path, target_path);
			printf("cmd = %s\n", cmd);
			system(cmd);
			
		}while(1);
		fclose(fp_source_path);
		fclose(fp_target_path);
		system("del /s /q source_path.dat");
		system("del /s /q target_path.dat");
		break;
	default:
		return 1;
	}
	
	return 0;
}

int isFile(const char *source_path)
{
	int result = 0;
	struct _stat buf;
	
	_stat(source_path, &buf );
	if(_S_IFDIR & buf.st_mode)
	{
		result = 1;
		//puts("folder"); 
	}
	else if(_S_IFREG & buf.st_mode)
	{
		result = 0;
		//puts("file");
	}
	else
	{
		result = -1;
		puts("unable to access"); 
	}
	
	return result;
}

void pathDispose(const char *source_path, char *target_path)
{
	while(*source_path)
	{
		*target_path = *source_path;
		if(*target_path == ' ')
		{
			*target_path = '\"';
			++target_path;
			*target_path = ' ';
			++target_path;
			*target_path = '\"';
		}
		++source_path;
		++target_path;
	}
	*target_path = '\0';
}

int check(char *path)
{
	int i = 0;
	
	while(*path)
	{
		if(*path == '\\')
		{
			++i;
		}
		++path;
	}
	--path;
	if(*path != '\\')
	{
		return 2;
	}
	
	return i;
}






