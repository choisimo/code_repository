myChar = input()

arr = []

for i in range(ord(myChar) - 97 + 1):
    arr.append(i)
print(arr)