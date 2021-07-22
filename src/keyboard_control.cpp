#include "keyboard_control.h"

namespace scut_racing
{
    bool KeyBoardControl::getCMDFromConsole()
    {
        // initialize
        D_ = 0;
        delta_ = 0;
        is_horn_ = 0;
        is_emergency_ = 0;

        int delay_count = 0;
        int press_count_go = 0;
        int press_count_turn = 0;
        bool exit_flag = 1;
        while (exit_flag && press_count_go < PRESS_COUNT_GO && press_count_turn < PRESS_COUNT_TURN) // 超过一定时间无按键输入车辆继续移动
        {
            while (!tools::keyboard::kbhit2())
            {
                delay_count++;
                if (delay_count > TIME_DELAY_COUNT)
                    break;
            }

            if (delay_count > TIME_DELAY_COUNT)
                break;

            char c_press = tools::keyboard::_getchar2(); // 有按键输入
            c_press = tolower(c_press);

            switch (c_press)
            {
            case KB_UP:
                press_count_go++;
                delay_count = 0; // 重新初始化
                D_ += 0.2 * press_count_go;
                break;
            case KB_DOWN:
                press_count_go++;
                delay_count = 0;
                D_ -= 0.2 * press_count_go;
                break;
            case KB_LEFT:
                press_count_turn++;
                delay_count = 0;
                delta_ += (0.5 / PRESS_COUNT_TURN);
                break;
            case KB_RIGHT:
                press_count_turn++;
                delay_count = 0;
                delta_ -= (0.5 / PRESS_COUNT_TURN);
                break;
            case KB_P:
                // show trajectory
                setShowTraj();
                exit_flag = 0;
                break;
            case KB_SPACE:
                // Emergency Stop!
                setEmergency();
                D_ = -1;
                delta_ = 0;
                exit_flag = 0;
                break;
            case KB_H:
                // horn
                setHorn();
                exit_flag = 0;
                break;
            case KB_Q:
                // quit
                return 0;
            default:
                break;
            }
        }
        cout << "Control Command: D -> " << D_ << endl;
        cout << "             Delta -> " << delta_ << endl;
        return 1;
    }
} // namespace scut_racing
