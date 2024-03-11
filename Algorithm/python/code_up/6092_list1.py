# 출석 번호 부른 횟수
n = int(input())
# 무작위로 부른 n개의 번호
random_n = input().split()

for i in range(n):
    random_n[i] = int(random_n[i])
    
d = []
for i in range(24):
    d.append(0)
    
for i in range(n):
    d[random_n[i]] += 1

for i in range (1,24):
    print(d[i], end="")

