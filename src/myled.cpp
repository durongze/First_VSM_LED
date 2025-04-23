#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <charconv>
#include "vsm.hpp"

using namespace std;

#define AUTH_KEY    0x12344321

#define PIN_IS_HIGH(pin) (ishigh((pin)->istate()))
#define PIN_IS_LOW(pin)  (islow((pin)->istate()))

class MYLED : public IACTIVEMODEL, public IDSIMMODEL
{
    virtual VOID         initialize   (ICOMPONENT* cpt) ;
    virtual ISPICEMODEL* getspicemodel(CHAR* primitive) ;
    virtual IDSIMMODEL*  getdsimmodel (CHAR* primitive) ;
    virtual VOID         plot         (ACTIVESTATE state) ;
    virtual VOID         animate      (INT element, ACTIVEDATA* newstate) ;
    virtual BOOL         actuate      (WORD key, INT x, INT y, DWORD flags) ;


    virtual INT  isdigital(CHAR *pinname);
    virtual VOID setup    (IINSTANCE *inst, IDSIMCKT *dsim);
    virtual VOID runctrl  (RUNMODES mode);
    virtual VOID actuate  (REALTIME time, ACTIVESTATE newstate);
    virtual BOOL indicate (REALTIME time, ACTIVEDATA *data);
    virtual VOID simulate (ABSTIME time, DSIMMODES mode);
    virtual VOID callback (ABSTIME time, EVENTID eventid);
private:
    IINSTANCE  *m_instance;   //PROSPICE仿真原始模型
    IDSIMCKT   *m_ckt;        //DSIM的数字元件
    ICOMPONENT *m_component;  //PROTEUS内部一个活动组件对象

    IDSIMPIN *m_p;
    IDSIMPIN *m_n;

    int m_stat;
};

// extern "C" IACTIVEMODEL*  __declspec(dllexport) createactivemodel (CHAR *device, ILICENCESERVER *ils){    return new MYLED;}
// extern "C" VOID           __declspec(dllexport) deleteactivemodel (IACTIVEMODEL *model){    delete (MYLED *)model;}

MYLED *g_pLed = NULL;
int g_pLedCnt = 0;

//构造数字电气模型实例
extern "C" IDSIMMODEL __declspec(dllexport)* createdsimmodel(CHAR* device, ILICENCESERVER* ils)
{
    ils->authorize(AUTH_KEY);
    if (g_pLed == NULL) {
        g_pLed = new MYLED;
        g_pLedCnt = 1;
    }
    else {
        g_pLedCnt++;
    }
    return g_pLed;
}

//析构数字电气模型实例
extern "C" VOID __declspec(dllexport) deletedsimmodel(IDSIMMODEL* model)
{
    if (g_pLedCnt > 0) {
        g_pLedCnt--;
    }

    if ((g_pLedCnt == 0) && (g_pLed != NULL)) {
        delete (MYLED*)model;
        g_pLed = NULL;
    }
}


// 绘图模型成员函数 

//当创建模型实例时被调用，做初始化工作
VOID MYLED::initialize(ICOMPONENT* cpt) 
{
    m_component = cpt;
};
//被PROSPICE调用，返回模拟电气模型
ISPICEMODEL *MYLED::getspicemodel(CHAR* primitive)   { return NULL; };
//被PROSPICE调用，返回数字电气模型
IDSIMMODEL  *MYLED::getdsimmodel (CHAR* primitive)   { return this; };
//当原理图需要重绘时被调用
VOID         MYLED::plot         (ACTIVESTATE state) {};
//当相应的电气模型产生活动事件时被调用，常用来更新图形
VOID MYLED::animate(INT element, ACTIVEDATA* newstate) 
{
    m_component->repaint(true);
    if (m_stat) {
        m_component->drawcircle(1, 1, 5);
    } else {
        m_component->drawcircle(1, 1, 1);
    }
};
//用来处理键盘和鼠标事件
BOOL MYLED::actuate(WORD key, INT x, INT y, DWORD flags) 
{
    return false; 
};

// 电气模型成员函数

//数字电路总是返回TRUE
INT MYLED::isdigital (CHAR *pinname)
{
    return 1;
}
//当创建模型实例时被调用，做初始化工作
VOID MYLED::setup (IINSTANCE *inst, IDSIMCKT *dsim)
{
    m_stat = 0;
    m_instance = inst;
    m_ckt = dsim;

    m_p = m_instance->getdsimpin((char*)"in,A,1", true);
    m_p->setstate(FLT);    //FLOAT
    m_n = m_instance->getdsimpin((char*)"out,K,2", true);
    m_n->setstate(FLT);
}
//仿真运行模式控制，交互仿真中每帧开始时被调用
VOID MYLED::runctrl (RUNMODES mode)
{
    
}
//交互仿真时用户改变按键等的状态时被调用
VOID MYLED::actuate (REALTIME time, ACTIVESTATE newstate)
{
    
}
//交互仿真时每帧结束时被调用，通过传递ACTIVEDATA数据与绘图模型通信，从而调用animate()进行绘图
BOOL MYLED::indicate (REALTIME time, ACTIVEDATA *data)
{
    return true;
}
//当引脚状态变化时被调用，主要用来处理数据输入和输出
VOID MYLED::simulate (ABSTIME time, DSIMMODES mode)
{
    if (PIN_IS_LOW(m_n) && PIN_IS_HIGH(m_p)) {
        m_stat = 1;
    } else {
        m_stat = 0;
    }
}
//可通过setcallback()设置在给定时间调用的回调函数
VOID MYLED::callback (ABSTIME time, EVENTID eventid)
{
    
}