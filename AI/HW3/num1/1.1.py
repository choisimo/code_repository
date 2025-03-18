import numpy as np

# 1차원 배열
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])

# 벡터 내적
result = np.dot(a, b)
print("벡터 내적:", result)  # 출력: 32


# 2-D array example (matrix multiplication)
a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])
result_2d = np.dot(a, b)
print(result_2d)
# Output:
# [[19 22]
#  [43 50]]

# N-D array example
a = np.arange(24).reshape((2, 3, 4))
b = np.arange(12).reshape((4, 3))
result_nd = np.dot(a, b)
print(result_nd.shape)  # Output: (2, 3, 3)


# 점곱을 위한 기본 einsum (벡터의 내적)
a = np.array([1, 2, 3])
b = np.array([4, 5, 6])
result_einsum_1d = np.einsum('i,i', a, b)
print(result_einsum_1d)  # 출력: 32

# einsum을 사용한 행렬 곱셈
a = np.array([[1, 2], [3, 4]])
b = np.array([[5, 6], [7, 8]])
result_einsum_2d = np.einsum('ij,jk->ik', a, b)
print(result_einsum_2d)
# 출력:
# [[19 22]
#  [43 50]]

# a의 마지막 축과 b의 끝에서 두 번째 축의 합 곱셈
a = np.arange(24).reshape((2, 3, 4))
b = np.arange(12).reshape((4, 3))
result_einsum_nd = np.einsum('ijk,kl->ijl', a, b)
print(result_einsum_nd.shape)  # 출력: (2, 3, 3)
