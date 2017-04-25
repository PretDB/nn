#include <stdio.h>
#include <stdlib.h>

#include "nn.h"
#include "file.h"

int main(int argc, char** argv) 
{
	double x = 1;
	double yita = 0.1;
	int state = 0;
	printf("hello nn\n");
	const char* confFile = "nn.conf";
	const char* sampleFile = "sample.txt";
	int lay = 0;
	int* cpl = NULL;
	double** sampleInput, ** sampleOutput;
	int widInput = 0, widOutput = 0;
	int cntSamples = 0;;
	
	state = ReadConf(confFile, &lay, &cpl);if(state != 0) return -1;
	printf("layers:%d\n", lay);
	for(int l = 0; l < lay; l++)
	{
		printf("cells:%d\n", cpl[l]);
	}

	state = ReadSample(sampleFile, &cntSamples, &widInput, &widOutput, &sampleInput, &sampleOutput);if(state != 0) return -1;
	
	
	nn* net = CreateNN(lay, cpl);
	printf("nn created\n");
	printf("this nn has %d layers\n", net->cntLayers);
	for(int l = 0; l < net->cntLayers; l++)
	{
		printf("\tlayer %d has %d cells\n", l, net->layers[l].cntCells);
	}

	// train fun may went wrong for it increasing mse.
	TrainNN(net, sampleInput, sampleOutput, cntSamples, yita, x);
	double s = 0.33;
	double d = 0;
	RunNN(net, &s, &d, x);
	printf("res of %lf: %lf\n", s, d);

	for(int l = 0; l < net->cntLayers; l++)
	{
		printf("layer %d\n", l);
		for(int c = 0; c < net->layers[l].cntCells; c++)
		{
			printf("\tcell %d, cntInput %d\n", c, net->layers[l].cells[c].cntInput);
			for(int i = 0; i < net->layers[l].cells[c].cntInput; i++)
			{
				printf("\t\tweight %lf\n", net->layers[l].cells[c].weight[i]);
			}
		}
	}


	printf("res of %lf: %lf\n", s, d);
	
	KillNN(net);
	return 0;
}
