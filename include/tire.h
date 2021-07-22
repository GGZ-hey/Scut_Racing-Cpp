#ifndef TIRE_H
#define TIRE_H

#include <string>
#include <fstream>
#include <iostream>

using string = std::string;

namespace scut_racing
{
    /**
     * @brief 创建一个轮胎类，获取轮胎相关数据
     * 
     */
    class Tire
    {
    public:
        Tire() : B_(0), C_(0), D_(0), wide_(0), r_(0) {}
        void setTireParam(string filename, double mass); // 读取文件参数

        // getter
        double getB() const { return B_; }
        double getC() const { return C_; }
        double getD() const { return D_; }

    private:
        double B_, C_, D_; // 轮胎侧向力参数
        float wide_;       // 宽度
        float r_;          // 半径
    };
} // namespace scut_racing

#endif