'''
1. Initialize weights w and bias b to zeros
2. For epoch in range(1, E + 1):
3.     Shuffle the training data
4.     For i in range(0, number of training examples, m):
5.         Select mini-batch X_m and y_m
6.         Compute predictions y_pred = X_m * w + b
7.         Compute error = y_pred - y_m
8.         Compute gradient for w: dw = (2/m) * X_m.T * error
9.         Compute gradient for b: db = (2/m) * sum(error)
10.        Update weights: w = w - η * dw
11.        Update bias: b = b - η * db
12. End
'''