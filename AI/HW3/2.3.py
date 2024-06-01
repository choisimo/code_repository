'''
1. Initialize weights w and bias b to zeros
2. Initialize best_loss to a large value and no_improvement_count to 0
3. Set patience (number of epochs with no improvement to wait before stopping)
4. For epoch in range(1, E + 1):
    a. Shuffle the training data
    b. For i in range(0, number of training examples, m):
        i. Select mini-batch X_m and y_m
        ii. Compute predictions y_pred = X_m * w + b
        iii. Compute error = y_pred - y_m
        iv. Compute gradient for w: dw = (2/m) * X_m.T * error
        v. Compute gradient for b: db = (2/m) * sum(error)
        vi. Update weights: w = w - η * dw
        vii. Update bias: b = b - η * db
    c. Compute validation loss (mean squared error on validation set)
    d. If validation loss < best_loss:
        i. best_loss = validation loss
        ii. no_improvement_count = 0
    e. Else:
        i. no_improvement_count += 1
    f. If no_improvement_count >= patience:
        i. Stop training early
5. End
'''