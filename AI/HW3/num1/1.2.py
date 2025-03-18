import numpy as np
#q67
'''
Considering a four dimensions array, 
how to get sum over the last two axis at once? (★★★)
'''
A = np.random.randint(0,10,(3,4,3,4))
sum1 = A.sum(axis=(-2, -1))
print(sum1)

sum2 = A.reshape(A.shape[:-2] + (-1,)).sum(axis=-1)
print(sum2)


#q69
'''
How to get the diagonal of a dot product? (★★★)
'''
A = np.random.uniform(0, 1,(5,5))
B = np.random.uniform(0,1,(5,5))
## np.diag = 행렬 곱 계산 후 대각선 추출
result_slow = np.diag(np.dot(A, B))
## np.sum = 해렬 곱의 대각선 직접 계산
result_fast = np.sum(A * B.T, axis = 1)
## np.einsum = 아인슈타인 합산 표기법 계산
result_faster = np.einsum(A,B)


#70
'''
Consider the vector [1, 2, 3, 4, 5], 
how to build a new vector with 3 consecutive zeros interleaved between each value? (★★★)
'''


#71
'''
Consider an array of dimension (5,5,3), 
how to mulitply it by an array with dimensions (5,5)? (★★★)
'''
A = np.random.random((5,5,3))
B = np.random.random((5,3))
B_alter = B[:,:,np.newaxis]
result = A * B_alter
print(result)

#75
'''
How to compute averages using a sliding window over an array? (★★★)
'''
def moving_average(arr, window_size):
    if window_size > len(arr):
        raise ValueError("window size exceeds length")

    shape = (arr.size - window_size + 1, window_size)
    strides = (arr.strides[0], arr.strides[0])
    sliding_window = np.lib.stride_tricks.as_strided(arr, shape=shape, strides=strides)
    return sliding_window.mean(axis=1)


#q77
'''
How to negate a boolean, or to change the sign of a float inplace? (★★★)
'''
bool_array = np.array([True, False, True])
bool_array[:] = ~bool_array
np.logical_not(bool_array, out=bool_array)
print(bool_array)

