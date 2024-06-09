import numpy as np

class LSTM:
    def __init__(self, input_size, hidden_size, output_size):
        self.hidden_size = hidden_size
        self.Wf = np.random.randn(hidden_size, input_size + hidden_size)
        self.Wi = np.random.randn(hidden_size, input_size + hidden_size)
        self.Wo = np.random.randn(hidden_size, input_size + hidden_size)
        self.Wc = np.random.randn(hidden_size, input_size + hidden_size)
        self.Wy = np.random.randn(output_size, hidden_size)
        self.bf = np.zeros((hidden_size, 1))
        self.bi = np.zeros((hidden_size, 1))
        self.bo = np.zeros((hidden_size, 1))
        self.bc = np.zeros((hidden_size, 1))
        self.by = np.zeros((output_size, 1))

    def forward(self, x):
        h = np.zeros((self.hidden_size, x.shape[1]))
        c = np.zeros((self.hidden_size, x.shape[1]))
        y = np.zeros((self.Wy.shape[0], x.shape[1]))
        for t in range(x.shape[1]):
            z = np.row_stack((h[:, t-1], x[:, t]))
            f = self.sigmoid(self.Wf @ z + self.bf)
            i = self.sigmoid(self.Wi @ z + self.bi)
            o = self.sigmoid(self.Wo @ z + self.bo)
            c_tilde = np.tanh(self.Wc @ z + self.bc)
            c[:, t] = f * c[:, t-1] + i * c_tilde
            h[:, t] = o * np.tanh(c[:, t])
            y[:, t] = self.Wy @ h[:, t] + self.by
        return y, h, c

    def backward(self, x, y, h, c, y_pred, learning_rate):
        dWf, dWi, dWo, dWc, dWy = np.zeros_like(self.Wf), np.zeros_like(self.Wi), np.zeros_like(self.Wo), np.zeros_like(self.Wc), np.zeros_like(self.Wy)
        dbf, dbi, dbo, dbc, dby = np.zeros_like(self.bf), np.zeros_like(self.bi), np.zeros_like(self.bo), np.zeros_like(self.bc), np.zeros_like(self.by)
        dh_next, dc_next = np.zeros_like(h[:, 0]), np.zeros_like(c[:, 0])
        loss = 0
        for t in reversed(range(x.shape[1])):
            dy = y_pred[:, t] - y[:, t]
            dWy += dy[:, None] @ h[:, t][None, :]
            dby += dy[:, None]
            dh = self.Wy.T @ dy + dh_next
            do = dh * np.tanh(c[:, t])
            do_raw = self.sigmoid_derivative(do)
            dbo += do_raw[:, None]
            dWo += do_raw[:, None] @ np.row_stack((h[:, t-1], x[:, t]))[None, :]
            dc = dh * do * (1 - np.tanh(c[:, t])**2) + dc_next
            di = dc * c[:, t-1]
            di_raw = self.sigmoid_derivative(di)
            dbi += di_raw[:, None]
            dWi += di_raw[:, None] @ np.row_stack((h[:, t-1], x[:, t]))[None, :]
            df = dc * c[:, t-1]
            df_raw = self.sigmoid_derivative(df)
            dbf += df_raw[:, None]
            dWf += df_raw[:, None] @ np.row_stack((h[:, t-1], x[:, t]))[None, :]
            dc_tilde = dc * i
            dc_tilde_raw = self.tanh_derivative(dc_tilde)
            dbc += dc_tilde_raw[:, None]
            dWc += dc_tilde_raw[:, None] @ np.row_stack((h[:, t-1], x[:, t]))[None, :]
            dh_next = self.Wf[:, :self.hidden_size].T @ df_raw + self.Wi[:, :self.hidden_size].T @ di_raw + self.Wo[:, :self.hidden_size].T @ do_raw + self.Wc[:, :self.hidden_size].T @ dc_tilde_raw
            dc_next = dc * f
            loss += np.sum(dy ** 2)
        self.Wf -= learning_rate * dWf
        self.Wi -= learning_rate * dWi
        self.Wo -= learning_rate * dWo
        self.Wc -= learning_rate * dWc
        self.Wy -= learning_rate * dWy
        self.bf -= learning_rate * dbf
        self.bi -= learning_rate * dbi
        self.bo -= learning_rate * dbo
        self.bc -= learning_rate * dbc
        self.by -= learning_rate * dby
        return loss / x.shape[1]

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def tanh_derivative(self, x):
        return 1 - np.tanh(x) ** 2

def train_lstm(train_data, input_size, hidden_size, output_size, epochs, learning_rate):
    lstm = LSTM(input_size, hidden_size, output_size)
    for epoch in range(epochs):
        total_loss = 0
        for (x, y) in train_data:
            y_pred, h, c = lstm.forward(x)
            loss = lstm.backward(x, y, h, c, y_pred, learning_rate)
            total_loss += loss
        print(f'Epoch {epoch + 1}, Loss: {total_loss / len(train_data)}')
    np.save('lstm_model.npy', [lstm.Wf, lstm.Wi, lstm.Wo, lstm.Wc, lstm.Wy, lstm.bf, lstm.bi, lstm.bo, lstm.bc, lstm.by])
    return lstm

# Example usage
# Assuming train_data is a list of (input_sequence, target_sequence) tuples
# input_size, hidden_size, output_size = 10, 20, 10
# epochs, learning_rate = 100, 0.01
# train_lstm(train_data, input_size, hidden_size, output_size, epochs, learning_rate)
