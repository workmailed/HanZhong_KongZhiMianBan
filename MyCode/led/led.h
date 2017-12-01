#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED_CPU PCout(13)// PA11

#define JDQ8 PAout(8)// PC13
#define JDQ7 PAout(11)// PC13
#define JDQ6 PAout(12)// PC13
#define JDQ5 PAout(15)// PC13

#define JDQ0 PBout(7)// PC13
#define JDQ1 PBout(6)// PC13
#define JDQ2 PBout(5)// PC13
#define JDQ3 PBout(4)// PC13
#define JDQ4 PBout(3)// PC13
#define JDQ9 PBout(15)// PC13
#define JDQ10 PBout(14)// PC13
#define JDQ11 PBout(13)// PC13

#define XingZou_OR_ShengJiang PAin(5)// PC13
#define ZhiXing_OR_XieXing PAin(6)// PC13
#define XiaJiang PAin(7)// PC13
#define JiTing PBin(0)// PC13
#define KuaiSu_OR_ManSu PBin(1)// PC13
#define QiSheng PBin(10)// PC13
#define YaoGan_Key PBin(11)// PC13
#define YaoKong_OR_KongZhiHe PBin(12)// PC13

extern void Init_LEDpin(void);

#endif


