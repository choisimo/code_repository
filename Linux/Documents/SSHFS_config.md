# SFTP를 통한 원격 서버 디렉토리 영구 마운트 

SFTP(SSH File Transfer Protocol)는 SSH를 기반으로 하는 안전한 파일 전송 프로토콜입니다. SSHFS를 사용하면 원격 서버의 디렉토리를 로컬 시스템에 마운트하여 마치 로컬 디렉토리처럼 사용할 수 있습니다.

## SSHFS 설치하기

```
sudo apt update
sudo apt install sshfs
```

Fedora와 같은 다른 배포판에서는 `dnf install fuse-sshfs`를 사용합니다

## 기본 마운트 방법

1. 로컬 시스템에 마운트 포인트 생성:
```
sudo mkdir /mnt/remote_dir
```

2. 원격 디렉토리 마운트:
```
sudo sshfs -o allow_other,default_permissions 사용자명@서버IP:/원격/경로 /mnt/remote_dir
```

3. 마운트 해제:
```
sudo umount /mnt/remote_dir
```
또는
```
sudo fusermount -u /mnt/remote_dir
```

## 영구 마운트 설정하기

시스템 재부팅 후에도 자동으로 마운트되도록 하려면 `/etc/fstab` 파일을 수정해야 합니다

1. `/etc/fstab` 파일 열기:
```
sudo nano /etc/fstab
```

2. 파일 끝에 다음 내용 추가:
```
사용자명@서버IP:/원격/경로 /mnt/remote_dir fuse.sshfs noauto,x-systemd.automount,_netdev,reconnect,identityfile=/home/로컬사용자명/.ssh/id_rsa,allow_other,default_permissions 0 0
```

### 중요 옵션 설명

- `fuse.sshfs`: 파일시스템 유형 지정
- `noauto,x-systemd.automount,_netdev,reconnect`: 네트워크 연결이 끊어졌을 때 자동으로 재연결 시도
- `identityfile=/home/로컬사용자명/.ssh/id_rsa`: SSH 키 파일 위치 지정
- `allow_other`: 다른 사용자도 마운트된 디렉토리 접근 허용
- `default_permissions`: 원격 서버의 파일 권한 유지
- `0 0`: 덤프와 파일시스템 점검 비활성화

## SSH 키 기반 인증 설정

비밀번호 없이 자동 마운트하려면 SSH 키 기반 인증을 설정해야 합니다

1. SSH 키 생성:
```
ssh-keygen -t rsa
```

2. 공개 키를 원격 서버에 복사:
```
ssh-copy-id -i ~/.ssh/id_rsa.pub 사용자명@서버IP
```

## AutoFS를 통한 자동 마운트 설정

AutoFS를 사용하면 필요할 때만 자동으로 마운트되도록 설정할 수 있습니다

1. AutoFS 설치:
```
sudo apt install autofs
```

2. `/etc/auto.master` 파일 수정:
```
/- /etc/auto.sshfs --timeout=30
```

3. `/etc/auto.sshfs` 파일 생성 및 설정:
```
/로컬/마운트경로 -fstype=fuse,allow_other,IdentityFile=/로컬/개인키경로 :sshfs\#사용자@원격호스트\:/원격/경로
```

4. AutoFS 서비스 시작:
```
sudo service autofs restart
```

### 중요

1. `/etc/fstab`에서 사용할 경우 부팅 시 네트워크 연결 전에 마운트를 시도할 수 있으므로 `_netdev`, `noauto`, `x-systemd.automount` 옵션이 중요합니다

2. 성능 측면에서 SSHFS는 모든 데이터가 암호화/복호화되므로 NFS보다 약간 느릴 수 있습니다

3. 보안을 위해 root 계정보다는 일반 사용자 계정으로 SSHFS를 사용하는 것이 좋습니다

4. 시스템 시작 시 자동 마운트가 실패할 경우 root crontab에 `@reboot` 명령을 추가하는 방법도 있습니다
```
@reboot sleep 60 && sshfs 사용자명@서버IP:/원격/경로 /로컬/마운트지점
```

이 방법으로 SFTP를 통해 원격 서버의 디렉토리를 로컬 파일 시스템에 영구적으로 마운트하여 편리하게 사용할 수 있습니다.
