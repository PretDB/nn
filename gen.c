#include <stdio.h>
#include <stdlib.h>

double fomular(double in)
{
	return in * 2;
}
int Gen(int cntSample)
{
	FILE* file = fopen("sample.txt", "w");
	if(file == NULL)
	{
	return -1;
	}
	fprintf(file, "iw:1\n");
	fprintf(file, "ow:1\n");
	fprintf(file, "count:%d\n", cntSample);
	
	double start = 0.0;
	for(int a = 0; a < cntSample; a++)
	{
		fprintf(file, "%lf\n", start);
		fprintf(file, "%lf\n", fomular(start));
		start += 0.003;
	}
	return 0;
}
int main(int argc, char** argv)
{
	int st = Gen(20);
	if (st == -1)
	{
	return -1;
	}
}
