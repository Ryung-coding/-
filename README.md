If you want to see more information? ⤵️
<br/><br/>
[Smithpring](https://www.notion.so/ryung-lab/2022-c0648cb8623e4954acecf932edb861fe)

**○ 개발 개요**

- 위 작품은 늘어나는 홈 트레이닝 머신의 수요에 따라 집에서도 웨이트 운동인 스쿼트를  할 수 있는 기구이다.
- 개발 목표는 홈 트레이닝이 가능한 크기와 무게로 기구를 설계하고, 비전 인식을 통해  사용자가 올바른 자세로 운동하도록 피드백해주는 것이다.

**○ 개발 목표**

- 협소한 공간에서도 사용가능하도록 기구의 크기를 최소화한다.
- 원판을 옮기는 불편함을 해소하기 위해 모터로 인공무게를 구현한다. - 인공무게 구현함으로서 다양한 무게를 구현하도록 한다.
- 사용자가 스스로 올바른 자세로 운동을 할 수 있게 도와주는 서비스를 제공한다.

# 하드웨어& 시스템 구성

### **Hardware 구성**

- 전체 구성도

전체적인 구조는 위 그림과 같이 영상처리를 통한 피드백시스템을 연산하는 PC와 본체인  Smithpring, 본체에게 정보를 제공하는 컨트롤러로 구성되어 있다. PC에서는 WebCam을  활용하여 사용자의 모션을 인식하고, 인식된 영상을 처리하여 사용자를 위한 자세 Feedback  서비스를 제공한다.

### **Controller**

![image](https://github.com/Ryung-coding/2022Embedded_Smithpring/assets/99808176/46b8a4ac-ed90-46df-b0d7-a90c3ac8b345)

본체를 조종하는 컨트롤러는 Arduino UNO에 SZH-EK05 조이스틱, I2C제어용 LCD,  Bluetooth HC-06 모듈을 연결하여 사용한다. 조이스틱을 움직여 MASS 변수값을 조정하고, 이를 LCD판 내에 표시한다. 또한 이 값은 본체인 Smithpring의 블루투스 통신장치인  HC-06과 서로 통신해 값을 넘겨주게 된다.

### **Smithpring (본체)**

![image (1)](https://github.com/Ryung-coding/2022Embedded_Smithpring/assets/99808176/7d0d70f2-4dc1-43f3-88f9-ceae405933c9)

Smithpring 장치는 하나의 Circuit Board를 통하여 제어를 진행한다. Circuit Board는  마이크로 컨트롤러의 일종인 Arduino에 연결되어 Load cell Amplifier인 HX711과  bluetooth HC-06 모듈, PWM(Pulse Width Modulation)을 생성하는 pca9685를 제어한다. Circuit Board에는 Motor에 명령을 입력하는 Motor Jack이 설치되어 있다.


## **Hardware 기능**

- 본 장치는 중심레일을 기점으로 사용자에게 힘을 가하는 운동부와 인공적인 부하를 만들어 내는 제어기부분으로 나눌 수 있다.
- 운동부의 경우, 중심 레일과 봉 사이 load cell 이 부착되어 있으며 이를 통하여 사용자에게 가해지는 힘을 추정한다.
- load cell에서 측정되는 값은 아래에 따라 전개되어 사용자가 느끼는 힘을 추정한다

**가정)**

- load cell의 변형은 무시한다.
- 레일은 고정지지로 가정한다.

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/a26b7072-7c0b-4bb2-b982-b2615a4256da/Untitled.png)

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/58a9d2a3-d47d-425c-abf0-9518ba5a75d2/Untitled.png)

레일을 원점이라고 한다면,

이므로   

이라고 치환한다면,

으로 표현할 수 있다.

실험을 진행한 결과,  그래프를 산출하였으며, 최종적으로 다음과 같은 형태로 데이터를 보정하여

사용자가 느끼는 중량을 예측하였다.

---

- 제어기부분의 경우, 모터에 연결된 풀리가 스트랩을 사용자의 움직임에 맞게 회전하여 스트랩을 감는다.
- 사용자의 움직임에 맞게 인공무게구현 제어기가 작동하여 사용자가 느끼는 무게를 구현한다.
- 인공무게구현은 모터의 토크가 풀리에 가해지며 생성되는 스트랩의 장력으로 구현된다. 이 과정에서 Load cell을 활용하여 피드백 시스템이 진행된다.
- 토크를 제어하는 과정은 아래와 같다.

**가정)**

- 스트랩의 변형은 존재하지 않는다.
- Motor에 가해지는 PWM 진동수(f)에 따라 즉각적으로 구동된다.
    
    **Note* pwm의 한 파장이 가해질 때, 단위step만큼 회전한다.**
    

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/02c52321-f1c0-4d22-848a-428a668fef42/Untitled.png)

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/01fe6586-efe4-43ee-bad5-9ea914a0c270/Untitled.png)

본 장치는 가해지는 힘 즉, 장력을 제어하는 기구에 해당한다. 그러므로 장력을 생성하는 토크를 제어해야 하며 이에 따라 반지름의 값을 알아야한다.

반지름의 임의의 크기를 r이라 하고, 감긴 줄의 길이를 L 이라고 한다면 반지름은 위와 같은 그래프의 형태로 변하게 될 것이다. 

그래프에 따라, 스트랩이 풀리에 감기면서 줄의 두께만큼 반지름이 증가하는 형태를 띈다. 그러므로 다음과 같은 형태로 토크와 감긴 스트랩의 길이를 정의를 할 수 있다.

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/5a553d71-acb8-4193-b340-21d44052504b/Untitled.png)

이러한 감긴 양을 시간으로 미분하면,

이다.

이때, Motor는 PWM의 한 파장이 인가될 때마다  만큼 회전하므로

이라 할 수 있다. 이를 물리 해석프로그램을 활용하면 분석을 통해 최적의 f를 도출했다.

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/5d906a5f-c038-4dcd-b0fd-9cf7bee47548/Untitled.png)

결과적으로 평균적인 운동속도가 0.4m/s 임을 구하였고, 이에 맞는 제어 구동 주파수(f)범위가 1000Hz부근임을 확인하였고, 풀리 설계에 필요한 치수값을 계산하였다.
