#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 256
#define INPUT_FILE "contribution.txt"

typedef struct Contributer
{
	char email[MAX_LEN];
	char date[MAX_LEN];
	unsigned lines;
} Contributer;

typedef struct Node
{
	Contributer val;
	struct Node* next;
} Node;

Node* create_node(char* email, unsigned lines, char* date)
{
	Node* newNode = (Node*)calloc(1, sizeof(Node));

	if (newNode == NULL)
		return NULL;

	strcpy(newNode->val.email, email);
	strcpy(newNode->val.date, date);
	newNode->val.lines = lines;
	newNode->next = NULL;

	return newNode;
}

void print_contributer(Node** list)
{
	Node* curNode = *list;

	while (curNode != NULL)
	{
		printf("%s %d %s\n", curNode->val.email, curNode->val.lines, curNode->val.date);
		curNode = curNode->next;
	}
}

void add_contributer(Node** list, Node* newNode)
{
	if (*list == NULL)
	{
		*list = newNode;
		return;
	}

	Node* curNode = *list;
	while (curNode != NULL)
	{
		if (strcmp(curNode->val.email, newNode->val.email) == 0)
		{
			curNode->val.lines += newNode->val.lines;
			return;
		}

		if (curNode->next == NULL)
		{
			curNode->next = newNode;
			return;
		}

		curNode = curNode->next;
	}
}

void free_contributers(Node** list)
{
	Node* curNode = *list;
	Node* nodeToFree;

	while (curNode != NULL)
	{
		nodeToFree = curNode;
		curNode = curNode->next;
		free(nodeToFree);
	}
}

int read_file(Node** contributersList)
{
	FILE* fptr;
	char curEmail[MAX_LEN] = { 0 };
	char curDate[MAX_LEN] = { 0 };
	unsigned curLines;

	if ((fptr = fopen(INPUT_FILE, "r")) == NULL)
	{
		printf("DAT_GRESKA");
		return -1;
	}

	while (fscanf(fptr, "%s %d %s", curEmail, &curLines, curDate) != EOF)
	{
		Node* newNode = create_node(curEmail, curLines, curDate);
		add_contributer(contributersList, newNode);
	}

	fclose(fptr);
	return 0;
}

int main()
{
	Node* contributersList = NULL;

	if (read_file(&contributersList) < 0)
		return 0;

	print_contributer(&contributersList);
	free_contributers(&contributersList);

	return 0;
}