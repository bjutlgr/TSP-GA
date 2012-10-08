#pragma once
#include "Poblacion.h"
#include "TSP.h"
#include "Ran.h"
class TSPUtils
{
public:
	TSPUtils(void);
	~TSPUtils(void);
	static double EuclideanDistance(pair<double, double> nodo1, pair<double, double> nodo2);
	static Poblacion GenerarPoblacionInicial(TSP newTSPInstance, Ran newRan);
	static Poblacion Greedy(TSP newTSPInstance, Ran newRan);
	static double FitnessEval(Individuo individuo, TSP newTSPInstance);
	static pair<Individuo, Individuo> OX(Individuo p1, Individuo p2, Ran newRan);
	static bool ValidateTour(vector<int> path);
	static void Inversion(Individuo &individuo, Ran newRan);
	static Poblacion FitnessSelection(Poblacion poblacion, TSP newTSPInstance, Ran newRan);
	static double GetBestFitness(Poblacion poblacion);
};

