import numpy as np
import pickle

def gen_random_dataset(R, d, N):
    R = 10
    # 1. true 파라미터 값의 랜덤 할당 (uniform 분포 랜덤 생성)
    w = np.random.uniform(-R, R, d)
    b = np.random.uniform(-R, R)

    # 2. dataset 생성
    x = np.random.uniform(-R, R, (N, d))
    y = np.dot(x, w) + b

    # 3. dataset 분리
    training_set = int (0.85 * N)
    dev_set = int(0.05 * N)
    test_set = int(0.1 * N)

    train_x = x[:training_set]
    train_y = y[:training_set]
    dev_x = x[training_set:training_set+test_set]
    dev_y = y[training_set:training_set + test_set]
    test_x = x[training_set+test_set:]
    test_y = y[training_set+test_set:]

    return (train_x, train_y), (dev_x, dev_y), (test_x, test_y)

def save(file, data):
    with open(file, 'wb') as file:
        pickle.dump(data, file)


if __name__ == "__main__":
    R = 10
    N = 1000
    d = 1
    alpha = 0.1

    (train_x, train_y), (dev_x, dev_y), (test_x, test_y) = gen_random_dataset(R, d, N)
    save("myrandomdataset.pkl",  {'train': (train_x, train_y), 'dev': (dev_x, dev_y), 'test': (test_x, test_y)})

    N = 10000
    (train_x, train_y), (dev_x, dev_y), (test_x, test_y) = gen_random_dataset(R, d, N)
    save("myrandomdataset2.pkl",  {'train': (train_x, train_y), 'dev': (dev_x, dev_y), 'test': (test_x, test_y)})

    N = 100000
    (train_x, train_y), (dev_x, dev_y), (test_x, test_y) = gen_random_dataset(R, d, N)
    save("myrandomdataset3.pkl",  {'train': (train_x, train_y), 'dev': (dev_x, dev_y), 'test': (test_x, test_y)})

    N = 1000000
    (train_x, train_y), (dev_x, dev_y), (test_x, test_y) = gen_random_dataset(R, d, N)
    save("myrandomdataset4.pkl",  {'train': (train_x, train_y), 'dev': (dev_x, dev_y), 'test': (test_x, test_y)})

