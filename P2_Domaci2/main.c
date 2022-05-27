#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LEN 256
#define DATE_LEN 11
#define INPUT_FILE "contribution.txt"
#define OUTPUT_FILE "result.txt"

typedef struct Contributer
{
	char email[MAX_LEN];
	char date[DATE_LEN];
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

int cmp_contributors(Contributer a, Contributer b)
{
	if (a.lines > b.lines)
		return -1;
	else if (a.lines == b.lines)
		return strcmp(a.email, b.email);
	else
		return 1;
}

void swap_nodes(Node* a, Node* b)
{
	Contributer temp = a->val;
	a->val = b->val;
	b->val = temp;
}

void sort_contributers(Node* list)
{
	int swapped;
	struct Node* ptr1;
	struct Node* lptr = NULL;

	if (list == NULL)
		return;

	do
	{
		swapped = 0;
		ptr1 = list;

		while (ptr1->next != lptr)
		{
			if(cmp_contributors(ptr1->val, ptr1->next->val) > 0)
			{
				swap_nodes(ptr1, ptr1->next);
				swapped = 1;
			}

			ptr1 = ptr1->next;
		}

		lptr = ptr1;
	} while (swapped);
}

void write_contributer(Node** list)
{
	FILE* output_file;
	if ((output_file = fopen(OUTPUT_FILE, "w")) == NULL)
	{
		printf("DAT_GRESKA");
		return;
	}

	Node* curNode = *list;

	while (true)
	{
		fprintf(output_file, "%s %d", curNode->val.email, curNode->val.lines);
		curNode = curNode->next;

		if (curNode != NULL)
			fprintf(output_file, "\n");
		else
			break;
	}

	fclose(output_file);
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
			free(newNode);
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

void free_contributers(Node* list)
{
	Node* tmp;

	while (list != NULL)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

int read_file(Node** contributersList)
{
	FILE* input_file;
	if ((input_file = fopen(INPUT_FILE, "r")) == NULL)
	{
		printf("DAT_GRESKA");
		return -1;
	}

	char curEmail[MAX_LEN] = { 0 };
	char curDate[DATE_LEN] = { 0 };
	unsigned curLines;

	while (fscanf(input_file, "%s %d %s", curEmail, &curLines, curDate) != EOF)
	{
		Node* newNode = create_node(curEmail, curLines, curDate);
		add_contributer(contributersList, newNode);
	}

	fclose(input_file);
	return 0;
}

int is_number(const char* input_string)
{
	if (input_string == NULL)
		return 0;

	int i;
	for(i = 0; input_string[i] != '\0'; i++)
	{
		if (!isdigit(input_string[i]))
			return 0;
	}

	return 1;
}

int is_leap_year(int year)
{
	if (year % 4 != 0)
		return 0;
	if (year % 100 == 0 && year % 400 != 0)
		return 0;

	return 1;
}

int is_valid_date(int day, int month, int year)
{
	if (month < 1 || month > 12)
		return 0;
	if (day < 1 || day > 31)
		return 0;

	if (month == 2)
	{
		if (is_leap_year(year))
			return day <= 29;
		else
			return day <= 28;
	}

	if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		if (day <= 30)
			return 1;
		else
			return 0;
	}

	return 1;
}

int unpack_date_string(const char* input_string, int* day, int* month, int* year)
{
	char dayString[5] = { 0 };
	char monthString[5] = { 0 };
	char yearString[5] = { 0 };

	strncpy(dayString, &input_string[8], 2);
	strncpy(monthString, &input_string[5], 2);
	strncpy(yearString, &input_string[0], 4);

	if (!is_number(dayString) || !is_number(monthString) || !is_number(yearString))
		return -1;

	*day = atoi(dayString);
	*month = atoi(monthString);
	*year = atoi(yearString);

	return 0;
}

int is_date_string(const char* input_string)
{
	if (strlen(input_string) != DATE_LEN - 1) // -1 for '\0'
		return 0;

	int day, month, year;
	if (unpack_date_string(input_string, &day, &month, &year) < 0)
		return 0;

	return is_valid_date(day, month, year);
}

int cmp_date_strings(const char* a, const char* b)
{
	int d1, m1, y1;
	if (unpack_date_string(a, &d1, &m1, &y1) < 0)
		return -2;

	int d2, m2, y2;
	if (unpack_date_string(b, &d2, &m2, &y2) < 0)
		return -2;

	if (y1 < y2)
		return -1;

	else if (y1 > y2)
		return 1;

	if (y1 == y2)
	{
		if (m1 < m2)
			return -1;
		else if (m1 > m2)
			return 1;
		else if (d1 < d2)
			return -1;
		else if (d1 > d2)
			return 1;
		else
			return 0;
	}
}

int main()
{
	Node* contributersList = NULL;

	if (read_file(&contributersList) < 0)
		return 0;

	sort_contributers(contributersList);
	write_contributer(&contributersList);
	free_contributers(contributersList);

	return 0;
}