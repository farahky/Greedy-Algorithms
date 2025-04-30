#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

// Define a structure to represent an edge
struct Edge {
    char dest;
    double weight;
    Edge(char d, double w) : dest(d), weight(w) {}
};

// Define a function to read the dataset from the file
void readDataset(const string& filename, unordered_map<char, vector<Edge>>& graph) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip the header line "Stars Information"

    // Read star data (skip this part as it is not necessary for Dijkstra)
    while (getline(file, line)) {
        if (line.empty()) break;
    }

    getline(file, line); // Skip the header line "Edges and Distances"

    // Read edge data
    while (getline(file, line)) {
       if (line.empty()) continue;
       char src = toupper(line[0]);
       char dest = toupper(line[2]);

       // Extract weight using std::istringstream
       double weight;
       istringstream iss(line.substr(3)); // Start from index 3 to skip source and destination
       iss >> weight;

       graph[src].emplace_back(dest, weight);
       graph[dest].emplace_back(src, weight); // Assuming undirected graph
    }

    file.close();
}

// Dijkstra's algorithm
void dijkstra(const unordered_map<char, vector<Edge>>& graph, char src, unordered_map<char, double>& distances, unordered_map<char, vector<char>>& paths) {
    distances.clear();
    paths.clear();

    // Initialize distances and paths
    for (const auto& node : graph) {
        distances[node.first] = numeric_limits<double>::max();
        paths[node.first] = {};
    }
    distances[src] = 0;
    paths[src] = {src};

    // Priority queue to store vertices with their distances
    priority_queue<pair<double, char>, vector<pair<double, char>>, greater<pair<double, char>>> pq;
    pq.emplace(0, src);

    while (!pq.empty()) {
        double dist = pq.top().first;
        char u = pq.top().second;
        pq.pop();

        if (dist > distances[u]) continue;

        for (const Edge& edge : graph.at(u)) {
            char v = edge.dest;
            double weight = edge.weight;
            double newDist = dist + weight;
            if (newDist < distances[v]) {
                distances[v] = newDist;
                paths[v] = paths[u];
                paths[v].push_back(toupper(v));
                pq.emplace(newDist, v);
            }
        }
    }
}

// Function to print the shortest paths and distances
void printShortestPaths(char src, const unordered_map<char, double>& distances, const unordered_map<char, vector<char>>& paths) {
    ofstream outFile("shortest_paths.txt");
    if (!outFile) {
        cerr << "Error: Unable to create output file" << endl;
        return;
    }

    outFile << "Shortest paths from Star " << src << ":\n" << endl;

    // Create a vector to store the nodes and their distances
    vector<pair<char, double>> nodes;
    for (const auto& node : distances) {
        if (node.first != src) {
            nodes.emplace_back(node.first, node.second);
        }
    }

    // Sort the nodes based on their distances
    sort(nodes.begin(), nodes.end(), [](const pair<char, double>& a, const pair<char, double>& b) {
        return a.second < b.second;
    });

    // Print the shortest paths in increasing order of distance
    for (const auto& node : nodes) {
        outFile << "Star " << node.first << ": ";
        if (node.second == numeric_limits<double>::max()) {
            outFile << "No path" << endl;
        } else {
            outFile << "Distance = " << node.second << ", Path = ";
            for (char step : paths.at(node.first)) {
                outFile << step << " ";
            }
            outFile << endl;
        }
    }

    outFile.close();
    cout << "Shortest paths and distances written to shortest_paths.txt" << endl;
}

int main() {
    unordered_map<char, vector<Edge>> graph;
    readDataset("stardata.txt", graph);

    unordered_map<char, double> distances;
    unordered_map<char, vector<char>> paths;
    dijkstra(graph, 'A', distances, paths);

    printShortestPaths('A', distances, paths);

    return 0;
}