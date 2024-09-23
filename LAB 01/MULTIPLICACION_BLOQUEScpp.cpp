#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void multiplyMatricesBlocked(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int blockSize) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();

    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < p; j += blockSize) {
            for (int k = 0; k < m; k += blockSize) {
                // Multiplicación de bloques
                for (int ii = i; ii < min(i + blockSize, n); ++ii) {
                    for (int jj = j; jj < min(j + blockSize, p); ++jj) {
                        for (int kk = k; kk < min(k + blockSize, m); ++kk) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

// Función para llenar una matriz con valores aleatorios
void fillMatrix(vector<vector<int>>& matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 10;  // Valores aleatorios entre 0 y 9
        }
    }
}

int main() {
    for (int size : {100, 200, 300, 400, 500, 1000, 10000}) {  // Diferentes tamaños de matriz
        vector<vector<int>> A(size, vector<int>(size));
        vector<vector<int>> B(size, vector<int>(size));
        vector<vector<int>> C(size, vector<int>(size, 0));

        fillMatrix(A, size);
        fillMatrix(B, size);

        int blockSize = 16;
        auto start = chrono::high_resolution_clock::now();
        multiplyMatricesBlocked(A, B, C, blockSize);
        auto end = chrono::high_resolution_clock::now();
        
        chrono::duration<double> duration = end - start;
        cout << "Tiempo de multiplicacion para matrices de " << size << "x" << size << ": " << duration.count() << " segundos" << endl;
    }

    return 0;
}
