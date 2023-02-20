#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book
{
	int pages;
	char* title;
} Book;

void save_to_binary_file(char* filename, Book* books, const long size)
{
	FILE* output = fopen(filename, "wb");

	if (!output)
	{
		printf("Error: Could not open the file");
		return;
	}

	fwrite(&size, sizeof(long), 1, output);

	for (int i = 0; i < size; i++)
	{
		fwrite(books[i].title, sizeof(char), 255, output);
		fwrite(&books[i].pages, sizeof(int), 1, output);
	}

	fclose(output);
}

// возвращает информацию о книге (только количество страниц)
// -1 - если не найдена информация
int find_book_in_file(char* filename, char* book_titlle)
{
	FILE* input = fopen(filename, "rb");

	if (!input)
	{
		printf("Error: Could not open the file");
		return;
	}

	int pages = -1;

	long size = 0;
	fread(&size, sizeof(long), 1, input);

	char temp_title[255] = "";
	int temp_pages = 0;

	for (int i = 0; i < size; i++)
	{
		fread(temp_title, sizeof(char), 255, input);
		fread(&temp_pages, sizeof(int), 1, input);

		if (!strcmp(book_titlle, temp_title))
		{
			pages = temp_pages;
			break;
		}
	}

	fclose(input);

	return pages;
}

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		long size = 0;
		printf("Enter books count: ");
		scanf("%d", &size);

		Book* books = (Book*)malloc(sizeof(Book) * size);

		for (int i = 0; i < size; i++)
		{
			char title[255];
			printf("Title: ");
			scanf("%s", title);

			int pages = 0;
			printf("Pages count: ");
			scanf("%d", &pages);

			books[i].title = (char*)malloc(sizeof(char) * 255);
			strcpy(books[i].title, title);
			books[i].pages = pages;
		}

		save_to_binary_file(argv[1], books, size);

		for (int i = 0; i < size; i++)
		{
			free(books[i].title);
		}
		free(books);
	}
	else if (argc == 3)
	{
		int pages = find_book_in_file(argv[1], argv[2]);

		if (pages != -1)
		{
			printf("Result\n");
			printf("Title: %s\n", argv[2]);
			printf("Pages count: %d\n", pages);
		}
		else
		{
			printf("The specified book could not be found\n");
		}
	}
	else
	{
		printf("Error: invalid args count\n");
	}

	return 0;
}
