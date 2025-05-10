import numpy as np

class RNN:
    def __init__(self, input_size, hidden_size, output_size):
        self.hidden_size = hidden_size
        self.Wx = np.random.randn(hidden_size, input_size)
        self.Wh = np.random.randn(hidden_size, hidden_size)
        self.Wy = np.random.randn(output_size, hidden_size)
        self.bh = np.zeros((hidden_size, 1))
        self.by = np.zeros((output_size, 1))

    def forward(self, x):
        h = np.zeros((self.hidden_size, x.shape[1]))
        y = np.zeros((self.Wy.shape[0], x.shape[1]))
        for t in range(x.shape[1]):
            h[:, t] = np.tanh(self.Wx @ x[:, t] + self.Wh @ h[:, t-1] + self.bh)
            y[:, t] = self.Wy @ h[:, t] + self.by
        return y, h

    def backward(self, x, y, h, y_pred, learning_rate):
        dWx, dWh, dWy = np.zeros_like(self.Wx), np.zeros_like(self.Wh), np.zeros_like(self.Wy)
        dbh, dby = np.zeros_like(self.bh), np.zeros_like(self.by)
        dh_next = np.zeros_like(h[:, 0])
        loss = 0
        for t in reversed(range(x.shape[1])):
            dy = y_pred[:, t] - y[:, t]
            dWy += dy[:, None] @ h[:, t][None, :]
            dby += dy[:, None]
            dh = self.Wy.T @ dy + dh_next
            dh_raw = (1 - h[:, t] ** 2) * dh
            dbh += dh_raw[:, None]
            dWx += dh_raw[:, None] @ x[:, t][None, :]
            dWh += dh_raw[:, None] @ h[:, t-1][None, :]
            dh_next = self.Wh.T @ dh_raw
            loss += np.sum(dy ** 2)
        self.Wx -= learning_rate * dWx
        self.Wh -= learning_rate * dWh
        self.Wy -= learning_rate * dWy
        self.bh -= learning_rate * dbh
        self.by -= learning_rate * dby
        return loss / x.shape[1]

def train_rnn(train_data, input_size, hidden_size, output_size, epochs, learning_rate):
    rnn = RNN(input_size, hidden_size, output_size)
    for epoch in range(epochs):
        total_loss = 0
        for (x, y) in train_data:
            y_pred, h = rnn.forward(x)
            loss = rnn.backward(x, y, h, y_pred, learning_rate)
            total_loss += loss
        print(f'Epoch {epoch + 1}, Loss: {total_loss / len(train_data)}')
    np.save('rnn_model.npy', [rnn.Wx, rnn.Wh, rnn.Wy, rnn.bh, rnn.by])
    return rnn

# Example usage
# Assuming train_data is a list of (input_sequence, target_sequence) tuples
# input_size, hidden_size, output_size = 10, 20, 10
# epochs, learning_rate = 100, 0.01
# train_rnn(train_data, input_size, hidden_size, output_size, epochs, learning_rate)
