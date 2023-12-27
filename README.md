If you want to see more information? ⤵️
<br\><br\>
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
