#pragma once
#include "Individuo.h"

class Poblacion
{
public:
	vector<Individuo> Individuos;
	double BestFitness;
	Poblacion(void);
	Poblacion(vector<Individuo> individuos);
	~Poblacion(void);
};

