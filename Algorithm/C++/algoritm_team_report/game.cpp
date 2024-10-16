#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Game ����ü ����
struct Game {
    string id;                         // ���� ���� �ĺ���
    bool rated;                        // ������ �Ű������� ���� (�� | ����)
    string victory_status;             // �¸� ���� (üũ����Ʈ, �׺� ��)
    string winner;                     // �¸��� �÷��̾� (��, ��, None == ���º�)
    int white_rating;                  // ��� �÷��̾� ����
    int black_rating;                  // ��� �÷��̾� ����
};

// CSV�� �ε��ϰ� Game ��ü�� ��ȯ�ϴ� Ŭ����
class CSVLoader {
private:
    string filename;
public:
    CSVLoader(const string& f) : filename(f) {}

    vector<Game> load() {
        ifstream file(filename);
        string line, cell;
        vector<Game> games;


        // header���� column ����
        getline(file, line);
        stringstream headerStream(line);
        vector<string> headers;

        while (getline(headerStream, cell, ',')) {
            headers.push_back(cell);
        }

        unordered_map<string, int> header_map;
        for (int i = 0; i < headers.size(); i++) {
            header_map[headers[i]] = i;
        }

        while (getline(file, line)) {
            stringstream lineStream(line);
            vector<string> row;
            while (getline(lineStream, cell, ',')) {
                row.push_back(cell);
            }
            // ����ü
            Game game;

            game.id = row[header_map["id"]];
            game.rated = (row[header_map["rated"]] == "TRUE");
            game.victory_status = row[header_map["victory_status"]];
            game.winner = row[header_map["winner"]];

            // ���� ó���� �߰��� white_rating ��ȯ
            try {
                if (!row[header_map["white_rating"]].empty()) {
                    game.white_rating = stoi(row[header_map["white_rating"]]);
                } else {
                    game.white_rating = 0; // �⺻�� ����
                }
            } catch (const std::invalid_argument& e) {
                cout << "Invalid white_rating: " << row[header_map["white_rating"]] << endl;
            }

            // ���� ó���� �߰��� black_rating ��ȯ
            try {
                if (!row[header_map["black_rating"]].empty()) {
                    game.black_rating = stoi(row[header_map["black_rating"]]);
                } else {
                    game.black_rating = 0; // �⺻�� ����
                }
            } catch (const std::invalid_argument& e) {
                cout << "Invalid black_rating: " << row[header_map["black_rating"]] << endl;
            }

            games.push_back(game);
        }

        return games;
    }
};

int main() {
    string filename = "C:/workspace/github/code_repository/Algorithm/C++/algoritm_team_report/games.csv";
    CSVLoader loader(filename);
    vector<Game> games = loader.load();

    // Game ������ ���
    for (const auto& game : games) {
        //if (game.victory_status == "checkmate"){
            cout << "Game ID: " << game.id
                 << ", Winner: " << game.winner
                << ", White Rating: " << game.white_rating
                << ", Black Rating: " << game.black_rating
                << ", winner: " << game.winner
                << ", victory status: " << game.victory_status << endl;

    }

    return 0;
}
