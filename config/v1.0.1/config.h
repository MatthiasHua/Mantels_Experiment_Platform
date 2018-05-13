#ifndef config_h
#define config_h

//==================================

//通用设置
//此号码为测试用
#define Token "X4UIl1Vruh4vFsEo"
#define Number 123
#define Device_Name "test-device-1"

//wifi设置
#define dSSID "ssid";
#define dPASSWORD "password";

//是否打开DEUG模式
#define DEBUG 0

//==================================

//硬件配置
//根据所选硬件平台配置

//Mantels Experimente Platform v1.0.1

//OLED屏 引脚
#define SDA_PIN 13// OLED SDA
#define SCL_PIN 12 // OLED SCL

//输入 引脚
#define KB_CLK 4
#define KB_SH 5
#define KB_QH 14

//输出 引脚
#define O_RCLK 3
#define O_SRCLK 1
#define O_SER 16

//位数
#define Order_Bits 8

//键盘
//首地址
#define Keyboard_Address 8
//上
#define KB_Up 0
//下
#define KB_Down 2
//左
//右
//确定
#define KB_Enter 4
//取消
#define KB_Cancel 5

//LED灯 首地址
#define LED_Address 16
//钮子开关 首地址
#define Toggle_Switch_Address 8

//==================================

#endif
