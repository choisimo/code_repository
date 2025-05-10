s = int(input())
print(~s)

arr = []

if(s < 0):
    s = abs(s)
    while(s > 0):
        arr.append(s % 2)
        s = s//2
    arr = 1 - arr.reverse()

if (s > 0):        
    while (s > 0):
        arr.append(s % 2)
        s = s//2
    arr = arr.reverse()


for i in range(len(arr)):
    arr[i] = 1 - arr[i]
    
sum = 0

for i in range(len(arr)):
    print(i," 번 째 배열 : ", arr[i])
    sum += arr[i] * (i**2)

print(sum)
