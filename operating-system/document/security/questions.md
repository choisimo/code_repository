# 7번 주제: 보안 및 보호 문제

## 문제 1
운영체제에서 보호(protection)와 보안(security)의 차이점을 설명하고, 사용자 식별 메커니즘이 이들을 구현하는 데 어떻게 사용되는지 설명하시오.

**답변:**
보호(Protection)는 시스템 자원에 대한 프로세스나 사용자의 접근을 제어하는 메커니즘으로, 컴퓨터 시스템이 정의한 자원에 대한 접근 규제를 의미합니다. 반면 보안(Security)은 외부 및 내부 공격으로부터 시스템을 방어하는 것으로, 바이러스, 웜, 서비스 거부 공격, 신원 도용 등의 위협으로부터 시스템을 보호하는 활동입니다.

사용자 식별 메커니즘:
- 운영체제는 사용자 이름과 사용자 식별자(UID) 목록을 유지
- Windows에서는 보안 ID(SID)라고 함
- 로그인 시 인증 과정을 통해 적절한 사용자 ID 결정
- 해당 ID는 사용자의 모든 프로세스 및 스레드와 연결됨
- 그룹 ID를 통해 사용자 집합에 대한 권한 관리
- 권한 상승(privilege escalation) 메커니즘 제공

**해설:**
보호와 보안은 밀접하게 관련되어 있지만 서로 다른 개념입니다. 보호는 주로 시스템 내부의 정의된 규칙에 따라 자원 접근을 제어하는 반면, 보안은 악의적인 외부 공격으로부터 시스템을 방어하는 데 중점을 둡니다.

사용자 식별은 이러한 보호와 보안의 기본 요소입니다. 운영체제는 각 사용자에게 고유한 식별자를 할당하고, 이를 통해 자원 접근 권한을 관리합니다. UNIX 시스템에서 setuid 속성과 같은 메커니즘은 일시적 권한 상승을 통해 제한된 작업을 수행할 수 있게 합니다. 또한 그룹 기능을 통해 사용자 집합에 대한 접근 권한을 효율적으로 관리할 수 있습니다.

보호 메커니즘은 인터페이스 오류를 조기에 감지함으로써 시스템 신뢰성을 향상시키고, 권한이 없는 사용자로부터 시스템 자원을 보호합니다. 현대 운영체제에서는 이러한 보호와 보안 기능이 점점 더 중요해지고 있으며, 다양한 정책과 메커니즘을 통해 구현되고 있습니다.

## 문제 2
파일 시스템 접근 제어 방식의 종류와 각 방식의 장단점을 설명하시오.

**답변:**
파일 시스템 접근 제어 방식:

1. 접근 제어 행렬(Access Control Matrix):
   - 구현: 행은 주체(사용자), 열은 객체(파일)를 나타내는 2차원 행렬
   - 장점: 가장 일반적이고 유연한 접근 제어 표현 가능
   - 단점: 대규모 시스템에서 희소 행렬로 인한 공간 낭비

2. 접근 제어 목록(Access Control Lists, ACL):
   - 구현: 각 파일/객체마다 허용된 사용자와 권한 목록 유지
   - 장점: 세밀한 접근 제어, 객체별 권한 관리 용이
   - 단점: 특정 사용자의 모든 권한 확인이 어려움, 관리 복잡성

3. 기능 기반 접근 제어(Capability-based Access Control):
   - 구현: 각 프로세스/사용자가 접근 가능한 객체와 권한 목록(토큰) 보유
   - 장점: 권한 위임 용이, 최소 권한 원칙 적용 쉬움
   - 단점: 권한 취소 어려움, 시스템 전체 접근 제어 상태 파악 어려움

4. 역할 기반 접근 제어(Role-Based Access Control, RBAC):
   - 구현: 사용자에게 역할 할당, 역할에 권한 부여
   - 장점: 관리 단순화, 사용자 변경에 따른 권한 변경 최소화
   - 단점: 적절한 역할 정의 어려움, 예외 처리 복잡

**해설:**
파일 시스템 접근 제어는 다중 사용자 환경에서 정보 보호의 핵심입니다. UNIX/Linux의 rwx 권한 비트는 단순하지만 효과적인 접근 제어 방식으로, 소유자, 그룹, 기타 사용자별로 읽기, 쓰기, 실행 권한을 설정합니다. 이는 접근 제어 행렬을 단순화한 형태로 볼 수 있습니다.

접근 제어 목록(ACL)은 UNIX 기본 권한 체계를 확장하여 더 세밀한 제어를 제공합니다. Windows NTFS와 최신 Linux 파일 시스템은 ACL을 지원하여 개별 사용자나 그룹에 대한 복잡한 권한 구성이 가능합니다.

기능 기반 접근 제어는 ACL과 철학적으로 반대 방향의 접근법으로, 객체가 아닌 주체(사용자/프로세스) 중심으로 권한을 관리합니다. 이는 "열쇠 꾸러미" 개념과 유사하며, 권한 위임이 용이하지만 취소가 어렵다는 단점이 있습니다.

역할 기반 접근 제어는 기업 환경에서 널리 사용되는 방식으로, 직무에 따른 접근 권한을 효율적으로 관리할 수 있게 합니다. 사용자는 하나 이상의 역할에 할당되고, 접근 권한은 역할에 연결됩니다. 이는 사용자 변경 시 권한 재구성의 필요성을 줄여줍니다.

현대 시스템은 이러한 모델들을 조합하여 사용하는 경우가 많으며, 속성 기반 접근 제어(ABAC) 같은 더 발전된 모델도 등장하고 있습니다.

## 문제 3
운영체제에서 인증(Authentication)과 인가(Authorization)의 차이점을 설명하고, 다중 요소 인증의 필요성과 구현 방식에 대해 논하시오.

**답변:**
인증(Authentication)과 인가(Authorization)의 차이점:

**인증(Authentication)**:
- 정의: 사용자가 주장하는 신원을 검증하는 과정
- 목적: "당신이 누구인지" 확인
- 방법: 패스워드, 생체 인식, 토큰, 인증서 등
- 시점: 시스템 접근 시도 시 최초 수행

**인가(Authorization)**:
- 정의: 인증된 사용자에게 특정 자원에 대한 접근 권한을 부여하는 과정
- 목적: "무엇을 할 수 있는지" 결정
- 방법: ACL, 역할, 정책, 권한 등
- 시점: 인증 이후, 자원 접근 시도마다 수행

다중 요소 인증(MFA)의 필요성:

1. 단일 요소 인증의 한계:
   - 패스워드 도난, 추측, 사회공학적 공격 취약성
   - 한 가지 방어선이 뚫리면 전체 시스템 손상

2. 다중 방어선 구축:
   - 여러 독립적 인증 방법 조합으로 보안 강화
   - 하나의 요소가 손상되어도 다른 요소가 방어 유지

3. 규제 및 컴플라이언스 요구사항:
   - 많은 산업 표준과 규제가 중요 시스템에 MFA 요구
   - 데이터 보호 법규 준수를 위한 필수 요소

다중 요소 인증 구현 방식:

1. 인증 요소 분류:
   - 지식 기반: 알고 있는 것(패스워드, PIN, 보안 질문)
   - 소유 기반: 가지고 있는 것(스마트카드, OTP 토큰, 휴대폰)
   - 속성 기반: 본인의 특성(지문, 홍채, 얼굴, 음성)
   - 행동 기반: 행동 패턴(키스트로크 다이내믹스, 서명 패턴)

2. 일반적인 구현 방식:
   - TOTP(Time-based One-Time Password): 시간 기반 일회용 암호
   - 푸시 알림: 등록된 모바일 장치로 승인 요청 전송
   - SMS/이메일 코드: 별도 채널로 인증 코드 전송
   - 생체인식: 지문, 얼굴 인식 등과 패스워드 조합
   - 하드웨어 토큰: USB 보안 키 등 물리적 장치

3. 구현 시 고려사항:
   - 사용자 경험: 보안과 편의성 균형
   - 대체 인증 방법: 기본 방법 사용 불가 시 대안
   - 위험 기반 접근: 상황별 인증 강도 조정
   - 확장성: 사용자 증가에 따른 시스템 성능

**해설:**
인증과 인가는 보안 시스템의 두 가지 핵심 기능입니다. 인증은 사용자가 본인이라고 주장하는 신원을 검증하는 과정이며, 인가는 해당 사용자가 특정 자원에 접근할 권한이 있는지 확인하는 과정입니다. 이 두 과정은 순차적으로 발생하며, 인증 없이 인가가 이루어질 수 없습니다.

다중 요소 인증은 "알고 있는 것", "가지고 있는 것", "본인의 특성" 중 두 가지 이상의 범주에서 인증 요소를 조합함으로써 보안을 강화합니다. 예를 들어, 패스워드(알고 있는 것)와 OTP 토큰(가지고 있는 것)을 함께 사용하는 방식입니다. 이는 한 가지 요소가 손상되더라도 다른 요소가 방어선을 유지하기 때문에 보안성이 크게 향상됩니다.

최근 클라우드 서비스, 금융 시스템, 기업 네트워크 등에서 MFA 도입이 증가하고 있으며, 특히 원격 근무 환경에서 그 중요성이 더욱 강조되고 있습니다. 구현 시에는 보안 강화와 함께 사용자 경험을 고려하여 불필요한 마찰을 최소화하는 것이 중요합니다. 리스크 기반 인증(RBA)과 같은 접근법은 상황과 위험도에 따라 인증 강도를 조정함으로써 보안과 편의성의 균형을 맞추고 있습니다.

## 문제 4
컴퓨터 시스템에서 발생할 수 있는 주요 보안 위협의 유형과 이에 대응하기 위한 운영체제 수준의 보호 메커니즘을 설명하시오.

**답변:**
주요 보안 위협 유형:

1. 악성 소프트웨어:
   - 바이러스: 다른 프로그램에 붙어 전파
   - 웜: 자체 전파 능력 보유, 네트워크 통해 확산
   - 트로이 목마: 정상 프로그램으로 위장
   - 랜섬웨어: 데이터 암호화 후 몸값 요구
   - 루트킷: 시스템 깊숙이 숨어 권한 획득

2. 권한 상승 공격:
   - 버퍼 오버플로우: 메모리 경계 침범으로 코드 실행
   - 레이스 컨디션: 시간차 공격으로 권한 획득
   - 취약점 악용: 소프트웨어 결함 이용

3. 네트워크 기반 공격:
   - 서비스 거부(DoS/DDoS): 자원 고갈로 서비스 마비
   - 스니핑: 네트워크 트래픽 도청
   - 스푸핑: 다른 엔티티로 위장

4. 사회공학적 공격:
   - 피싱: 신뢰할 수 있는 기관으로 위장
   - 스피어 피싱: 특정 대상 겨냥한 맞춤형 피싱
   - 프리텍스팅: 거짓 시나리오로 정보 획득

운영체제 수준의 보호 메커니즘:

1. 메모리 보호:
   - 주소 공간 격리: 프로세스별 독립적 가상 메모리
   - DEP(Data Execution Prevention): 데이터 영역 코드 실행 방지
   - ASLR(Address Space Layout Randomization): 메모리 레이아웃 무작위화
   - 스택 보호: 스택 가드, 카나리 값 활용

2. 접근 제어:
   - 최소 권한 원칙: 필요 최소한의 권한만 부여
   - 강제적 접근 제어(MAC): 시스템 정책 기반 접근 제어
   - 임의적 접근 제어(DAC): 소유자 기반 접근 제어
   - 역할 기반 접근 제어(RBAC): 역할에 따른 권한 부여

3. 인증 및 인가:
   - 강력한 인증 메커니즘: 다중 요소 인증
   - 세션 관리: 안전한 세션 생성 및 만료
   - 권한 검증: 모든 작업 전 권한 확인

4. 시스템 무결성:
   - 보안 부팅(Secure Boot): 부팅 과정 무결성 검증
   - 코드 서명: 실행 파일 디지털 서명 확인
   - 무결성 측정: 시스템 파일 변조 감지

5. 감사 및 로깅:
   - 보안 이벤트 로깅: 중요 이벤트 기록
   - 이상 행동 탐지: 비정상 패턴 식별
   - 감사 추적: 모든 중요 작업 추적 기록

6. 샌드박싱:
   - 애플리케이션 격리: 제한된 환경에서 프로그램 실행
   - 컨테이너화: 애플리케이션 및 의존성 격리
   - 권한 분리: 기능별 분리된 권한 부여

**해설:**
컴퓨터 시스템에 대한 보안 위협은 지속적으로 진화하고 있으며, 운영체제는 이러한 위협에 대응하기 위한 다층적 방어 체계를 구축해야 합니다. 기본적인 방어 메커니즘은 이중 모드 운영(커널/사용자 모드)과 메모리 보호에서 시작합니다.

현대 운영체제는 ASLR과 DEP와 같은 기술을 통해 메모리 기반 공격을 방어합니다. ASLR은 프로세스의 메모리 구조를 무작위화하여 공격자가 특정 메모리 위치를 예측하기 어렵게 만들고, DEP는 데이터 영역에서 코드가 실행되는 것을 방지합니다.

접근 제어는 사용자와 프로세스가 접근할 수 있는 자원을 제한합니다. Linux의 SELinux와 AppArmor, Windows의 UAC(User Account Control)와 같은 메커니즘은 강력한 접근 제어를 구현하여 권한 상승 공격을 방지합니다.

샌드박싱은 애플리케이션을 격리된 환경에서 실행하여 잠재적 피해를 제한합니다. 모바일 OS와 브라우저에서 널리 사용되는 이 접근법은 악성 코드가 중요 시스템 리소스에 접근하는 것을 방지합니다.

감사 및 로깅은 보안 사고 발생 시 탐지 및 대응을 위한 중요한 도구입니다. 운영체제는 로그인 시도, 권한 변경, 중요 파일 접근 등의 이벤트를 기록하여 이상 행동을 식별하고 사후 분석을 지원합니다.

이러한 다중 방어 접근법은 어떤 단일 방어책도 완벽하지 않다는 인식하에, 여러 보호 계층을 통해 전체적인 시스템 보안을 강화합니다.

## 문제 5
권한 상승(Privilege Escalation) 공격의 유형과 이를 방지하기 위한 운영체제 수준의 대응 방안을 설명하시오.

**답변:**
권한 상승 공격의 유형:

1. 수직적 권한 상승(Vertical Privilege Escalation):
   - 일반 사용자에서 관리자/루트 권한으로 상승
   - 커널 취약점 악용
   - setuid/setgid 프로그램 취약점 이용
   - 버퍼 오버플로우, 포맷 스트링 공격 등

2. 수평적 권한 상승(Horizontal Privilege Escalation):
   - 동일 권한 수준의 다른 사용자 계정 접근
   - 세션 하이재킹
   - 접근 제어 누락 악용
   - 잘못 구성된 권한 설정 악용

3. 주요 공격 벡터:
   - 메모리 손상 취약점(버퍼 오버플로우, 힙 오버플로우)
   - 경쟁 조건(Race Condition) 취약점
   - 구성 오류 및 기본 설정 취약점
   - 패스워드 및 인증 관련 취약점
   - 잘못된, 액세스 제어 구현

운영체제 수준의 대응 방안:

1. 메모리 보호 기법:
   - 데이터 실행 방지(DEP/NX bit): 데이터 영역 코드 실행 방지
   - 주소 공간 배치 무작위화(ASLR): 메모리 레이아웃 예측 어렵게 함
   - 스택 보호(Stack Canaries): 스택 오버플로우 탐지
   - 포인터 암호화(Pointer Encryption): 중요 메모리 주소 보호

2. 권한 관리 강화:
   - 최소 권한 원칙 적용: 필요 최소한의 권한만 부여
   - 특권 분리: 관리 작업 분리 및 권한 세분화
   - setuid/setgid 프로그램 최소화 및 주기적 검토
   - 임시 권한 상승(sudo)과 감사 로깅 결합

3. 격리 기술:
   - 샌드박싱(Sandboxing): 제한된 환경에서 애플리케이션 실행
   - 컨테이너화: 애플리케이션과 의존성 격리
   - 가상화: 완전한 시스템 격리 제공
   - 보안 컴퓨팅 모드(seccomp): 시스템 콜 제한

4. 커널 보호:
   - 커널 패치 관리: 보안 패치 신속 적용
   - SMEP/SMAP: 커널이 사용자 공간 코드 실행/접근 방지
   - 커널 무결성 보호: 서명된 커널 모듈만 로드
   - kptr_restrict: 커널 포인터 노출 제한

5. 보안 모니터링:
   - 이상 행동 탐지: 비정상적인 권한 사용 모니터링
   - 통합 감사 로깅: 모든 권한 변경 이벤트 기록
   - 무결성 검증: 중요 시스템 파일 변경 감지
   - 실시간 알림: 의심스러운 활동 즉시 알림

**해설:**
권한 상승 공격은 공격자가 시스템에서 초기에 부여받은 것보다 더 높은 권한을 획득하려는 시도입니다. 수직적 권한 상승은 일반 사용자에서 관리자 권한으로의 상승을, 수평적 권한 상승은 동일 권한 수준의 다른 사용자 계정으로의 접근을 의미합니다.

이러한 공격에 대응하기 위해 현대 운영체제는 여러 보호 계층을 구현합니다. DEP와 ASLR은 코드 실행 공격을 어렵게 만들며, 특히 버퍼 오버플로우와 같은 전통적인 공격 방법의 효과를 크게 감소시킵니다. Windows의 UAC(User Account Control)나 Linux의 sudo와 같은 메커니즘은 관리자 권한 사용을 명시적 승인 과정을 통해 제한합니다.

최소 권한 원칙은 모든 프로세스와 사용자가 기능 수행에 필요한 최소한의 권한만 가지도록 하는 핵심 보안 원칙입니다. 이를 통해 공격이 성공하더라도 피해 범위를 제한할 수 있습니다. 컨테이너화와 가상화 같은 격리 기술은 이를 더욱 강화하여, 공격자가 한 시스템을 손상시키더라도 다른 시스템으로의 확산을 방지합니다.

커널 수준 보호는 특히 중요합니다. SMEP(Supervisor Mode Execution Prevention)와 SMAP(Supervisor Mode Access Prevention)은 Intel CPU의 보안 기능으로, 커널이 사용자 공간 코드를 실행하거나 데이터에 부주의하게 접근하는 것을 방지합니다. 또한 서명된 커널 모듈만 로드하도록 하여 악성 커널 확장을 방지할 수 있습니다.

마지막으로, 지속적인 모니터링과 감사는 공격 시도를 조기에 탐지하고 대응하는 데 필수적입니다. 이상 행동 탐지 시스템은 비정상적인 권한 사용 패턴을 식별하여 잠재적 권한 상승 공격을 알릴 수 있습니다.

## 문제 6
보안 강화 운영체제(Security-Enhanced OS)의 개념과 주요 구현 사례(SELinux, AppArmor 등)를 비교 설명하고, 이들이 제공하는 보안 기능의 장단점을 논하시오.

**답변:**
보안 강화 운영체제의 개념:

보안 강화 운영체제는 기존 운영체제에 추가적인 보안 제어 메커니즘을 통합하여 보안 정책 시행, 권한 분리, 데이터 보호 기능을 강화한 운영체제입니다. 주로 강제적 접근 제어(MAC, Mandatory Access Control) 모델을 기반으로 하며, 전통적인 임의적 접근 제어(DAC, Discretionary Access Control)를 보완합니다.

주요 구현 사례 비교:

1. SELinux(Security-Enhanced Linux):
   - 개발: NSA와 오픈소스 커뮤니티
   - 접근 방식: 타입 강제(Type Enforcement) 기반 MAC
   - 정책 모델: 세밀한 유형 기반 정책, 역할 기반 접근 제어(RBAC) 지원
   - 구현: 리눅스 커널에 통합된 LSM(Linux Security Module)
   - 주요 기능: 프로세스 도메인 분리, 세밀한 파일 라벨링, 다중 범주 보안

2. AppArmor:
   - 개발: Canonical(Ubuntu), SUSE
   - 접근 방식: 경로 기반 MAC
   - 정책 모델: 애플리케이션 중심 프로파일
   - 구현: 리눅스 커널의 LSM 프레임워크 활용
   - 주요 기능: 파일 경로 기반 제어, 간소화된 프로파일 문법

3. Grsecurity/PaX:
   - 개발: Open Source Security, Inc.
   - 접근 방식: 커널 수준 강화, RBAC 시스템
   - 정책 모델: 역할 기반, 학습 모드 지원
   - 구현: 커널 패치 세트
   - 주요 기능: 메모리 보호 강화, 정보 노출 제한, 커널 공격 표면 감소

4. Windows 장치 보호(Device Guard):
   - 개발: Microsoft
   - 접근 방식: 코드 무결성 강제, 가상화 기반 보안
   - 정책 모델: 애플리케이션 허용 목록, 신뢰 기반
   - 구현: Windows 운영체제 내장 기능
   - 주요 기능: 신뢰할 수 있는 애플리케이션만 실행 허용, 하드웨어 기반 격리

장단점 분석:

SELinux:
- 장점: 세밀한 접근 제어, 강력한 격리, 광범위한 정책 옵션
- 단점: 복잡한 설정과 학습 곡선, 잘못된 구성 시 시스템 기능 장애

AppArmor:
- 장점: 직관적인 경로 기반 정책, 간편한 설정, 낮은 성능 오버헤드
- 단점: SELinux보다 덜 세밀한 제어, 하드 링크/심볼릭 링크 우회 가능성

Grsecurity/PaX:
- 장점: 포괄적인 커널 보호, 제로데이 공격 방어, 메모리 보호 강화
- 단점: 일부 버전 유료화, 호환성 문제 가능성, 커널 업데이트에 따른 유지보수

Windows 장치 보호:
- 장점: 윈도우 환경 통합, 하드웨어 가상화 활용, 관리 도구 제공
- 단점: 하드웨어 요구사항, 일부 레거시 애플리케이션 호환성 문제

**해설:**
보안 강화 운영체제는 기본 보안 모델을 넘어선 보호 기능을 제공하여 악성 코드나 권한 상승 공격으로부터 시스템을 보호합니다. 이들은 주로 강제적 접근 제어(MAC) 원칙을 구현하여, 시스템 정책이 모든 주체(사용자, 프로세스)와 객체(파일, 소켓 등) 간의 상호작용을 제어합니다.

SELinux는 가장 포괄적인 접근 방식을 제공하며, 타입 강제 모델을 통해 세밀한 보안 정책을 정의할 수 있습니다. 각 프로세스와 객체에 보안 컨텍스트가 할당되며, 이 컨텍스트에 기반하여 접근 결정이 이루어집니다. 그러나 이러한 세밀함은 복잡성을 수반하며, SELinux의 학습 곡선은 상당히 가파릅니다.

AppArmor는 더 직관적인 경로 기반 접근 방식을 채택하여 특정 애플리케이션이 접근할 수 있는 파일과 기능을 제한합니다. 상대적으로 설정이 간단하고 성능 오버헤드가 적지만, SELinux만큼 세밀한 제어를 제공하지는 않습니다.

Grsecurity는 커널 수준에서 다양한 보안 강화 기능을 제공하며, PaX와 함께 메모리 보호 기능을 크게 향상시킵니다. 이는 제로데이 취약점에 대한 보호에 특히 효과적이지만, 최근 일부 기능이 유료화되어 접근성이 제한되었습니다.

Windows 환경에서는 Device Guard와 같은 기능이 코드 무결성 검사와 가상화 기반 보안을 통해 유사한 보호를 제공합니다. 이는 Windows 생태계와 잘 통합되어 있지만, 하드웨어 요구사항과 호환성 고려사항이 있습니다.

실제 환경에서는 보안 요구사항, 관리 능력, 성능 고려사항 등을 종합적으로 평가하여 적절한 솔루션을 선택해야 합니다. 많은 기업 환경에서는 심층 방어(Defense in Depth) 전략의 일환으로 이러한 보안 강화 기술을 다른 보안 제어와 함께 사용합니다.

## 문제 7
커널 수준 보안 기능과 사용자 공간 보안 기능의 차이점을 설명하고, 운영체제 보안에서 샌드박싱(Sandboxing) 기술의 역할과 구현 방식에 대해 설명하시오.

**답변:**
커널 수준 보안 기능과 사용자 공간 보안 기능의 차이점:

**커널 수준 보안 기능**:
- 실행 위치: 커널 공간에서 특권 모드로 실행
- 보호 범위: 전체 시스템 자원 및 하드웨어에 대한 직접 제어
- 구현 방식: 커널 코드, 모듈, 드라이버로 구현
- 장점: 우회 어려움, 포괄적 보호, 하드웨어 수준 접근
- 단점: 버그 시 전체 시스템 영향, 수정/업데이트 어려움
- 예시: 커널 ASLR, NX 비트 강제, SELinux, 접근 제어 리스트

**사용자 공간 보안 기능**:
- 실행 위치: 사용자 공간에서 비특권 모드로 실행
- 보호 범위: 특정 애플리케이션 또는 제한된 자원 집합
- 구현 방식: 데몬, 서비스, 라이브러리, 애플리케이션으로 구현
- 장점: 격리된 실패(하나의 기능 장애가 전체에 영향 적음), 쉬운 업데이트
- 단점: 우회 가능성 높음, 하드웨어 직접 제어 불가
- 예시: 방화벽 애플리케이션, 안티바이러스, 애플리케이션 샌드박스

샌드박싱(Sandboxing) 기술:

**개념과 역할**:
- 정의: 프로그램이 격리된 환경에서 실행되도록 제한하는 보안 메커니즘
- 목적: 악의적이거나 신뢰할 수 없는 코드의 영향 범위 제한
- 역할: 제로데이 취약점 완화, 악성 코드 피해 최소화, 특권 분리
- 원칙: 최소 권한, 자원 격리, 보안 경계 설정

**주요 구현 방식**:

1. 시스템 콜 필터링:
   - 구현: seccomp(Linux), Seatbelt(macOS), Windows 필터링 플랫폼
   - 작동: 프로세스가 사용할 수 있는 시스템 콜 제한
   - 특징: 낮은 오버헤드, 세밀한 권한 제어
   - 사용: Chrome, Firefox 등 브라우저

2. 애플리케이션 컨테이너:
   - 구현: Docker, LXC, Firejail
   - 작동: 네임스페이스와 컨트롤 그룹을 통한 자원 격리
   - 특징: 파일 시스템, 네트워크, 프로세스 격리
   - 사용: 클라우드 서비스, 마이크로서비스 아키텍처

3. 가상화 기반 샌드박스:
   - 구현: Xen, KVM, Hyper-V 등 하이퍼바이저
   - 작동: 완전한 하드웨어 가상화로 격리 환경 제공
   - 특징: 강력한 격리, 높은 오버헤드
   - 사용: 보안 연구, 엔터프라이즈 환경

4. 언어/런타임 기반 샌드박스:
   - 구현: JavaScript V8 엔진, Java SecurityManager
   - 작동: 언어 런타임 수준에서 접근 제어 시행
   - 특징: 플랫폼 독립적, 이식성 높음
   - 사용: 웹 브라우저, 모바일 앱

5. 규칙 기반 접근 제어:
   - 구현: AppArmor 프로파일, SELinux 정책
   - 작동: 애플리케이션별 허용 작업 명시적 정의
   - 특징: 기존 OS에 통합 용이, 정책 관리 필요
   - 사용: 서버 환경, 데스크톱 리눅스

**샌드박스 설계 고려사항**:
- 보안 경계 정의: 어떤 자원과 기능을 격리할 것인지
- 성능 영향: 오버헤드와 보안 강도 균형
- 사용성: 정상 기능 수행 가능성 확보
- 이스케이프(탈출) 방지: 샌드박스 우회 가능성 최소화

**해설:**
커널 수준 보안과 사용자 공간 보안의 주요 차이점은 실행 권한과 접근 범위에 있습니다. 커널 수준 보안은 운영체제의 핵심부에서 특권 모드로 실행되어 포괄적인 시스템 보호를 제공하지만, 버그가 있을 경우 전체 시스템에 영향을 미칠 수 있습니다. 반면, 사용자 공간 보안은 일반 애플리케이션처럼 제한된 권한으로 실행되어 영향 범위가 제한적이지만, 근본적인 보호 기능도 제한적입니다.

샌드박싱은 이러한 보안 모델 중 중요한 개념으로, 특히 신뢰할 수 없는 코드를 안전하게 실행하는 데 필수적입니다. 웹 브라우저는 샌드박싱의 대표적인 응용 사례로, Chrome과 같은 브라우저는 각 탭이나 확장 프로그램을 별도의 샌드박스에서 실행하여 한 웹사이트의 취약점이 전체 브라우저나 시스템에 영향을 미치지 않도록 합니다.

시스템 콜 필터링은 효율적인 샌드박싱 방법으로, Linux의 seccomp-bpf는 프로세스가 사용할 수 있는 시스템 콜을 정밀하게 제어합니다. 컨테이너 기술은 이보다 더 넓은 격리를 제공하며, 네임스페이스와 컨트롤 그룹을 통해 프로세스, 네트워크, 파일 시스템을 격리합니다.

가상화 기반 샌드박스는 가장 강력한 격리를 제공하지만, 그만큼 오버헤드도 큽니다. Qubes OS와 같은 보안 중심 운영체제는 가상화를 기반으로 각 애플리케이션이나 작업 영역을 격리합니다.

효과적인 샌드박스 설계는 보안 목표, 허용 가능한 성능 영향, 사용자 경험을 종합적으로 고려해야 합니다. 너무 제한적인 샌드박스는 애플리케이션의 정상 기능을 방해할 수 있고, 너무 느슨한 샌드박스는 충분한 보호를 제공하지 못합니다. 이러한 균형을 맞추는 것이 샌드박스 설계의 핵심 과제입니다.

## 문제 8
사용자 인증 관련 암호 저장 방식의 진화와 현대적인 패스워드 해싱 기법(bcrypt, Argon2 등)의 보안 특성을 비교 설명하시오.

**답변:**
암호 저장 방식의 진화:

1. 평문 저장(Plaintext Storage):
   - 초기 시스템에서 사용
   - 구현: 사용자 비밀번호를 그대로 저장
   - 문제점: 데이터베이스 유출 시 모든 비밀번호 즉시 노출

2. 단방향 해시(One-way Hash):
   - 1970년대부터 사용 (UNIX crypt)
   - 구현: MD5, SHA-1, SHA-256 등의 해시 함수 적용
   - 개선점: 원본 복원 어려움
   - 취약점: 레인보우 테이블, 빠른 해시 계산으로 무차별 대입 공격 가능

3. 솔팅(Salting) 도입:
   - 구현: 각 사용자별 무작위 값(salt)을 비밀번호에 추가 후 해시
   - 개선점: 동일 비밀번호도 다른 해시값 생성, 레인보우 테이블 방어
   - 취약점: 여전히 빠른 해시 계산 속도로 GPU 기반 공격에 취약

4. 반복 해싱(Iterative Hashing):
   - 구현: 해시 함수를 수천~수만 번 반복 적용 (PBKDF2)
   - 개선점: 계산 비용 증가로 무차별 대입 공격 속도 저하
   - 취약점: GPU/ASIC 가속에 여전히 취약할 수 있음

5. 현대적 해싱 알고리즘:
   - 메모리-하드 함수, 계산 비용 조정 가능
   - 대표 알고리즘: bcrypt, scrypt, Argon2
   - 개선점: 병렬 처리 저항성, 하드웨어 가속 방어

현대적 패스워드 해싱 기법 비교:

1. bcrypt:
   - 개발: 1999년, Niels Provos와 David Mazières
   - 기반: Blowfish 암호화 알고리즘
   - 특징: 
     * 작업 계수(work factor) 조정 가능
     * 솔트 내장 (128비트)
     * 느린 키 설정 알고리즘
   - 강점: 구현 간단, 오랜 검증 기간, GPU 가속 저항성
   - 약점: 메모리 요구량 고정, 병렬화 일부 가능

2. scrypt:
   - 개발: 2009년, Colin Percival
   - 기반: PBKDF2와 메모리-하드 함수
   - 특징:
     * CPU 및 메모리 비용 조정 가능
     * 높은 메모리 요구량
     * 솔트 지원
   - 강점: 메모리-하드 특성으로 ASIC 저항성 강화
   - 약점: 구현 복잡성, 일부 매개변수 조합에서 시간-메모리 트레이드오프 가능

3. Argon2:
   - 개발: 2015년, Alex Biryukov 등 (Password Hashing Competition 우승)
   - 기반: Blake2 해시 함수
   - 특징:
     * 세 가지 변형: Argon2d, Argon2i, Argon2id
     * 메모리 크기, 반복 횟수, 병렬도 독립 조정 가능
     * 솔트 및 추가 데이터 지원
   - 강점: 현대적 설계, 다양한 공격 방어, 세밀한 매개변수 조정
   - 약점: 상대적으로 새로운 알고리즘으로 장기 검증 부족

알고리즘 비교표:

| 특성 | bcrypt | scrypt | Argon2 |
|------|--------|--------|--------|
| 출시 연도 | 1999 | 2009 | 2015 |
| 메모리 요구량 | 약 4KB (고정) | 구성 가능 (높음) | 구성 가능 (높음) |
| 병렬화 저항성 | 중간 | 높음 | 매우 높음 |
| 구현 복잡성 | 낮음 | 중간 | 중간 |
| 매개변수 유연성 | 제한적 | 높음 | 매우 높음 |
| 검증 기간 | 긴 (20년+) | 중간 (10년+) | 짧음 (5년+) |
| 권장 사용 사례 | 일반적 인증 | 높은 가치 데이터 | 최신 시스템, 높은 보안 요구 |

**해설:**
암호 저장 방식은 컴퓨팅 능력의 발전과 공격 기술의 진화에 대응하여 지속적으로 발전해왔습니다. 초기의 평문 저장 방식은 명백한 보안 위험을 가졌으며, 단방향 해시 도입은 중요한 첫 단계였지만 무차별 대입 공격에 취약했습니다.

현대적인 패스워드 해싱 알고리즘의 핵심 원칙은 "느리고 비용이 많이 드는" 함수를 사용하여 공격자의 무차별 대입 시도를 지연시키는 것입니다. 이는 정당한 사용자 인증에는 거의 눈치채지 못할 지연만 추가하지만, 수백만 개의 비밀번호를 시도해야 하는 공격자에게는 상당한 부담이 됩니다.

bcrypt는 20년 이상의 검증된 역사를 가진 안정적인 알고리즘으로, 작업 계수를 통해 계산 비용을 조정할 수 있습니다. 하지만 메모리 요구량이 고정되어 있어 최신 하드웨어에서의 병렬 공격에 일부 취약할 수 있습니다.

scrypt는 메모리-하드 특성을 도입하여 대규모 병렬 처리 하드웨어(GPU, ASIC)를 사용한 공격을 더욱 어렵게 만들었습니다. 필요한 메모리 양을 조정할 수 있어 고가치 데이터 보호에 적합합니다.

Argon2는 최신 알고리즘으로, 이전 알고리즘들의 교훈을 바탕으로 설계되었습니다. 세 가지 변형을 제공하여 다양한 공격 시나리오에 대응할 수 있으며, 메모리 사용량, 시간 비용, 병렬도를 독립적으로 조정할 수 있어 최대의 유연성을 제공합니다.

실제 구현 시에는 알고리즘 선택뿐만 아니라 적절한 매개변수 설정도 중요합니다. 너무 높은 비용 매개변수는 정상 사용자 경험을 저해하고 서비스 거부 공격 가능성을 높일 수 있으며, 너무 낮은 설정은 충분한 보호를 제공하지 못합니다. OWASP 권장사항에 따르면, 인증 처리 시간이 약 1초 미만이 되도록 매개변수를 조정하는 것이 좋습니다.

## 문제 9
운영체제 보안에서 신뢰 실행 환경(TEE, Trusted Execution Environment)과 보안 부팅(Secure Boot)의 역할과 작동 원리를 설명하시오.

**답변:**
신뢰 실행 환경(TEE)의 역할과 작동 원리:

**TEE 개념과 역할**:
- 정의: 메인 프로세서 내에 격리된 보안 영역으로, 민감한 데이터와 코드를 보호하는 환경
- 주요 목적: 일반 운영체제와 분리된 환경에서 보안이 중요한 작업 실행
- 용도: 암호화 키 관리, 생체인식 처리, DRM, 모바일 결제, 디지털 인증

**TEE 아키텍처**:
1. 이중 실행 환경:
   - 일반 실행 환경(REE, Rich Execution Environment): 일반 OS 실행
   - 신뢰 실행 환경(TEE): 보안이 필요한 신뢰 애플리케이션 실행
   
2. 하드웨어 격리:
   - CPU 보안 모드 활용(ARM TrustZone, Intel SGX)
   - 메모리 격리 기술로 접근 제한
   - 보안 주변장치(암호화 가속기, 난수 생성기) 전용 접근

3. 보안 통신 채널:
   - REE와 TEE 간 통신 인터페이스
   - 엄격한 API를 통한 제한적 상호작용
   - 매개변수 유효성 검사 및 권한 확인

**주요 TEE 구현**:
- ARM TrustZone: 프로세서 수준에서 보안 세계와 일반 세계 분리
- Intel SGX(Software Guard Extensions): 애플리케이션 내 보안 엔클레이브 생성
- AMD SEV(Secure Encrypted Virtualization): 가상 머신 메모리 암호화
- Apple Secure Enclave: 키 관리 및 생체 인식 데이터 처리 전용

보안 부팅(Secure Boot)의 역할과 작동 원리:

**보안 부팅 개념과 역할**:
- 정의: 시스템 시작 시 부트로더와 운영체제의 무결성을 검증하는 메커니즘
- 주요 목적: 부팅 프로세스 초기부터 시스템 무결성 보장
- 용도: 부트킷 방지, 허가되지 않은 OS 로드 차단, 시스템 변조 감지

**보안 부팅 작동 원리**:
1. 신뢰 체인(Chain of Trust) 구축:
   - 하드웨어에 내장된 루트 키로 시작
   - 각 부팅 단계가 다음 단계를 검증
   - 서명 확인 실패 시 부팅 중단

2. 부팅 과정 단계별 검증:
   - 펌웨어/UEFI 초기화와, 자체 검증
   - 부트로더 디지털 서명 확인
   - 커널 및 초기 램디스크 서명 확인
   - 드라이버 및 모듈 로드 시 서명 확인

3. 키 관리:
   - 플랫폼 키(PK): 펌웨어 설정 변경 권한
   - 키 교환 키(KEK): 서명 데이터베이스 업데이트 권한
   - 서명 데이터베이스(db): 허용된 부팅 이미지 서명
   - 거부 데이터베이스(dbx): 금지된 이미지 서명

**TEE와 보안 부팅의 상호보완성**:
- 보안 부팅: 시스템 시작 시 신뢰성 보장
- TEE: 실행 중 민감한 작업 보호
- 함께 사용 시 부팅부터 실행까지 완전한 보안 체인 구성
- TEE에 보안 부팅 키와 정책 저장으로 무결성 보장

**주요 구현 및 사례**:
- UEFI 보안 부팅: Windows, Linux 등 다양한 OS 지원
- Android Verified Boot: 안드로이드 기기의 보안 부팅
- Apple Secure Boot: iOS 및 macOS 기기의 부팅 보호
- TPM(Trusted Platform Module): 측정 부팅 및 키 저장

**해설:**
신뢰 실행 환경(TEE)과 보안 부팅(Secure Boot)은 현대 운영체제 보안의 핵심 구성 요소로, 시스템의 무결성과 신뢰성을 시작부터 실행까지 보장합니다.

TEE는 민감한 작업을 메인 운영체제와 분리된 안전한 환경에서 실행할 수 있게 합니다. ARM TrustZone은 모바일 기기에서 널리 사용되는 TEE 구현으로, 프로세서 수준에서 "보안 세계"와 "일반 세계"를 분리합니다. 이를 통해 생체 인식 정보, 암호화 키, 결제 정보 등이 악성 코드로부터 보호받습니다. Intel SGX는 애플리케이션 수준의 격리를 제공하며, 클라우드 환경에서도 안전한 계산을 가능하게 합니다.

보안 부팅은 "신뢰 체인" 개념을 기반으로 합니다. 이는 부팅 프로세스의 각 단계가 다음 단계를 검증하는 방식으로, 하드웨어에 내장된 신뢰 루트(Root of Trust)에서 시작합니다. 이 과정에서 디지털 서명을 사용하여 부트로더, 커널, 드라이버 등의 무결성을 확인합니다. UEFI 보안 부팅은 가장 널리 알려진 구현으로, Windows와 대부분의 Linux 배포판에서 지원됩니다.

TEE와 보안 부팅은 상호보완적인 기술입니다. 보안 부팅은 시스템 시작 시 신뢰할 수 있는 소프트웨어만 로드되도록 보장하며, TEE는 시스템 실행 중에 민감한 작업을 보호합니다. 이 둘을 함께 사용하면 부팅부터 실행까지 완전한 보안 체인을 구축할 수 있습니다. 특히 TEE에 보안 부팅 키와 정책을 저장함으로써 보안 부팅 자체의 무결성을 강화할 수 있습니다.

이러한 기술들은 랜섬웨어, 루트킷, 부트킷과 같은 지속성 위협으로부터 시스템을 보호하는 데 중요한 역할을 합니다. 또한 기업 환경에서 BYOD(Bring Your Own Device) 정책과 원격 작업 증가로 인한 보안 위험을 관리하는 데도 필수적입니다.

## 문제 10
다중 요소 인증(MFA) 시스템의 설계 원칙과 구현 방식을 설명하고, 생체 인식을 포함한 다양한 인증 요소의 장단점을 비교하시오.

**답변:**
다중 요소 인증(MFA) 시스템의 설계 원칙과 구현 방식:

**설계 원칙**:

1. 독립성 원칙:
   - 서로 다른 범주의 인증 요소 조합
   - 한 요소 손상이 다른 요소에 영향 최소화
   - 서로 다른 공격 벡터 대응

2. 심층 방어(Defense in Depth):
   - 다중 보안 계층 구축
   - 개별 방어 계층 우회 어렵게 설계
   - 보안과 사용성 균형 유지

3. 위험 기반 접근법:
   - 자원 중요도와 위험 수준에 따른 인증 강도 조정
   - 상황적 요인(시간, 위치, 기기 등) 고려
   - 이상 행동 감지 시 추가 인증 요구

4. 대체 경로 설계:
   - 기본 인증 방법 사용 불가 시 대안 제공
   - 복구 메커니즘 구현
   - 인증 실패 시 단계적 조치

**구현 방식**:

1. 시퀀셜 MFA:
   - 순차적 인증 단계 진행
   - 각 단계별 성공/실패 확인
   - 구현 간단, 사용자 이해 쉬움

2. 병렬 MFA:
   - 여러 인증 요소 동시 제출
   - 단일 작업으로 여러 요소 검증
   - 사용자 경험 향상, 복잡성 증가

3. 적응형 MFA:
   - 상황과 위험 수준에 따라 요구 요소 조정
   - 행동 분석 및 비정상 감지 활용
   - 저위험 상황에서 사용성 향상

4. 통합 아키텍처:
   - 중앙화된 ID 관리 시스템과 통합
   - 단일 인증 정책 관리
   - 다양한 시스템 및 애플리케이션 지원

인증 요소별 특성 비교:

1. 지식 기반 요소(알고 있는 것):
   - 유형: 비밀번호, PIN, 보안 질문
   - 장점: 구현 용이, 비용 효율적, 익숙함
   - 단점: 기억 부담, 유출/도난 위험, 재사용 문제
   - 주요 위협: 피싱, 키로깅, 사회공학적 공격

2. 소유 기반 요소(가지고 있는 것):
   - 유형: OTP 토큰, 스마트카드, 모바일 기기
   - 장점: 물리적 소유 필요, 상대적 높은 보안성
   - 단점: 분실/도난 가능성, 추가 비용, 휴대 불편
   - 주요 위협: 물리적 도난, 복제, 중간자 공격

3. 생체 인식 요소(본인의 특성):
   
   **지문 인식**:
   - 장점: 빠른 인식, 높은 정확도, 사용자 친화적
   - 단점: 손상된 지문 인식 어려움, 복제 가능성, 변경 불가
   - 정확도: FAR 0.1%, FRR 3% 내외
   
   **얼굴 인식**:
   - 장점: 비접촉식, 사용 편의성, 하드웨어 요구사항 낮음
   - 단점: 조명/각도 영향, 쌍둥이 문제, 사진/동영상 스푸핑
   - 정확도: 구현에 따라 FAR 0.1-5%, FRR 2-15%
   
   **홍채 인식**:
   - 장점: 매우 높은 고유성, 안정성, 스푸핑 어려움
   - 단점: 전문 하드웨어 필요, 비용 높음, 일부 질환 영향
   - 정확도: FAR 0.0001%, FRR 0.5-1%
   
   **음성 인식**:
   - 장점: 원격 인증 가능, 추가 하드웨어 불필요
   - 단점: 소음 영향, 목소리 변화, 녹음 재생 공격
   - 정확도: FAR 2-5%, FRR 5-10%

4. 행동 생체 인식:
   - 유형: 키스트로크 다이내믹스, 보행 패턴, 서명 패턴
   - 장점: 지속적 인증 가능, 비침투적, 사용자 인식 없음
   - 단점: 낮은 정확도, 상황/시간에 따른 변화, 구현 복잡
   - 주요 위협: 모방 공격, 변화하는 패턴 대응 어려움

MFA 시스템 설계 시 고려사항:

1. 사용자 경험:
   - 마찰 최소화 및 사용 편의성
   - 명확한 오류 메시지 및 지침
   - 접근성 고려(장애인 지원)

2. 보안 수준:
   - 보호할 자산의 가치에 비례한 보안 강도
   - 인증 요소 간 독립성 확보
   - 우회 경로 최소화

3. 확장성 및 호환성:
   - 사용자 증가 대응 능력
   - 다양한 기기 및 플랫폼 지원
   - 표준 프로토콜 준수(FIDO2, WebAuthn 등)

4. 운영 및 관리:
   - 등록 및 복구 프로세스 보안
   - 감사 및 모니터링 기능
   - 비용 및 유지보수 고려

**해설:**
다중 요소 인증(MFA)은 "알고 있는 것", "가지고 있는 것", "본인인 것" 등 서로 다른 범주의 인증 요소를 조합하여 보안을 강화하는 방식입니다. MFA의 핵심 원칙은 독립성으로, 한 요소가 손상되더라도 다른 요소가 보호 상태를 유지하여 무단 접근을 방지하는 것입니다.

지식 기반 요소(비밀번호 등)는 가장 전통적인 인증 방식이지만, 피싱이나 사회공학적 공격에 취약합니다. 소유 기반 요소(OTP 토큰 등)는 물리적 소유를 요구하여 보안성을 높이지만, 분실이나 도난 위험이 있습니다. 생체 인식은 높은 편의성과 고유성을 제공하지만, 변경 불가능성(한번 손상되면 영구적)과 프라이버시 우려가 있습니다.

생체 인식 기술 중 지문 인식은 가장 널리 사용되는 방식으로, 빠른 인식과 상대적으로 높은 정확도를 제공합니다. 얼굴 인식은 비접촉식으로 사용자 경험이 우수하지만, 환경 조건에 민감합니다. 홍채 인식은 매우 높은 정확도를 제공하지만 전문 하드웨어가 필요합니다. 각 기술은 FAR(False Acceptance Rate, 타인 오인식률)과 FRR(False Rejection Rate, 본인 거부율)의 균형을 고려하여 설계됩니다.

위험 기반 접근법을 활용한 적응형 MFA는 현대적인 구현 방식으로, 사용자 행동, 위치, 시간 등 상황적 요인을 분석하여 위험 수준에 맞게 인증 강도를 조정합니다. 이는 보안과 사용성의 균형을 맞추는 데 효과적입니다.

FIDO2와 WebAuthn 같은 현대적 인증 표준은 강력한 암호화, 기기 기반 인증, 생체 인식을 결합하여 안전하고 사용자 친화적인 MFA 구현을 지원합니다. 이러한 표준은 피싱 저항성을 갖추고 프라이버시를 보호하면서 다양한 플랫폼에서 일관된 사용자 경험을 제공합니다.

MFA 시스템 설계 시에는 보안 강도뿐만 아니라 사용자 경험, 접근성, 복구 메커니즘, 운영 비용 등을 종합적으로 고려해야 합니다. 지나치게 복잡한 인증 과정은 사용자 저항을 초래하고 생산성을 저하시킬 수 있으므로, 보호 자산의 가치에 적절한 수준의 보안을 제공하는 것이 중요합니다.

## 문제 11
버퍼 오버플로우 공격의 원리와 이를 방지하기 위한 운영체제 및 컴파일러 수준의 보호 메커니즘을 설명하시오.

**답변:**
버퍼 오버플로우 공격의 원리:

1. 기본 개념:
   - 프로그램이 할당된 메모리 버퍼 경계를 넘어 데이터를 쓸 때 발생
   - 인접 메모리 영역 덮어쓰기로 프로그램 흐름 변경 가능
   - 주로 경계 검사가 없는 언어(C, C++)에서 발생

2. 대표적인 유형:
   - 스택 기반 오버플로우: 지역 변수와 리턴 주소 손상
   - 힙 기반 오버플로우: 동적 할당 메모리 영역 손상
   - 정수 오버플로우: 정수 연산 오버플로우로 인한 메모리 할당 오류
   - 형식 문자열 취약점: 잘못된 형식 문자열 지정자 악용

3. 공격 단계(스택 오버플로우 예시):
   - 버퍼 경계 식별
   - 리턴 주소 위치 계산
   - 악성 코드(셸코드) 작성
   - 리턴 주소를 셸코드 주소로 덮어쓰기
   - 프로그램 흐름 하이재킹

버퍼 오버플로우 방지 메커니즘:

1. 컴파일러 수준 보호:

   a. 스택 카나리(Stack Canary):
   - 동작: 함수 프롤로그에서 스택에 임의 값 삽입, 에필로그에서 확인
   - 장점: 오버플로우 발생 즉시 감지, 구
