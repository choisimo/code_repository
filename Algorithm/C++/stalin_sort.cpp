#include <iostream>
#include <vector>
#include <queue>
#include <iterator>
#include <algorithm>

/**
* 스탈린 정렬 (Stalin Sort)
* 순서에 맞지 않는 요소들을 제거하는 정렬 알고리즘
*/

using namespace std;

// 벡터 출력 함수
template<typename T>
void printVector(const std::vector<T>& vec, const std::string& label) {
    std::cout << label;
    std::copy(vec.begin(), vec.end(), 
              std::ostream_iterator<T>(std::cout, " "));
    std::cout << std::endl;
}

// 큐 출력 함수
template<typename T>
void printQueue(std::queue<T> q, const std::string& label) {
    std::cout << label;
    
    // 큐가 비어있는 경우 처리
    if (q.empty()) {
        std::cout << "큐가 비어있습니다." << std::endl;
        return;
    }
    
    // 첫 번째 요소는 별도로 처리하여 쉼표 문제 해결
    std::cout << q.front();
    q.pop();
    
    // 나머지 요소 출력
    while (!q.empty()) {
        std::cout << ", " << q.front();
        q.pop();
    }
    std::cout << std::endl;
}

template<class RU>
class stalinMethod {
private:
    queue<RU> deadman;     
public:
    void StalinSort(vector<RU>& L) {
        if (L.empty()) return;  // 벡터가 비어있는지 확인

        // write1: 마지막으로 정렬된 요소의 위치를 가리키는 반복자
        auto write1 = L.begin();
        // read1: 원본 데이터를 순회하는 반복자
        auto read1 = write1;
        ++read1;                // 두 번째 요소부터 시작 (첫 번째 요소는 이미 정렬된 것으로 간주)

        while (read1 != L.end()) {
            // 현재 요소가 순서를 유지하면 정렬된 부분에 추가
            if (*read1 >= *write1) {
                *++write1 = *read1;
            }
            // 그렇지 않으면 아무 작업도 수행하지 않음 - 이 요소들은 결과에 포함되지 않음 (스탈린 방식)
            else {
                deadman.push(*read1);
            }
            ++read1;
        }
        // 정렬 후 사용되지 않은 요소 제거
        if (!L.empty()) {
            L.erase(++write1, L.end());
        }
    }

    queue<RU>& getDeadman() {
        return deadman;
    }
};

int main() {
    stalinMethod<int> SM;
    std::vector<int> l = {1, 7, 3, 5, 4, 6, 4, 8, 10};
    
    printVector(l, "원본 배열: ");
    
    SM.StalinSort(l);
    
    printVector(l, "스탈린 정렬 후: ");
    printQueue(SM.getDeadman(), "숙청된 요소들: ");

    return 0;
}
