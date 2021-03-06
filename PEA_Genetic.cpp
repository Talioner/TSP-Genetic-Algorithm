#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;


//Stoepr
class Stopwatch {
private:
	clock_t tstart;
	clock_t tstop;
public:
	double time;
	Stopwatch::Stopwatch() {

	}
	void Stopwatch::start() {
		tstart = clock();
		time = 0;
	}
	void Stopwatch::stop() {
		tstop = clock();
		time = ((double)(tstop - tstart)) / CLOCKS_PER_SEC;
	}
	double Stopwatch::read() {
		return time;
	}
};

int populationSize = 20;
int bestTourCost = INT_MAX;
int* bestTour;

//Oblicza koszt trasy 
int cost(int* population, vector<vector<int>> m) {
	int cost = 0;
	for (int i = 0; i < m.size() - 1; i++) cost += m[population[i]][population[i+1]];
	cost += m[population[m.size() - 1]][population[0]];
	return cost;
}


// Losowo generuje startowa populacje
int** generatePopulation(vector<vector<int>> m) {
	//tworzy tablice
	int** population = new int* [populationSize];
	for (int i = 0; i < populationSize; i++) {
		population[i] = new int[m.size()];
		for (int j = 0; j < m.size(); j++) {
			population[i][j] = j;
		}
	}
	//miesza losowa kolejnosc wierzcholkow
	for (int i = 0; i < populationSize; i++) {
		for (int j = 0; j < m.size(); j++) {
			swap(population[i][rand() % (m.size())], population[i][rand() % (m.size())]);
		}
	}
	
	return population;
}


// Wybiera rodzica metoda turniejowa (losuje dwoch i wybiera lepszego - tansza droge)
int* choseParent(int** population, vector<vector<int>> m) {
	
	int* winnerParent;
	int parentA, parentB;
	parentA = rand() % populationSize;
	parentB = rand() % populationSize;
	while (parentB == parentA) parentB = rand() % populationSize;
	//cout << cost(population[parentA], m) << " vs " << cost(population[parentB], m) << endl;
	if (cost(population[parentA], m) < cost(population[parentB], m)) winnerParent = population[parentA];
	else winnerParent = population[parentB];

	return winnerParent;
}


// Sprawdza czy wierzcholek juz wystepuje w danej trasie (v - wierzcholek)
bool contains(int* offspring, int v, int matrixSize) {
	bool isThere = false;
	for (int i = 0; i < matrixSize; i++) {
		if (offspring[i] == v) isThere = true;
	}
	if (isThere) return true;
	else return false;
}

// Tworzy dzieci
int** createOffspring(int** parents, vector<vector<int>> m, int parentsAmount) {
	//srand(time(NULL));
	int** fullOffspring = new int*[populationSize];										//stworzenie tablicy calego potomstwa


	int* parentA, *parentB;																//rodzice
	int firstCutPoint, secondCutPoint;													//punkty przeciec

	for (int i = 0; i < populationSize; i++) {											//
		int* offspring = new int[m.size()];												//dziecko do stworzenia
		for (int z = 0; z < m.size(); z++) {											//
			offspring[z] = -1;															//przygotowuje nowe dziecko do tworzenia, wypelnia charakterystyczna wartoscia dla pozniejszego rozroznienia nieuzupelnionych pol
		}																				//
		parentA = parents[rand() % parentsAmount];										//
		parentB = parents[rand() % parentsAmount];										//wybiera rodzicow do wydania potomstwa
		while (parentB == parentA) parentB = parents[rand() % parentsAmount];			//

		int cutPoint1 = rand() % (m.size() - 1) + 1;									//
		int cutPoint2 = rand() % (m.size() - 1) + 1;									//
		while (cutPoint2 == cutPoint1) cutPoint2 = rand() % (m.size() - 1) + 1;			//
		if (cutPoint2 < cutPoint1) {													//
			firstCutPoint = cutPoint2;													//
			secondCutPoint = cutPoint1;													//wybiera punkty przeciecia rodzicow
		}																				//
		else {																			//
			firstCutPoint = cutPoint1;													//
			secondCutPoint = cutPoint2;													//
		}																				//

		for (int j = firstCutPoint; j < secondCutPoint; j++) {							//
			offspring[j] = parentA[j];													//kopiuje wyciety fragment z pierwszego rodzica do dziecka
		}																				//


		int nextIndex = secondCutPoint;													//indeks do ktorego szukamy wierzcholka
		for (int j = secondCutPoint; j < m.size(); j++) {								//
			if (!contains(offspring, parentB[j], m.size())) {							//
				offspring[nextIndex % m.size()] = parentB[j];							//uzupelnia brakjace wierzcholki u dziecka z drugiego rodzica
				nextIndex++;															//zaczynajac od drugiego punktu przeciecia do konca drugiego rodzica
			}																			//
		}																				//

		for (int j = 0; j < secondCutPoint; j++) {										//
			if (!contains(offspring, parentB[j], m.size())) {							//
				offspring[nextIndex % m.size()] = parentB[j];							//uzupelnia brakujace wierzcholki u dziecka z drugiego rodzica
				nextIndex++;															//zaczynajac od poczatku drugiego rodzica do drugiego punktu przeciecia
			}																			//
		}																				//

		int mutationRoll = rand() % 1000;
		if (mutationRoll > 998) {
			//cout << "Zaszla mutacja" << endl;
			swap(offspring[rand() % m.size()], offspring[rand() % m.size()]);			//mutacja polegajaca na zamianie dwoch losowych wierzcholkow
		}

		fullOffspring[i] = offspring;
	}

	return fullOffspring;
}


// Wybiera rodzicow z aktualnej populacji
int** createNewGeneration(int** population, vector<vector<int>> m) {
	bool isAlreadyAParent = false;
	int* checkParent = new int[m.size()];
	int** parents = new int*[populationSize];							//tablica wybranych rodzicow (niepelna - zostanie uzupelniona dziecmi)
	int** fullOffspring;												//tablica nowego potomstwa
								
	int i = 0;															//indeks kolejnego wybranego rodzica
	for (int p = 0; p < populationSize; p++) {
		checkParent = choseParent(population, m);						//wybieramy rodzica do sprawdzenia
		for (int j = 0; j < populationSize; j++) {
			if (parents[j] == checkParent) isAlreadyAParent = true;		//sprawdzamy czy nie zostaj juz wybrany
		}
		if (!isAlreadyAParent) {
			parents[i] = checkParent;									//jesli nie zostal wybrany to dodajemy go do listy rodzicow
			i++;
		}
		isAlreadyAParent = false;
	}

	for (int x = i; x < populationSize; x++) {							//reszte tablicy uzupelniamy 0
		parents[x] = new int[m.size()];
		for (int k = 0; k < m.size(); k++) {
			parents[x][k] = 0;
		}
	}

	fullOffspring = createOffspring(parents, m, i);						//stworzenie potomstwa (i - liczba rodzicow)

	int** newGeneration = new int*[populationSize];						//stworzenie nowego pokolenia (najelpszy osobnik z poprzedniego pokolenia + dzieci)
	int bestParentCost = INT_MAX;	
	int* bestParent = new int[m.size()];								//
	for (int i = 0; i < populationSize; i++) {							//
		if (cost(population[i], m) < bestParentCost) {					//
			bestParentCost = cost(population[i], m);					//znalezienie najlepszego osobnika aktualnego pokolenia
			bestParent = population[i];									//
		}																//
	}																	//

	newGeneration[0] = bestParent;										//dodanie najlepszego osobnika aktualnego pokolenia do nowego pokolenia
	for (int j = 1; j < populationSize; j++) {							//
		newGeneration[j] = fullOffspring[j];							//dodanie dzieci do nowego pokolenia
	}																	//
	//delete[] checkParent;
	delete[] parents;
	delete[] fullOffspring;
	return newGeneration;												//zwraca nowe pokolenie
}


//Sprawdza czy w danej generacji jest rozwiazanie lepsze od najlepszego dotychczasowego
bool checkForBest(int** population, int best, vector<vector<int>> m) {
	bool isBest = false;
	for (int i = 0; i < populationSize; i++) {
		if (cost(population[i], m) < best) {
			bestTourCost = cost(population[i], m);
			best = bestTourCost;
			bestTour = population[i];
			isBest = true;
		}
	}
	return isBest;
}


int geneticAlgorithm(vector<vector<int>> m) {

	int iterations = 1000;												//liczba iteracji głownej petli programu
	srand(time(NULL));

	int** population = generatePopulation(m);							//generujemy startowa populacje
	cout << "KOSZT NAJLEPSZEGO" << endl;
	checkForBest(population, bestTourCost, m);
	cout << bestTourCost << endl;
	int** newGeneration = createNewGeneration(population, m);			//tworzymy pierwszą nową generację

	for (int i = 0; i < iterations; i++) {
		newGeneration = createNewGeneration(newGeneration, m);			//tworzy nowe pokolenie (tutaj znajduje się największa część algorytu)
		if (checkForBest(newGeneration, bestTourCost, m));
			//cout << bestTourCost << "  " << i << endl;
	}
	
	
	return 0;
}



int main()
{

	vector<vector<int>> start;
	Stopwatch clock;
	char salesmanSelection = '111';
	while (salesmanSelection != '0') {
		cout << "1 - wygeneruj losowe dane" << endl;
		cout << "2 - wyswietl dane" << endl;
		cout << "3 - genetic algorithm" << endl;
		cout << "4 - wczytaj dane z tsp_data.txt" << endl;
		cout << "Aby zakonczyc - 0" << endl;
		cout << "Wybierz: ";
		cin >> salesmanSelection;
		cout << endl;

		switch (salesmanSelection)
		{
		case '1':
		{

			/*while(!start.matrixV.empty()) start.matrixV.pop_back();
			while (!start.fullRoute.empty()) start.fullRoute.pop_back();
			start.lb = 0;*/
			int dimension;
			cout << "Liczba miast: ";
			cin >> dimension;
			cout << endl;

			srand(time(NULL));

			for (int i = 0; i < dimension; i++) {
				start.push_back(vector<int>());
				for (int j = 0; j < dimension; j++) {
					if (i == j) start[i].push_back(-1);
					else start[i].push_back((rand() % (99 - 10) + 10));
				}
			}
			break;

		}
		break;
		case '2':
		{
			for (int i = 0; i < start.size(); i++) {
				for (int j = 0; j < start[i].size(); j++) {
					cout << start[i][j] << "   ";
				}
				cout << endl;
			}
			break;
		}
		break;
		case '3':
		{
			clock.start();
			geneticAlgorithm(start);
			clock.stop();
			cout << "Czas wykonywania: " << clock.read() << endl;
			cout << "Najlepszy: " << bestTourCost << endl;
			break;
		}
		break;
		case '4':
		{
			int dimension;
			int buf = 0;
			fstream file;

			file.open("ftv38.atsp");
			file >> dimension;
			for (int i = 0; i < dimension; i++) {
				start.push_back(vector<int>());
				for (int j = 0; j < dimension; j++) {
					file >> buf;
					start[i].push_back(buf);
				}
			}

			/*file.open("gr17.tsp");
			file >> dimension;
			for (int i = 0; i < dimension; i++) {
			start.push_back(vector<int>());
			for (int j = 0; j < dimension; j++) {
			start[i].push_back(0);
			}
			}

			for (int i = 0; i < dimension; i++) {
			for (int j = 0; j <= i; j++) {
			file >> buf;
			if (j == i) start[i][j] = -1;
			else {
			start[i][j] = buf;
			start[j][i] = start[i][j];
			}
			}
			}*/

			break;
		}
		}
	}


	getchar();
	return 0;
}

