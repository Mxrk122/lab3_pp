#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

void generateNumbers(const char* filename, int N) {

    // cargar el archivo
    ofstream file(filename);
    if (!file) {
        cerr << "Error al abrir el archivo para escribir." << endl;
        return;
    }

    // generador de aleatorios
    srand(time(0));
    for (int i = 0; i < N; ++i) {
        // generar un número aleatorio entre 0 y 999
        int numeroAleatorio = rand() % 1000;
        file << numeroAleatorio;

        // separar por comas
        if (i < N - 1) {
            file << ",";
        }
    }

    file.close();
}


void quickSort(int* arr, int l, int h) {
    if (l < h) {
        // decidi tomar el ultimo elemento como pivote
        int pivot = arr[h];

        // obtener el index del principio
        int i = l - 1;


        for (int j = l; j <= h - 1; j++) {
            // si se encuentra un elemento menor al pivote
            if (arr[j] < pivot) {
                i++;
                // intercambiar con el mas peque;o
                swap(arr[i], arr[j]);
            }
        }
        
        // intercambiar el pivote
        swap(arr[i + 1], arr[h]);
        // indice resultante
        int pivotIndex = i + 1;

        // ordenar los subarrays correspondietes
        quickSort(arr, l, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, h);
    }
}

int main() {
    const char* unsorted = "unsorted.txt";
    const char* sorted = "sorted.txt";
    int N = 10;

    // generar los numeros
    generateNumbers(unsorted, N);

    // asignar memoria en el heap
    int* arregloNumeros = new int[N];

    // almacenar numeros en array
    FILE* f = fopen(unsorted, "r");
    char coma;
    for (int i = 0; i < N; ++i) {
        // leer ints
        if (fscanf(f, "%d", &arregloNumeros[i]) != 1) {
            fprintf(stderr, "error al leer el número.\n");
            return 1;
        }
        // leer chars, qeu seiran las comas
        if (i < N - 1) {
            if (fscanf(f, "%c", &coma) != 1 || coma != ',') {
                fprintf(stderr, "error al leer coma.\n");
                return 1;
            }
        }
    }

    fclose(f);

    // ordenar numeros
    quickSort(arregloNumeros, 0, N - 1);

    FILE* output = fopen(sorted, "w");

    // realizar el imso proceso pero apra escribir
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

    // liberar la memoria
    delete[] arregloNumeros;

    return 0;
}