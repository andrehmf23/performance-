#include "monitor.cpp"

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>


using namespace std;

const int MAX = 1024; // tamanho máximo da pilha
int MATRIX_SIZE = 1024;
int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

void clearMatrix(int M[MAX][MAX]) {
    for (int i = 0; i < MATRIX_SIZE; ++i)
        for (int j = 0; j < MATRIX_SIZE; ++j)
            M[i][j] = 0;
}

void print(int M[MAX][MAX]) {
    if (max(MATRIX_SIZE, MATRIX_SIZE) < 31)
    {
        cout << "\nNew Matrix" << endl;
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            cout << '|';
            for (int j = 0; j < MATRIX_SIZE; ++j)
                cout << setw(3) << M[i][j] << " ";
            cout << '|' << endl;
        }
    }
}

// Gerar matriz aleatória
void generateMatrix(int M[MAX][MAX]) {
    for (int i = 0; i < MATRIX_SIZE; ++i)
        for (int j = 0; j < MATRIX_SIZE; ++j)
            M[i][j] = rand() % 5;
}

// 1️⃣ Multiplicação ingênua
void ex1() {
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            for (int k = 0; k < MATRIX_SIZE; k++)
                C[i][j] += A[i][k] * B[k][j];
}

// 2️⃣ Multiplicação com Loop Tiling
void ex2() {
    int T = 32; // tamanho do bloco
    for (int ii = 0; ii < MATRIX_SIZE; ii += T)
        for (int jj = 0; jj < MATRIX_SIZE; jj += T)
            for (int kk = 0; kk < MATRIX_SIZE; kk += T)
                for (int i = ii; i < min(ii + T, MATRIX_SIZE); i++)
                    for (int j = jj; j < min(jj + T, MATRIX_SIZE); j++)
                        for (int k = kk; k < min(kk + T, MATRIX_SIZE); k++)
                            C[i][j] += A[i][k] * B[k][j];
}

// 3️⃣ Loop Unrolling
void ex3() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int k;
            for (k = 0; k < MATRIX_SIZE; k += 10) {
                C[i][j] += A[i][k] * B[k][j];
                C[i][j] += A[i][k+1] * B[k+1][j];
                C[i][j] += A[i][k+2] * B[k+2][j];
                C[i][j] += A[i][k+3] * B[k+3][j];
                C[i][j] += A[i][k+4] * B[k+4][j];
                C[i][j] += A[i][k+5] * B[k+5][j];
                C[i][j] += A[i][k+6] * B[k+6][j];
                C[i][j] += A[i][k+7] * B[k+7][j];
                C[i][j] += A[i][k+8] * B[k+8][j];
                C[i][j] += A[i][k+9] * B[k+9][j];
            }
            for (; k < MATRIX_SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 4️⃣ Loop Interchange
void ex4() {
    for (int j = 0; j < MATRIX_SIZE; j++)
        for (int i = 0; i < MATRIX_SIZE; i++)
            for (int k = 0; k < MATRIX_SIZE; k++)
                C[i][j] += A[i][k] * B[k][j];
}

// Função auxiliar de benchmark
template<typename Func>
double benchmark(Func func, const string &name) {
    // resetar matriz C
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            C[i][j] = 0;

    cout << "Running " << name << "..." << endl;
    auto start = chrono::high_resolution_clock::now();

    func();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << "Execution time (" << name << "): "
         << duration.count() << " ms" << endl << endl;
    return duration.count();
}

int main() {
    // ler dimensões
    while (true) {
        cout << "MATRIX_SIZE of matrix: ";
        cin >> MATRIX_SIZE;

        if (MATRIX_SIZE <= MAX) {
            break;
        }
        cout << "Incompatible dimensions, try again.\n";
    }

    srand(time(0)); // inicializar semente aleatória

    generateMatrix(A);
    print(A);
    generateMatrix(B);
    print(B);

    
    // Executar benchmarks
    clearMatrix(C);
    benchmark(ex1, "Naive Multiplication");
    clearMatrix(C);
    benchmark(ex2, "Loop Tiling");
    clearMatrix(C);
    benchmark(ex3, "Loop Unrolling x10");
    clearMatrix(C);
    benchmark(ex4, "Loop Interchange");

    show();

    print(C);

    return 0;
}
