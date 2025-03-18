Nginx나 다른 웹 서버와 연동하지 않고 `certbot`만으로 인증서를 다운로드받으려면 **DNS-01 인증 방식** 또는 **HTTP-01 인증 방식**을 사용할 수 있습니다. 이 중 DNS-01 인증 방식은 Nginx와 같은 웹 서버 없이도 도메인 소유권을 인증할 수 있는 가장 독립적인 방법입니다.

아래는 `certbot`을 사용해 인증서만 다운로드받는 과정입니다.

---

### **1. DNS-01 인증 방식 (추천)**
1. **Certbot 설치**
   Certbot이 설치되어 있지 않다면 설치합니다.

   ```bash
   sudo apt update
   sudo apt install certbot
   ```

2. **DNS-01 인증 시작**
   `--manual` 옵션을 사용하여 Certbot이 DNS TXT 레코드를 생성하도록 합니다.

   ```bash
   sudo certbot certonly --manual --preferred-challenges dns -d example.com -d *.example.com
   ```

   여기서:
   - `--manual`: Certbot이 DNS 인증을 위한 명령어를 제공.
   - `--preferred-challenges dns`: DNS-01 인증 방식 사용.
   - `-d`: 인증서에 포함할 도메인(와일드카드 도메인 포함 가능).

3. **TXT 레코드 추가**
   명령을 실행하면 Certbot이 TXT 레코드를 생성하라는 메시지를 표시합니다. 예를 들어:

   ```plaintext
   Please deploy a DNS TXT record under the name:
   _acme-challenge.example.com
   with the following value:
   KJd8jfi8Jdks93jsdfl83jflks92kdlfd
   ```

   **DNS 설정에서 TXT 레코드 추가:**
   - `_acme-challenge.example.com`에 위 값을 추가.
   - TTL(Time to Live)은 최소화(예: 300초)로 설정.

4. **DNS 변경 사항 전파 기다리기**
   DNS 레코드가 전파되는 데 몇 분이 소요될 수 있습니다. 이후 Certbot이 진행되도록 터미널에서 Enter를 누릅니다.

5. **인증서 다운로드 완료**
   인증이 완료되면 인증서가 다음 경로에 저장됩니다:
   - 인증서: `/etc/letsencrypt/live/example.com/fullchain.pem`
   - 개인 키: `/etc/letsencrypt/live/example.com/privkey.pem`

---

### **2. HTTP-01 인증 방식**
이 방법은 Certbot이 생성한 파일을 도메인의 특정 경로에 배치하여 소유권을 확인하는 방식입니다.

#### **요구사항**
- 도메인에 대해 HTTP 요청을 처리할 수 있어야 함(웹 서버 없이).
- HTTP 요청을 처리하기 위해 `--standalone` 옵션을 사용.

1. **Certbot 실행**
   ```bash
   sudo certbot certonly --standalone -d example.com -d www.example.com
   ```

   여기서:
   - `--standalone`: Certbot이 자체 HTTP 서버를 실행.
   - `-d`: 인증할 도메인 이름.

2. **80 포트 사용 여부 확인**
   Certbot은 80 포트를 사용하여 Let's Encrypt 서버와 통신합니다. 80 포트가 이미 사용 중인 경우, 다른 서비스를 중지하거나 포트를 변경해야 합니다.

3. **인증 완료**
   인증이 완료되면 인증서는 다음 위치에 저장됩니다:
   - 인증서: `/etc/letsencrypt/live/example.com/fullchain.pem`
   - 개인 키: `/etc/letsencrypt/live/example.com/privkey.pem`

---

### **3. 인증서 갱신**
Certbot으로 발급받은 인증서는 기본적으로 90일 동안 유효합니다. 갱신하려면 다음 명령을 사용합니다.

#### **DNS-01 방식 갱신**
```bash
sudo certbot renew --manual --preferred-challenges dns
```

#### **HTTP-01 방식 갱신**
```bash
sudo certbot renew --standalone
```

---

### **4. 인증서 확인**
다운로드된 인증서가 제대로 생성되었는지 확인하려면:

```bash
sudo certbot certificates
```

---

### **5. 주의사항**
- DNS-01 인증 방식은 DNS 설정에 따라 전파 시간이 달라질 수 있습니다.
- HTTP-01 인증 방식은 80 포트를 사용해야 하며, 해당 포트에 접근할 수 없는 경우 실패합니다.
- 다운로드된 인증서를 수동으로 웹 서버 또는 애플리케이션에 연결해야 합니다.

---

### **결론**
Nginx 등과 연동하지 않고도 DNS-01 또는 HTTP-01 인증 방식을 사용하여 Certbot으로 인증서를 독립적으로 다운로드받을 수 있습니다. 와일드카드 인증서가 필요한 경우 DNS-01 방식을 사용하는 것을 추천합니다.
