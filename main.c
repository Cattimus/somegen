#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1024

void genHash(const char* hashPrefix, const char* filename, char* hash);
void sanitize(char* dest, const char* src, int sz);
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
	FILE* fileTest = fopen(argv[1], "r");
	if(fileTest == NULL)
	{
		fprintf(stderr, "Cannot open file: %s\n", argv[1]);
		exit(1);
	}
	fclose(fileTest);
	
	//create "safe" filename string
	char filename[BUFF_SIZE];
	memset(filename, 0, BUFF_SIZE);
	sanitize(filename, argv[1], BUFF_SIZE);

	//create buffers and generate each hash
	char md5Buff[BUFF_SIZE];
	memset(md5Buff, 0, BUFF_SIZE);
	genHash("md5sum ", filename, md5Buff);

	char sha1Buff[BUFF_SIZE];
	memset(sha1Buff, 0, BUFF_SIZE);
	genHash("sha1sum ", filename, sha1Buff);

	char sha256Buff[BUFF_SIZE];
	memset(sha256Buff, 0, BUFF_SIZE);
	genHash("sha256sum ", filename, sha256Buff);

	char sha512Buff[BUFF_SIZE];
	memset(sha512Buff, 0, BUFF_SIZE);
	genHash("sha512sum ", filename, sha512Buff);

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

//sanitize string (escape with quotes to hopefully prevent attacks)
void sanitize(char* dest, const char* src, int sz)
{
	memset(dest, 0, sz);
	strcat(dest, "\"");
	strncat(dest, src, sz - 3);
	strcat(dest, "\"");
}