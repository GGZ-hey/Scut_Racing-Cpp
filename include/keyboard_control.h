#ifndef KEYBOARD_CONTROL_H
#define KEYBOARD_CONTROL_H

#include "kb_tool.h"
#include <ctype.h>
#include <iostream>

using std::cout;
using std::endl;

namespace scut_racing
{
    const int KB_UP = 119;    // w
    const int KB_DOWN = 115;  // s
    const int KB_LEFT = 97;   // a
    const int KB_RIGHT = 100; // d
    const int KB_P = 112;     // p
    const int KB_H = 104;     // h
    const int KB_Q = 113;     // q
    const int KB_SPACE = 32;  // SPACE

    const int PRESS_COUNT_GO = 3;      // 前进/后退控制平滑
    const int PRESS_COUNT_TURN = 5;    // 转向控制平滑
    const int TIME_DELAY_COUNT = 1000; // 时延

    /**
     * @brief 控制器类，处理控制台输入
     * 
     */
    class KeyBoardControl
    {
    public:
        bool getCMDFromConsole(); // 控制台读取键盘输入

        bool isEmergency() const { return is_emergency_; }
        bool isHorn() const { return is_horn_; }
        bool isShowTraj() const { return is_show_traj_; }
        // getter
        const double getD() const { return D_; }
        const double getDelta() const { return delta_; }

        // setter(for test)
        void setD(double D) { D_ = D; }
        void setDelta(double delta) { delta_ = delta; }
        // setter(used)
        void setEmergency() { is_emergency_ = 1; }
        void setHorn() { is_horn_ = 1; }
        void setShowTraj()
        {
            is_show_traj_ = 1 - is_show_traj_; // 取反
        }

    private:
        double D_;     // 油门开度
        double delta_; // 前轮转角

        bool is_emergency_ = 0; // 紧急制动
        bool is_horn_ = 0;      //　鸣喇叭
        bool is_show_traj_ = 0; // 绘制轨迹
    };
}
#endif