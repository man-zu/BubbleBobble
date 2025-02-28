# BubbleBobble
![Image](https://github.com/user-attachments/assets/02059ed3-c25b-4501-9b19-9dca300e5085)
* 개발기간 : 2주
* 개발인원 : 개인
* 사용언어 및 개발환경 : WinAPI, C++

  

## 동영상
https://youtu.be/k9s_RgOud3U?si=FAepQVOTGlni79OV


## 구현
### 1.	Player
 - 애니메니션, 점프, 중력, 충돌처리 구현
 - 맵 외곽의 경우 이탈 불가능, 내부 블럭 하단은 충돌 무시하여
윗 블럭 점프이동 가능
 - 방향키 입력시 하단 블럭으로 내려가기 구현
 - 플레이어 스킨 2종(초록/빨강) 선택 스테이지 구현

### 2. 맵 제작(일반 3개, 이벤트 1개, 보스 1개)
 - 클릭으로 블럭 영역 지정 및 저장 가능
 - 맵 저장 및 파싱
   
### 3. UI / 사운드(FMod)
 - 점수처리, 생명, 무적 쉴드, Hurry Up 구현

### 4. 몬스터, 아이템, 충돌처리
 - 몬스터 모션처리, 충돌처리
 - 랜덤 아이템 생성(체리/수박/바나나/케잌 등), 충돌처리
 - 보스 이동패턴, 공격패턴 3종 구현 ( 좌공격 / 하단 공격 / 번개패턴 )
 - 플레이어 뱡향으로 보스 Look 방향 변경
