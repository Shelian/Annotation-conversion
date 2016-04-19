#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

typedef enum State
{
	C_BEGIN,
	C_END,
}State;

void Convert(FILE* fIn, FILE* fOut)
{
	assert(fIn);
	assert(fOut);

	char first;
	char second;
	State tag = C_END;

	do{
		first = fgetc(fIn);
		//1.һ�����
		switch (first)
		{
		case '/':
			second = fgetc(fIn);
			if (second == '*')
			{
				//3.ƥ������
				if (tag == C_END)
				{
					fputc('/', fOut);
					fputc('/', fOut);
					tag = C_BEGIN;
				}
				else
				{
					fputc(first, fOut);
					fputc(second, fOut);
				}
				
			}
			else if (second == '/')
			{
				//7.C++ע������
				fputc('/', fOut);
				fputc('/', fOut);
				char next;
				do{
					next = fgetc(fIn);
					if (next != EOF)
					fputc(next, fOut);
				} while (next != '\n' && next != EOF);
			}
			else
			{
				fputc(first, fOut);
				fputc(second, fOut);
			}
			break;
		//4.����ע������
		case '\n':
				fputc('\n', fOut);
				if (tag == C_BEGIN)
				{
					fputc('/', fOut);
					fputc('/', fOut);
				}

				break;
		case '*':
			second = fgetc(fIn);
			if (second == '/')
			{
				//2.��������
				char next = fgetc(fIn);
				if (next != '\n' && next != EOF)
				{
					//5.����ע������
					if (next == '/')
					{
						fputc('\n', fOut);
						fseek(fIn, -1, SEEK_CUR);
					}
					else
					{
						fputc('\n', fOut);
						fputc(next, fOut);
					}
				}
				else
				{
					fputc('\n', fOut);
				}
				tag = C_END;
			}
			else
			{
				fputc(first, fOut);
				//6.������/***/����
				if (second == '*')
				{
					fseek(fIn, -1, SEEK_CUR);
				}
				else
				{
					fputc(second, fOut);
				}
			}
			break;
		default:
			if (first != EOF)
			{
				fputc(first, fOut);
			}
			break;
		}
	} while (first != EOF);
}


void AnnotationConvert(const char* inputFile, const char* outputFile)
{
	FILE* fIn = fopen(inputFile,"r");
	if (fIn == NULL)
	{
		printf("���ļ�%sʧ�ܣ�errno: %d\n", inputFile, errno);
		return;
	}
	FILE* fOut = fopen(outputFile, "w");
	if (fOut == NULL)
	{
		fclose(fIn);
		printf("���ļ�%sʧ�ܣ�errno: %d\n", outputFile, errno);
		return;
	}

	Convert(fIn, fOut);

	fclose(fIn);
	fclose(fOut);

}

int main()
{
	AnnotationConvert("input.c", "output.c");
	system("pause");
	return 0;
}