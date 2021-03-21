#include "polynomial.h"

Polynomial::Polynomial() {
    m_coefficient = new int[0];
    *this->m_coefficient = 0;
    m_degree = new int[0];
    *this->m_degree = 0;
    m_size = 1;
}

Polynomial::Polynomial(int min, int max, int *coefficient) {
    int size = max - min + 1;
    int tempDegree = min;

    m_degree = new int[size];
    m_coefficient = new int[size];
    m_size = size;

    for (int i = 0; i < size; i++) {
        this->m_degree[i] = tempDegree;
        this->m_coefficient[i] = *coefficient;
        tempDegree++;
        coefficient++;
    }
}

Polynomial::Polynomial(const Polynomial &other) {
    m_size = other.m_size;

    m_coefficient = new int[m_size];
    m_degree = new int[m_size];

    for (int i = 0; i < m_size; i++) {
        this->m_degree[i] = other.m_degree[i];
        this->m_coefficient[i] = other.m_coefficient[i];
    }
}

Polynomial::~Polynomial() = default;

stringstream &operator<<(stringstream &out, const Polynomial &other) {
    int tempSize = other.m_size;

    if (tempSize == 1 and other.m_coefficient[0] == 0) {
        out << "0";
    } else {
        for (int i = tempSize - 1; i >= 0; i--) {
            if (other.m_coefficient[i] != 0) {

                if (i < tempSize - 1 && other.m_coefficient[i] > 0 && !out.str().empty() && out.str().back() != '\n')
                    out << "+";
                if (other.m_coefficient[i] == -1 && other.m_degree[i] != 0)
                    out << "-";
                else if (other.m_coefficient[i] == 1 && other.m_degree[i] != 0) {}
                else
                    out << other.m_coefficient[i];

                if (other.m_degree[i] != 0) {
                    out << "x";
                    if (other.m_degree[i] != 1)
                        out << "^" << other.m_degree[i];
                }
            }
        }
    }

    int temp = 0;

    for (int i = 0; i < other.m_size; i++) {
        if (other.m_coefficient[i] == 0)
            temp++;
    }

    if (temp == other.m_size && out.str().empty())
        out << "0";
    return out;
}

Polynomial &Polynomial::operator=(const Polynomial &other) = default;

bool operator==(const Polynomial &other1, const Polynomial &other2) {
    stringstream ss1, ss2;
    ss1 << other1;
    ss2 << other2;
    return ss1.str() == ss2.str();
}

bool operator!=(const Polynomial &other1, const Polynomial &other2) {
    return !(other1 == other2);
}

Polynomial operator+(const Polynomial &other1, const Polynomial &other2) {
    int tempMin = min(other1.m_degree[0], other2.m_degree[0]);
    int tempMax = max(other1.m_degree[other1.m_size - 1], other2.m_degree[other2.m_size - 1]);
    int tempSize = tempMax - tempMin + 1;
    int tempCoefficient[tempSize];
    int tempDegree = tempMin;

    for (int i = 0; i < tempSize; i++) {
        tempCoefficient[i] = 0;
        for (int j = 0; j < other1.m_size; j++) {
            if (other1.m_degree[j] == tempDegree)
                tempCoefficient[i] += other1.m_coefficient[j];
        }

        for (int j = 0; j < other2.m_size; j++) {
            if (other2.m_degree[j] == tempDegree)
                tempCoefficient[i] += other2.m_coefficient[j];
        }
        tempDegree++;
    }

    return Polynomial(tempMin, tempMax, tempCoefficient);
}

Polynomial operator-(const Polynomial &other) {
    Polynomial c = other;
    for (int i = 0; i < c.m_size; i++) {
        c.m_coefficient[i] *= -1;
    }
    return c;
}

Polynomial operator-(const Polynomial &other1, const Polynomial &other2) {
    return other1 + (-other2);
}

Polynomial operator+=(Polynomial &other1, const Polynomial &other2) {
    other1 = other1 + other2;
    return other1;
}

Polynomial operator-=(Polynomial &other1, const Polynomial &other2) {
    other1 = other1 - other2;
    return other1;
}