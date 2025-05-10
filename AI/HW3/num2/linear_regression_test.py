import numpy as np
import pickle
from sklearn.datasets import load_diabetes
from linear_regression import SGD

def dataset_loader(path):
    with open(path, 'rb') as f:
        data = pickle.load(f)
    return data


if __name__ == '__main__':
    datasets = ['myrandomdataset.pkl', 'myrandomdataset2.pkl', 'myrandomdataset3.pkl', 'myrandomdataset4.pkl']
    for dataset in datasets:
        data = dataset_loader(dataset)
        train_x, train_y = data['train']
        dev_x, dev_y = data['dev']
        test_x, test_y = data['test']

        model = SGD(0.01, 100, 10, 10)
        model.fit(train_x, train_y, dev_x, dev_y)
        prediction = model.predict(test_x)

        print(dataset)
        print("Prediction: ", prediction)
        print("Actual: ", test_y)

    diabetes = load_diabetes()
    X = diabetes.data
    y = diabetes.target

    N = len(y)
    train_size = int(0.85 * N)
    dev_size = int(0.05 * N)

    X_train, y_train = X[:train_size], y[:train_size]
    X_dev, y_dev = X[train_size:train_size + dev_size], y[train_size:train_size + dev_size]
    X_test, y_test = X[train_size + dev_size:], y[train_size + dev_size:]

    model = SGD(0.1, 100, 10, 10)
    model.fit(X_train, y_train, X_dev, y_dev)
    predictions = model.predict(X_test)

    print("\nScikit-learn Diabetes Dataset")
    print("Prediction: ", predictions)
    print("Actual: ", y_test)
