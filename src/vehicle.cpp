#include "vehicle.h"

namespace scut_racing
{
    RaceCar::RaceCar(string car_filename, string ftire_filename, string rtire_filename)
    {
        std::ifstream in_file(car_filename);
        if (!in_file)
        {
            std::cout << car_filename << ":文件不存在!!!" << std::endl;
            return;
        }

        in_file >> car_long_ >> car_wide_ >> car_height_ >> hg_ >> wheel_base_ >> track_b_ >> l_f_ >>
            l_r_ >> wind_square_ >> m_ >> Iz_ >> Cm1_ >> Cm2_ >> Cr0_ >> Cr2_ >> P_tv_;
        std::cout << "the car's mass is: " << m_ << " kg" << std::endl;
        producer_ = "SCUT Racing";
        color_ = "blue";
        ts_ = 0.05;
        price_ = 1e10;

        double mass_front = m_ * l_r_ / wheel_base_;
        double mass_rear = m_ * l_f_ / wheel_base_;
        std::cout << "the car front mass is : " << mass_front << " kg" << std::endl;
        std::cout << "the car rear mass is : " << mass_rear << " kg" << std::endl;

        front_tire_.setTireParam(ftire_filename, mass_front);
        rear_tire_.setTireParam(rtire_filename, mass_rear);

        // FIXME: 缺少数据数量不匹配检查
    }
    void RaceCar::showPosition()
    {
        std::cout << "my state is:" << std::endl;
        std::cout << "x: " << state_.x_ << std::endl;
        std::cout << "y: " << state_.y_ << std::endl;
        std::cout << "phi: " << state_.phi_ << std::endl;
        std::cout << "vx: " << state_.vx_ << std::endl;
        std::cout << "vy: " << state_.vy_ << std::endl;
        std::cout << "omega: " << state_.r_ << std::endl;
        std::cout << "that's all!" << std::endl;
    }

    void RaceCar::horn()
    {
        system("play ../Music/horn.wav");
    }

    Eigen::Matrix<double, NUM_STATE, 1> RaceCar::state2Vector(CarState cs) const
    {
        Eigen::Matrix<double, NUM_STATE, 1> state_vec;
        state_vec(sv_index.x) = cs.x_;
        state_vec(sv_index.y) = cs.y_;
        state_vec(sv_index.phi) = cs.phi_;
        state_vec(sv_index.vx) = cs.vx_;
        state_vec(sv_index.vy) = cs.vy_;
        state_vec(sv_index.r) = cs.r_;
        return state_vec;
    }

    CarState RaceCar::vec2State(Eigen::Matrix<double, NUM_STATE, 1> sv) const
    {
        CarState state;
        state.x_ = sv(sv_index.x);
        state.y_ = sv(sv_index.y);
        state.phi_ = sv(sv_index.phi);
        state.vx_ = sv(sv_index.vx);
        state.vy_ = sv(sv_index.vy);
        state.r_ = sv(sv_index.r);

        return state;
    }

    Eigen::Matrix<double, NUM_INPUT, 1> RaceCar::input2Vector(Input ipt) const
    {
        Eigen::Matrix<double, NUM_INPUT, 1> iv;
        iv(iv_index.D) = ipt.D_;
        iv(iv_index.delta) = ipt.delta_;
        return iv;
    }

    Input RaceCar::vec2Input(Eigen::Matrix<double, NUM_INPUT, 1> iv) const
    {
        Input ipt;
        ipt.D_ = iv(iv_index.D);
        ipt.delta_ = iv(iv_index.delta);

        return ipt;
    }

    /**
     * @brief 微分
     * 
     * @param xk : 状态量
     * @param uk : 输入量
     * @return Eigen::Matrix<double, NUM_STATE, 1> : 状态量微分值 
     */
    Eigen::Matrix<double, NUM_STATE, 1> RaceCar::getF(const Eigen::Matrix<double, NUM_STATE, 1> &xk,
                                                      const Eigen::Matrix<double, NUM_INPUT, 1> &uk) const
    {
        // assert xk.vx >= 0 m/s
        const double &phi = xk(sv_index.phi);
        const double &v_x = xk(sv_index.vx);
        const double &v_y = xk(sv_index.vy);
        const double &omega = xk(sv_index.r);

        const double &D = uk(iv_index.D);
        const double &delta = uk(iv_index.delta);

        const double alpha_f = -std::atan2(l_f_ * omega + v_y, v_x) + delta;
        const double alpha_r = std::atan2(l_r_ * omega - v_y, v_x);

        const double F_fy = front_tire_.getD() * sin(front_tire_.getC() * atan(front_tire_.getB() * alpha_f));
        const double F_ry = rear_tire_.getD() * sin(rear_tire_.getC() * atan(rear_tire_.getB() * alpha_r));

        const double F_rx = (Cm1_ * D - Cm2_ * D * v_x - Cr0_ - Cr2_ * v_x * v_x);

        Eigen::Matrix<double, NUM_STATE, 1> f = Eigen::Matrix<double, NUM_STATE, 1>::Zero();
        f(0) = v_x * cos(phi) - v_y * sin(phi);
        f(1) = v_y * cos(phi) + v_x * sin(phi);
        f(2) = omega;
        f(3) = 1 / m_ * (F_rx - F_fy * sin(delta) + m_ * v_y * omega);
        f(4) = 1 / m_ * (F_ry + F_fy * cos(delta) - m_ * v_x * omega);
        f(5) = 1 / Iz_ * (F_fy * l_f_ * cos(delta) - F_ry * l_r_) + P_tv_ / Iz_ * (delta * v_x / (l_f_ + l_r_) - omega);

        return f;
    }

    /**
     * @brief RK4微分方程组求解方法
     * 
     * @param x : 状态量
     * @param u : 输入量
     * @param ts : 时间步长
     * @return CarState : 时间步长后的状态量
     */
    CarState RaceCar::RK4(const CarState &x, const Input &u, const double ts) const
    {
        // 4th order Runge Kutta (RK4) implementation
        // 4 evaluation points of continuous dynamics
        const auto x_vec = state2Vector(x);

        const auto u_vec = input2Vector(u);
        // evaluating the 4 points
        const Eigen::Matrix<double, NUM_STATE, 1> k1 = getF(x_vec, u_vec);
        const Eigen::Matrix<double, NUM_STATE, 1> k2 = getF(x_vec + ts / 2. * k1, u_vec);
        const Eigen::Matrix<double, NUM_STATE, 1> k3 = getF(x_vec + ts / 2. * k2, u_vec);
        const Eigen::Matrix<double, NUM_STATE, 1> k4 = getF(x_vec + ts * k3, u_vec);
        // combining to give output
        const Eigen::Matrix<double, NUM_STATE, 1> x_next_vec = x_vec + ts * (k1 / 6. + k2 / 3. + k3 / 3. + k4 / 6.);

        CarState x_next;
        x_next.x_ = x_next_vec(0);
        x_next.y_ = x_next_vec(1);
        x_next.phi_ = x_next_vec(2);
        x_next.vx_ = x_next_vec(3);
        x_next.vy_ = x_next_vec(4);
        x_next.r_ = x_next_vec(5);

        return x_next;
    }
    /**
     * @brief 仿真主函数
     * 
     * @param x 
     * @param u 
     * @param ts 
     * @return std::vector<CarState> : 仿真轨迹
     */
    std::vector<CarState> RaceCar::simTimeStep(const CarState &x, const Input &u, const double ts) const
    {
        // integrate time step
        double fine_time_step = 0.001;

        std::vector<CarState> traj;
        traj.push_back(x);

        CarState x_next = x;
        const int integration_steps = (int)(ts / fine_time_step); // 0.001s 为一个最小仿真间隔

        if (ts / fine_time_step != integration_steps)
        {
            std::cout << "Warning" << std::endl;
        }

        for (int i = 0; i < integration_steps; ++i)
        {
            x_next = RK4(x_next, u, fine_time_step);
            traj.push_back(x_next);
        }

        return traj;
    }

    void RaceCar::step(KeyBoardControl cmd)
    {
        if (ts_ < 0)
        {
            std::cout << "[ERROE]:Assert(ts >= 0), now lower than 0!!!" << std::endl;
            return;
        }

        double D = cmd.getD();
        double delta = cmd.getDelta();

        // clip
        if (D > MAX_D)
            D = MAX_D;
        else if (D < -MAX_D)
            D = -MAX_D;
        if (delta > MAX_DELTA)
            delta = MAX_DELTA;
        else if (delta < -MAX_DELTA)
            delta = -MAX_DELTA;

        Input ipt;
        ipt.D_ = D;
        ipt.delta_ = delta;

        std::vector<CarState> traj = simTimeStep(state_, ipt, ts_);
        traj_ = traj; // 更新轨迹

        // update state
        state_ = traj.back();
        cum_traj_.push_back(state_);

        // The speed cannot be less than 0
        if (state_.vx_ < 0)
        {
            state_.vx_ = 0;
            state_.vy_ = 0;
            state_.r_ = 0;
        }
    }

    /**
     * @brief 改变赛车赞助商
     * 
     * @param rc :赛车对象
     * @param sponsor : 待改赞助商
     */
    void changeSponsor(RaceCar &rc, string sponsor)
    {
        rc.sponsor_ = sponsor;
    }
} // namespace scut_racing