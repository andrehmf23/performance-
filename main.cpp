#include "experimental.cpp"

#include <chrono>
#include <cstdlib>
#include <ctime>

// Função auxiliar de benchmark
template<typename Func>
double benchmark(Func func, const string &name) {
    cout << "Running " << name << "..." << endl;
    auto start = chrono::high_resolution_clock::now();

    func();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << "Execution time (" << name << "): " << duration.count() << " ms" << endl << endl;
    return duration.count();
}

int main() {
    srand(time(0)); // inicializar semente aleatória

    // ler dimensões
    while (true) {
        cout << "Size of matrix: ";
        cin >> SIZE;

        if (SIZE <= MAX) {
            break;
        }
        cout << "Incompatible dimensions, try again.\n";
    }

    generate(A, SIZE, SIZE);
    show(A, SIZE, SIZE);
    generate(B, SIZE);
    show(B, SIZE);

    // Executar benchmarks
    double v1 = benchmark(ex1, "Naive Multiplication");
    clear(C, SIZE);
    double v2 = benchmark(ex2, "Loop Tiling");
    clear(C, SIZE);
    double v3 = benchmark(ex3, "Loop Unrolling x10");
    clear(C, SIZE);
    double v4 = benchmark(ex4, "Loop Interchange");

    show(v1, v2, "Loop Tiling");
    show(v1, v3, "Loop Unrolling x10");
    show(v1, v4, "Loop Interchange");

    show(C, SIZE);
    return 0;
}
