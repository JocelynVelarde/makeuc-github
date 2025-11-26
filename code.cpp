// g++ main.cpp -o main.exe
// main.exe < in.txt
#include <iostream>
#include <vector>
#include <ctime>
#include <string> // Agregado para seguridad en manejo de strings
using namespace std;
#define INF 9999999

string ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";

// Prototipos
void initialPopulation(vector<string> &poblacion, string objetivo);
vector<int> objectiveFunction(vector<string> &poblacion, string objetivo);
void printPopulation(vector<string> &poblacion, vector<int> &fitness);
vector<string> crossover(vector<string> &poblacion, vector<int> &fitness);
void mutation(vector<string> &hijos, int nMutacion);
void reinsertion(vector<string> &poblacion, vector<int> &fitness, vector<string> &hijos, vector<int> &fitnessHijos);
int findBest(vector<int> &fitness);

// Complejidad: O(Iteraciones * (N * M))
// El ciclo principal domina la complejidad, ejecutando cruce, mutación y evaluación repetidamente.
int main() {
  srand(time(NULL));
  string objetivo = "Tec de Monterrey", best;
  int iteraciones = 10000, sizePop = 30, nMutacion = 3, globalBest = INF, idx;
  vector<string> poblacion(sizePop), hijos;
  vector<int> fitness, fitnessHijos;
  
  initialPopulation(poblacion, objetivo);
  fitness = objectiveFunction(poblacion, objetivo);
  
  cout << "Poblacion inicial:" << endl;
  printPopulation(poblacion, fitness);
  
  int iter = 0;
  while (iter < iteraciones && globalBest > 0) {
    iter++;
    hijos = crossover(poblacion, fitness);
    mutation(hijos, nMutacion);
    fitnessHijos = objectiveFunction(hijos, objetivo);
    reinsertion(poblacion, fitness, hijos, fitnessHijos);
    
    idx = findBest(fitness);
    if (globalBest > fitness[idx]) {
      globalBest = fitness[idx];
      best = poblacion[idx];
      cout << iter << " " << best << " " << globalBest << endl;
    }
  }
  cout << "Poblacion final:" << endl;
  printPopulation(poblacion, fitness);
}

// Complejidad: O(N * M)
// Se recorren N individuos y se generan M caracteres para cada uno.
void initialPopulation(vector<string> &poblacion, string objetivo) {
  int n = poblacion.size(), m = objetivo.size();
  string individuo;
  for (int i=0; i<n; i++) {
    individuo = "";
    for (int j=0; j<m; j++) {
      individuo += ABC[rand() % ABC.size()];
    }
    poblacion[i] = individuo;
  }
}

// Complejidad: O(N * M)
// Se recorren N individuos y se comparan M caracteres por individuo.
vector<int> objectiveFunction(vector<string> &poblacion, string objetivo) {
  // Algoritmo de la funcion objetivo
  int n = poblacion.size();
  int m = objetivo.size();
  vector<int> fitness(n);

  for(int i = 0; i < n; i++) {
      int score = 0; // Entre menor sea el score, mejor es el individuo (0 es perfecto)
      for(int j = 0; j < m; j++) {
          // Si el caracter no coincide, penalizamos aumentando el valor de fitness
          if(poblacion[i][j] != objetivo[j]) {
              score++;
          }
      }
      fitness[i] = score;
  }
  return fitness;
}

// Complejidad: O(N)
// Imprimir toma tiempo proporcional al tamaño de la población.
void printPopulation(vector<string> &poblacion, vector<int> &fitness) {
  int n = poblacion.size();
  for (int i=0; i<n; i++) {
    cout << poblacion[i] << " " << fitness[i] << endl;
  }
  cout << endl;
}

// Complejidad: O(N * M)
// Se generan N hijos. La creación de substrings y concatenación toma tiempo proporcional a M.
vector<string> crossover(vector<string> &poblacion, vector<int> &fitness) {
  int n = poblacion.size(), m = poblacion[0].size();
  vector<string> hijos(n);
  string padre1, padre2;
  int a,b,c,d,p;
  // Torneo binario para selección
  for (int i=0; i<n; i+=2) {
    a = rand() % n;
    b = rand() % n;
    c = rand() % n;
    d = rand() % n;
    padre1 = fitness[a] < fitness[b] ? poblacion[a] : poblacion[b];
    padre2 = fitness[c] < fitness[d] ? poblacion[c] : poblacion[d];
    p = rand() % m;
    // Cruce de un punto
    hijos[i] = padre1.substr(0, p) + padre2.substr(p);
    hijos[i+1] = padre2.substr(0, p) + padre1.substr(p);
  }
  return hijos;
}

// Complejidad: O(N * K) 
// Donde K es nMutacion. Se realizan K accesos directos por cada uno de los N individuos.
void mutation(vector<string> &hijos, int nMutacion) {
  int n = hijos.size(), m = hijos[0].size(), p;
  for (int i=0; i<n; i++) {
    for (int j=0; j<nMutacion; j++) {
      p = rand() % m;
      hijos[i][p] = ABC[rand() % ABC.size()];
    }
  }
}

// Complejidad: O(N * M)
// En el peor caso (si se reemplazan todos), la asignación de strings toma O(M) por cada uno de los N individuos.
void reinsertion(vector<string> &poblacion, vector<int> &fitness, vector<string> &hijos, vector<int> &fitnessHijos) {
  int n = poblacion.size();
  for (int i=0; i<n; i++) {
    // Reemplazo determinista si el hijo supera al padre
    if (fitness[i] > fitnessHijos[i]) {
      poblacion[i] = hijos[i];
      fitness[i] = fitnessHijos[i];
    }
  }
}

// Complejidad: O(N)
// Búsqueda lineal simple a través del vector de fitness.
int findBest(vector<int> &fitness) {
  int n = fitness.size(), mejor = INF, idx=-1;
  for (int i=0; i<n; i++) {
    if (mejor > fitness[i]) {
      mejor = fitness[i];
      idx = i;
    }
  }
  return idx;
}
