import numpy as np

class LinearRegressionSGD:
    def __init__(self, learning_rate=0.01, epochs=1000, batch_size=32, patience=10):
        self.learning_rate = learning_rate  # 학습률
        self.epochs = epochs  # 에포크 수
        self.batch_size = batch_size  # 미니배치 크기
        self.patience = patience  # 조기 종료를 위한 인내심
        self.w = None  # 가중치
        self.b = None  # 편향

    def fit(self, X, y, X_val=None, y_val=None):
        m, n = X.shape
        self.w = np.zeros(n)  # 가중치를 0으로 초기화
        self.b = 0  # 편향을 0으로 초기화

        best_loss = float('inf')
        no_improvement_count = 0

        for epoch in range(self.epochs):
            # Shuffle the training data
            indices = np.arange(m)
            np.random.shuffle(indices)
            X = X[indices]
            y = y[indices]

            for i in range(0, m, self.batch_size):
                X_mini = X[i:i + self.batch_size]
                y_mini = y[i:i + self.batch_size]

                y_pred = np.dot(X_mini, self.w) + self.b  # 예측값 계산
                error = y_pred - y_mini  # 오차 계산

                dw = (2 / X_mini.shape[0]) * np.dot(X_mini.T, error).reshape(-1)  # 가중치에 대한 기울기 계산
                db = (2 / X_mini.shape[0]) * np.sum(error)  # 편향에 대한 기울기 계산

                self.w -= self.learning_rate * dw  # 가중치 업데이트
                self.b -= self.learning_rate * db  # 편향 업데이트

            # Compute validation loss
            if X_val is not None and y_val is not None:
                y_val_pred = np.dot(X_val, self.w) + self.b
                val_loss = np.mean((y_val_pred - y_val) ** 2)

                if val_loss < best_loss:
                    best_loss = val_loss
                    no_improvement_count = 0
                else:
                    no_improvement_count += 1

                if no_improvement_count >= self.patience:
                    print(f"Early stopping at epoch {epoch}")
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

    model = LinearRegressionSGD(learning_rate=0.1, epochs=1000, batch_size=20, patience=10)
    model.fit(X_train, y_train, X_val, y_val)
    predictions = model.predict(X_val)

    print("Predicted values:", predictions[:5])
    print("Actual values:", y_val[:5])
