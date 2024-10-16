#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

vector<vector<string>> loadCSV(const string& filename) {
    ifstream file(filename);
    string line, cell;
    vector<vector<string>> data;

    while (getline(file, line)) {
        stringstream lineStream(line);
        vector<string> row;
        while (getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    return data;
}

int main() {
    string filename = "C:/workspace/github/code_repository/Algorithm/C++/algoritm_team_report/games.csv";
    vector<vector<string>> data = loadCSV(filename);

    // CSV 데이터 출력
    for (const auto& row : data) {
        for (const auto& cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }

    return 0;
}
