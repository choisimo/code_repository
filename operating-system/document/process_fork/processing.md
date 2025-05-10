레지스터 (상단 이미지 기준)
PC (Program Counter): 다음에 실행할 명령어의 주소를 저장

AC (Accumulator): 연산 결과나 임시 데이터를 저장하는 레지스터

IR (Instruction Register): 현재 해독(decode)되고 있는 명령어를 저장

일반 레지스터 (Register File): 다양한 데이터 값을 저장하는 범용 레지스터

MAR (Memory Address Register): 메모리 접근 시 사용할 주소를 저장

MBR/MDR (Memory Buffer/Data Register): 메모리에서 읽거나 쓰는 데이터를 임시 저장

버스
데이터 버스(Data Bus): 프로세서 내부 구성요소 간에 데이터를 전송

주소 버스(Address Bus): 메모리 주소를 전송

연산 및 제어 장치
ALU (Arithmetic Logic Unit): 산술 및 논리 연산 수행

디코더(Decoder): 명령어를 해석하여 필요한 작업 결정

제어 장치(Control): 각 구성요소에 제어 신호 전송

---

1. 명령어 패치(Fetch)
PC에 저장된 주소에서 명령어를 가져옴

이미지에서 PC가 가리키는 주소(예: 200)의 명령어를 IR로 로드

PC는 자동으로 +4(MIPS에서는 명령어가 4바이트)되어 다음 명령어를 가리킴

2. 명령어 해독(Decode)
IR에 저장된 명령어를 해독하여 어떤 작업을 수행할지 결정

명령어의 각 필드(opcode, registers, immediate values 등)를 분석

하단 이미지에서 Instruction[31-26], [25-21] 등으로 표시된 부분

3. 실행(Execute)
ALU에서 필요한 연산 수행

레지스터에서 값을 읽어와 연산 수행 (예: Add 연산)

하단 이미지의 ALU 부분에서 이루어짐

4. 메모리 접근(Memory Access)
필요한 경우 데이터 메모리에 접근

Load 명령어: 메모리에서 데이터 읽기

Store 명령어: 메모리에 데이터 쓰기

5. 결과 저장(Write Back)
연산 결과를 목적지 레지스터에 저장

---

### 싱글 사이클 작동 흐름

200: Load B (B 값 로드)
201: 3 (값 3)
202: Ac+B (누산기와 B 값을 더함)
203: Save Ac (누산기 값 저장)
204: 00
205: 05
예시 1: "Load B" 명령어 실행
패치: PC=200, "Load B" 명령어를 IR로 가져옴

해독: "Load B"는 메모리에서 B의 값을 읽어 AC에 저장하는 명령어로 해석

실행/메모리 접근: 주소 201에서 값 3을 읽음

결과 저장: 읽은 값(3)을 AC에 저장

PC 업데이트: PC=202 (다음 명령어 주소)

예시 2: "Ac+B" 명령어 실행
패치: PC=202, "Ac+B" 명령어를 IR로 가져옴

해독: AC와 B의 값을 더하는 명령어로 해석

실행: ALU에서 AC(=3)와 B의 값을 더함

결과 저장: 연산 결과를 AC에 저장

PC 업데이트: PC=203

예시 3: "Save Ac" 명령어 실행
패치: PC=203, "Save Ac" 명령어를 IR로 가져옴

해독: AC의 값을 메모리에 저장하는 명령어로 해석

메모리 접근: AC의 값을 지정된 메모리 위치에 저장

PC 업데이트: PC=204

데이터패스 작동 과정 (하단 이미지 기준)
명령어 패치: PC를 통해 Instruction memory에서 명령어 가져옴

레지스터 읽기: 명령어에서 레지스터 번호를 추출하여 Register 파일에서 값 읽기

ALU 연산: 레지스터 값으로 ALU 연산 수행

메모리 접근: 필요시 Data memory에 접근

결과 저장: 결과를 다시 레지스터에 저장

