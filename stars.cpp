#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <set>
#include <utility>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <queue>

using namespace std;

struct Star {
    string name;
    int x, y, z;
    int weight, profit;
};

//calculating the distance using the formula given
double calcdistance(const Star& star1, const Star& star2) {
    return sqrt(pow(star2.x - star1.x, 2) +
                pow(star2.y - star1.y, 2) +
                pow(star2.z - star1.z, 2));
}

//getting the unique digits from the result of the sum of the 3 IDs
vector<char> digits(unsigned long long value) {
    set<char> thedigits;
    string value_str = to_string(value);
    for (char c : value_str) {
        thedigits.insert(c);
    }
    return vector<char>(thedigits.begin(), thedigits.end());
}

//generating random numbers for the coordinates
int coordinate(const vector<char>& limits) {
    string coordinate;
    for (int i = 0; i < 3; i++) {
        coordinate += limits[rand() % limits.size()];
    }
    return stoi(coordinate);
}

//generating random numbers for weight and profit 
int weightnprofit(const vector<char>& limits) {
    string wnp;
    for (int i = 0; i < 2; i++) {
        wnp += limits[rand() % limits.size()];
    }
    return stoi(wnp);
}

int main() {
    unsigned long long aida = 1211103282;
    unsigned long long najwa = 1211101390;
    unsigned long long vikraman = 1221303198;

    //sum of the other members' ID
    unsigned long long seed = aida + najwa + vikraman;
    srand(time(0));
    vector<char> limits = digits(seed);

    vector<Star> stars;
    for (int i = 0; i < 20; i++) {
        Star star;
        star.name = "Star " + string(1, 'A' + i);
        star.x = coordinate(limits);
        star.y = coordinate(limits);
        star.z = coordinate(limits);
        star.weight = weightnprofit(limits);
        star.profit = weightnprofit(limits);
        stars.push_back(star);
    }

    set<pair<int, int>> edges;
    vector<int> connectedstar[20];

    vector<bool> inMST(20, false);
    priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<pair<double, pair<int, int>>>> pq;
    
    inMST[0] = true;
    for (int i = 1; i < 20; i++) {
        pq.push({calcdistance(stars[0], stars[i]), {0, i}});
    }

    while (!pq.empty() && edges.size() < 19) {
        auto edge = pq.top(); pq.pop();
        int u = edge.second.first;
        int v = edge.second.second;
        if (inMST[v]) continue;

        inMST[v] = true;
        edges.insert({min(u, v), max(u, v)});
        connectedstar[u].push_back(v);
        connectedstar[v].push_back(u);

        for (int i = 0; i < 20; i++) {
            if (!inMST[i]) {
                pq.push({calcdistance(stars[v], stars[i]), {v, i}});
            }
        }
    }

    //ensuring each star has at least 3 connections
    for (int i = 0; i < 20; i++) {
        while (connectedstar[i].size() < 3) {
            int j = rand() % 20;
            if (i != j && edges.find({min(i, j), max(i, j)}) == edges.end()) {
                edges.insert({min(i, j), max(i, j)});
                connectedstar[i].push_back(j);
                connectedstar[j].push_back(i);
            }
        }
    }

    //make extra edges until it totals up to 54 unique edges
    while (edges.size() < 54) {
        int i = rand() % 20;
        int j = rand() % 20;
        if (i != j && edges.find({min(i, j), max(i, j)}) == edges.end()) {
            edges.insert({min(i, j), max(i, j)});
            connectedstar[i].push_back(j);
            connectedstar[j].push_back(i);
        }
    }

    //saving the data
    ofstream outputFile("stardata.txt");
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file." << endl;
        return 1; 
    }

    outputFile << "Stars Information : \n" << endl;
    outputFile << left << setw(10) << "Name" 
                       << setw(8) << "x" 
                       << setw(8) << "y" 
                       << setw(8) << "z" 
                       << setw(8) << "weight" 
                       << setw(8) << "profit" << endl;

    for (const auto& star : stars) {
        outputFile << left << setw(10) << star.name
                           << setw(8) << star.x
                           << setw(8) << star.y
                           << setw(8) << star.z
                           << setw(8) << star.weight
                           << setw(8) << star.profit << endl;
    }

    outputFile << "\nEdges and Distances :" << endl;

    for (const auto& edge : edges) {
        const Star& star1 = stars[edge.first];
        const Star& star2 = stars[edge.second];
        double distance = calcdistance(star1, star2);
        outputFile << left << setw(1) << star1.name.substr(5) << " " 
                           << setw(7) << star2.name.substr(5) << " "
                           << distance << endl;
    }

    outputFile.close();
    cout << "Data saved stardata.txt successfully!" << endl;

    return 0;
}
