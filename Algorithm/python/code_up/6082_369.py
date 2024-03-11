x = int(input())
arr = []

for i in range (x+1):
    if (i%10 == 3 or i%10 == 6 or i%10 == 9):
        arr.append("X")
    elif (i == 0):
        continue
    else:
        arr.append(i)

for j in range(len(arr)):
    print(arr[j], end= " ")