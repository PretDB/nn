#include <stdio.h>
#include <stdlib.h>

#include "file.h"

int ReadConf(const char* fileName, int* layers, int** cellsPerLayer)
{
	FILE* file = fopen(fileName, "r");
	if(file == NULL)
	{
		return -1;
	}

	int la = 0;
	fscanf(file, "layers:%d\n", &la);
	*cellsPerLayer = (int*)malloc(sizeof(int) * la);
	*layers = la;
	for(int l = 0; l < la; l++)
	{
		fscanf(file, "%d\n", *cellsPerLayer + l);
		printf("layer[%d]:%d\n", l, *(*cellsPerLayer + l));
	}
	return 0;
}

int ReadSample(const char* fileName, int* count, int* widInput, int* widOutput, double*** input, double*** stdOutput)
{
	FILE* file = fopen(fileName, "r");
	if(file == NULL)
	{
		printf("open file failed\n");
		return -1;
	}
	
	int iw = 0, ow = 0, coun = 0;
	fscanf(file, "iw:%d\n", &iw);
	fscanf(file, "ow:%d\n", &ow);
	fscanf(file, "count:%d\n", &coun);

	*widInput = iw;
	*widOutput = ow;
	*count = coun;
	*input = (double**)malloc(sizeof(double*) * coun);
	*stdOutput = (double**)malloc(sizeof(double*) * coun);
	printf("Sample cnt:%d\n", coun);
	for(int s = 0; s < coun; s++)
	{
		*(*input + s) = (double*)malloc(sizeof(double) * iw);
		*(*stdOutput + s) = (double*)malloc(sizeof(double) * ow);
		for(int i = 0; i < iw; i++)
		{
			fscanf(file, "%lf\n", *((*input + s) + i));
			printf("input sample[%d] = %lf\n", s, **((*input + s) + i));
		}
		for(int i = 0; i < ow; i++)
		{
			fscanf(file, "%lf\n", *((*stdOutput + s) + i));
			printf("output sample[%d] = %lf\n", s, **((*stdOutput + s) + i));
		}
	}
	return 0;
}

int Example()
{
	const char fileName[] = "./sample.txt";
	int wi = 0, wo = 0, count = 0;
	double **input = NULL, **stdOutput = NULL;;
	ReadSample(fileName, &count, &wi, &wo, &input, &stdOutput);
	printf("input width:%d, output width:%d\n", wi, wo);
	
	for(int c = 0; c < count; c++)
	{
		printf("sample[%d]:\n", c);
		for(int i = 0; i < wi; i++)
		{
			printf("\tinput[%d]:%lf\n", i, *(*(input + c) + i));
		}
		for(int i = 0; i < wo; i++)
		{
			printf("\tinput[%d]:%lf\n", i, *(*(stdOutput + c) + i));
		}
	}
	return 0;
}
