#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

void genHash(const char* hashPrefix, const char* filename, char* hash);
void trimHash(char* dest, const char* hash);
int findSpace(const char* input);
void printHelpText();

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Insufficient arguments.\n\n");
		printHelpText();
		exit(1);
	}

	//check that file exists
	char* filename = argv[1];
	FILE* fileTest = fopen(filename, "r");
	if(fileTest == NULL)
	{
		fprintf(stderr, "Cannot open file: %s\n", filename);
		exit(1);
	}
	fclose(fileTest);

	//create buffers and generate each hash
	char md5Buff[BUFF_SIZE];
	memset(md5Buff, 0, BUFF_SIZE);
	genHash("md5sum ", argv[1], md5Buff);

	char sha1Buff[BUFF_SIZE];
	memset(sha1Buff, 0, BUFF_SIZE);
	genHash("sha1sum ", argv[1], sha1Buff);

	char sha256Buff[BUFF_SIZE];
	memset(sha256Buff, 0, BUFF_SIZE);
	genHash("sha256sum ", argv[1], sha256Buff);

	char sha512Buff[BUFF_SIZE];
	memset(sha512Buff, 0, BUFF_SIZE);
	genHash("sha512sum ", argv[1], sha512Buff);

	printf("md5:    %s\n", md5Buff);
	printf("sha1:   %s\n", sha1Buff);
	printf("sha256: %s\n", sha256Buff);
	printf("sha512: %s\n", sha512Buff);

	return 0;
}

void genHash(const char* hashPrefix, const char* filename, char* hash)
{
	//assemble command
	char commandString[BUFF_SIZE];
	memset(commandString, 0, BUFF_SIZE);

	strcat(commandString, hashPrefix);
	strcat(commandString, filename);

	//open process
	FILE* process = popen(commandString, "r");
	if(process == NULL)
	{
		fprintf(stderr, "Failed to open %s\n.", commandString);
		exit(1);
	}

	//grab results from stdout
	char results[BUFF_SIZE];
	memset(results, 0, BUFF_SIZE);
	fgets(results, BUFF_SIZE, process);
	pclose(process);

	//copy hash to new buffer and trim end
	trimHash(hash, results);
}

//show usage information
void printHelpText()
{
	printf("[Usage]\n");
	printf("somegen [filename]\n");
}

void trimHash(char* dest, const char* hash)
{
	int newSize = findSpace(hash);

	//copy only hash portion
	if(newSize > 0)
	{
		strncpy(dest, hash, newSize);
	}
	else
	{
		strcpy(dest, hash);
	}
}

//helper function for trimhash
int findSpace(const char* input)
{
	for(int i = 0; i < strlen(input); i++)
	{
		if(input[i] == ' ')
		{
			return i;
		}
	}

	//-1 on failure
	return -1;
}