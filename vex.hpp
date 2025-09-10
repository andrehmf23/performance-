#ifndef VEX_HPP
#define VEX_HPP

#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
struct vex {
    private:
        T *A;
    public:
        unsigned int rows, columns;

        vex() : rows(0), columns(0), A(nullptr) {}

        vex(unsigned int rows) : rows(rows), columns(1) {
            A = new T[rows];
            clear();
        }

        vex(unsigned int rows, unsigned int columns) : rows(rows), columns(columns) {
            A = new T[rows * columns];
            clear();
        }

        vex& operator=(const vex& other) {
            if (this != &other) {
                delete[] A;
                rows = other.rows;
                columns = other.columns;
                if (other.A) {
                    A = new T[rows * columns];
                    copy(other.A, other.A + rows * columns, A);
                } else {
                    A = nullptr;
                }
            }
            return *this;
        }

        ~vex() {
            delete[] A;
        }
        
        T& operator()(unsigned int i) {
            return A[i];
        }

        T& operator()(unsigned int i, unsigned int j) {
            return A[i * columns + j];
        }

        // Construtor de cópia
        vex(const vex& other) : rows(other.rows), columns(other.columns) {
            if (other.A) {
                A = new T[rows * columns];
                copy(other.A, other.A + rows * columns, A);
            } else {
                A = nullptr;
            }
        }

        // Construtor de movimento
        vex(vex&& other) noexcept 
            : rows(other.rows), columns(other.columns), A(other.A) {
            other.rows = other.columns = 0;
            other.A = nullptr;
        }

        // Atribuição por movimento
        vex& operator=(vex&& other) noexcept {
            if (this != &other) {
                delete[] A;
                rows = other.rows;
                columns = other.columns;
                A = other.A;
                other.A = nullptr;
                other.rows = other.columns = 0;
            }
            return *this;
        }

        void clear() {
            for (size_t i = 0; i < rows; ++i)
                for (size_t j = 0; j < columns; ++j)
                    A[i * columns + j] = 0;
        }

        void generate(unsigned int max = 5) {
            int n = (rows * columns);
            for (size_t i = 0; i < n; ++i){
                A[i] = i % max;
            }
        }

        void print() {
            if (rows == 0 || columns == 0)
                return;
            else if (columns < 30 && rows < 30)
            {
                unsigned short width = to_string(max()).length();
                for (size_t i = 0; i < rows; ++i) {
                    cout << "[ ";
                    for (size_t j = 0; j < columns; ++j)
                        cout << setw(width) << A[i * columns + j] << " ";
                    cout << "]" << endl;
                }
                cout << endl;
            }
            else cout << "Matrix too large to print" << endl;
            
            
        }

        T max() {
            if (rows == 0 || columns == 0)
                return T{};
            
            unsigned int n = (rows * columns);
            T max = A[0];
            for (size_t i = 1; i < n; ++i) {
                if (A[i] > max)
                    max = A[i];
            }
            return max;
        }
};

#endif