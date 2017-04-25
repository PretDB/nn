#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "nn.h"

#define DEBUG
#define DEBUGV


nn* CreateNN(int cntLayers, int* cntCellPerLayer)
{
	nn* net = (nn*)malloc(sizeof(nn));
	net->cntLayers = cntLayers;
	net->layers = (layer*)malloc(sizeof(layer) * cntLayers);
	for(int a = 0; a < cntLayers; a++)
	{
		net->layers[a].cntCells = cntCellPerLayer[a];
		net->layers[a].cells = (cell*)malloc(sizeof(cell) * net->layers[a].cntCells);
	}
	// the first layer
	for(int a = 0; a < net->layers[0].cntCells; a++)
	{
		net->layers[0].cells[a].cntInput = 1;
		net->layers[0].cells[a].weight = (double*)malloc(sizeof(double));
		net->layers[0].cells[a].input = (double*)malloc(sizeof(double));
		*(net->layers[0].cells[a].weight) = 1;
		net->layers[0].cells[a].output = 0;
		net->layers[0].cells[a].threshold = 0;
	}
	for(int l = 1; l < net->cntLayers; l++)
	{
		for(int c = 0; c < net->layers[l].cntCells; c++)
		{
			net->layers[l].cells[c].cntInput = cntCellPerLayer[l - 1];
			net->layers[l].cells[c].weight = (double*)malloc(sizeof(double) * net->layers[l].cells[c].cntInput);
			net->layers[l].cells[c].input = (double*)malloc(sizeof(double) * net->layers[l].cells[c].cntInput);
			for(int i = 0; i < net->layers[l].cells[c].cntInput; i++)
			{
				net->layers[l].cells[c].weight[i] = 1;
			}
			for(int i = 0; i < net->layers[l].cells[c].cntInput; i++)
			{
				net->layers[l].cells[c].input[i] = 0;
			}
			net->layers[l].cells[c].output = 0;
			net->layers[l].cells[c].threshold = 0;
		}
	}
	net->cntOutput = net->layers[net->cntLayers - 1].cntCells;
	net->outLayer = &(net->layers[net->cntLayers - 1]);
#ifdef DEBUG
	for(int l = 0; l < net->cntLayers; l++)
	{
		printf("\tlayer info:\n");
		printf("\t\tcell count:%d\n", net->layers[l].cntCells);
		for(int c = 0; c < net->layers[l].cntCells; c++)
		{
			printf("\t\tcell info:\n");
			printf("\t\t\tinput count:%d\n", net->layers[l].cells[c].cntInput);
			printf("\t\t\tthreashold: %lf\n", net->layers[l].cells[c].threshold);
		}
	}
#endif
	return net;
}


void KillNN(nn* net)
{
	for(int l = 0; l < net->cntLayers; l++)
	{
		for(int c = 0; c < net->layers[l].cntCells; c++)
		{
			free(net->layers[l].cells[c].input);
			free(net->layers[l].cells[c].weight);
		}
		free(net->layers[l].cells);
	}
	free(net->layers);
	free(net);
	return;
}

double _Active(double x, double input)
{
	return x / (1 + exp(0 - input));
}
double _MSE(double* stdData, double* data, int count)
{
	double mse = 0.0;
	for(int c = 0; c < count; c++)
	{
		mse = mse + (data[c] - stdData[c]) * (data[c] - stdData[c]);
	}
	mse = mse / 2;
	return mse;
}

int RunNN(nn* net, double* input, double* output, double x)
{
	for(int i = 0; i < net->layers[0].cntCells; i++)
	{
		net->layers[0].cells[i].input[0] = input[i];
	}
	for(int l = 0; l < net->cntLayers; l++)
	{
		for(int c = 0; c < net->layers[l].cntCells; c++)
		{
			for(int i = 0; i < net->layers[l].cells[c].cntInput; i++)
			{
				net->layers[l].cells[c].output += net->layers[l].cells[c].input[i];
			}
			net->layers[l].cells[c].output = net->layers[l].cells[c].output - net->layers[l].cells[c].threshold;
			net->layers[l].cells[c].output = _Active(x, net->layers[l].cells[c].output);
		}
	}
	for(int o = 0; o < net->cntOutput; o++)
	{
		output[o] = net->outLayer->cells[o].output;
	}
	return 0;
}

// signle train operation, that means if you have lots of samples, you should call this many times.
int TrainNN(nn* net, double** input, double** stdOutput, int cntSample, double yita, double x)
{
	double** out = (double**)malloc(cntSample * sizeof(double*));
	double*  msem = (double*)malloc(sizeof(double) * cntSample);			// mse for each sample
	double   mse = 0;

	// get msem for each sample, then compute mse
	do
	{
		mse = 0.0;
		// get error
		memset(out, 0, sizeof(double*) * net->cntOutput);					// clear
		memset(msem, 0, sizeof(double) * cntSample);

		// run arg tuning below
		for(int cs = 0; cs < cntSample; cs++)
		{
			out[cs] = (double*)malloc(sizeof(double) * net->layers[0].cntCells);
			memset(out[cs], 0, sizeof(double) * net->layers[0].cntCells);
			RunNN(net, input[cs], out[cs], x);
			msem[cs] = _MSE(stdOutput[cs], out[cs], net->layers[0].cntCells);
#ifdef DEBUGV
			printf("current sample:[%d]\n=================================\n", cs);
			for(int s = 0; s < net->layers[0].cntCells; s++)
			{
				printf("input:%lf\tout:%lf\n", input[cs][s], stdOutput[cs][s]);
			}
			printf("---------------------------------\n");
#endif

			
			// tune for the last layer. for each cell
			for(int c = 0; c < net->outLayer->cntCells; c++)
			{
				double curOut = net->outLayer->cells[c].output;
				double std = *(stdOutput[cs] + c);
				double gj = x * curOut * (1 - curOut) * (std - curOut);
				net->outLayer->cells[c].gj = gj;				// save mistake info
#ifdef DEBUGV
				printf("\tcurrent out:%lf\n", curOut);
				printf("\tstanded out:%lf\n", std);
				printf("\tgj:%lf\n", gj);
				printf("-----------last layer------------\n");
#endif

				// for each input, tune each weight
				// delta weight = yita * gj * bh
				for(int i = 0; i < net->outLayer->cells[c].cntInput; i++)
				{
					double del = yita * net->outLayer->cells[c].input[i] * net->outLayer->cells[c].gj;
					net->outLayer->cells[c].weight[i] += del;
#ifdef DEBUGV
					printf("\t\tinput[%d] = %lf\n", i, net->outLayer->cells[c].input[i]);
					printf("\t\tdelta[%d] = %lf\n", i, del);
					printf("\t\tweight[%d] = %lf\n", i, net->outLayer->cells[c].weight[i]);
#endif
				}
				net->outLayer->cells[c].threshold = net->outLayer->cells[c].threshold - yita * gj;
#ifdef DEBUGV
				printf("\t\tthreashold:%lf\n", net->outLayer->cells[c].threshold);
#endif
			}
			// tune for each layer
			for(int l = net->cntLayers - 2; l >= 0; l--)						// curent layer
			{
#ifdef DEBUGV
				printf("============layer %d=============\n", l);
#endif
				for(int c = 0; c < net->layers[l].cntCells; c++)				// current cell
				{
#ifdef DEBUGV
				printf("------------cell %d-------------\n", c);
#endif
					double gj = 0;
					double seg = 0;
					// get gj for current cell;
					for(int j = 0; j < net->layers[l + 1].cntCells; j++)	// next layer
					{
						// get sigma(whj * gj)
						seg += net->layers[l + 1].cells[j].weight[c] * net->layers[l + 1].cells[j].gj;
					}
					double curOut = net->layers[l].cells[c].output;
					gj = x * curOut * (1 - curOut) * seg;
					net->layers[l].cells[c].gj = gj;
#ifdef DEBUGV
					printf("\tcurrent out:%lf\n", curOut);
					printf("\tstanded out:%lf\n", seg);
					printf("\tgj:%lf\n", gj);
#endif
					for(int i = 0; i < net->layers[l].cells[c].cntInput; i++)	// for each input
					{
						double del = yita * gj * net->layers[l].cells[c].input[i];
						net->layers[l].cells[c].weight[i] += del;
#ifdef DEBUGV
						printf("\t\tdelta[%d] = %lf\n", i, del);
						printf("\t\tweight[%d] = %lf\n", i, net->layers[l].cells[c].weight[i]);
#endif
					}
					net->layers[l].cells[c].threshold += - yita * gj;
#ifdef DEBUGV
					printf("\t\tthreashold:%lf\n", net->layers[l].cells[c].threshold);
#endif
				}
			}
			mse += msem[cs];
		}
		mse = mse / cntSample;													// run for mse
#ifdef DEBUG
		printf("current mse:%lf\n", mse);
#endif
		fflush(NULL);
	}while(mse > PRECISION);
	return 0;
}

int DumpNN(nn* net)
{
	FILE* file = fopen("nn.dump", "w");
	if(file == NULL)
	{
	return -1;
	}
	fprintf(file, "layers:%d\n", net->cntLayers);
	for(int l = 0; l < net->cntLayers; l++)
	{
		fprintf(file, "col:%d=%d\n", l, net->layers[l].cntCells);
	}
	for(int l = 0; l < net->cntLayers; l++)
	{
		for(int c = 0; c < net->layers[l].cntCells; c++)
		{
			for(int i = 0; i < net->layers[l].cells[c].cntInput; i++)
			{
				fprintf(file, "i:%lf\n", net->layers[l].cells[c].weight[i]);
			}
		}
	}
}
