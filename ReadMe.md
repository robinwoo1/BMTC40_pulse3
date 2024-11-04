# QB Project (BMT40)

BLDC + Battery Driver 

## Feature

- BLDC 모터
- SVPWM 제어
- BISS 통신
- Encoder
- RS9116 - WIFI 모듈


---
## 세부사항

QB 프로젝트 설정

1. BISS 엔코더 사용.
2. RS9116 - WIFI 모듈 사용.
3. TIM1: 10800 주기 - 동작 시간 정확히 모름 (아마 50us)
4. TIM14: 5400 (50us) - Current 제어 주기
5. TIM13: 10800 (100us) - BISS 통신 주기


---
## 문제점

1. 토크 센서가 반대로 읽힘



---
## 해결

1. QSPI 메모리 저장 에러 발생.  (Optimize를 None으로 하면 정상, Speed로 설정하면 에러   또는 D Cache를 사용하지 않으면 정상)
   (CubeIDE 버전을 1.12.0 으로 변경하여 해결)

2. Start 레버 : 자석에 너무 민감하여 아주 살짝 레버를 건드려도 동작 신호가 발생 (가만히 있어도 발생 - 현재는 Filter를 걸어서 동작 제한, PCB 별로 차이가 존재함)


---
## 추가해야할 사항?

1. BARCODE 테스트 확인
2. BAR_GOODREAD 신호


---
## 특이사항
1. 고해상도 그래프 사용 시 main loop에서 3ms 정도 Delay 발생 (외부로 전송할 때 - WIFI 전송에 시간이 많이 소요)
2. 와이파이 소켓 연결 시 Delay 발생 - 처음 연결할 때만 Delay 발생.




# 확인 사항
1. ADC1
2. SPI1
3. SPI2
4. SPI3
5. SPI4
6. I2C1
7. QSPI
8. IWDG
9. UART1
10. UART3
11. RTC


