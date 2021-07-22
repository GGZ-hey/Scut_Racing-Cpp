#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <cmath>
#include "stdlib.h"
#include "tire.h"
#include "keyboard_control.h"

using string = std::string;

/**
 * @brief create a new namespace -> scut_racing
 * 
 */
namespace scut_racing
{
    const int NUM_STATE = 6; // 为了方便起见定义两个常量
    const int NUM_INPUT = 2;

    /**
     * @brief 为了方便，直接用一个类代替6个位置信息，2个输入信息
     * 同时加入转化的索引
     * 
     */
    struct CarState
    {
        double x_ = 0;   // x坐标(m)
        double y_ = 0;   // y坐标(m)
        double phi_ = 0; // 航偏角(rad)
        double vx_ = 0;  // 汽车坐标系下x方向速度(m/s)
        double vy_ = 0;  // 汽车坐标系下y方向速度(m/s)
        double r_ = 0;   // 角速度(rad/s)
    };
    struct State2VecIndex
    {
        const int x = 0;
        const int y = 1;
        const int phi = 2;
        const int vx = 3;
        const int vy = 4;
        const int r = 5;
    };
    struct Input
    {
        double D_;     // 油门开度，[-1,1]
        double delta_; // 方向盘转角，单位：rad
    };
    struct Input2VecIndex
    {
        const int D = 0;
        const int delta = 1;
    };

    /**
     * @brief 抽象汽车类
     * 
     */
    class Vehicle
    {
    public:
        // Vehicle(); // 抽象类也需要构造函数以防报错
        // setter
        void setInitialState(CarState init_state) { state_ = init_state; }
        // getter
        inline const CarState getState() const;
        std::vector<CarState> getTraj() const { return traj_; }
        std::vector<CarState> getCumTraj() const { return cum_traj_; }
        std::tuple<float, float, float> getSize() const // 获取尺寸
        {
            return std::tuple<float, float, float>(car_long_, car_wide_, car_height_);
        }
        // public method
        virtual void horn() = 0;                    // 鸣笛
        virtual void step(KeyBoardControl cmd) = 0; // 移动一个控制步
    protected:
        // 汽车自身信息
        string producer_; // 生产者
        string color_;    // 颜色

        // 汽车内部信息
        float car_long_;    // 长
        float car_wide_;    // 宽
        float car_height_;  // 高
        float hg_;          // 重心高度
        float wheel_base_;  // 轴距
        float track_b_;     // 轮距
        float l_f_;         // 前轮与重心距离
        float l_r_;         // 后轮与重心距离
        float wind_square_; // 迎风面积
        float m_;           // 重量
        float Iz_;          // 绕z转动惯量
        Tire front_tire_;   // 前轮胎
        Tire rear_tire_;    // 后轮胎
        double ts_ = -1;    // 仿真时间步长

        // 驱动力
        double Cm1_; // 驱动力系数
        double Cm2_;
        double Cr0_;
        double Cr2_;
        double P_tv_; // 力矩系数

        float price_; // 价格

        // 汽车位置信息
        CarState state_;
        std::vector<CarState> traj_;     // 仿真轨迹(局部)
        std::vector<CarState> cum_traj_; // 仿真轨迹(全部)
    };

    /**
     * @brief 继承抽象类的赛车类
     * 
     */
    class RaceCar : public Vehicle
    {
    public:
        RaceCar(string car_filename, string ftire_filename, string rtire_filename); // 从文件中加载参数
        // getter
        inline const string getSponsor() const;

        // public method
        void showPosition();            // 打印位置
        void horn();                    // 鸣笛
        void step(KeyBoardControl cmd); // 移动一个控制步
    private:
        string sponsor_; // 赞助商

        const double MAX_D = 1;       // 最大油门开度
        const double MAX_DELTA = 0.5; // 最大前轮转角rad

        const State2VecIndex sv_index; // 状态量以及对应的索引值
        const Input2VecIndex iv_index; // 输入量以及对应的索引值

        Eigen::Matrix<double, NUM_STATE, 1> state_vector_; // 矩阵形式的状态

        Eigen::Matrix<double, NUM_STATE, 1> state2Vector(CarState cs) const; // struct -> matrix
        Eigen::Matrix<double, NUM_INPUT, 1> input2Vector(Input ipt) const;   // struct -> matrix
        CarState vec2State(Eigen::Matrix<double, NUM_STATE, 1> sv) const;    // matrix -> struct
        Input vec2Input(Eigen::Matrix<double, NUM_INPUT, 1> iv) const;       // matrix -> struct

        Eigen::Matrix<double, NUM_STATE, 1> getF(const Eigen::Matrix<double, NUM_STATE, 1> &xk,
                                                 const Eigen::Matrix<double, NUM_INPUT, 1> &uk) const; // 求微分
        CarState RK4(const CarState &x, const Input &u, const double ts) const;                        // 龙格-库塔法求解微分方程
        std::vector<CarState> simTimeStep(const CarState &x, const Input &u, const double ts) const;   // 求取仿真轨迹

        friend void changeSponsor(RaceCar &rc, string sponsor); // 更改赞助商
        void showSponsor() const;                               // 打印赞助商信息
    };

    /**
     * @brief 货车类，示范使用
     * 
     */
    class Truck
    {
    public:
        Truck(string filename); // 从文件中加载参数
        // getter
        inline const CarState getPosition();
        // public method
        void horn();                                 // 鸣笛
        void step(KeyBoardControl cmd);              // 移动一个控制步
        void loading(string category, float weight); // 装载
    private:
        float load_;           // 载货量
        string load_category_; // 货物类型
        // 汽车自身信息
        string producer_; // 生产者
        string color_;    // 颜色

        // 汽车内部信息
        float car_long_;    // 长
        float car_wide_;    // 宽
        float car_height_;  // 高
        float hg_;          // 重心高度
        float wheel_base_;  // 轴距
        float track_b_;     // 轮距
        float front_wb_;    // 前轮与重心距离
        float rear_wb_;     // 后轮与重心距离
        float wind_square_; // 迎风面积
        Tire tire_;         // 轮胎
        float price_;       // 价格
        // 汽车位置信息
        CarState state_;
    };

    /**
     * @brief 客车类，示范使用
     * 
     */
    class Coach
    {
    public:
        Coach(string filename); // 从文件中加载参数
        // getter
        inline const CarState getPosition();
        // public method
        void horn();                    // 鸣笛
        void step(KeyBoardControl cmd); // 移动一个控制步
        void getInTheCar(string name);  // 乘客上车
    private:
        int passengers_;            // 载客量
        int limit_passengers_nums_; // 允许载客量
        // 汽车自身信息
        string producer_; // 生产者
        string color_;    // 颜色

        // 汽车内部信息
        float car_long_;    // 长
        float car_wide_;    // 宽
        float car_height_;  // 高
        float hg_;          // 重心高度
        float wheel_base_;  // 轴距
        float track_b_;     // 轮距
        float front_wb_;    // 前轮与重心距离
        float rear_wb_;     // 后轮与重心距离
        float wind_square_; // 迎风面积
        Tire tire_;         // 轮胎
        float price_;       // 价格
        // 汽车位置信息
        CarState state_;
    };

    /**
     * @brief Get the Position object
     * 
     * @return const CarState 
     */
    const CarState Vehicle::getState() const
    {
        return this->state_;
    }
    const string RaceCar::getSponsor() const
    {
        return this->sponsor_;
    }
}

#endif