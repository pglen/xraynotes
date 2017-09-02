//////////////////////////////////////////////////////////////////////
//
// INeuron.h: interface for the INeuron class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INEURON_H__EF4F0F2B_0A3A_49BF_9041_CF0424967919__INCLUDED_)
#define AFX_INEURON_H__EF4F0F2B_0A3A_49BF_9041_CF0424967919__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class INeuron  

{

public:
	double Transfer(double val);

	int Test();

	void PrintVal();
	void Init_Vars();
	void Dump();
	void Alloc(int num);
	void Alloc(int num, int axx);
	void Noise(int maxval);
	void Connect(INeuron *ptr);
	void Rand(int maxval = RAND_MAX);
	void Fire();

	INeuron();
	INeuron(int num);
	INeuron(int inum, int onum);


	virtual ~INeuron();

	int		*synapse_arr;			// Synapse array
	int		*weight_arr;			// Every synapse has a weight
	int		*offset_arr;			// Every synapse has an offset
	
	int		val;					// Current neuron value
	int		hidden;
	

protected:

	int		magic;
	int		synapse_num;			// Number of synapses
	

	int		axon_num;				// Number of dependent axons
	int		**axon;					// Dependent axons

	int		connect_num;
	int		incoming_num;

	// Future implementations
	
	int		treshold;				// Firing treshold
	int		decay;					// Neuron decay


};

#endif // !defined(AFX_INEURON_H__EF4F0F2B_0A3A_49BF_9041_CF0424967919__INCLUDED_)
