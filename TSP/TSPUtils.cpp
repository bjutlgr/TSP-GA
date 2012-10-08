#include "TSPUtils.h"
#include "Ran.h"
#include "EAGlobals.h"
#include <algorithm>
#include <functional>
#include <math.h>


TSPUtils::TSPUtils(void)
{
}


TSPUtils::~TSPUtils(void)
{
}

//validar un path
bool TSPUtils::ValidateTour(vector<int> path)
{
	sort(path.begin(), path.end());
	bool res=true;
	for(int i=0; i<(int)path.size();i++)
	{
		if(i+1==(int)path.size()) break;
		if(path.at(i) == path.at(i+1)) res=false;
	}

	return res;
}

//retorna la distancia euclideana en 2D
double TSPUtils::EuclideanDistance(pair<double, double> nodo1, pair<double, double> nodo2)
{
	double distance = sqrt(pow(nodo2.first-nodo1.first,2) + pow(nodo2.second-nodo1.second, 2));
	return distance;
}

double TSPUtils::FitnessEval(Individuo individuo, TSP newTSPInstance)
{
	double fitness=0;
	double eucDist;
	for(int i=0; i<(int)individuo.Genotipo.size(); i++)
	{
		if(i+1==(int)individuo.Genotipo.size()) break;
		eucDist = EuclideanDistance(newTSPInstance.coordenadas.at(individuo.Genotipo.at(i)-1),
			newTSPInstance.coordenadas.at(individuo.Genotipo.at(i+1)-1));//se resta 1 ya que las posiciones estan corridas
		fitness += eucDist; 
	}
	//sumamos la distancia de la ultima ciudad a la primera
	eucDist = EuclideanDistance(newTSPInstance.coordenadas.at(individuo.Genotipo.back()-1),
		newTSPInstance.coordenadas.at(individuo.Genotipo.front()-1));
	fitness +=eucDist;
	return fitness * (-1.0);
}

double TSPUtils::GetBestFitness(Poblacion poblacion)
{
	vector<Individuo> individuos = poblacion.Individuos;
	vector<double> fitness;
	for(int i=0; i<(int)individuos.size(); i++)
	{
		fitness.push_back(individuos.at(i).Fitness);
	}
	sort(fitness.begin(), fitness.end());

	return fitness.back();
}

Poblacion TSPUtils::GenerarPoblacionInicial(TSP newTSPInstance, Ran newRan)
{
	Poblacion PoblacionInicial;
	Individuo nuevoIndividuo;
	vector<Individuo> individuos;  //individuos para la poblacion inicial
	vector<int> pathRepresentation;  //auxiliar para ir eligiendo nodos
	vector<int> path;				//vector para guardar un camino aleatorio
	int randNumber;
	int numNodos = newTSPInstance.coordenadas.size();
	int aux = numNodos;
	double newFitness; 


	for(int j=0; j<TAM_POBLACION; j++)
	{
		aux = numNodos;
		for(int i=0; i<numNodos; i++) pathRepresentation.push_back(i);

		//creamos permutaciones aleatorias para los individuos de la poblacion inicial
		for(int i=0; i<numNodos; i++)
		{
			randNumber = newRan.int64() % (aux);  //aleatorio entre 0 y numNodos-1
			path.push_back(pathRepresentation.at(randNumber));  //agregamos el aleatorio al camino nuevo
			pathRepresentation.erase(pathRepresentation.begin()+randNumber); //lo borramos del conjunto
			aux--; //disminuimos el conjunto de donde se pueden seleccionar los nodos
		}

		// arreglo de error: se debe sumar +1 a cada valor ya que queda entre 0 y n-1
		for(int i=0; i<(int)path.size(); i++) path.at(i) += 1;

		nuevoIndividuo.Genotipo = path;
		path.clear();
		pathRepresentation.clear();
		newFitness = FitnessEval(nuevoIndividuo, newTSPInstance);
		nuevoIndividuo.Fitness = newFitness;
		individuos.push_back(nuevoIndividuo);

	}
	PoblacionInicial.Individuos = individuos;
	double bestFitness = GetBestFitness(PoblacionInicial);
	PoblacionInicial.BestFitness = bestFitness;
	return PoblacionInicial;
}



//order crossover, recibe 2 padres y entrega 2 hijos
pair<Individuo, Individuo> TSPUtils::OX(Individuo p1, Individuo p2, Ran newRan)
{
	Individuo h1, h2;
	/*
	Pasos: 
	1.-elegir dos puntos aleatorios para el corte
	2.-copiar los segmentos en cada hijo
	3.-para hijo1: ir copiando desde el segundo punto de corte
	4.-si alguno ya se encuentra en en el hijo se omite
	5.-idem para elprimer hijos
	*/
	int tamGen = p1.Genotipo.size();
	//elegimos los puntos aleatorios para el corte

	int ran1 = newRan.int64() % tamGen;
	int ran2 = newRan.int64() % tamGen;
	while(ran1 == ran2) {ran2 = newRan.int64() % tamGen;}   //en caso de que sean iguales
	int punto1,punto2;
	if (ran1 < ran2) //verificamos el mayor
	{
		punto1 = ran1;
		punto2 = ran2;
	}
	else

	{
		punto1 = ran2;
		punto2 = ran1;
	}

	//inicializamos vectores para genomas de los hijos
	vector<int> genHijo1(tamGen, 0);
	vector<int> genHijo2(tamGen, 0);

	//copiar segmento en hijos
	for(int i=punto1; i<punto2+1; i++)
	{
		genHijo1.at(i) = p1.Genotipo.at(i);
		genHijo2.at(i) = p2.Genotipo.at(i);
	}

	//para hijo 1

//	int tamSegmento = punto2 - punto1;
	//copiamos la parte despues del punto 2
	int aux1 = 0;
	int aux2 = 0;
	for(int i=0; i<tamGen; i++)
	{
		//nos saltamos el segmento 
		if(aux2>=punto1 && aux2<=punto2)
		{
			aux2++;
			i--;
			continue;
		}
		//verificamos que no este
		if(find(genHijo1.begin(), genHijo1.end(), p2.Genotipo.at(aux1))==genHijo1.end())
		{
			genHijo1.at(aux2) = p2.Genotipo.at(aux1);
			aux1++;
			aux2++;
		}
		else
		{
			aux1++;
		}
	}

	//para hijo2:


	aux1 = 0;
	aux2 = 0;
	for(int i=0; i<tamGen; i++)
	{
		//nos saltamos el segmento 
		if(aux2>=punto1 && aux2<=punto2)
		{
			aux2++;
			i--;
			continue;
		}
		//verificamos que no este
		if(find(genHijo2.begin(), genHijo2.end(), p1.Genotipo.at(aux1))==genHijo2.end())
		{
			genHijo2.at(aux2) = p1.Genotipo.at(aux1);
			aux1++;
			aux2++;
		}
		else
		{
			aux1++;
		}
	}

	h1.Genotipo = genHijo1;
	h2.Genotipo = genHijo2;
	return make_pair(h1, h2);

}

void TSPUtils::Inversion(Individuo &individuo, Ran newRan)
{
	//creamos un segmento aleatorio para la inversion
	int tamGen = individuo.Genotipo.size();
	//elegimos los puntos aleatorios para el seguemtno

	int ran1 = newRan.int64() % tamGen;
	int ran2 = newRan.int64() % tamGen;
	while(ran1 == ran2) {ran2 = newRan.int64() % tamGen;}   //en caso de que sean iguales
	int punto1,punto2;
	if (ran1 < ran2) //verificamos el mayor
	{
		punto1 = ran1;
		punto2 = ran2;
	}
	else

	{
		punto1 = ran2;
		punto2 = ran1;
	}


	//for para invertir el segmento
	

	//hacemos reverse
	vector<int>::iterator i = individuo.Genotipo.begin();
	reverse(i+punto1, i+punto2);

}

Poblacion TSPUtils::FitnessSelection(Poblacion poblacion, TSP newTSPInstance, Ran newRan)
{
	/*
	1.- Se evalua el fitness normalizado para cada individuo
	2.- Se ordena la poblacion de manera descendente de acuerdo a su fitness
	3.- Se computan los fitness normalizados acumulados (fitness propio+todos los anteriores)
	4.- se genera un aleatorio entre 0 y 1
	5.- El elegido es el primer individuo cuyo valor acumulado normalizado sea mayor que R

	*/
	Poblacion nuevaPoblacion;
	vector<Individuo> individuos = poblacion.Individuos;
	double corrimiento = 0;
	//ordenamos el vector de individuos y hacemos corrimiento si es que hay valores negativos
	sort(individuos.begin(), individuos.end(), greater<Individuo>());
	if(individuos.at(poblacion.Individuos.size()-1).Fitness < 0)
	{
		double tempFitness = individuos.at(poblacion.Individuos.size()-1).Fitness* -1.0;
		corrimiento = tempFitness*2;    //se realiza un corrimiento de 2 veces el fitness negativo*(-1)
	}

	//hacemos el corrimiento a todos los individuo (si no hay negativos corrimiento es 0)
	for(int i=0; i<(int)individuos.size() ; i++)
	{
		double tempFitness = individuos.at(i).Fitness;
		individuos.at(i).Fitness = tempFitness + corrimiento;
	}

	//ahora tenemos todos los fitness positivos y ordenados
	//calculamos los fitness normalizados
	double fitnessTotal = 0;
	for(int i=0; i<(int)individuos.size(); i++)
	{
		fitnessTotal += individuos.at(i).Fitness;
	}
	//seteamos el fitness de cada indiviuo a su fitness normalizado
	for(int i=0; i<(int)individuos.size(); i++)
	{
		double tempFitness = individuos.at(i).Fitness / fitnessTotal;
		individuos.at(i).Fitness = tempFitness;
	}

	//ordenamos el vector de manera descendiente : hecho arriba


	//computamos los fitness normalizados acumulados
	double acumFitness=0;
	for(int i=0; i<(int)individuos.size(); i++)
	{
		acumFitness += individuos.at(i).Fitness;
		individuos.at(i).Fitness = acumFitness;
	}

	/*Elitismo: pasamos los dos mejores a la nueva poblacion
	actualizamos su fitness y actualizamos el mejor fitness de la poblacion
	si es que es mejor que el anterior*/

	double bestFitness1 = FitnessEval(individuos.at(0), newTSPInstance);
	double bestFitness2 = FitnessEval(individuos.at(1), newTSPInstance);
	Individuo best(individuos.at(0));
	Individuo best2(individuos.at(1));
	best.Fitness = bestFitness1;
	best2.Fitness = bestFitness2;
	nuevaPoblacion.Individuos.push_back(best);
	nuevaPoblacion.Individuos.push_back(best2);
	if(poblacion.BestFitness<bestFitness1)
		nuevaPoblacion.BestFitness = bestFitness1;
	else nuevaPoblacion.BestFitness = poblacion.BestFitness;

	//escogemos los individuos para la nueva poblacion
	while(nuevaPoblacion.Individuos.size() != poblacion.Individuos.size())
	{
		//generamos un aleatorio
		double randNumber = newRan.doub();
		for(int i=0; i<(int)individuos.size(); i++)
		{
			if(individuos.at(i).Fitness >= randNumber)
			{
				//escogemos a este individuo y entra en la poblacion nueva con su fitness recalculado
				double tempFitness = FitnessEval(individuos.at(i), newTSPInstance);
				Individuo nuevo(individuos.at(i).Genotipo, tempFitness);
				nuevaPoblacion.Individuos.push_back(nuevo);
				break;
			}
		}
	}

	//se tiene la nueva poblacion
	//TODO: elitismo y volver los fitness a la normalidad
	return nuevaPoblacion;

}

