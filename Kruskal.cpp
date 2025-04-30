#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

struct Edge {
    int src, dest;
    double weight;
    Edge(int s, int d, double w) : src(s), dest(d), weight(w) {}
};

bool cmp(const Edge& a, const Edge& b) {
    return a.weight < b.weight;
}

int findRoot(vector<int>& parent, int x) {
    if (parent[x] == x)
        return x;
    return parent[x] = findRoot(parent, parent[x]);
}

void unionSets(vector<int>& parent, int x, int y) {
    int xRoot = findRoot(parent, x);
    int yRoot = findRoot(parent, y);
    parent[xRoot] = yRoot;
}

int main() {
    ifstream file("stardata.txt");
    if (!file.is_open()) {
        cerr << "Unable to open file stardata.txt" << endl;
        return 1;
    }

    string line;
    unordered_map<string, int> starIndices;
    unordered_map<int, string> starNames;
    int starCount = 0;

    // Skip to the "Edges and Distances" section
    while (getline(file, line)) {
        if (line.find("Edges and Distances") != string::npos) {
            break;
        }
    }

    vector<Edge> edges;

    // Read edges and distances
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.find("Edges") != string::npos || line.find("Distance") != string::npos) continue;
        istringstream iss(line);
        string uName, vName;
        double distance;
        iss >> uName >> vName >> distance;

        if (starIndices.find(uName) == starIndices.end()) {
            starIndices[uName] = starCount;
            starNames[starCount] = uName;
            starCount++;
        }
        if (starIndices.find(vName) == starIndices.end()) {
            starIndices[vName] = starCount;
            starNames[starCount] = vName;
            starCount++;
        }

        int u = starIndices[uName];
        int v = starIndices[vName];
        edges.emplace_back(u, v, distance);
    }

    file.close();

    sort(edges.begin(), edges.end(), cmp);

    vector<int> parent(starCount);
    for (int i = 0; i < starCount; ++i) {
        parent[i] = i;
    }

    vector<Edge> mst;
    double totalWeight = 0;

    for (const Edge& e : edges) {
        int srcRoot = findRoot(parent, e.src);
        int destRoot = findRoot(parent, e.dest);

        if (srcRoot != destRoot) {
            mst.push_back(e);
            totalWeight += e.weight;
            unionSets(parent, srcRoot, destRoot);
        }
    }

    // Output the complete MST
    ofstream output("MST.txt");
    if (output.is_open()) {
        output << "Edges       Distance" << endl;
        for (const Edge& e : mst) {
            output << starNames[e.src] << " -- " << starNames[e.dest] << "      " << e.weight << endl;
        }
        output.close();
    } else {
        cerr << "Unable to open file MST.txt" << endl;
    }

    cout << "Minimum Spanning Tree edges have been written to MST.txt\n";

    return 0;
}
