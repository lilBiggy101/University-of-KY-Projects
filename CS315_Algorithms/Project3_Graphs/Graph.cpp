#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <queue>
#include <stdexcept>
#include <set>
#include <unordered_map>
using namespace std;

enum colors { white, grey, black };

// Structure for a node in the linked list
struct City {
    string name;
    colors color;
    int distance; //distacne from source for BFS
    int Weight;
    City* next;
    string pi;

    City() : name(""), color(white), Weight(0), next(nullptr), distance(INT32_MAX), pi("") {}
};

// class for the minheap
class minheap {
	public:
	vector<City*> A;
	int heapSize = 0;
	
	// psuedocode for Max-Heapify but reversed for min
	void minHeapify(int i) {
		int smallest;
		int l = left(i);
		int r = right(i);
		if (l < heapSize && A[l] && A[l]->distance < A[i]->distance) {
			smallest = l;
		}
		else {
			smallest = i;
		}
		if (r < heapSize && A[r] && A[r]->distance < A[smallest]->distance) {
			smallest = r;
		}
		if (smallest != i) {
			swap(A[i], A[smallest]);
			minHeapify(smallest);
		}
		return;
	}
	
	void buildMinHeap() {
		heapSize = A.size();
		for (int i = ((heapSize/2)-1); i >= 0; i--) {
			minHeapify(i);
		}
		return;
	}
	
	void insert(City* v) {
		heapSize++;
		A.push_back(v);
		decreaseKey(heapSize-1, v->distance);
		return;
	}
	
	//psuedocode for Heap-Extract-Max but for extract min
	City* extractMin() {
		if (heapSize < 1) {
			cout << "heap underflow\n";
			return NULL;
		}
		City* min = A[0];
		A[0] = A[heapSize-1];
		heapSize--;
		minHeapify(0);
		return min;
	}
	
	//psuedocode for HEAP-INCREASE-KEY but for min
	void decreaseKey(int i, int key) {
		if (A[i] && key > A[i]->distance) {
			cout << "new key is larger than current key\n";
			return;
		}
		A[i]->distance = key;
		while (i > 0 && A[parent(i)]->distance > A[i]->distance) {
			swap(A[i], A[parent(i)]);
			i = parent(i);
		}
		return;
	}
	
	int parent(int i){
		return ((i-1)/2);
	}
	
	int left(int i) {
		return ((2*i)+1);
	}
	
	int right(int i) {
		return ((2*i)+2);
	}
};

// Helper functions for string manipulation
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

string toLower(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

// Reset for Dijstras
void Dijkstra(vector<City*>& Adjlst, string source) {
    // Initialize all cities
    for (City* city : Adjlst) {
        city->distance = INT32_MAX;
        city->pi = "";
        city->color = white;
    }

    // Find the source city
    City* srcCity = nullptr;
    for (City* city : Adjlst) {
        if (toLower(city->name) == toLower(source)) {
            srcCity = city;
            break;
        }
    }
    if (!srcCity) {
        cout << "Source city not found: " << source << endl;
        return;
    }

    srcCity->distance = 0;  // Distance to itself is 0

    // Priority queue (min-heap)
    minheap Q;
    for (City* city : Adjlst) {
        Q.insert(city);
    }

    // Dijkstra's Algorithm
    while (Q.heapSize > 0) {
        City* u = Q.extractMin(); // Get city with the smallest distance
        u->color = black;        // Mark as processed

        // Traverse neighbors of u
        City* neighbor = u->next;
        while (neighbor != nullptr) {
            // Find the neighbor city object
            City* neighborCity = nullptr;
            for (City* city : Adjlst) {
                if (toLower(city->name) == toLower(neighbor->name)) {
                    neighborCity = city;
                    break;
                }
            }

            // Relax the edge (u, neighborCity)
            if (neighborCity && neighborCity->color != black) {
                int newDist = u->distance + neighbor->Weight;
                if (newDist < neighborCity->distance) {
                    neighborCity->distance = newDist;
                    neighborCity->pi = u->name;
                    for (int i = 0; i < Q.A.size(); ++i) {
                        if (Q.A[i]->name == neighborCity->name) {
                            Q.decreaseKey(i, newDist);
                            break;
                        }
                    }
                }
            }
            neighbor = neighbor->next; // Move to the next neighbor
        }
    }
}



// BFS
void BFS(City* startCity, vector<City*>& Adjlst) {  
    // Start by marking the source city as grey and enqueue it
    startCity->color = grey;
    startCity->distance = 0;  // Distance from start city is 0
    queue<City*> q;
    q.push(startCity);


    while (!q.empty()) 
    {
        City* currentCity = q.front();
        q.pop();
        // Traverse through the adjacency list of the current city
        City* neighbor = currentCity->next;
        while (neighbor != NULL) 
        {
            // Check if the neighbor has not been visited yet (color == white)
            City* neighborCity = nullptr;
            for (City* city : Adjlst) {
                if (toLower(city->name) == toLower(neighbor->name)) {
                    neighborCity = city;
                    break;
                }
            }

            if (neighborCity != nullptr && neighborCity->color == white) {
                // Mark the neighbor as grey, set its distance and parent (pi)
                neighborCity->color = grey;
                neighborCity->distance = currentCity->distance + 1; // Distance is current + 1
                neighborCity->pi = currentCity->name;
                q.push(neighborCity);  // Enqueue the neighbor for further exploration
            }

            // Move to the next neighbor
            neighbor = neighbor->next;
        }

        // After processing the current city, mark it as black
        currentCity->color = black;
    }

    cout << endl;  // Print a newline after BFS traversal
}

// PP function
void PrintPath(vector <City*> Adjlst, string s, string v)
{
    for(int i = 0; i < Adjlst.size(); i++)
    {
        if (Adjlst[i]->name == v)
        {
            if (v == s)
            {
                cout << s << endl;
            }
            else if (Adjlst[i]->pi == "")
            {
                cout << "no path"<< endl;
            }
            else
            {
                PrintPath(Adjlst, s, Adjlst[i]->pi);
                cout << v << endl;
            }
        }
    }
    return;
}




int main() {
    // ADJ LST implementation
    vector<City*> Adjlst;

    // File paths
    string filePathv = "MiddleEarthVertices.txt";
    string filePathE = "MiddleEarthEdges.txt";

    // Open vertices file
    ifstream file1(filePathv);
    if (!file1.is_open()) {
        cerr << "Failed to open file: " << filePathv << endl;
        return 1;
    }

    // Read vertices
    string line;
    while (getline(file1, line)) {
        City* cityNode = new City();
        cityNode->name = trim(line);  // Trim whitespace
        Adjlst.push_back(cityNode);
    }
    file1.close();

    // Open edges file
    ifstream file2(filePathE);
    if (!file2.is_open()) {
        cerr << "Failed to open file: " << filePathE << endl;
        return 1;
    }

    // Read edges
    while (getline(file2, line)) {
        stringstream ss(line);
        string source, destination, weightStr;
        if (!(getline(ss, source, ',') && getline(ss, destination, ',') && getline(ss, weightStr))) {
            cerr << "Malformed edge: " << line << endl;
            continue;
        }
        source = trim(source);  // Trim whitespace
        destination = trim(destination);  // Trim whitespace
        int weight = stoi(weightStr);


        // Find source city
        bool cityFound = false;
        for (City* city : Adjlst) {
            if (toLower(city->name) == toLower(source)) {
                City* newNeighbor = new City();
                newNeighbor->name = destination;
                newNeighbor->Weight = weight;
                newNeighbor->next = city->next;
                city->next = newNeighbor;
                cityFound = true;
                break;
            }
        }
        cityFound = false;
        for (City* city : Adjlst) {
            if (toLower(city->name) == toLower(destination)) {
                City* newNeighbor = new City();
                newNeighbor->name = source;
                newNeighbor->Weight = weight;
                newNeighbor->next = city->next;
                city->next = newNeighbor;
                cityFound = true;
                break;
            }
        }
        if (!cityFound) {
            cerr << "Source city not found: " << source << endl;
        }
    }
    file2.close();

    // Print adjacency list
   City* Head;
    cout << "Adjacency List:" << endl;
    for (int i = 0; i < Adjlst.size(); i++) {
        // Print the name of the source city
        cout << Adjlst[i]->name;
        // Traverse its neighbors
        Head = Adjlst[i]->next;
        while (Head != NULL) {
        cout << " -> (" << Head->name << ", " << Head->Weight << ")";
        Head = Head->next;
        }
        cout << endl;
    }

    //Breadth search
    string startCityName;
    cout << "Enter the city to start BFS/Dijkstras: ";
    cin >> startCityName;

    // Find the starting city in the adjacency list
    City* startCity = nullptr;
    for (City* city : Adjlst) {
        if (toLower(city->name) == toLower(startCityName)) {
            startCity = city;
            break;
        }
    }

    if (startCity) {
        BFS(startCity, Adjlst);  // Perform BFS starting from the found city
    } else {
        cout << "City not found in the graph!" << endl;
    }
    cout << endl;
    cout << "BFS:"<< endl;
    PrintPath(Adjlst,"Hobbiton","MountDoom");
    cout << endl;
    cout << "Dijkstra:"<< endl;
    Dijkstra(Adjlst, startCityName);
    PrintPath(Adjlst,"Hobbiton","MountDoom");
    return 0;
}
