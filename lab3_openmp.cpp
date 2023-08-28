#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <omp.h> // Incluir biblioteca OpenMP

using namespace std;

void generateNumbers(const char* filename, int N) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error al abrir el archivo para escribir." << endl;
        return;
    }
    srand(time(0));
    for (int i = 0; i < N; ++i) {
        int numeroAleatorio = rand() % 1000;
        file << numeroAleatorio;
        if (i < N - 1) {
            file << ",";
        }
    }
    file.close();
}

void quickSort(int* arr, int l, int h) {
    if (l < h) {
        int pivot = arr[h];
        int i = l - 1;

        #pragma omp parallel for shared(arr, pivot) schedule(static) 
        for (int j = l; j <= h - 1; j++) {
            if (arr[j] < pivot) {
                #pragma omp critical
                {
                    i++;
                    swap(arr[i], arr[j]);
                }
            }
        }
        swap(arr[i + 1], arr[h]);
        int pivotIndex = i + 1;

        #pragma omp task firstprivate(arr, l, pivotIndex)
        quickSort(arr, l, pivotIndex - 1);
        #pragma omp task firstprivate(arr, h, pivotIndex)
        quickSort(arr, pivotIndex + 1, h);
    }
}

int main() {
    const char* unsorted = "unsorted.txt";
    const char* sorted = "sorted.txt";
    int N = 10;

    generateNumbers(unsorted, N);

    int* arregloNumeros = new int[N];

    FILE* f = fopen(unsorted, "r");
    char coma;
    for (int i = 0; i < N; ++i) {
        if (fscanf(f, "%d", &arregloNumeros[i]) != 1) {
            fprintf(stderr, "error al leer el número.\n");
            return 1;
        }
        if (i < N - 1) {
            if (fscanf(f, "%c", &coma) != 1 || coma != ',') {
                fprintf(stderr, "error al leer coma.\n");
                return 1;
            }
        }
    }
    fclose(f);

    double startTime = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        quickSort(arregloNumeros, 0, N - 1);
    }
    double endTime = omp_get_wtime();
    cout << "Tiempo de ejecución paralelo: " << endTime - startTime << " segundos." << endl;

    FILE* output = fopen(sorted, "w");
    for (int i = 0; i < N; ++i) {
        if (fprintf(output, "%d", arregloNumeros[i]) < 0) {
            fprintf(stderr, "error al escribir un numero en el archivo.\n");
            return 1;
        }
        if (i < N - 1) {
            if (fprintf(output, ",") < 0) {
                fprintf(stderr, "error al escribir una coma en el archivo.\n");
                return 1;
            }
        }
    }
    fclose(output);

    delete[] arregloNumeros;

    return 0;
}
