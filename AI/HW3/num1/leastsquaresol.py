import numpy as np

def least_squares(X, Y):
    return np.dot(np.linalg.pinv(X), Y)


if __name__ == "__main__":
    A = np.array([[1,2],[3,4]])
    B = np.array([5,6])

    print(least_squares(A, B))

'''
출력
[-4.   4.5]

'''