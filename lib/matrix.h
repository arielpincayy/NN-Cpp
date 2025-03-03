#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

class Matrix {
public:
    float* matrix; // Puntero para almacenar los datos de la matriz
    int n;         // Número de filas
    int m;         // Número de columnas

    // Constructores
    Matrix(float* matrix_rw, int n_s, int m_s); // Constructor que copia datos de un arreglo
    Matrix(int n_s, int m_s);                  // Constructor para matriz vacía

    // Destructor
    ~Matrix();

    // Métodos
    Matrix *get_row(int i); // Obtener una fila de la matriz
    static Matrix* randomMatrix(int n_s, int m_s, float min_val = 0.0, float max_val = 1.0);

    // Operadores sobrecargados
    Matrix* operator*(const Matrix& other); // Multiplicación de matrices
    Matrix* operator*(float scale);         // Escalado de matriz
    Matrix* operator+(const Matrix& other); // Suma de matrices
    Matrix* operator-(const Matrix& other); // Resta de matrices
};

// Sobrecarga del operador * para escalar * matriz
Matrix* operator*(float scale, Matrix& other);

// Sobrecarga del operador << para imprimir la matriz
std::ostream& operator<<(std::ostream& COUT, const Matrix& matrix);

#endif // MATRIX_H
