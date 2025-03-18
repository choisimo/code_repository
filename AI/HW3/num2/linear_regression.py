import numpy as np
from sklearn.datasets import make_regression
from sklearn.datasets import load_diabetes
from sklearn import datasets, linear_model


class SGD:
    def __init__(self, learning_rate=0.01, epochs=100, batch_size=10, patience=10):
        self.learning_rate = learning_rate
        self.epochs = epochs
        self.batch_size = batch_size  # size of mini-batch
        self.patience = patience  # early stopping patience
        self.w = None  # weight
        self.b = None  # bias
        self.diabetes = datasets.load_diabetes()

    def fit(self, X, y, X_val=None, y_val=None):
        m, n = X.shape
        self.w = np.zeros(n)  # weight 초기화
        self.b = 0  # bias 초기화

        count = 0
        best_loss = float('inf')

        for epoch in range(self.epochs):
            indices = np.arange(m)
            np.random.shuffle(indices)
            X = X[indices]
            y = y[indices]

            for i in range(0, m, self.batch_size):
                X_mini = X[i:i + self.batch_size]
                y_mini = y[i:i + self.batch_size].reshape(-1)

                y_pred = np.dot(X_mini, self.w) + self.b  # 예측값 계산
                error = y_pred - y_mini  # 오차 계산

                dw = (1 / X_mini.shape[0]) * np.dot(X_mini.T, error)  # weight 기울기 계산
                db = (1 / X_mini.shape[0]) * np.sum(error)  # bias 기울기 계산

                self.w -= self.learning_rate * dw
                self.b -= self.learning_rate * db

            if X_val is not None and y_val is not None:
                predict = np.dot(X_val, self.w) + self.b
                val_loss = np.mean((predict - y_val) ** 2)

                if val_loss < best_loss:
                    best_loss = val_loss
                    count = 0
                else:
                    count += 1

                if count >= self.patience:
                    print("early stopping : ")
                    print(epoch)
                    break

    def predict(self, X):
        return np.dot(X, self.w) + self.b  # 예측값 반환


# 예제 사용법
if __name__ == "__main__":
    # 예제 데이터 생성
    X = 2 * np.random.rand(100, 1)
    y = 4 + 3 * X + np.random.randn(100, 1)

    # 학습 데이터와 검증 데이터로 나누기
    X_train, X_val = X[:80], X[80:]
    y_train, y_val = y[:80], y[80:]

    model = SGD(0.1, 100, 10, 10)
    model.fit(X_train, y_train, X_val, y_val)
    predictions = model.predict(X_val)

    print("예측 값:", predictions[:5].flatten())
    print("실제 값:", y_val[:5].flatten())

    # 샘플 데이터
    X, y = make_regression(n_samples=100)

'''
출력
early stopping : 
18
예측 값: [7.45366607 7.44666807 7.41929656 7.61389375 7.65277277 7.41313783
 7.58496366 7.67356239 7.77393383 7.72440354 7.41539073 7.38023678
 7.24273789 8.22028333 8.41558933 7.21179993 8.07495501 8.52002458
 9.02423348 8.77542154 7.43048026 7.40642641 7.31234373 7.98122255
 8.11485935 7.29117463 7.88178264 8.18631846 8.53131997 8.3610721
 7.43361416 7.41186566 7.32679997 7.93157267 8.05240157 7.30765975
 7.84166319 8.11701195 8.42894812 8.275017   7.42426801 7.39564434
 7.28368755 8.07964201 8.23866755 7.25849668 7.9613102  8.32370268
 8.73424863 8.53165654]
실제 값: [4.86452734 8.74439296 7.50467894 6.72941638 8.50902448]
'''