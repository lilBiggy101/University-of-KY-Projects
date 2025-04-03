#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <cctype>
#include <set>
#include <map>
#include <sstream>
#include <vector>
#include <stack>
#include <cmath>
#include <utility>

// Run code by typing ./sorting *file name* 
// it can only run with one file argument at a time

using namespace std;

void Merge_Sort(vector<pair<string, double>>& A, int p, int r);

void Merge(vector<pair<string, double>>& A, int p, int q, int r);

void Quicksort(vector<pair<string, double>>& A, int p, int r);

int Partition(vector<pair<string, double>>& A, int p, int r);

int main(int argc, char* argv[])
{
    const int COMM_NUM = 1;
    // Check whether the number of command line arguments is exactly one 
    if (argc != COMM_NUM + 1)
    {
        cout << "Warning: need exactly " << COMM_NUM << " command line argument." << endl;
        cout << "Usage: " << argv[0] << " inputfile_name" << endl;
        return 1;
    }
    
    ifstream in_file;
    in_file.open(argv[COMM_NUM]);

    // Check whether the input file can be open successfully or not
    if (!in_file.good())
    {
        cout << "Warning: cannot open file named " << argv[COMM_NUM] << "!" << endl;
        return 2;
    }

    // Read data from the input file, assume "good data" from the input file
    // each line of the input file: Title, Money
    // read one line at a time then add that line to a vector pair

    vector<pair<string, double>> Games;
    int n = 0;
    
    while (!in_file.eof())
    {
        string line;
        getline(in_file, line);
        string Title, Moneys;
		double Moneyd;
        // check if the text line is not empty then tokenize it
        if (line.length() != 0)
        {
	    // for each line, break the name and money value apart
            istringstream iss(line);
            getline(iss, Title, ',');
	    	getline(iss, Moneys);	    
            
	    	//put the name and value into a vector
	    	Moneyd = stod(Moneys);
	    	Games.push_back(make_pair(Title,Moneyd));

	    	// keep track of number of elements in vector
	    	n++;
		}
    }
    // close the input file
    in_file.close();

    // print vector
    cout << "Vector before sorted:\n" ;
    for (int i=0; i<n; i++)
	{
    	cout << Games[i].first << " " << Games[i].second << endl; 
	}

    //insertion sort--------------
    vector<pair<string, double>> Gamesi;
    for (int i=0; i<n; i++)
	{
	    Gamesi.push_back(Games[i]); 
	}
    
    int f = 0;
    for (int j = 1; j < n; j++) {
	    pair<string, double> key = Gamesi[j];
	    double keyCompare = Gamesi[j].second;
    
	    // insert Gamesi[i] into the sorted sequence Gamesi[1..i-1]
	    int i = j - 1;
	    while (i>=0 && Gamesi[i].second<keyCompare)
		{
		    Gamesi[i+1] = Gamesi[i];
		    i--;
	    }
	    Gamesi[i+1] = key;
        
    }

    // Print it
    cout << "\n" << endl;
    cout << "Vector after insetion sort:\n" ;
    for (int i=0; i<n; i++) 
	{
	    cout << Gamesi[i].first << " " << Gamesi[i].second << endl; 
	}
        cout << f << endl;
	//Merge sort---------------
    vector<pair<string, double>> Gamesm;
    for (int i=0; i<n; i++)
	{
	    Gamesm.push_back(Games[i]);  
	}

    Merge_Sort(Gamesm, 0, (n-1));	

	//FLIP IT
    stack<pair<string, double>> stackm;
    for (int i=0; i<n; i++) {
	    stackm.push(Gamesm[i]); }
    for (int i=0; i<n; i++) {
	    Gamesm[i] = stackm.top();
	    stackm.pop(); }

	// Print it
	cout << "\n" << endl;
    cout << "Vector after sorted with merge sort:\n" ;
    for (int i=0; i<n; i++) {
	    cout << Gamesm[i].first << " " << Gamesm[i].second << endl; }



	// quick sort---------
    vector<pair<string, double>> Gamesq;
    for (int i=0; i<n; i++){
	    Gamesq.push_back(Games[i]);  }
    
    Quicksort(Gamesq, 0, (n-1));
    
    
    
    //FLIP IT
    stack<pair<string, double>> stackq;
    for (int i=0; i<n; i++) {
	    stackq.push(Gamesq[i]); }
    for (int i=0; i<n; i++) {
	    Gamesq[i] = stackq.top();
	    stackq.pop(); }
   
    //PRINT IT
    cout << "\n" << endl;
    cout << "Vector after sorted with quick sort:\n" ;
    for (int i=0; i<n; i++) {
	    cout << Gamesq[i].first << " " << Gamesq[i].second << endl; }

}


void Merge(std::vector<std::pair<std::string, double>>& A, int p, int q, int r) {
    int n1 = q - p + 1; // Size of left subarray
    int n2 = r - q;     // Size of right subarray

    // Create temporary arrays
    std::vector<std::pair<std::string, double>> L(n1);
    std::vector<std::pair<std::string, double>> R(n2);

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++) {
        L[i] = A[p + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = A[q + 1 + j];
    }

    // Sentinel values
    L.push_back({"", std::numeric_limits<double>::infinity()});
    R.push_back({"", std::numeric_limits<double>::infinity()});

    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray

    // Merge the temporary arrays back into A[]
    for (int k = p; k <= r; k++) {
        if (L[i].second <= R[j].second) {
            A[k] = L[i];
            i++;
        } else {
            A[k] = R[j];
            j++;
        }
    }
}

void Merge_Sort(std::vector<std::pair<std::string, double>>& A, int p, int r) {
    if (p < r) {
        int q = p + (r - p) / 2; // Avoid potential overflow
        Merge_Sort(A, p, q);
        Merge_Sort(A, q + 1, r);
        Merge(A, p, q, r);
    }
}

int Partition(std::vector<std::pair<std::string, double>>& A, int p, int r) {
    double x = A[r].second; // Pivot value (the last element)
    int i = p - 1; // Index of smaller element

    for (int j = p; j < r; j++) { // Iterate through the array
        if (A[j].second <= x) {
            i++;
            // Swap A[i] and A[j]
            std::swap(A[i], A[j]);
        }
    }
    // Place the pivot in the correct position
    std::swap(A[i + 1], A[r]);
    return (i + 1); // Return the index of the pivot
}

// Quick Sort function
void Quicksort(std::vector<std::pair<std::string, double>>& A, int p, int r) {
    if (p < r) {
        int q = Partition(A, p, r); // Partition the array
        Quicksort(A, p, q - 1); // Recursively sort the left part
        Quicksort(A, q + 1, r); // Recursively sort the right part
    }
}
