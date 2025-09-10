
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "vex.hpp"
#include <iomanip>

using namespace std;

unsigned int SIZE;

vex<unsigned char> A;
vex<unsigned char> B;
vex<unsigned char> C;

// _________________________________(EXPERIMENTAL)___________________________________

// 1️⃣ Multiplicação ingênua
void ex1() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            for(int k = 0; k < SIZE; k++)
                C(i,j) += A(i,k) * B(k,j);
    }
}

// 2️⃣ Multiplicação com Loop Tiling
void ex2() {
    int T = 32; // tamanho do bloco
    int limit = (SIZE / T) * T; // maior múltiplo de T <= SIZE

    // Parte principal (blocos completos)
    for (int ii = 0; ii < limit; ii += T)
        for (int jj = 0; jj < limit; jj += T)
            for (int kk = 0; kk < limit; kk += T)
                for (int i = ii; i < ii + T; i++)
                    for (int j = jj; j < jj + T; j++)
                        for (int k = kk; k < kk + T; k++)
                            C(i,j) += A(i,k) * B(k,j);

    // 🔹 Resto no k (profundidade extra)
    for (int ii = 0; ii < limit; ii += T)
        for (int jj = 0; jj < limit; jj += T)
            for (int i = ii; i < ii + T; i++)
                for (int j = jj; j < jj + T; j++)
                    for (int k = limit; k < SIZE; k++)
                        C(i,j) += A(i,k) * B(k,j);

    // 🔹 Resto nas linhas
    for (int i = limit; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
                C(i,j) += A(i,k) * B(k,j);

    // 🔹 Resto nas colunas
    for (int i = 0; i < limit; i++)
        for (int j = limit; j < SIZE; j++)
            for (int k = 0; k < SIZE; k++)
                C(i,j) += A(i,k) * B(k,j);
}

// 3️⃣ Loop Unrolling (Matriz x Matriz)
void ex3() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int k;
            for (k = 0; k + 9 < SIZE; k += 10) {
                C(i,j) += A(i, k)     * B(k, j);
                C(i,j) += A(i, k + 1) * B(k + 1, j);
                C(i,j) += A(i, k + 2) * B(k + 2, j);
                C(i,j) += A(i, k + 3) * B(k + 3, j);
                C(i,j) += A(i, k + 4) * B(k + 4, j);
                C(i,j) += A(i, k + 5) * B(k + 5, j);
                C(i,j) += A(i, k + 6) * B(k + 6, j);
                C(i,j) += A(i, k + 7) * B(k + 7, j);
                C(i,j) += A(i, k + 8) * B(k + 8, j);
                C(i,j) += A(i, k + 9) * B(k + 9, j);
            }
            // resto
            for (; k < SIZE; k++) {
                C(i,j) += A(i, k) * B(k, j);
            }
        }
    }
}


// 4️⃣ Loop Interchange (Matriz x Matriz)
void ex4() {
    for (int j = 0; j < SIZE; j++) {
        for (int k = 0; k < SIZE; k++) {
            for (int i = 0; i < SIZE; i++) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

// Função auxiliar de benchmark
template<typename Func>
double benchmark(Func func, const string &name) {
    //cout << "Running " << name << "..." << endl;
    auto start = chrono::high_resolution_clock::now();

    func();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << "Execution time (" << name << "): " << duration.count() << " ms" << endl;
    return duration.count();
}

void show(double value1, double value2, const string &name){
    cout << '|' << setw(20) << name << " | from " << fixed << setprecision(4) << value1 << " ms > " << value2 << " ms |" << endl;
}

void reset() {
    A = vex<unsigned char>(SIZE, SIZE);
    B = vex<unsigned char>(SIZE, SIZE);
    C = vex<unsigned char>(SIZE, SIZE);
    A.generate();
    B.generate();
    C.clear();
}

int main() {
    srand(time(0)); // inicializar semente aleatória

    // ler dimensões
    while (true) {
        try
        {
            cout << "Size of matrix: ";
            cin >> SIZE;
            break;
        }
        catch(const std::exception& e)
        {
            cerr << e.what() << '\n';
        }
    }
    reset();
    benchmark(ex1, "Naive Multiplication");
    reset();
    benchmark(ex2, "Loop Tiling");
    reset();
    benchmark(ex3, "Loop Unrolling x10");
    reset();
    benchmark(ex4, "Loop Interchange");
    return 0;
}
