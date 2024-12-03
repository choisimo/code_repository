#include <stdio.h>
#include <stdlib.h> // rand(), srand()
#include <time.h> // time() 함수 사용을 위한 헤더파일

int fib(int n){
    if(n == 0) return 0;
    else if (n == 1) return 1;
    else return fib(n-1) + fib(n-2);
}

int main(){
    srand(time(NULL)); // 난수 고정 방법 2 : srand(time(NULL))    시드값을 현재 시간으로 설정
    printf("%d ", rand()); // 난수 고정 방법 1 : srand(시드값)    시드값을 고정하면 같은 난수가 발생 (시드값을 고정하지 않으면 실행할 때마다 다른 난수 발생)
    printf("%d ", rand());
    printf("%d ", rand());
    printf("%d ", rand());
    int n;
    scanf("%d", &n);
    printf("%d\n", fib(n));
    return 0;
}