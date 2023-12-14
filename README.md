# 소켓 프로그래밍을 활용한 끝말잇기 게임
### 2023-2 유닉스 프로그래밍 프로젝트
---
### 게임 설명
- 클라이언트 두 명이 서버 PortNum/IP address에 접속하면 게임 실행
- ex) ./ser 9000 255.255.255.1
- 먼저 참여한 사람이 제시어를 받고, 이를 이어가는 끝말잇기를 진행하면 됨

### 파일 설명
- *.c 소스코드
- *.h 생성한 라이브러리 코드
- *.txt 함수에서 사용한 텍스트 파일

### 코드 설명
- server.c ) 서버 메인 코드
- client.c ) 클라이언트 메인 코드
- error.c ) 에러 처리 간편화
- playRelayWordGame.c ) 지정 소켓에서 게임 진행
- randword.c ) 랜덤 단어를 받아옴. playRelayWordGame.c에서 클라이언트 1에게 제시어를 보낼 때 사용
- sendBySocket.c ) send함수 간편화
- compare_first_last ) 끝말잇기 규칙 검사 ( 한글버전 )
- printManual.c ) 클라이언트 실행 시 게임 매뉴얼 출력
