#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <ctime>

using namespace std;

//to get the unique digits in leader's ID number
vector<char> digits(unsigned long long value) {
    set<char> thedigits;
    string value_str = to_string(value);
    for (char c : value_str) {
        thedigits.insert(c);
    }
    return vector<char>(thedigits.begin(), thedigits.end());
}

//to generate random numbers based on the uniques digits
string rndm(const vector<char>& limits, int length) {
    string random;
    for (int i = 0; i < length; i++) {
        random += limits[rand() % limits.size()];
    }
    return random;
}

int main()
{
    //Farah Kamila's ID
    unsigned long long LeaderID = 1211103293;
    vector<char> limits = digits(LeaderID);
    srand(time(0));

    //6 sets of data
    const int setsizes[] = {100, 1000, 10000, 100000, 500000, 1000000};
    const int numsets = sizeof(setsizes) / sizeof(setsizes[0]);

    //saving the datasets
    for (int setIndex = 0; setIndex < numsets; setIndex++) {
        int currentSetSize = setsizes[setIndex];
        string fileName = "set" + to_string(setIndex + 1) + ".txt";

        // open the file for writing
        ofstream outFile(fileName);
        if (!outFile) {
            cerr << "Error opening file: " << fileName << endl;
            return 1;
        }

        outFile << "Set " << setIndex + 1 << " (" << currentSetSize << " numbers):\n\n";

        for (int i = 0; i < currentSetSize; i++) {
            int length = rand() % 10 + 1;
            string randomnumber = rndm(limits, length);
            outFile << randomnumber << "\n";
        }

        outFile.close();
        cout << "Set " << setIndex + 1 << " random numbers are generated and stored in " << fileName << " successfully.\n";
    }
    return 0;
}
