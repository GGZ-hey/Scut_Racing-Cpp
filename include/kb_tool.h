#ifndef KB_TOOL_H
#define KB_TOOL_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

namespace scut_racing
{
    namespace tools
    {
        namespace keyboard
        {
            static struct termios oldt, newt;
            char _getchar2(void);
            int kbhit2(void); // 判断键盘是否有输入
                              // 与put搭配

            // 下面几个搭配使用
            static struct termios initial_settings, new_settings;
            static int peek_character = -1;
            int kbhit(void);
            int _getch(void);
            void init_keyboard();
            void close_keyboard();
        }
    }
}
#endif