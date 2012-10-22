#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Individuo.h"
#include "Poblacion.h"
#include "TSP.h"
#include "Ran.h"
#include "EAGlobals.h"
#include "TSPUtils.h"
#include "tclap/CmdLine.h"
#include <algorithm>

using namespace TCLAP;
using namespace std;

int main(int argc, char **argv)
{
	// Define the command line object.
	CmdLine cmd("Command description message", ' ', "0.9");
	// Define a value argument and add it to the command line.
	ValueArg<string> fileArg("f", "file", "Path to file", true, "homer", "string");
	ValueArg<int> popSizeArg("p","popSize","Population size",true,5,"int");
	ValueArg<int> genNumberArg("g","genNumber","Numero de generaciones",true,5,"int");
	ValueArg<double> mutRateArg("m","mutRate","Mutation rate",true,5,"double");
	ValueArg<double> crossRateArg("c","crossRate","Crossover rate",true,5,"double");
	ValueArg<double> seedArg("s","seed","Seed",true,5,"double");
	
	cmd.add( fileArg );
	cmd.add( popSizeArg );
	cmd.add( genNumberArg );
	cmd.add( mutRateArg );
	cmd.add( crossRateArg );
	cmd.add( seedArg );

	// Parse the args.
	cmd.parse( argc, argv );

	// Get the value parsed by each arg. 
	string file = fileArg.getValue();
	int popSize = popSizeArg.getValue();
	int genNumber = genNumberArg.getValue();
	double mutRate = mutRateArg.getValue();
	double crossRate = crossRateArg.getValue();
	double seed = seedArg.getValue();

	// Do what you intend too...
	
	//nueva instancia de TSP tipo EUCLID_2D
	TSP newTSP;
	Ran newRan(seed);
	//leemos las coordenadas
	///home/pablo/.codelite/code/TSP/ALL_tsp/berlin52.tsp
	
	ifstream node (file.c_str());
	string linea;
	int flag=0;
	vector< pair <double, double> > coordenadas;
	int num;
	double cX, cY;
	flag=0;

	if(node.is_open())
	{
		while(node.good())
		{
			if(flag==0) //aun no se leen coordenadas
			{
				getline(node, linea);
				if(linea.compare("NODE_COORD_SECTION")==0) flag=1;
			}
			if(flag==1) //comenzo seccion de corrdenadas
			{
				getline(node, linea);
				if(linea.compare("EOF")==0) break; //fin de archivo
				istringstream line(linea);
				line >> num >> cX >> cY;
				coordenadas.push_back(make_pair(cX, cY));
				//creamos las coordenadas en el vector 

			}
		}

		node.close();
	}

	//CREAMOS LA INSTANCIA PARA ESTE TSP

	newTSP.coordenadas = coordenadas;
	newTSP.tamPoblacion = popSize;

	//Poblacion poblacion = TSPUtils::Greedy(newTSP, newRan);
	Poblacion poblacion = TSPUtils::GenerarPoblacionInicial(newTSP, newRan);

	//INVERSION: recibe un individuo y lo devuelve mutado
	//CRUZAMIENTO: recibe 2 individuos y devuelve un pair de hijos
	//MUTACION NO ACTUALIZA EL FITNESS DE LOS HIJOS

	Poblacion intermedia, poblacionFinal;
	//generaciones
	//para cruzamiento y mutacion
	intermedia = poblacion;
	
	for(int i=0; i<genNumber; i++)
	{
		//realizar cruzamiento
		//elegimos dos individuos para cruzar
		//seleccion

		intermedia  = TSPUtils::FitnessSelection(intermedia, newTSP, newRan);
		
		//caso en que la poblacion sea = a 1
		
		//si la poblacion = a 1 o a 3 solo pasamos un individuo
		if(popSize==3 || popSize==1) poblacionFinal.Individuos.push_back(intermedia.Individuos.at(0));
		
		//pasamos los dos mejores a la poblacion final
		else
		{
		poblacionFinal.Individuos.push_back(intermedia.Individuos.at(0));
		poblacionFinal.Individuos.push_back(intermedia.Individuos.at(1));
		}
		while (poblacionFinal.Individuos.size() != intermedia.Individuos.size())
		{
			
			//cruzamiento
			int p1 = newRan.int64() % poblacion.Individuos.size();
			int p2 = newRan.int64() % poblacion.Individuos.size();
			//verificamos si hay cruzamiento
			if(newRan.doub() <= crossRate)
			{
				pair<Individuo, Individuo> hijos = TSPUtils::OX(intermedia.Individuos.at(p1), intermedia.Individuos.at(p2), newRan);

				//realizamos mutacion probabilistica antes de insertar a los hijos
				if(newRan.doub() <= mutRate)
				{
					TSPUtils::Inversion(hijos.first, newRan);
				}
				if(newRan.doub() <= mutRate)
				{
					TSPUtils::Inversion(hijos.second, newRan);
				}
				poblacionFinal.Individuos.push_back(hijos.first);
				poblacionFinal.Individuos.push_back(hijos.second);
			}
			else
			{
				//se pasan los padres a la siguiente generacion
				poblacionFinal.Individuos.push_back(intermedia.Individuos.at(p1));
				poblacionFinal.Individuos.push_back(intermedia.Individuos.at(p2));
			}
			
			//caso en que la poblacion sea impar
			if(poblacionFinal.Individuos.size() == intermedia.Individuos.size() -1)
			{
				//escogemos un individuo para ingresarlo
				int rand = newRan.int32() % popSize;
				poblacionFinal.Individuos.push_back(intermedia.Individuos.at(rand));
			}
		}
		//recalculamos los fitness
		for(int i=0; i<(int)poblacionFinal.Individuos.size(); i++)
		{
			poblacionFinal.Individuos.at(i).Fitness = TSPUtils::FitnessEval(poblacionFinal.Individuos.at(i), newTSP);
		}

		intermedia = poblacionFinal;
		poblacionFinal.Individuos.clear();

	}

	/*cout << " mejor solucion tour final es" << endl;
	for(int i=0; i<(int)intermedia.Individuos.at(0).Genotipo.size(); i++)
	{
		cout << intermedia.Individuos.at(0).Genotipo.at(i)<<"-";
		
	}*/
	//cout << TSPUtils::ValidateTour(intermedia.Individuos.at(0).Genotipo);
	cout << TSPUtils::FitnessEval(intermedia.Individuos.at(0), newTSP)*1.0<<endl;
	
}