# final_project
### 2022년 졸업작품

## [ 비접촉 출입정보 관리기 ]
#### 접촉을 최소화하여 건물의 출입자의 체온을 측정하고 출입자의 정보를 자동으로 기록하는 프로그램


## 제작배경
### 제작동기
코로나가 심각해지면서 건물마다 체온을 측정하고 방문자를 기록하기 시작했다.  
하지만 건물마다 관리자가 배치되어 방문자기록을 남겨달라고 요청하거나 체온을 측정하는 불편함이 있고,  
카메라 체온계를 설치할 경우도 약300만원의 높은 가격대로 판매 되고있기 때문에 비용이 많이 들게 된다.


### 문제해결
이번 프로젝트를 통해 일반적인 카메라 체온계보다 저렴한 가격의 제품을 제작해 비용을 절약하고,  
수기로 방문자를 기록했던 불편함을 QR코드를 이용해 자동으로 기록 저장은 물론,  
비접촉 방식을 이용해 측정자의 감염 위험도 감소하는 효과를 기대한다.  
또한 자동 높이 조절을 통해 다양한 키의 사람은 물론 휠체어를 사용하는 사람도 편하게 이용 가능하도록 설계하였다.

## 프로젝트 설명
### 구동순서
[ 1 ] 사람인식 및 초음파 센서를 이용한 신장센싱  
[ 2 ] 모터와 리미티드 스위치를 이용해 해당 스텝에 맞춰 길이조절  
[ 3 ] QR코드 인식  
[ 4 ] 체온측정 및 계산  
[ 5 ] 패스 논패스 판단  
[ 6 ] EXCEL에 출입자 정보 및 체온 데이터 저장

### 프로젝트 특징
#### [ 1 ] 자동높이조절
초음파 센서통해 사용자의 키를 측정하고 해당 스텝에 맞는 높으로 모터와 리미트 스위치를 이용해 조절  


#### [ 2 ] QR코드
QR코드 리더기를 이용해 사용자의 정보( 이름 전화전호 소속 등)을 빠르게 읽어들여 정보저장


#### [ 3 ] 온도보상
물체의 온도와 공기중의 온도를 측정하는 센서를 이용해 사람의 체온만 받아들여 총 12개의 값을 읽어들이고  
그중 센싱과정에서 튀는값을 제거하기 위해 최대온도와 최소온도를 제거해 평균을 계산후 평균계산  
이 과정을 세번에 걸쳐 정상범위의 체온인지 아닌지 판단하는 알고리즘 설계    
![실행화면](https://raw.githubusercontent.com/ZZO-ZHO/final_project/main/photo/pnp.png)


#### [ 4 ] 자동기록
QR을 통해 읽어들인 정보와 측정한 체온과 현재시간을 정리해 엑셀에 저장하여
확신자 발생기 확진자의 빠른 동선 파악 가능\
![실행화면](https://raw.githubusercontent.com/ZZO-ZHO/final_project/main/photo/plx_daq.png)


#### [ 5 ] LCD 패널 및 부저 이용
LCD 패널을 통해 사용자가 절차에 따라 QR코드를 스탠하고 체온을 측정하는등 순조롭게 진행되도록 도우며  
사용자의 체온과 패스 논패스 여부를 알려주어 현재상황을 체크할수있도록 도움  
또한 부저를 이용하여 단계가 진행되고있음을 알려주며 사용성을 높힘


#### [ 6 ] 듀얼 아두이노
두개의 아두이노를 통신을 통해 메인 아두이노와 모터 아두이노로 나누어 제어  
이를 통해 각 단계에 서로 미칠수있는 영향을 배제해 절차수행이 수월하게 진행되도록 도움


### 외형 및 회로도


![실행화면](https://raw.githubusercontent.com/ZZO-ZHO/final_project/main/photo/fin.png)



![실행화면](https://raw.githubusercontent.com/ZZO-ZHO/final_project/main/photo/line.png)



### 추가하고싶은 기능
#### [ 1 ] 출입문 통제  
계획 단계에서 패스일 경우는 문을 열고 논패스일경우 문을 열어주지 않음으로  
비정상 체온일경우 출입을 제한 하는 기능을 생각했으나  
비용적인 측면에서 구현해내지 못함  


#### [ 2 ] 한국어 지원  
LCD패널의 모듈이 영어만 지원하며 픽셀의 한계로 한국어는 구현하지 못하고 영어로 대처  
모듈을 교체하거나 한국어를 픽셀로 제작해 구현하는 방법등 고민중
