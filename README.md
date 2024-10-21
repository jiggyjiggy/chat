# 암호화

## 비대칭키 적용

### 설정
1. 서버의 공개키를 클라이언트에게 전송 <br>
<img width="1624" alt="서버의 공개키를 클라이언트에게 전송" src="https://github.com/user-attachments/assets/56660a7d-8878-4b93-a5ec-8fd990f91810">

2. 클라이언트의 공개키를 서버에게 전송 <br>
<img width="1624" alt="클라이언트의 공개키를 서버에게 전송" src="https://github.com/user-attachments/assets/bad2a1bc-e5dd-43f3-9aaa-0bee02c63431">

---

3. 클라이언트 -> 서버: 서버의 공개키로 암호화한 메시지를 전송 <br>
<img width="1624" alt="클라이언트가 서버에게 서버의 공개키로 암호화한 메시지를 전송" src="https://github.com/user-attachments/assets/cb5d45a7-e9a6-48c5-af59-2b70dbe47515">
 
4. 서버 -> 클라이언트: 클라이언트의 공개키로 암호화한 메시지를 전송 <br>
<img width="1624" alt="서버가 클라이언트에게 클라이언트의 공개키로 암호화한 메시지를 전송" src="https://github.com/user-attachments/assets/0125745b-4178-437e-98fb-ee1d62342744">

---

## 현대의 암호화 체계는 하이브리드체계이다

### 메시지 발신자 (암호화)
- 평문을 대칭키로 암호화한다 (==암호화된 메시지)
- 대칭키를 수신자의 공개키로 암호화한다 (==암호화된 공개키)
- 두 개를 합쳐서 발신한다

<img src="https://github.com/user-attachments/assets/b675a1a0-76ca-4c95-ac71-243be0db71ec" alt="하이브리드 암호 시스템의 암호화 과정" width="50%" />

### 메시지 수신자 (복호화)
- 암호화된 메시지와 암호화된 공개키를 분리한다
- 암호화된 대칭키를 본인의 비밀키로 복호화한다
- 복호화된 대칭키를 이용하여 메시지를 복호화한다

<img src="https://github.com/user-attachments/assets/e2bedcd5-8584-425c-9dd9-4f1e835a2025" alt="하이브리드 암호 시스템의 복호화 과정" width="50%" />

---