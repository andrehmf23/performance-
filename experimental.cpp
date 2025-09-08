#include <iostream>
#include <iomanip>

using namespace std;

const int MAX = 1536; // tamanho máximo da pilha
int SIZE = 0;
int A[MAX][MAX], B[MAX], C[MAX];

// _________________________________[EXPERIMENTAL]___________________________________

// 1️⃣ Multiplicação ingênua
void ex1() {
    for (int i = 0; i < SIZE; i++) {
        C[i] = 0; // garantir inicialização
        for (int j = 0; j < SIZE; j++)
            C[i] += A[i][j] * B[j];
    }
}

// 2️⃣ Multiplicação com Loop Tiling
void ex2() {
    int T = 32; // tamanho do bloco
    int limit = (SIZE / T) * T; // maior múltiplo de T <= SIZE

    // Parte principal (blocos completos)
    for (int ii = 0; ii < limit; ii += T)
        for (int jj = 0; jj < limit; jj += T)
            for (int i = ii; i < ii + T; i++)
                for (int j = jj; j < jj + T; j++)
                    C[i] += A[i][j] * B[j];

    // Resto das linhas (parte que sobra além de 'limit')
    for (int i = limit; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            C[i] += A[i][j] * B[j];

    // Resto das colunas (em linhas já processadas até 'limit')
    for (int i = 0; i < limit; i++)
        for (int j = limit; j < SIZE; j++)
            C[i] += A[i][j] * B[j];
}

// 3️⃣ Loop Unrolling
void ex3() {
    for (int i = 0; i < SIZE; i++) {
        C[i] = 0; // inicializar
        int j;
        for (j = 0; j + 9 < SIZE; j += 10) {
            C[i] += A[i][j] * B[j];
            C[i] += A[i][j + 1] * B[j + 1];
            C[i] += A[i][j + 2] * B[j + 2];
            C[i] += A[i][j + 3] * B[j + 3];
            C[i] += A[i][j + 4] * B[j + 4];
            C[i] += A[i][j + 5] * B[j + 5];
            C[i] += A[i][j + 6] * B[j + 6];
            C[i] += A[i][j + 7] * B[j + 7];
            C[i] += A[i][j + 8] * B[j + 8];
            C[i] += A[i][j + 9] * B[j + 9];
        }
        // resto
        for (; j < SIZE; j++)
            C[i] += A[i][j] * B[j];
    }
}

// 4️⃣ Loop Interchange
void ex4() {
    for (int j = 0; j < SIZE; j++)
        for (int i = 0; i < SIZE; i++)
            C[i] += A[j][i] * B[j];
}

// _________________________________[TOOLS]___________________________________

void show(int M[][MAX], int columns, int rows) {
    if (SIZE < 31)
    {
        cout << "\nNew Matrix" << endl;
        for (int i = 0; i < columns; ++i) {
            cout << '|';
            for (int j = 0; j < rows; ++j)
                cout << setw(3) << M[i][j] << " ";
            cout << '|' << endl;
        }
    }
}

void show(int M[], int columns) {
    if (SIZE < 31)
    {
        cout << "\nNew Vector" << endl;
        cout << '|';
        for (int i = 0; i < columns; ++i) {
            cout << setw(3) << M[i] << " ";
        }
        cout << '|' << endl;
    }
}

void show(double value1, double value2, const string &name){
    cout << '|' << setw(20) << name << " | from " << fixed << setprecision(4) << value1 << " ms > " << value2 << " ms |" << endl;
}

void clear(int* M, int columns, int rows) {
    for (int i = 0; i < columns; ++i)
        for (int j = 0; j < rows; ++j)
            *(M + i * rows + j) = 0;
}

void clear(int* M, int columns) {
    for (int i = 0; i < columns; ++i)
        *(M + i) = 0;
}

void generate(int M[][MAX], int columns, int rows) {
    for (int i = 0; i < columns; ++i)
        for (int j = 0; j < rows; ++j)
            M[i][j] = rand() % 5;
}

void generate(int M[], int columns) {
    for (int i = 0; i < columns; ++i)
        M[i] = rand() % 5;
}