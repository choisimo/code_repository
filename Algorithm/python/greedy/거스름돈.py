# 시간 복잡도 big-O (n(K)) // K는 동전의 개수
import time
n = int(input())
count = 0

array = [500, 100, 50, 10]
start_time = time.time()

for coin in array:
    count += n // coin  # 동전의 개수를 더해줍니다.
    n %= coin  # 남은 금액을 계산합니다.
    
end_time = time.time()
print(count)
print("시간 : ", end_time - start_time)