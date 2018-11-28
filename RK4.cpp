#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <numeric> 
#include <time.h>
#include "3DVectClass.h"
#include "hulpfuncties.h"


/******************************
*							  *
*  Runge-Kutta Method (RK4)	  *
*							  *
******************************/



void RK4(std::vector<double> m, std::vector<Vec> r, std::vector<Vec> v, int N, int iteraties, double h, std::string naam) {

	// maak een file aan waar de posities van de deeltjes wordt bijgehouden
	std::ofstream outfile1(naam + "_RK4.txt");
	outfile1 << std::setprecision(15);

	// beginposities meegeven
	for (int j = 0; j < N; j++) {
		outfile1 << r[j].x() << ' ' << r[j].y() << ' ' << r[j].z() << '\t';
	}
	outfile1 << std::endl;

	// maak een file aan waar de energieŽn worden bijgehouden
	std::ofstream outfile2(naam + "_RK4_E.txt");
	outfile2 << std::setprecision(15);

	// maak een file aan waar de relatieve fouten van de energieŽn worden bijgehouden
	std::ofstream outfile3(naam + "_RK4_E_err.txt");
	outfile3 << std::setprecision(15);

	// hou de startenergie van het systeem bij
	double start_energie = Energie(r, v, m);

	std::vector<double> tijd_iteratie;

	// begint te itereren over het aantal iteraties die je wilt uitvoeren
	
	

	for (int i = 0; i < iteraties; i++) {
		
		double h_var = variabele_h(h, r);

		std::vector<Vec> kr1;
		std::vector<Vec> kv1;
		std::vector<Vec> kr2;
		std::vector<Vec> kv2;
		std::vector<Vec> kr3;
		std::vector<Vec> kv3;
		std::vector<Vec> kr4;
		std::vector<Vec> kv4;

		clock_t sstart = clock();

		for (int j = 0; j < N; j++) {
			kr1.push_back(v[j]);
			kv1.push_back(a(m, r, j, N));
		}

		for (int j = 0; j < N; j++) {
			kr2.push_back(v[j] + .5*h_var*kv1[j]);
			kv2.push_back(a(m, r + .5*h_var*kr1, j, N));
		}

		for (int j = 0; j < N; j++) {
			kr3.push_back(v[j] + .5*h_var*kv2[j]);
			kv3.push_back(a(m, r + .5*h_var*kr2, j, N));
		}

		for (int j = 0; j < N; j++) {
			kr4.push_back(v[j] + h_var * kv3[j]);
			kv4.push_back(a(m, r + h_var * kr3, j, N));
		}
		

		r = r + (h_var / 6) * (kr1 + 2 * kr2 + 2 * kr3 + kr4);
		v = v + (h_var / 6) * (kv1 + 2 * kv2 + 2 * kv3 + kv4);

		tijd_iteratie.push_back((clock() - sstart) / (CLOCKS_PER_SEC/1000));

		for (int j = 0; j < N; j++) {
			outfile1 << r[j].x() << ' ' << r[j].y() << ' ' << r[j].z() << '\t';
		}


		outfile1 << std::endl;
		outfile2 << Energie(r, v, m) << std::endl;
		outfile3 << error_energie(r, v, m, start_energie) << std::endl;
		
	}

	std::cout << "Posities werden bijgehouden in bestand " << naam << "_RK4.txt" << std::endl;
	std::cout << "Energie werd bijgehouden in bestand " << naam << "_RK4_E.txt" << std::endl;
	std::cout << "Relatieve energiefouten werden bijgehouden in bestand " << naam << "_RK4_E_err.txt" << std::endl;
	outfile1.close();
	outfile2.close();
	outfile3.close();

	double tijd_gemiddelde;
	tijd_gemiddelde = accumulate(tijd_iteratie.begin(), tijd_iteratie.end(), 0.0) / tijd_iteratie.size();

	std::cout << tijd_gemiddelde << ' ' << "milliseconden per iteratie" << std::endl;
}