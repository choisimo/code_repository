#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Game 구조체 정의
struct Game {
    string id;                         // 게임 고유 식별자
    bool rated;                        // 평점이 매겨졌는지 여부 (참 | 거짓)
    string victory_status;             // 승리 상태 (체크메이트, 항복 등)
    string winner;                     // 승리한 플레이어 (백, 흑, None == 무승부)
    int white_rating;                  // 백색 플레이어 평점
    int black_rating;                  // 흑색 플레이어 평점
};

// CSV를 로드하고 Game 객체로 변환하는 클래스
class CSVLoader {
private:
    string filename;
public:
    CSVLoader(const string& f) : filename(f) {}

    vector<Game> load() {
        ifstream file(filename);
        string line, cell;
        vector<Game> games;


        // header에서 column 추출
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
            // 구조체
            Game game;

            game.id = row[header_map["id"]];
            game.rated = (row[header_map["rated"]] == "TRUE");
            game.victory_status = row[header_map["victory_status"]];
            game.winner = row[header_map["winner"]];

            // 예외 처리를 추가한 white_rating 변환
            try {
                if (!row[header_map["white_rating"]].empty()) {
                    game.white_rating = stoi(row[header_map["white_rating"]]);
                } else {
                    game.white_rating = 0; // 기본값 설정
                }
            } catch (const std::invalid_argument& e) {
                cout << "Invalid white_rating: " << row[header_map["white_rating"]] << endl;
            }

            // 예외 처리를 추가한 black_rating 변환
            try {
                if (!row[header_map["black_rating"]].empty()) {
                    game.black_rating = stoi(row[header_map["black_rating"]]);
                } else {
                    game.black_rating = 0; // 기본값 설정
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

    // Game 데이터 출력
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
