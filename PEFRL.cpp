#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <numeric> 
#include "3DVectClass.h"
#include "hulpfuncties.h"
#include "kost_integratie.h"

using namespace std;

/*******************************************************
*										               *
*  Position Extended Forest-Ruth Like (PEFRL) Method   *
*										               *
*******************************************************/

void PEFRL(std::vector<double> m, std::vector<Vec> r, std::vector<Vec> v, int N, double integratietijd, double h, std::string naam, double gebruiken_var_h) {

	// maak een file aan waar de posities van de deeltjes wordt bijgehouden
	std::ofstream outfile1(naam + "_PEFRL.txt");
	outfile1 << std::setprecision(15);

	//lijst voor bijhouden van alle h's
	std::vector<double> h_lijst;

	// beginposities meegeven
	for (int j = 0; j < N; j++) {
		outfile1 << r[j].x() << ' ' << r[j].y() << ' ' << r[j].z() << '\t';
	}
	outfile1 << std::endl;

	// maak een file aan waar de energieŽn worden bijgehouden
	std::ofstream outfile2(naam + "_PEFRL_E.txt");
	outfile2 << std::setprecision(15);

	// maak een file aan waar de relatieve fouten van de energieŽn worden bijgehouden
	std::ofstream outfile3(naam + "_PEFRL_E_err.txt");
	outfile3 << std::setprecision(15);

	// hou de startenergie van het systeem bij
	double start_energie = Energie(r, v, m);

	double lambda = -0.2123418310626054;
	double xi = 0.1786178958448091;
	double chi = -0.06626458266981849;

	std::vector<Vec> acc;
	std::vector<double> tijd_iteratie;

	for (int j = 0; j < N; j++) {
		acc.push_back(Vec(0., 0., 0.));
	}
	// iteratie over aantal integraties
	double verstreken_tijd = 0;
	while (verstreken_tijd < integratietijd) {

		double h_var = h;
		if (gebruiken_var_h)
			double h_var = variabele_h(h, r);
		verstreken_tijd += h_var;
		h_lijst.push_back(h_var);

		clock_t sstart = clock();

		//iteratie over aantal deeltjes
		for (int i = 0; i < N; i++) {
			// substep 1
			r[i] = r[i] + (xi*h_var)*v[i];
		}
		for (int i = 0; i < N; i++) {
			// berekenen van de versnelling
			acc[i] = a(m, r, i, N);
		}
		for (int i = 0; i < N; i++) {
			// substeps 2 & 3
			v[i] = v[i] + 0.5*(1. - 2. * lambda)* h_var*acc[i];
			r[i] = r[i] + chi * h_var*v[i];
		}
		for (int i = 0; i < N; i++) {
			// berekenen van de versnelling
			acc[i] = a(m, r, i, N);
		}
		for (int i = 0; i < N; i++) {
			//substeps 4 & 5
			v[i] = v[i] + (lambda * h_var) * acc[i];
			r[i] = r[i] + (1. - 2. * (chi + xi))*h_var*v[i];
		}
		for (int i = 0; i < N; i++) {
			// berekenen van de versnelling
			acc[i] = a(m, r, i, N);
		}
		for (int i = 0; i < N; i++) {
			// substeps 6 & 7
			v[i] = v[i] + (lambda * h_var)*acc[i];
			r[i] = r[i] + chi * h_var*v[i];
		}
		for (int i = 0; i < N; i++) {
			// berekenen van de versnelling
			acc[i] = a(m, r, i, N);
		}
		for (int i = 0; i < N; i++) {
			// substeps 8 & 9
			v[i] = v[i] + 0.5*(1. - 2. * lambda)* h_var*acc[i];
			r[i] = r[i] + (xi*h_var)*v[i];
		}
			
		tijd_iteratie.push_back((clock() - sstart) / (CLOCKS_PER_SEC / 1000));
		for (int i = 0; i < N; i++) {

			//uitschrijven naar file
			outfile1 << r[i].x() << ' ' << r[i].y() << ' ' << r[i].z() << '\t';
		}
		outfile1 << std::endl;


		//uitschrijven van energie en error_energie
		outfile2 << Energie(r, v, m) << std::endl;
		outfile3 << error_energie(r, v, m, start_energie) << '\t' << dichtste_afstand(r) << std::endl;
	}
	outfile1.close();
	outfile2.close();
	outfile3.close();

	std::cout << "De kost bedroeg " << kost_int_methode_varh(h_lijst, N, 6) << std::endl;
	std::cout << "Posities werden bijgehouden in bestand " << naam << "_PEFRL.txt" << std::endl;
	std::cout << "Energie werd bijgehouden in bestand " << naam << "_PEFRL_E.txt" << std::endl;
	std::cout << "Relatieve energiefouten en dichtste afstanden werden bijgehouden in bestand " << naam << "_PEFRL_E_err.txt" << std::endl;

	double tijd_gemiddelde;
	tijd_gemiddelde = accumulate(tijd_iteratie.begin(), tijd_iteratie.end(), 0.0) / tijd_iteratie.size();

	std::cout << tijd_gemiddelde << ' ' << "milliseconden per iteratie" << std::endl;

}