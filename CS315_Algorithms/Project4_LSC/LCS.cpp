#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Struct to hold the LCS length and the b matrix
struct LCSResult {
    int length;                     // Length of the LCS
    vector<vector<string>> b;       // Direction matrix
};

// Function to compute LCS length and the b matrix
LCSResult LCSLength(string X, string Y, int m, int n)
{
    vector<vector<string>> b(m, vector<string>(n));
    vector<vector<int>> c(m + 1, vector<int>(n + 1, 0));

    // Fill the tables
    for (int p = 1; p <= m; p++) {
        for (int l = 1; l <= n; l++) {
            char x = X[p - 1]; // Adjust indexing to 0-based
            char y = Y[l - 1]; // Adjust indexing to 0-based
            if (x == y) {
                c[p][l] = c[p - 1][l - 1] + 1;
                b[p - 1][l - 1] = "Diag";
            } else if (c[p - 1][l] >= c[p][l - 1]) {
                c[p][l] = c[p - 1][l];
                b[p - 1][l - 1] = "Up";
            } else {
                c[p][l] = c[p][l - 1];
                b[p - 1][l - 1] = "Right";
            }
        }
    }

    return {c[m][n], b}; // Return both the LCS length and the b matrix
}


// Function to reconstruct the LCS using the b matrix
string reconstructLCS(const vector<vector<string>>& b, const string& X, int m, int n)
{
    string lcs = "";
    int i = m - 1, j = n - 1; // Start from the bottom-right corner of b
    while (i >= 0 && j >= 0) {
        if (b[i][j] == "Diag") {
            lcs = X[i] + lcs; // Add character to LCS
            i--;
            j--;
        } else if (b[i][j] == "Up") {
            i--; // Move up
        } else {
            j--; // Move left
        }
    }
    return lcs;
}

int main()
{
    string X = "ACCGGTCGAGTGCGCGGAAGCCGGCCGAA";
    string Y = "GTCGTTCGGAATGCCGTTGCTCTGTAA";
    int m = X.length();
    int n = Y.length();

    // Get the LCS result
    LCSResult result = LCSLength(X, Y, m, n);

    // Output the LCS length
    cout << "The length of the LCS is: " << result.length << endl;

    // Reconstruct and output the LCS
    string lcs = reconstructLCS(result.b, X, m, n);
    cout << "The LCS is: " << lcs << endl;

    return 0;
}


