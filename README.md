# Wireless-Charging 无线充电

## 关于本项目

> 无线充电+恒功率控制+自适应最大功率+超级电容

本项目是基于自适应恒功率控制的超级电容无线充电方案，该系统采用了TI公司的`BQ24640`作为电源管理芯片，并使用`STC8A8K`作为主控芯片，对充电的电压与电流进行监控与闭环控制。

充电效果：在无线充电发射线圈功率限制在30w，10s将5个串联的2.7V 15F超级电容充电充到12V。

由该项目组成的直立节能小车在第十五届全国大学生智能汽车竞赛中获得了全国二等奖的成绩。

## 硬件说明

`Hardware`文件夹中的是恒功率控制板，包含恒功率控制、电流检测、电压检测。

我在自己硬件开发的过程中，遇到过如下问题：

### 输出电压只有1.67V

输出电压只有1.6V左右，要求的充电电压为12v左右，但输出一直为1.67v，Vbat为12v，指示灯PG与STAT均为常亮。原理图几乎和datasheet给的例子一样，就是改了采样电阻为100mΩ，电感只有68uH，以及控制输出电压得R1与R2。可超级电容充电端电压只有1.67V，无电流，详细TI链接🔗[TI：输出电压只有1.67V](https://e2echina.ti.com/support/power-management/f/power-management-forum/190098/bq24640-1-67v)

后来的解决的方法是重新画了一版PCB，按照datasheet中的layout要求，然后此问题就解决了。

### 无线线圈干扰问题

在充电过程中，如果接收线圈内有铜铁异物（PCB敷铜、铁钉、电机），整套充电系统就会崩掉，功率就几乎没有。初步认为是固件控制系统不稳定的问题。此问题暂没有解决。


## 固件说明

固件框架是基于`STC8`开发完成的，想必大家都是从`STC89C51`51单片机一路过来的，`STC8`可以看做是51单片机的升级升级款。在此说明，由于第十五届智能汽车竞赛直立节能组比赛规则限制，主控芯片只能选择宏晶STC系列的。

其中开发环境是`Keil C51`，个人自从接触了JetBrains家的IDE后就再也回不去Keil的远古界面，之后我项目的主控都将会在CLion下开发，主控也会选择ESP32或者STM32系列的芯片。

`Firmware`文件夹中，主要控制代码在`User`目录下，主要控制思想在`isr.c`与`main.c`中体现，`Lib`中是一些库。

## 开发经历

由于比赛要求，想要小车跑得快的话，就必须极大地缩短充电时间，我大概是从2020-05-25开始尝试解决一个个子系统，直到智能车比赛前一天。我大二时候那孜孜不倦的钻研精神一直停在了2020-08-10比赛当天（那天我还去灵隐拜了佛）。期间有很多支持我的人，有些或许已经不在身边了，这是我青春最狂热也最美好的时间点！希望以后的自己能不变初心！加油💪FION！附上我开发过程中的Commit🤩，最后感谢那些一路上支持我的人🥲

```
’648486f,FION,2021-06-26 21:05:58 ,Create LICENSE’
’09c08ec,FION,2020-10-06 16:08:43 ,最后比赛’
’35e1299,FION,2020-08-07 23:16:09 ,正在给董少俊测试，目前稳定，无出现big problem’
’53bea2d,FION,2020-08-07 16:19:04 ,勉强能用，待测试’
’17f1a2d,FION,2020-08-07 16:08:25 ,勉强能用，将自适应与充电判断分离开’
’bd7342c,FION,2020-08-07 11:42:54 ,不稳定，一直在振荡’
’4e2e479,FION,2020-08-06 22:24:25 ,它倒闭了。。。’
’b0691cb,FION,2020-08-06 22:23:02 ,增加定时保存，但不稳定’
’3c1fe94,FION,2020-08-06 22:18:13 ,增加试探范围’
’00793d7,FION,2020-08-06 22:15:50 ,充电试探v1.1，调节速度较快’
’2114ff3,FION,2020-08-06 22:12:34 ,充电试探v0.9，可用，但调节速度慢’
’7cee8cd,FION,2020-08-06 21:46:13 ,自动试探功率v1’
’295c5af,FION,2020-08-06 19:25:14 ,新充电板程序’
’322145b,FION,2020-08-05 19:48:06 ,程序出问题了，power_out为0’
’2ea2491,FION,2020-08-03 22:03:00 ,比较两个pi，考虑使用fyj以前写的pi’
’d5cacee,FION,2020-08-03 19:28:21 ,功率小可以恢复，功率大会一直在稳定与不稳定间徘徊’
’193e2bf,FION,2020-08-02 21:35:12 ,现在不用每次出现充电异常断电重启，只需要将线圈移至正确位置即可使用’
’680552f,FION,2020-08-02 18:48:25 ,换思路，准备写进入暴毙模式退不出来’
’992da21,FION,2020-07-28 19:10:58 ,eeprom每次烧程序被擦除解决’
’69ff009,FION,2020-07-28 17:02:18 ,每次烧写程序，会将eeprom擦除’
’cb43d41,FION,2020-07-28 15:00:05 ,eeprom保存读取数据完成’
’63be4e2,FION,2020-07-27 23:24:49 ,增加eeprom读写，保存小数的时候出现bug’
’da52d21,FION,2020-07-26 03:27:56 ,在赛道上测试，有几次超功率后裂开，问题暴露不明显’
’ae21f3f,FION,2020-07-25 22:39:55 ,增加LED显示充电状态’
’1cf4d33,FION,2020-07-25 16:39:34 ,查出敷铜对线圈的影响，将线圈移至固定杆。 更新充电第二版 in all 2 程序计划大换血，更改控制思路，现不稳定，计划增加充电环节被破坏后的补救措施’
’635df84,FION,2020-07-23 23:17:46 ,yjh帮忙查出来，可能是线圈带载后，电压达不到12v的缘故，重新选线圈’
’9551ec2,FION,2020-07-23 19:48:39 ,出了一个神奇的bug’
’9b82915,FION,2020-07-23 19:35:05 ,hwe v2.0’
’eea1db2,FION,2020-07-22 14:41:13 ,它又好了，找不出上次暴毙的原因’
’f786b29,FION,2020-07-22 13:12:27 ,又倒闭了，查不出问题’
’52facad,FION,2020-07-20 23:24:36 ,change control cycle, 15s’
’c3bd728,FION,2020-07-20 10:24:12 ,裂开了’
’b371267,FION,2020-07-17 21:19:04 ,add in-all pcb doc by lzw’
’811d5c3,FION,2020-07-17 19:57:10 ,change system from pi control to open-loop control’
’92d417a,FION,2020-07-17 14:32:49 ,first success to wireless charge. nice!’
’f8bf191,FION,2020-07-15 22:00:00 ,mlgb hwe couldn't work could charge but when power reach target power, it suddenly disappear.’
’3777df8,FION,2020-07-08 23:31:55 ,finish getting power(not accurate)’
’c00ef7f,FION,2020-07-07 23:20:52 ,keep working directing for power control’
’72267fb,FION,2020-06-30 22:51:16 ,control voltage through pwm, but voltage feedback failed, divider resistance may short’
’d30e8b6,FION,2020-06-30 12:34:10 ,start charging control system design’
’ff293fa,FION,2020-06-18 14:58:37 ,sort hwe & swe’
’fc61c91,FION,2020-06-10 20:09:22 ,stage for safety, when setting voltage overflow, output error’
’b43b7d7,FION,2020-06-01 20:38:32 ,sync zf src code cb32035’
’15b3a0b,FION,2020-06-01 19:15:55 ,start mcp41’
’6d25579,FION,2020-05-25 16:21:45 ,finish dac, set voltage’
’e0e6835,FION,2020-05-25 11:24:46 ,help dsj test pwm, now start test tlc5615’
’8e0e60e,FION,2020-05-20 20:56:20 ,mpu6050 works, barely balanced’
’9cf264a,FION,2020-05-20 16:01:52 ,encode works’
’a37da10,FION,2020-05-18 20:43:39 ,sync zf src code v2.2.1’
’c6bb9b0,FION,2020-05-18 19:13:51 ,upper system shows data but unsmooth’
’aa37b75,FION,2020-05-18 08:55:15 ,commit for stage preventing bomb’
’b6ffa32,FION,2020-05-17 14:58:40 ,upper system no data’
’2c660a2,FION,2020-05-17 14:20:13 ,add VCAN_computer.c compiled’
’ce2096b,FION,2020-05-17 14:02:22 ,pwm control motor ok’
’a0b65e3,FION,2020-05-17 13:50:38 ,add MY_pid.c, compiled’
’b9b9e61,FION,2020-05-17 13:46:38 ,add MY_control.c, no problem’
’a988530,FION,2020-05-13 18:13:11 ,sync zf src code v2.1.4’
’13e0c69,FION,2020-05-13 15:46:13 ,finish eeprom’

```

## 其他的后续再补充，有用的话记得点星星~

