/**
 * Demo：
 *    测试ESP8266 demo
 *    打印ESP8266模块信息
 *    1.打印Arduino Core For ESP8266 版本,笔者是2.4.2版本
 *    2.打印Flash的唯一性芯片id（读者可以思考一下是否可以用来做点什么唯一性参考）
 *    3.打印Flash实际大小
 *    4.打印IDE配置的使用Flash大小
 *    5.打印IDE配置的Flash连接通信的频率
 *    6.打印Flash连接模式：QIO QOUT DIO DOUT，可以理解为Flash传输速率
 * @author 单片机菜鸟
 * @date 2018/10/22
 */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //使能软件看门狗的触发间隔
  ESP.wdtEnable(5000);
}

void loop() {
  //喂狗
  ESP.wdtFeed();
  FlashMode_t ideMode = ESP.getFlashChipMode();
  String coreVersion = ESP.getCoreVersion();
  Serial.print(F("Arduino Core For ESP8266 Version: "));
  Serial.println(coreVersion);
  Serial.printf("Flash real id（唯一标识符）:   %08X\n", ESP.getFlashChipId());
  Serial.printf("Flash 实际大小: %u KBytes\n", ESP.getFlashChipRealSize()/1024);
  Serial.printf("IDE配置Flash大小: %u KBytes,往往小于实际大小\n", ESP.getFlashChipSize()/1024);
  Serial.printf("IDE配置Flash频率 : %u MHz\n", ESP.getFlashChipSpeed()/1000000);
  Serial.printf("Flash ide mode:  %s\n\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
  
  delay(1000);
}
