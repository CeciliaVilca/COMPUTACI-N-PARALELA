#include <iostream>
#include <chrono>

using namespace std;

#define MAX 100

void multiplyFirstLoop(double A[MAX][MAX], double x[MAX], double y[MAX]) {
    for (int i = 0; i < MAX; i++) {
        y[i] = 0.0; // Reiniciar y[i]
        for (int j = 0; j < MAX; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

void multiplySecondLoop(double A[MAX][MAX], double x[MAX], double y[MAX]) {
    for (int j = 0; j < MAX; j++) {
        for (int i = 0; i < MAX; i++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main() {
    double A[MAX][MAX], x[MAX], y[MAX];

    // Inicializar A y x
    for (int i = 0; i < MAX; i++) {
        x[i] = i * 1; 
        for (int j = 0; j < MAX; j++) {
            A[i][j] = i + j * 2;
        }
    }

    // Primer bucle
    auto start = chrono::high_resolution_clock::now();
    multiplyFirstLoop(A, x, y);
    auto stop = chrono::high_resolution_clock::now();
    auto durationFirst = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Y - primer bucle:" << endl;
    for (int i = 0; i < MAX; i++) {
        cout << "y[" << i << "] = " << y[i] << endl;
    }
    cout << "Tiempo de ejecución del primer bucle: " << durationFirst.count() << " microsegundos" << endl;

    // Reiniciar Y
    for (int i = 0; i < MAX; i++) {
        y[i] = 0.0;
    }

    // Segundo bucle
    start = chrono::high_resolution_clock::now();
    multiplySecondLoop(A, x, y);
    stop = chrono::high_resolution_clock::now();
    auto durationSecond = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Y - segundo bucle:" << endl;
    for (int i = 0; i < MAX; i++) {
        cout << "y[" << i << "] = " << y[i] << endl;
    }
    cout << "Tiempo de ejecución del segundo bucle: " << durationSecond.count() << " microsegundos" << endl;

    return 0;
}
