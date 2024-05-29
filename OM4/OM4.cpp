#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// 1.Suprogramuokite simplekso algoritmą tiesinio programavimo uždaviniams.
// 2.Užrašykite duotą uždavinį matriciniu pavidalu standartine forma.
// 3.Išspręskite uždavinį suprogramuotu simplekso algoritmu.
// 4.Pakeiskite apribojimų dešinės pusės konstantas į a, b ir c – studento knygelės numerio “1x1xabc” skaitmenis.Išspręskite individualų uždavinį suprogramuotu simplekso algoritmu.
// 5.Palyginkite uždavinių sprendimo rezultatus : minimali tikslo funkcijos reikšmė, optimalus sprendinys ir bazė
const double ZeroCheck = 0;  // A small constant to check for zero

void printTable(const vector<vector<double>>& table, int m, int n) {
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            cout << setw(10) << setprecision(5) << table[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int findPivotColumn(const vector<vector<double>>& table, int m, int n) {
    int pivotCol = -1;
    double minValue = 0;
    for (int j = 0; j < n; j++) {
        if (table[m][j] < minValue) {
            minValue = table[m][j];
            pivotCol = j;
        }
    }
    return pivotCol;
}

int findPivotRow(const vector<vector<double>>& table, int pivotCol, int m, int n) {
    int pivotRow = -1;
    double minRatio = INFINITY;
    for (int i = 0; i < m; i++) {
        if (table[i][pivotCol] > ZeroCheck) {
            double ratio = table[i][n] / table[i][pivotCol];
            if (ratio < minRatio) {
                minRatio = ratio;
                pivotRow = i;
            }
        }
    }
    return pivotRow;
}

void pivot(vector<vector<double>>& table, int pivotRow, int pivotCol, int m, int n) {
    double pivotValue = table[pivotRow][pivotCol];
    for (int j = 0; j <= n; j++) {
        table[pivotRow][j] /= pivotValue;
    }
    for (int i = 0; i <= m; i++) {
        if (i != pivotRow) {
            double factor = table[i][pivotCol];
            for (int j = 0; j <= n; j++) {
                table[i][j] -= factor * table[pivotRow][j];
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

    vector<double> solutionx(n, 0);
    for (int i = 0; i < m; i++) {
        int basicVarIndex = -1;
        for (int j = 0; j < n; j++) {
            if (fabs(tableau[i][j] - 1) < ZeroCheck) {
                if (basicVarIndex == -1) {
                    basicVarIndex = j;
                }
                else {
                    basicVarIndex = -1;
                    break;
                }
            }
            else if (fabs(tableau[i][j]) > ZeroCheck) {
                basicVarIndex = -1;
                break;
            }
        }
        if (basicVarIndex != -1) {
            solutionx[basicVarIndex] = tableau[i][n];
        }
    }
    return solutionx;
}

vector<double> simpley(vector<vector<double>>& tableus, int m, int n) {
    while (true) {
        int pivotCol = findPivotColumn(tableus, m, n);
        if (pivotCol == -1) break;  // Optimal solution found

        int pivotRow = findPivotRow(tableus, pivotCol, m, n);
        if (pivotRow == -1) {
            cout << "Unbounded solution." << endl;
            return {};
        }

        pivot(tableus, pivotRow, pivotCol, m, n);
    }

    vector<double> solutiony(n, 0);
    for (int i = 0; i < m; i++) {
        int basicVarIndex = -1;
        for (int j = 0; j < n; j++) {
            if (fabs(tableus[i][j] - 1) < ZeroCheck) {
                if (basicVarIndex == -1) {
                    basicVarIndex = j;
                }
                else {
                    basicVarIndex = -1;
                    break;
                }
            }
            else if (fabs(tableus[i][j]) > ZeroCheck) {
                basicVarIndex = -1;
                break;
            }
        }
        if (basicVarIndex != -1) {
            solutiony[basicVarIndex] = tableus[i][n];
        }
    }
    return solutiony;
}

int main() {
    // Number
    int m = 3;
    // Lenght
    int n = 7;

    // Augmented matrix (tableau)
    vector<vector<double>> tableau = {
        {-1, 1, -1, -1, 1, 0, 0, 8},  // -x + y - c - z + s1 = 8
        {2, 4, 0, 0, 0, 1, 0, 10},    // 2x + 4y + s2 = 10
        {0, 0, 1, 1, 0, 0, 1, 3},     // c + z + s3 = 3
        {2, -3, -5, 0, 0, 0, 0, 0}    // Objective function: 2x - 3y - 5c
    };

    // Augmented matrix (tableus)
    vector<vector<double>> tableus = {
        {-1, 1, -1, -1, 1, 0, 0, 2},  // -x + y - c - z + s1 = 2
        {2, 4, 0, 0, 0, 1, 0, 7},    // 2x + 4y + s2 = 7
        {0, 0, 1, 1, 0, 0, 1, 3},     // c + z + s3 = 3
        {2, -3, -5, 0, 0, 0, 0, 0}    // Objective function: 2x - 3y - 5c
    };


    cout << "Initial Tableau:" << endl;
    printTable(tableau, m, n);

    // Perform the simplex algorithm
    vector<double> solutionx = simplex(tableau, m, n);

    // Print the final table
    cout << "Final Tableau:" << endl;
    printTable(tableau, m, n);

    cout << "Optimal value of the objective function: " << -tableau[m][n] << endl << endl;

    cout << "Initial Tableus:" << endl;
    printTable(tableus, m, n);

    // Perform the simpley algorithm
    vector<double> solutiony = simpley(tableus, m, n);

    // Print the final table
    cout << "Final Tableus:" << endl;
    printTable(tableus, m, n);

    cout << "Optimal value of the objective function: " << -tableus[m][n] << endl;
    return 0;
}
