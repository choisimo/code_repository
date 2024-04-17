#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class process {
private:
    vector<int> lifes;
    vector<int> delete_orders;
    queue<int> orders;

public:
    process(int N, int K, int D) : lifes(N, D) {
        for (int i = 0; i < N; ++i) {
            orders.push(i);
        }
        // K ��° ã�� ���� ����
        int count = 1;

        while (!orders.empty()) {
            int current = orders.front();
            orders.pop();
            // ���簡 K ��°�� �ƴ϶��
            if (count != K){
                //cout << "not K : " << current << endl;
                orders.push(current);
            } else {
                //cout << "this is K : " << current << endl;
                lifes[current]--;
                //cout << "K's remain life : " << lifes[current] << endl;
                if (lifes[current] > 0){
                    // ����� ���
                    orders.push(current);
                    count = 1;
                } else {
                    // �׾��� ���
                    delete_orders.push_back(current + 1);
                    count = 0;
                }
            }
            count++;

            if (orders.size() == 1)
            {
                delete_orders.push_back(orders.front() + 1);
                orders.pop();
            }
        }
        printResult();
    }
    void printResult(){
        cout << "<";
        for (int i = 0; i < delete_orders.size(); i++){
            cout << delete_orders[i];
            if (i < delete_orders.size() -1)
            {
                cout << ",";
            }
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