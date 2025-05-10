//
// Created by csimo on 3/17/2024.
//
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class process{
private:
    vector<int> lifes;
    vector<int> delete_orders;
    queue<int> orders;
public:
    process(int N, int K, int D) : lifes(N, D){
        circulate(N, K, D);
    }
    void circulate(int N, int K, int D) {

        for(int i = 0; i < N; ++i) {
            orders.push(i);
        }

        while (!orders.empty()) {
            for(int i = 0; i < K - 1; ++i) {
                orders.push(orders.front());
                orders.pop();
            }

            int current = orders.front();
            orders.pop();
            lifes[current]--;

            if (lifes[current] > 0) {
                orders.push(current);
            } else {
                delete_orders.push_back(current + 1);
            }
        }

        cout << "<";
        for (size_t i = 0; i < delete_orders.size(); i++) {
            cout << delete_orders[i];
            if (i < delete_orders.size() - 1) cout << ", ";
        }
        cout << ">" << endl;
    }
};



int main() {
    int N, K, D;
    cin >> N >> K >> D;
    process proc(N, K, D);
    return 0;
}
