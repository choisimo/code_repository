### **스크립트 설명**

이 스크립트는 Nginx에서 각 도메인별로 `.well-known/acme-challenge` 경로를 설정하고, Certbot을 사용해 SSL 인증서를 발급하는 과정을 자동화한 것입니다.

---

### **1. 주요 변수 및 초기 작업**
- **`NGINX_CONF_DIR`**:
  - Nginx의 개별 도메인 설정 파일이 저장된 디렉토리 경로입니다.
  - 기본값은 `/data/nginx/proxy-host`.

- **`ACME_CHALLENGE_PATH`**:
  - Certbot 인증 파일을 저장할 디렉토리 경로입니다.
  - 기본값은 `/var/www/.well-known/acme-challenge`.

- **디렉토리 생성**:
  ```bash
  mkdir -p "$ACME_CHALLENGE_PATH"
  chmod -R 755 /var/www
  ```
  `.well-known/acme-challenge` 경로를 생성하고, 적절한 권한을 부여합니다.

---

### **2. Nginx 설정 파일 순회**

#### **(1) Nginx 설정 파일 처리**
```bash
for CONF_FILE in "$NGINX_CONF_DIR"/*.conf; do
    DOMAIN=$(basename "$CONF_FILE" .conf)
```
- Nginx 설정 디렉토리 내의 `.conf` 파일을 순회하며, 설정 파일 이름에서 도메인을 추출.
  - 예: `example.com.conf` → 도메인: `example.com`.

#### **(2) `.well-known/acme-challenge` 경로 블록 추가**
```bash
if ! grep -q ".well-known/acme-challenge" "$CONF_FILE"; then
    cat <<EOL >>"$CONF_FILE"

# Added for Certbot validation
location /.well-known/acme-challenge/ {
    alias $ACME_CHALLENGE_PATH/;
    allow all;
}
EOL
```
- 설정 파일에 `.well-known/acme-challenge` 경로 블록이 없으면 추가합니다.
  - `alias $ACME_CHALLENGE_PATH`: 인증 파일 경로를 Nginx 설정에 맵핑.
  - `allow all`: 누구나 접근 가능하도록 허용.

#### **(3) Nginx 설정 테스트 및 재시작**
```bash
nginx -t && systemctl reload nginx
```
- Nginx 설정을 테스트한 후, 문제가 없으면 설정을 재시작.

---

### **3. Certbot SSL 인증서 발급**

#### **Certbot 명령어**
```bash
certbot certonly --webroot -w /var/www -d "$DOMAIN" --non-interactive --agree-tos -m your-email@example.com
```

**옵션 설명**:
- `certonly`: Certbot이 인증서만 발급하고 자동으로 Nginx와 연동하지 않음.
- `--webroot -w /var/www`: 인증 경로로 `/var/www`를 사용.
- `-d "$DOMAIN"`: 인증서를 발급받을 도메인 지정.
- `--non-interactive`: 사용자 입력 없이 실행.
- `--agree-tos`: 이용 약관에 자동 동의.
- `-m your-email@example.com`: Certbot에 등록할 이메일 주소.

#### **성공/실패 메시지 출력**
- 성공: `SSL certificate successfully issued for $DOMAIN`
- 실패: `Failed to issue SSL certificate for $DOMAIN`

---

### **4. 수동 설정 및 실행**

#### **1) Nginx 경로 맵핑**
도메인별로 `.well-known/acme-challenge` 경로를 설정하려면 Nginx 설정 파일을 수동으로 수정해야 합니다.

**예제 (Nginx 설정)**:
```nginx
server {
    listen 80;
    server_name example.com;

    location /.well-known/acme-challenge/ {
        alias /var/www/.well-known/acme-challenge/;
        allow all;
    }

    location / {
        proxy_pass http://127.0.0.1:3000; # 도메인 서비스 경로
    }
}
```

#### **설정 저장 후 Nginx 재시작**
```bash
sudo nginx -t
sudo systemctl reload nginx
```

---

#### **2) Certbot 명령 실행**
Nginx 경로 설정이 완료되면, Certbot을 사용해 인증서를 수동으로 발급받습니다:
>  /var/www/html/.well-known/acme-challenge

```bash
sudo certbot certonly --webroot -w /var/www -d sub.example.com --non-interactive --agree-tos -m nodove@example.com -v
```

#### **발급 확인**
발급된 인증서를 확인하려면:
```bash
sudo certbot certificates
```

---

### **5. 주요 파일 경로**
Certbot이 발급한 인증서는 다음 경로에 저장됩니다:
- **인증서**: `/etc/letsencrypt/live/<도메인>/fullchain.pem`
- **개인 키**: `/etc/letsencrypt/live/<도메인>/privkey.pem`

---

### **6. Nginx에 인증서 적용**
SSL 인증서를 적용하려면, Nginx 설정 파일을 아래처럼 수정합니다:

```nginx
server {
    listen 443 ssl;
    server_name example.com;

    ssl_certificate /etc/letsencrypt/live/example.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/example.com/privkey.pem;

    location / {
        proxy_pass http://127.0.0.1:3000;
    }
}
```

설정을 저장한 후, Nginx를 다시 로드합니다:
```bash
sudo nginx -t
sudo systemctl reload nginx
```

---

### **7. 인증서 갱신**
Certbot 인증서를 갱신하려면:
```bash
sudo certbot renew
```

자동으로 갱신되도록 설정하려면 다음 명령을 크론탭에 추가합니다:
```bash
crontab -e
```

```plaintext
0 3 * * * certbot renew --quiet && systemctl reload nginx
```

---

### **요약**
1. `.well-known/acme-challenge` 경로를 설정.
2. Nginx에 인증 경로를 매핑.
3. Certbot으로 인증서를 발급.
4. 발급된 인증서를 Nginx에 적용.
5. 자동 갱신을 설정하여 인증서를 유지 관리.

스크립트를 기반으로 수동으로도 쉽게 인증서를 발급할 수 있습니다! 추가 질문이 있으면 알려주세요. 😊
