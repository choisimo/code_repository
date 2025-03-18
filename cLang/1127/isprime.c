//
// Created by nodove on 24. 11. 27.
//
#include <stdio.h>

int check_prime(int n);

int main(){
    int flag = 0, k;
    printf("정수 입력 : ");
    scanf("%d", &k);

    for (int i = 2; i < k; i++){
        if (check_prime(i) == 1) // 첫 번째 i가 소수인가?
            if (check_prime(k - i) == 1) // 두 번째 i가 소수인가?
            {
                printf("%d = %d + %d\n", k, i, k - i);
                flag = 1;
            }
        if (flag == 0) printf("골드바흐의 추측이 틀립니다.\n");
    }

    if (flag == 0) printf("소수가 아닙니다.\n");

}

int check_prime(int n){
    int is_prime = 1;
    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            is_prime = 0;
            break;
        }
    }
    return is_prime;
}