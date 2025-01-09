# UE_Assignment1

과제 5 : 랜덤으로 10번 이동하기

과제는 top down 기본 프로젝트를 생성하여 진행하였습니다.
바닥에 tile texture 를 추가하여 이동했을 때의 위치 변화를 보다 수월하게 인지할 수 있도록 했습니다.
일정 시간이 지나면 이동하는 것이 아니라 space bar 키를 눌렀을 때 랜덤하게 이동하도록 했습니다.
게임이 시작되면 PlayerCharacter 가 spawn 됩니다. 이때 UE_LOG 와 AddOnScreen 함수를 통해서 로그 창과 viewport 상에 Player Character 의 초기 위치가 출력됩니다.
한번 움직일 때 상하 좌우로 100 만큼 이동 하며 이는 랜덤하게 정해집니다.
이동할 때 50퍼센트의 확률로 폭발 파티클이 출력됩니다.
이동할 때마다 현재의 위치와 누적된 이동 거리가 로그창과 viewport 상에 출력됩니다.