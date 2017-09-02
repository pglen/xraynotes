//////////////////////////////////////////////////////////////////////
//
// INeuNet.h: interface for the INeuNet class.
//
//////////////////////////////////////////////////////////////////////

#include "INeuron.h"


#if !defined(AFX_INEUNET_H__2D787EFC_3981_45F0_B33F_E89B1332EA79__INCLUDED_)
#define AFX_INEUNET_H__2D787EFC_3981_45F0_B33F_E89B1332EA79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LAYER_COUNT	3


class INeuNet  

{
public:

	int		stop;
	void	*parent;

protected:

	INeuron **inneu;
	INeuron **midneu;
	INeuron **outneu;

	int		save_weight;
	int		*save_ptr;

public:

	int		sleeptime;

	int		incnt;
	int		midcnt;
	int		outcnt;
	int		inputcnt;
		
	// Constructor/Destructor

public:

	INeuNet();
	int Init(int ninput, int ninp, int nmid, int noutp);
	virtual ~INeuNet();

public:
	int GetOutVal(int index);
	void TestNet();

// Operations:

	void	Train();
	void	FireAll();
	void	TrainOne(int *input, int inlen, int *output, int outlen);
	int		AssignData(int *data, int len);
	int		AssignData(CWordArray *arr);
	void	Stop();
	void	Byte2Bits(unsigned char cc, int oneval, int zeroval, int *arr);
	void	TrainOneFromArray(CWordArray *arr, int *output, int outlen);
	double	SumDeviation(int *refout, int outlen);

protected:

	
	void	PokeNext(int val, INeuron **layer, int idx, int num);
	void	UnPoke();
};

#endif // !defined(AFX_INEUNET_H__2D787EFC_3981_45F0_B33F_E89B1332EA79__INCLUDED_)
