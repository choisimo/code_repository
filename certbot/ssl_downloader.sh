#!/bin/bash

# Nginx conf 디렉토리 경로
NGINX_CONF_DIR="/data/nginx/proxy-host"
 
# sudo mkdir -p /var/www/.well-known/acme-challenge

# Certbot의 인증 경로
ACME_CHALLENGE_PATH="/var/www/.well-known/acme-challenge"

# 1. .well-known/acme-challenge 디렉토리 생성
mkdir -p "$ACME_CHALLENGE_PATH"
chmod -R 755 /var/www

# 2. Nginx conf 파일 순회
for CONF_FILE in "$NGINX_CONF_DIR"/*.conf; do
    # 파일 이름에서 도메인 추출
    DOMAIN=$(basename "$CONF_FILE" .conf)
    echo "Processing domain: $DOMAIN"

    # location 블록이 이미 존재하는지 확인
    if ! grep -q ".well-known/acme-challenge" "$CONF_FILE"; then
        echo "Adding location block for $DOMAIN"

        # location 블록 추가
        cat <<EOL >>"$CONF_FILE"

# Added for Certbot validation
location /.well-known/acme-challenge/ {
    alias $ACME_CHALLENGE_PATH/;
    allow all;
}
EOL
    else
        echo "Location block already exists for $DOMAIN"
    fi

    # Nginx 설정 테스트 및 재시작
    nginx -t && systemctl reload nginx
    if [ $? -ne 0 ]; then
        echo "Nginx configuration test failed for $DOMAIN. Skipping Certbot."
        continue
    fi

    # 3. Certbot을 사용하여 SSL 인증서 발급
    certbot certonly --webroot -w /var/www -d "$DOMAIN" --non-interactive --agree-tos -m your-email@example.com
    if [ $? -eq 0 ]; then
        echo "SSL certificate successfully issued for $DOMAIN"
    else
        echo "Failed to issue SSL certificate for $DOMAIN"
    fi
done

echo "Script execution completed."
