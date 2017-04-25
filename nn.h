#define PRECISION 0.001

typedef struct _CELL_
{
	double* input;
	double*	weight;
	int		cntInput;
	double	output;
	double	threshold;
	double 	gj;				// e for error
}cell;

typedef struct _LAYER_
{
	cell*	cells;
	int		cntCells;
}layer;

typedef struct _NN_
{
	layer*	layers;
	int		cntLayers;
	int		cntOutput;
	layer*	outLayer;
}nn;

nn* CreateNN(int cntLayers, int* cntCellsPerLayer);
void KillNN(nn* net);

int RunNN(nn* net, double* input, double* output, double x);
int TrainNN(nn* net, double** input, double** stdOutput, int cntSample, double yita, double x);
