#ifndef HULPFUNCTIES_H
#define HULPFUNCTIES_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "3DVectClass.h"

//functie voor berekenen kleinere h indien nodig (2 objecten dicht bij elkaar)
float variabele_h(float h, std::vector<Vec> posities);

// m = massa	r = positie		N = # deeltjes		i = beschouwde deeltje
Vec a(std::vector<float> m, std::vector<Vec> r, int i, int N);

//functie voor berekenen totale energie systeem
double Energie(std::vector<Vec> poslist, std::vector<Vec> velolist, std::vector<double> masslist);

//functie voor berekenen fout op energie
double error_energie(std::vector<Vec> poslist, std::vector<Vec> velolist, std::vector<double> masslist, double start_energie);

//functie voor wegschrijven alle posities of snelhedennaar een tekstbestand
void wegschrijven(std::vector<std::vector<Vec>> coords, std::string naamn, int deeltjes);

#endif