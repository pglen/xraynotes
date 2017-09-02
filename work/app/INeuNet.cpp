
/* =====[ INeuNet.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the INeuNet.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/26/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////
//
// INeuNet.cpp: implementation of the INeuNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xraynotes.h"
#include "math.h"

#include "xraynotes.h"

#define	OUTLAYER	0	
#define	MIDLAYER	1
#define	INLAYER		2

//define NUM_MID_NEURON	((NUM_OUT_NEURON + NUM_IN_NEURON)/2)

#include "INeuNet.h"
#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

INeuNet::INeuNet()

{
	//P2N("Constructed INeuron\r\n");
	//INeuNet(16, 16, 16, 16);
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    INeuNet::INeuNet
// Description:      <empty description>
// Return type:      
// Argument:         int input		-- number of inputs
// Argument:         int output		-- number of outputs
//
//////////////////////////////////////////////////////////////////////////

//INeuNet::INeuNet(int input, int output)
//
//{
//	P2N("Constructed INeuron input=%d output=%d\r\n", input, output);
//	INeuNet(1, input, (output + input) / 2, output);
//}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    INeuNet::INeuNet
// Description:      <empty description>
// Return type:      
// Argument:         int input	-- number of inputs per neuron
// Argument:         int inp	-- number of input neurons
// Argument:         int mid	-- number of intermediate neurons
// Argument:         int out	-- number of outputs
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
// Function name:    INeuNet::INeuNet
// Description:      <empty description>
// Return type:      
// Argument:         int input	-- number of inputs per neuron
// Argument:         int inp	-- number of input neurons
// Argument:         int mid	-- number of intermediate neurons
// Argument:         int out	-- number of outputs
//
//////////////////////////////////////////////////////////////////////////

int INeuNet::Init(int input, int inp, int mid, int out)

{
	int loop;

	inputcnt = input;
	incnt = inp; 
	midcnt = mid; 
	outcnt = out; 

	//P2N("Inited INeuron, %d %d %d %d\r\n", inputcnt, incnt, midcnt, outcnt);

	save_weight = 0;
	save_ptr = NULL;
	stop = 0;
	sleeptime = 1;

	inneu  = (INeuron **) malloc(inp * sizeof(void*) );
	midneu = (INeuron **) malloc(mid * sizeof(void*) );
	outneu = (INeuron **) malloc(out * sizeof(void*) );

	// Allocate input neurons
	for(loop = 0; loop < inp; loop++)
		{
		inneu[loop] =  new INeuron;
		inneu[loop]->Alloc(inputcnt, midcnt);
		inneu[loop]->Rand();	
		}
	
	// Allocate middle neurons
	for(loop = 0; loop < mid; loop++)
		{
		midneu[loop] =  new INeuron;
		midneu[loop]->Alloc(incnt, outcnt); midneu[loop]->hidden = true;
		midneu[loop]->Rand();
		}
	
	// Allocate output neurons
	for(loop = 0; loop < out; loop++)
		{
		outneu[loop] =  new INeuron;

		// BUG
		//outneu[loop]->Alloc(outcnt, 1);
		outneu[loop]->Alloc(midcnt, 1);
		outneu[loop]->Rand();
		}

	// Connect neurons out and mid
	//P2N("Connecting mid->out\r\n");

	for(loop = 0; loop < midcnt; loop++)
		{
		for(int loop2 = 0; loop2 < outcnt; loop2++)
			{
			midneu[loop]->Connect(outneu[loop2]);
			}
		}	

	// Connect neurons in and mid
	//P2N("Connecting mid->in\r\n");

	for(loop = 0; loop < incnt; loop++)
		{
		for(int loop3 = 0; loop3 < midcnt; loop3++)
			{
			inneu[loop]->Connect(midneu[loop3]);
			}
		}	

	//P2N("Neuron counts after connect : %d %d %d %d\r\n", inputcnt, incnt, midcnt, outcnt);

	//TestNet();
	return 0;
}

//////////////////////////////////////////////////////////////////////////

INeuNet::~INeuNet()

{
	int loop;

	// De allocate neurons
	for(loop = 0; loop < incnt; loop++)
		{
		delete(inneu[loop]);
		}

	for(loop = 0; loop < midcnt; loop++)
		{
		delete(midneu[loop]);
		}

	for(loop = 0; loop < outcnt; loop++)
		{
		delete(outneu[loop]);
		}

	free(inneu);
	free(midneu);
	free(outneu);
}


//////////////////////////////////////////////////////////////////////////
// Awaiting implementation

void INeuNet::Train()

{

}

//////////////////////////////////////////////////////////////////////////

void INeuNet::FireAll()

{
	int loop;
	
	for(loop = 0; loop < incnt; loop++)
		{
		inneu[loop]->Fire();
		}

	for(loop = 0; loop < midcnt; loop++)
		{
		midneu[loop]->Fire();
		}

	for(loop = 0; loop < outcnt; loop++)
		{
		outneu[loop]->Fire();
		}
}

//////////////////////////////////////////////////////////////////////////

double	INeuNet::SumDeviation(int *refout, int outlen)

{
	int		loop;
	double	errsum = 0, ret = 0;

	if(outlen > outcnt)
		{
		P2N("Reference output (%d) is larger then neuron count (%d)\r\n", outlen, outcnt);
		//return 0;
		outlen = outcnt;
		}

	for(loop = 0; loop < outlen; loop++)
		{
		double diff =  outneu[loop]->val - refout[loop];
		errsum =  SQRT( SQR(errsum) + SQR(diff) );
		//errsum +=  ABS(diff);
		}

	ret = errsum / outlen;

	//P2N("Deviation=%d\r\n", ret);

	return(ret);
}

//////////////////////////////////////////////////////////////////////////

void INeuNet::TrainOneFromArray(CWordArray *arr, int *output, int outlen)

{
	int inlen  = arr->GetSize();
	int *arr2 = (int *)malloc(inlen * sizeof(int));

	ASSERT(arr2);

	//P2N("INeuNet::TrainOneFromArray Neuron counters: %d %d %d\r\n", incnt, midcnt, outcnt);

	// Build array
	for(int loop = 0; loop < inlen; loop++)	
		{
		arr2[loop] = arr->GetAt(loop);
		}

	//P2N("Dumping input:\r\n");
	//for(loop = 0; loop < inlen; loop++)	
	//	{
	//	P2N("%03d  ", arr2[loop]);
	//	}

	//P2N("Dumping output:\r\n");
	//for(loop = 0; loop < outlen; loop++)	
	//	{
	//	P2N("%03d  ", output[loop]);
	//	}

	TrainOne(arr2, inlen, output, outlen);

	//P2N("INeuNet::TrainOneFromArray Neuron counters done: %d %d %d\r\n", incnt, midcnt, outcnt);

	free(arr2);
}

//////////////////////////////////////////////////////////////////////////

void INeuNet::TrainOne(int *input, int inlen, int *output, int outlen)

{
	int layer, idx, inp, dead = 0, cnt = 0;
	double	olderr, newerr; 

	stop = false;

	AssignData(input, inlen);

	FireAll();
		
	olderr = newerr = SumDeviation(output, outlen);

	P2N("Training layer ... ");
		
	for(layer = 0; layer < LAYER_COUNT; layer++)
		{
		int	 num_neu, num_input;
		INeuron	 **ptr;

		P2N("%d ... ", layer);
			
		// This is to allow the computer to function during training
		if(sleeptime)
			{
			support.YieldToWinEx();
			Sleep(sleeptime);
			}

		if(stop)
			break;

		switch(layer)
			{
			case OUTLAYER: num_neu   = outcnt; num_input = midcnt;	
				ptr = outneu; 
				break;

			case MIDLAYER: num_neu   = midcnt; num_input = incnt;	
				ptr = midneu; 
				break;

			case INLAYER: num_neu   = incnt;  num_input = inputcnt; 
				ptr = inneu;  
				break;
			}

		for(idx = 0; idx < num_neu; idx++)
			{
			if(stop)
				break;

			//CString str; str.Format(
			//		"Train layer: %d neuron %d  (%d of %d reversed)", 
			//			layer, idx, dead, cnt);
			//
			//parent.m_prog.SetWindowText(str);
			//P2N("%s\r\n", str);

			cnt = 0; dead = 0;
			for(inp = 0; inp < num_input; inp++)
				{
				PokeNext((int)newerr / 2, ptr, idx, inp);
				//PokeNext((int)newerr , ptr, idx, inp);

				FireAll();

				if(sleeptime)
					{
					if(!(cnt % 50))
						{
						support.YieldToWinEx(); 
						Sleep(sleeptime);
						}
					}

				newerr = SumDeviation(output, outlen);

				//P2N("olderr=%f newerr=%f\r\n", olderr, newerr);

				int badpos = false;
				cnt++;
				if(olderr <= newerr)
					{
					UnPoke(); dead++;
					badpos = true;
					//P2N("[Unpoked=%.2f]  ", testerr);
					}
				else
					{
					//double	testerr = SumDeviation(output, outlen);
					//P2N("[OK=%.2f]  ", testerr);
					olderr = newerr;
					}

				if(stop)
					break;
			
				if(badpos)
					{
					PokeNext((int)-newerr / 2, ptr, idx, inp);
					//PokeNext((int)-newerr, ptr, idx, inp);

					FireAll();

					newerr = SumDeviation(output, outlen);
					//P2N("olderr=%f newerr=%f\r\n", olderr, newerr);
					cnt++;
					if(olderr <= newerr)
						{
						UnPoke(); dead++;
						//P2N("[Unpoked=%.2f]  ", testerr);
						}
					else
						{
						//double	testerr = SumDeviation(output, outlen);
						//P2N("[OK=%.2f]  ", testerr);
						olderr = newerr;
						}
					}

				if(stop)
					break;
				}
			}
		}	

	P2N("OK.\r\n");
	
}

//////////////////////////////////////////////////////////////////////////

void INeuNet::PokeNext(int val, INeuron **layer, int idx, int num)

{
	layer[idx]->Test();

	save_weight = layer[idx]->weight_arr[num];
	save_ptr = &(layer[idx]->weight_arr[num]);
	layer[idx]->weight_arr[num] += val;
}

//////////////////////////////////////////////////////////////////////////

void INeuNet::UnPoke()

{
	if(!save_ptr)
		{
		P2N("Duplicate Unpoke\r\n");
		return;
		}
	*save_ptr = save_weight;		
	save_ptr = 0;
}

//////////////////////////////////////////////////////////////////////////
// Data comes in distance::value 
// Data is pointer to integer vectors

int INeuNet::AssignData(int *data, int len)

{
	int *data2 = data;
	int	len2 = 0;
	int len3 = len; // sizeof(int);
	int loop, loop2;

	// Assign in data to inputs
	for(loop = 0; loop < incnt; loop++)
		{
		for(loop2 = 0; loop2 < inputcnt; loop2++)
			{
			if(len2 < len3 ) // * sizeof(int) )
				{
				inneu[loop]->synapse_arr[loop2]  = *data2++; len2++;
				//in2neu[loop].synapse_arr[loop2] = *data2++; len2++;
				}
			else
				{
				// Padd end of data with nulls
				inneu[loop]->synapse_arr[loop2]  = 0; len2++;
				//in2neu[loop].synapse_arr[loop2] = 0; len2++;
				}
			}
		}	
	return(0);
}
 
int	INeuNet::AssignData(CWordArray *arr)

{
	//int *data2 = data;
	int	len2 = 0;
	int len3 = arr->GetSize();
	int loop, loop2;

	// Assign in data to inputs
	for(loop = 0; loop < incnt; loop++)
		{
		for(loop2 = 0; loop2 < inputcnt; loop2++)
			{
			if(len2 < len3 ) // * sizeof(int) )
				{
				inneu[loop]->synapse_arr[loop2]  = arr->GetAt(len2++);
				//in2neu[loop].synapse_arr[loop2] = *data2++; len2++;
				}
			else
				{
				// Padd end of data with nulls
				inneu[loop]->synapse_arr[loop2]  = 0; len2++;
				//in2neu[loop].synapse_arr[loop2] = 0; len2++;
				}
			}
		}	
	return(0);
}



void INeuNet::Stop()

{
	stop = true;
}


#if 0

void	INeuNet::TryOne()

{
	PokeNext((int)newerr / 2, ptr, idx, inp);
	FireAll();

	if(sleeptime)
		{
		if(!(cnt % 50))
			{
			YieldToWin(); Sleep(sleeptime);
			}
		}

	newerr = SumDeviation(output, outlen);

	//P2N("olderr=%f newerr=%f\r\n", olderr, newerr);

	cnt++;
	if(olderr <= newerr)
		{
		UnPoke(); dead++;
		//P2N("[Unpoked=%.2f]  ", testerr);
		}
	else
		{
		//double	testerr = SumDeviation(output, outlen);
		//P2N("[OK=%.2f]  ", testerr);
		olderr = newerr;
		}
}

#endif

void INeuNet::TestNet()

{
	int loop;

	for(loop = 0; loop < incnt; loop++)
		{
		inneu[loop]->Test();
		}
	for(loop = 0; loop < midcnt; loop++)
		{
		midneu[loop]->Test();
		}
	for(loop = 0; loop < outcnt; loop++)
		{
		outneu[loop]->Test();
		}
}

//////////////////////////////////////////////////////////////////////////
//
// Function name:    INeuNet::Byte2Bits
// Description:      <empty description>
// Return type:      
// Argument:         unsigned char cc
// Argument:         int oneval		-- value if bit is zero (100)
// Argument:         int zeroval	-- value if bit is one	(10000)
// Argument:         int *arr		-- array of 8 integers
//
//////////////////////////////////////////////////////////////////////////

void	INeuNet::Byte2Bits(unsigned char cc, int oneval, int zeroval, int *arr)

{
	unsigned char mask = 0x80;

	//P2N("Byte2Bits (0x%x) %d\r\n", cc, cc);

	for(int loop = 0; loop < 8; loop++)
		{
		if(cc & (mask >> loop))
			arr[loop] = oneval;
		else
			arr[loop] = zeroval;
		}
}

//////////////////////////////////////////////////////////////////////////

int INeuNet::GetOutVal(int index)

{
	int ret;

	if(index < outcnt)
		{
		ret = outneu[index]->val;
		}
	else
		{
		P2N("INeuNet::GetOutVal() Index is past last output neuron\r\n");
		ret = -1;
		}
	return(ret);
}

