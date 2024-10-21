#!/bin/bash

PATH_MAKE=$(pwd)
PATH_PROJECT=$PATH_MAKE
PATH_SERVER="$PATH_PROJECT/chat_server/chat_server.out"
PATH_CLIENT="$PATH_PROJECT/chat_client/chat_client.out"

# ----------------- #
#     서버
# ----------------- #
echo "서버 실행"
$PATH_SERVER 1> /dev/null &
# 서버 PID 저장
PID_SERVER=$!


# 서버가 시작될 시간을 대기
sleep 5

# ----------------- #
#     클라이언트
# ----------------- #
PIDS_CLIENT=()  # 클라이언트 PID를 저장할 배열
count=50
for ((i = 1; i <= count; i++)); 
do
	echo "Client 접속 요청 : $i / $count"
	$PATH_CLIENT 1> /dev/null &
	PIDS_CLIENT+=($!)

done

# wait
# echo "Client 접속 종료"

# ----------------- #
#     리소스 모니터링
# ----------------- #
# sleep 5
echo "서버 리소스 모니터링 시작 (PID: $PID_SERVER)"
# 주기적으로 서버 리소스 사용량을 모니터링
(
    for ((j = 0; j < 10; j++)); 
	do 
        ps -p $PID_SERVER -o %cpu,%mem
        sleep 1
    done
) &

# ----------------- #
#     리소스 정리
# ----------------- #

sleep 30
echo  리소스 정리

# 모든 클라이언트 종료
echo "클라이언트 종료"
pkill -f chat_client.out

# 서버 종료
echo "서버 종료"
pkill -f chat_server.out
