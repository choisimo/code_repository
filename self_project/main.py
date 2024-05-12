import tkinter as tk
from tkinter import messagebox
import requests


def track_package():
    tracking_number = entry.get()
    url = "https://api.somecourier.com/track"
    api_key = "your_api_key_here"
    params = {
        "tracking_number": tracking_number,
        "api_key": api_key
    }

    response = requests.get(url, params=params)
    if response.status_code == 200:
        data = response.json()
        # 가상의 데이터 필드, 실제 API 응답에 따라 수정이 필요할 수 있습니다.
        status = data.get("status", "No status found.")
        messagebox.showinfo("Package Status", f"Status: {status}")
    else:
        messagebox.showerror("Error", f"Failed to retrieve data: {response.status_code}")


# 메인 윈도우 설정
root = tk.Tk()
root.title("Package Tracking System")

# 레이블 설정
label = tk.Label(root, text="Enter Tracking Number:")
label.pack(pady=10)

# 입력 필드 설정
entry = tk.Entry(root, width=50)
entry.pack(pady=5)

# 조회 버튼 설정
button = tk.Button(root, text="Track Package", command=track_package)
button.pack(pady=20)

# 윈도우 실행
root.mainloop()
