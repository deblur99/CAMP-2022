# 바이너리 파일을 objdump로 열어 어셈블리 코드로 보기
mips-linux-gnu-objdump -d <파일경로>

# 4/16 한 일
- 파서 제작
 - 바이너리 파일에서 명령어 한 줄씩 읽어와서, 각 명령어를 리틀 엔디안 구조로 변환한 후 정적 메모리에 저장함
 - 가용 메모리가 충분치 않아 가급적 구동은 Assam 서버에서 하는 걸 추천
 - 현재는 메모리 크기를 0x10000로 잡았지만, 실제 제출용 코드에서는 0xFFFFFFFF로 잡아야 함

# 4/24 한 일
- Decode 구현 완료, Execute 구현하기 앞서 코드 구조 개선
- Execute R타입 ADD, JR / I타입 ADDI, ADDIU / J타입 J, JAL 구현 완료

# 이후 해야할 일 (제출마감 4/24 일요일. Freeday는 5일 있음. 4/25 월요일 기준 Freeday 1일 소모중)
1) Decode (완)
2) Execute (진행중)
3) Write memory (진행중)
4) Write register (진행중)
5) Update PC (진행중)
6) 매 명령어 실행 끝날 때마다 레지스터의 현재 state, 사용한 명령어의 개수 등등 출력 (예정)

# 레지스터 지정 및 레지스터에 저장된 값 관련
- inst 구조체 (struct _INSTRUCT)
    - rs, rt, rd 멤버
        - 명령어에 있는 피연산자들이 각각 지정한 레지스터의 순번을 가리킨다.
        - 예를 들어, 피연산자 rs에 0x11 (10진수로 17) 값이 저장되어 있다고 하자. 그러면 피연산자 rs가 가리키는 레지스터는 0 + 0x11로, 17번째 레지스터 s1 ($17)를 가리키는 것이다.
     
- regMemory 배열 (int32_t regMemory[0x20])
    - 요소가 int32_t 자료형이고, 크기가 32인 배열이다. 0번째 인덱스부터 31번째 인덱스는 각각의 레지스터에 저장된 값을 가리킨다.

- regs 구조체 (struct _REGISTERS)
    - PC (Program Counter)와 32가지 레지스터 ($0 ~ $31)의 정보를 가리키는 구조체

# 4/27
호출된 함수에서 값을 반환하지 않으면 업데이트가 되지 않음
-> 구조체 중심으로 레지스터 저장값 나타내는 regMemory 배열, 명령어 정보 나타내는 INSTRUCT 구조체, Single Cycle 결과값 나타내는 COUNTER 구조체를 SCYCLE_INFO 구조체로 통합
-> 함수에 넘기고 업데이트한 구조체를 다시 반환하고, 이를 l-value에 위치한 구조체 변수에 다시 저장한다.

* 구조체 할당해주고 -> 구조체 멤버 할당해주고 -> 할당된 구조체 멤버 접근해서 초기화해주고

# 구현된 / 구현해야 할 명령어 목록
* 메모리 접근하는 명령어는 LW, SW밖에 없고, 모두 I Type이다.

* R Type
- MOVE (ADDU와 겹침)
- ADD
- SUB
- SUBU
- MULT
- MULTU
- DIV
- DIVU

- AND
- NOR
- OR

- SLT
- SLTU
- SLL
- SRL
- JR

* I Type
- LI
- LW
- SW
- SLTI
- SLTIU
- ADDI
- ADDIU
- ANDI
- ORI
- BEQ
- BNE

* J Type
- J
- JAL