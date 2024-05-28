#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

const double EPSILON = 1e-9;  // A small constant to check for zero

void printTableau(const vector<vector<double>>& tableau, int m, int n) {
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            cout << setw(10) << setprecision(5) << tableau[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int findPivotColumn(const vector<vector<double>>& tableau, int m, int n) {
    int pivotCol = -1;
    double minValue = 0;
    for (int j = 0; j < n; j++) {
        if (tableau[m][j] < minValue) {
            minValue = tableau[m][j];
            pivotCol = j;
        }
    }
    return pivotCol;
}

int findPivotRow(const vector<vector<double>>& tableau, int pivotCol, int m, int n) {
    int pivotRow = -1;
    double minRatio = INFINITY;
    for (int i = 0; i < m; i++) {
        if (tableau[i][pivotCol] > EPSILON) {
            double ratio = tableau[i][n] / tableau[i][pivotCol];
            if (ratio < minRatio) {
                minRatio = ratio;
                pivotRow = i;
            }
        }
    }
    return pivotRow;
}

void pivot(vector<vector<double>>& tableau, int pivotRow, int pivotCol, int m, int n) {
    double pivotValue = tableau[pivotRow][pivotCol];
    for (int j = 0; j <= n; j++) {
        tableau[pivotRow][j] /= pivotValue;
    }
    for (int i = 0; i <= m; i++) {
        if (i != pivotRow) {
            double factor = tableau[i][pivotCol];
            for (int j = 0; j <= n; j++) {
                tableau[i][j] -= factor * tableau[pivotRow][j];
            }
        }
    }
}

vector<double> simplex(vector<vector<double>>& tableau, int m, int n) {
    while (true) {
        int pivotCol = findPivotColumn(tableau, m, n);
        if (pivotCol == -1) break;  // Optimal solution found

        int pivotRow = findPivotRow(tableau, pivotCol, m, n);
        if (pivotRow == -1) {
            cout << "Unbounded solution." << endl;
            return {};
        }

        pivot(tableau, pivotRow, pivotCol, m, n);
    }

    vector<double> solution(n, 0);
    for (int i = 0; i < m; i++) {
        int basicVarIndex = -1;
        for (int j = 0; j < n; j++) {
            if (fabs(tableau[i][j] - 1) < EPSILON) {
                if (basicVarIndex == -1) {
                    basicVarIndex = j;
                }
                else {
                    basicVarIndex = -1;
                    break;
                }
            }
            else if (fabs(tableau[i][j]) > EPSILON) {
                basicVarIndex = -1;
                break;
            }
        }
        if (basicVarIndex != -1) {
            solution[basicVarIndex] = tableau[i][n];
        }
    }
    return solution;
}

int main() {
    // Number of constraints
    int m = 3;
    // Number of variables including slack variables
    int n = 7;

    // Augmented matrix (tableau)
    vector<vector<double>> tableau = {
        {-1, 1, -1, -1, 1, 0, 0, 8},  // -x + y - c - z + s1 = 8
        {2, 4, 0, 0, 0, 1, 0, 10},    // 2x + 4y + s2 = 10
        {0, 0, 1, 1, 0, 0, 1, 3},     // c + z + s3 = 3
        {2, -3, -5, 0, 0, 0, 0, 0}    // Objective function: 2x - 3y - 5c
    };

    // Print the initial tableau
    cout << "Initial Tableau:" << endl;
    printTableau(tableau, m, n);

    // Perform the simplex algorithm
    vector<double> solution = simplex(tableau, m, n);

    // Print the final tableau
    cout << "Final Tableau:" << endl;
    printTableau(tableau, m, n);

    // Print the solution
    cout << "Optimal solution:" << endl;
    for (int i = 0; i < solution.size(); i++) {
        cout << "Variable " << i + 1 << " = " << solution[i] << endl;
    }

    cout << "Optimal value of the objective function: " << -tableau[m][n] << endl;

    return 0;
}
