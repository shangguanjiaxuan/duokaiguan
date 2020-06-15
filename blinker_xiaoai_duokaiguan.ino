/*
  五路开关模式
  1~4对应各路开关
  01/01s只有4个io口  所以只能输出4路
*/
#define BLINKER_WIFI
#define BLINKER_MIOT_MULTI_OUTLET                                               //设置为小爱多个插座的模式
#include <Blinker.h>
BlinkerButton Button0("Q");
BlinkerButton Button1("W");                                                     //设置blinkerapp内数据键名
BlinkerButton Button2("E");            
BlinkerButton Button3("R");
char i;
char auth[] = "***";                                                            //点灯app内申请的秘钥
char ssid[] = "***";                                                            //WiFi名
char pswd[] = "***";                                                            //WiFi密码
bool oState[5] = { false };                                                     //设置5路开关控制
void miotPowerState(const String & state, uint8_t num)                          //小爱同学控制指令
{
  BLINKER_LOG("need set outlet: ", num, ", power state: ", state);              //每路开关通道号
  if        (num == 0)
  {
    i = 0;
  }
  else if   (num == 1)
  {
    i = 1;
  }
  else if   (num == 2)
  {
    i = 2;
  }
  else if    (num == 3)
  {
    i = 3;
  }

  if (state == BLINKER_CMD_ON) {
    digitalWrite(i, HIGH);
    BlinkerMIOT.powerState("on");
    BLINKER_LOG("亮灯!");
    BlinkerMIOT.print();
    oState[num] = false;
  }
  else if (state == BLINKER_CMD_OFF) {
    digitalWrite(i, LOW);
    BlinkerMIOT.powerState("off");
    BLINKER_LOG("灭灯!");
    BlinkerMIOT.print();
    oState[num] = false;

    if (num == 0)
    {
      for (uint8_t o_num = 0; o_num < 5; o_num++)
      {
        oState[o_num] = false;
      }
    }
  }
}

void miotQuery(int32_t queryCode, uint8_t num)
{
  BLINKER_LOG("AliGenie Query outlet: ", num, ", codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
  }
}

void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

//第0路开关
void button0_callback(const String & state)                                      //点灯app内控制按键触发
{
  if (digitalRead(0) == HIGH) 
  {                                                                              //如果state是on状态
    BLINKER_LOG("灭灯!");                                                        //串口打印
    digitalWrite(0, LOW);                                                       //调整输出为低电平
    Button0.color("#999999");                                                   //设置app按键是深蓝色
    Button0.text("灭");
    Button0.print("off");                                                       //反馈回按键状态是开
  }
  else if (digitalRead(0) == LOW) 
  {                                                                             //如果state是off状态
    BLINKER_LOG("亮灯!");                                                        //串口打印
    digitalWrite(0, HIGH);                                                      //调整输出为高电平
    Button0.color("#0066FF");                                                   //设置app按键是浅蓝色
    Button0.text("亮");
    Button0.print("on");                                                        //反馈回按键状态是关
  }

}
//第1路开关
void button1_callback(const String & state)     
{
  if (digitalRead(1) == HIGH) {          
    BLINKER_LOG("灭灯!");                           
    digitalWrite(1, LOW);
    Button1.color("#999999");                     
    Button1.text("灭");
    Button1.print("off");                         
  }
  else if (digitalRead(1) == LOW) {     
    BLINKER_LOG("亮灯!");                       
    digitalWrite(1, HIGH);
    Button1.color("#0066FF");               
    Button1.text("亮");
    Button1.print("on");                    
  }
}
//第2路开关
void button2_callback(const String & state)   
{
  if (digitalRead(2) == HIGH) {           
    BLINKER_LOG("灭灯!");                       
    digitalWrite(2, LOW);
    Button2.color("#999999");                   
    Button2.text("灭");
    Button2.print("off");                     
  }
  else if (digitalRead(2) == LOW) {     
    BLINKER_LOG("亮灯!");                     
    digitalWrite(2, HIGH);
    Button2.color("#0066FF");                
    Button2.text("亮");
    Button2.print("on");                    
  }
}
//第3路开关
void button3_callback(const String & state)     
{
  if (digitalRead(3) == HIGH) {           
    BLINKER_LOG("灭灯!");                         
    digitalWrite(3, LOW);
    Button3.color("#999999");                     
    Button3.text("灭");
    Button3.print("off");                       
  }
  else if (digitalRead(3) == LOW) {    
    BLINKER_LOG("亮灯!");                        
    digitalWrite(3, HIGH);
    Button3.color("#0066FF");               
    Button3.text("亮");
    Button3.print("on");                      
  }
}
void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);
  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachQuery(miotQuery);
  Button0.attach(button0_callback);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
  Button3.attach(button3_callback);
}
void loop()
{
  Blinker.run();
}
