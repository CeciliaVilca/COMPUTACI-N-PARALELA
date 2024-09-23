#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void fillMatrix(vector<vector<int>>& matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 100;
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

        auto start = chrono::high_resolution_clock::now();
        multiplyMatrices(A, B, C);
        auto end = chrono::high_resolution_clock::now();
        
        chrono::duration<double> duration = end - start;
        cout << "Tiempo de multiplicacion para matrices de " << size << "x" << size << ": " << duration.count() << " segundos" << endl;
    }

    return 0;
}
