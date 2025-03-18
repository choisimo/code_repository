import numpy as np

def solve_linear(A, B):
    if (A.shape[0] != A.shape[1]):
        raise ValueError("not square matrix")
    return np.dot(np.linalg.inv(A), B)


if __name__ == "__main__":
    A = np.array([[3,1],[1,2]])
    B = np.array([9,8])

    print(solve_linear(A, B))


'''
출력
[2. 3.]
'''