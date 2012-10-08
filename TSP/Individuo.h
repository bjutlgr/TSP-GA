#pragma once
#include <vector>

using namespace std;
class Individuo
{
public:
	vector<int> Genotipo;
	double Fitness;
	Individuo(void);
	Individuo(vector<int> genotipo, double Fitness);
	~Individuo(void);
	bool operator < (const Individuo& individuo) const    //para poder ordenar un vector de individuos con sort()
	{
		return (Fitness < individuo.Fitness);
	}
	bool operator > (const Individuo& individuo) const    //para poder ordenar un vector de individuos con sort()
	{
		return (Fitness > individuo.Fitness);
	}
};

