#!/bin/bash

# 클라이언트 실행 파일 경로 설정
PATH_CLIENT="/home/junho/Client_test"  # 절대 경로로 수정

# ----------------- #
#     서버
# ----------------- #
# 서버 실행 부분 설정
PATH_SERVER="/home/junho/program/chat_server/chat_server.out"
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
    sleep 0.1  # 각 클라이언트 실행 간 짧은 지연 추가
done

# 클라이언트 PID 확인
echo "클라이언트 PIDs: ${PIDS_CLIENT[@]}"

# ----------------- #
#     리소스 모니터링
# ----------------- #
sleep 5
echo "서버 리소스 모니터링 시작 (PID: $PID_SERVER)"
# 주기적으로 서버 리소스 사용량을 모니터링
(
    while kill -0 $PID_SERVER 2>/dev/null; do
        echo "메모리 사용량:"
        pmap $PID_SERVER | tail -n 1
        echo "CPU 사용량:"
        ps -p $PID_SERVER -o %cpu
        sleep 5
    done
) &

# ----------------- #
#     종료 처리
# ----------------- #
wait
