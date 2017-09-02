//////////////////////////////////////////////////////////////////////////
// INeuron.cpp: implementation of the INeuron class.
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "INeuron.h"
#include "math.h"

#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

INeuron::INeuron()

{
	// Create an neuron without 'tenticles'
	//P2N("Construct %p\r\n", this);
	Init_Vars();
}

//////////////////////////////////////////////////////////////////////////

INeuron::INeuron(int num)

{
	// Create a itialized neuron, 'num' of tenticles in and out
	Init_Vars();
	//P2N("Construct (num) %p\r\n", this);
	Alloc(num, num);
}


//////////////////////////////////////////////////////////////////////////

INeuron::INeuron(int inum, int onum)

{
	// Create a itialized neuron, 'num' of tenticles in and out
	Init_Vars();
	//P2N("Construct (num) %p\r\n", this);
	Alloc(inum, onum);
}

//////////////////////////////////////////////////////////////////////////

INeuron::~INeuron()

{
	//P2N("Destruct %p\r\n", this);

	if(synapse_arr)
		free(synapse_arr);

	if(weight_arr)
		free(weight_arr);

	if(axon)
		free(axon);
}

//////////////////////////////////////////////////////////////////////////
// Fire neuron.
//////////////////////////////////////////////////////////////////////////
//
// --------------------------------
//
//	Test for zero -- randomize
//	Sum all the weighted inputs 
//	Call tranfer/limiter function
//	Out put to dependent neurons
//
// --------------------------------
//
//////////////////////////////////////////////////////////////////////////

void INeuron::Fire()

{
	double val2 = 0;  // prevent overflow with more neurons

	Test();

	// Scan all inputs
	for(int loop = 0; loop < synapse_num; loop++)
		{
		if(synapse_arr[loop] == 0)
			{
			synapse_arr[loop] =  1 + rand() % 12;
			}
		int sum = (synapse_arr[loop]) * weight_arr[loop] / RAND_MAX;
		val2 += sum;
		}

	val = (int)Transfer(val2);

	// Output to dependent axons
	for(int loop2 = 0; loop2 < connect_num; loop2++)
		{
		*axon[loop2] = val;
		}	
}

//////////////////////////////////////////////////////////////////////////
// Connect this neuron's axons to another one's synapse dentrite

void INeuron::Connect(INeuron *ptr)

{
	if(connect_num < axon_num)
		{
		if(ptr->incoming_num < ptr->synapse_num)
			{
			axon[connect_num] = &ptr->synapse_arr[ptr->incoming_num];
			connect_num ++; ptr->incoming_num ++;
			}
		else
			{
			P2N("Attempted to connect %p beyond allocated synapse number\r\n", this);
			}
		}
	else
		{
		P2N("Attempted to connect %p beyond allocated axon number\r\n", this);
		}
}

//////////////////////////////////////////////////////////////////////////
/// Randomize weight values

void INeuron::Rand(int maxval)

{
	for(int loop = 0; loop < synapse_num; loop++)
		{
		int newval = ((rand() % maxval) - maxval/2);
		//P2N("Set %p %d\r\n", this, newval);
		weight_arr[loop] = newval;
		newval = ((rand() % maxval) - maxval/2);
		//offset_arr[loop] = newval;
		}	
}

//////////////////////////////////////////////////////////////////////////
/// Add Noise to weight values

void INeuron::Noise(int maxval)

{
	for(int loop = 0; loop < synapse_num; loop++)
		{
		int noise = ((rand() % maxval) - maxval/2);
		//P2N("Added noise %d\r\n", noise);
		weight_arr[loop] += noise;
		}	
}

//////////////////////////////////////////////////////////////////////////
// Allocate number of synapses an axxons

void INeuron::Alloc(int num)

{
	Alloc(num, num);
}

//////////////////////////////////////////////////////////////////////////
// Allocate number of synapses

void INeuron::Alloc(int num, int axx)

{
	// Free previous neuron allocations
	if(synapse_arr)
		free(synapse_arr);

	if(weight_arr)
		free(weight_arr);

	if(axon)
		free(axon);

	if(offset_arr)
		free(offset_arr);

	// Create a itialized neuron, 'num' of tenticles in and out
	synapse_num = num;								// Number of synapses

	synapse_arr = (int*)malloc(num * sizeof(int));	// Synapse array
	weight_arr = (int*)malloc(num * sizeof(int));	// Every synapse has a weight
	offset_arr = (int*)malloc(num * sizeof(int));	// Every synapse has an offset

	val = 0;										// Current neuron value
	treshold = 0;									// Firing treshold
	decay = 0;										// Neuron decay

	axon_num = axx;
	axon = (int **)malloc(axon_num * sizeof(int*) );// Dependent axons

	connect_num	= 0;
	incoming_num = 0;

	// Clear arrays
	memset(synapse_arr, 0, num * sizeof(int));
	memset(weight_arr, 0, num * sizeof(int));
	memset(offset_arr, 0, num * sizeof(int));
	memset(axon, 0, axon_num * sizeof(int *));
	
}

//////////////////////////////////////////////////////////////////////////
// Limit to dump the first 20

void INeuron::Dump()

{
	P2N("----- Neuron dump: %p val=%d\r\n" , this, val);

	int synapse_dump = synapse_num;
	int axon_dump = axon_num;

	synapse_dump = min(synapse_dump, 20);
	axon_dump = min(axon_dump, 20);

	int loop;

	P2N(     "Synapses: ");
	for(loop = 0; loop < synapse_dump; loop++)
		{
		P2N("%p->%-6d ", &synapse_arr[loop], synapse_arr[loop]);
		}	

	P2N("\r\nWeights:   ");
	for(loop = 0; loop < synapse_dump; loop++)
		{
		P2N("%6d ", weight_arr[loop]);
		}	

	P2N("\r\nAxon connections:   ");
	for(loop = 0; loop < axon_dump; loop++)
		{
		P2N("%p ", axon[loop]);
		}	
	P2N("\r\n");
}

//////////////////////////////////////////////////////////////////////////

void INeuron::Init_Vars()

{
	synapse_num = 0;			// Number of synapses
	connect_num	= 0;
	incoming_num = 0;
	
	synapse_arr = NULL;			// Synapse array
	weight_arr = NULL;			// Every synapse has a weight
	offset_arr = NULL;

	val = 0;					// Current neuron value
	treshold = 0;				// Firing treshold
	decay = 0;					// Neuron decay
	axon_num = 0;				// Dependent number of
	axon = NULL;				// Dependent axons
	hidden	= 0;
	magic = 0x1234;
}

//////////////////////////////////////////////////////////////////////////

void INeuron::PrintVal()
{
	P2N("this=%p val=%-6d ", this, val);
}

int	INeuron::Test()

{
	int ret;
	if(magic == 0x1234)
		{
		ret = 1;
		}
	else
		{
		P2N("Neural test failed %p\r\n", this);
		ret = 0;
		}
	return ret;
}

//////////////////////////////////////////////////////////////////////////

double INeuron::Transfer(double val)

{
	double yyy = val, xxx;

	// Limit variable logarithmic
	if(yyy >= 0)
		{
		xxx = log(yyy/10000 + 1) * 20000;
		//if(xxx > 60000)
		//	xxx = 60000;
		}
	else
		{
		xxx = -(log(-yyy/10000 + 1) * 20000);
		//if(xxx < - 60000)
		//	xxx = -60000;
		}
	return xxx ;
}
