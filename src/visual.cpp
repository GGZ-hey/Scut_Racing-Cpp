#include "visual.h"

namespace scut_racing
{
    void Visualizer::visualCar(const Vehicle *car)
    {
        plt::cla();
        CarState state = car->getState();
        std::tuple<float, float, float> size = car->getSize();

        std::map<string, string> keywords;

        std::vector<double> state_x_vec;
        std::vector<double> state_y_vec;
        state_x_vec.push_back(state.x_);
        state_y_vec.push_back(state.y_);
        plt::plot(state_x_vec, state_y_vec, "ro");

        plotCarBox(state, size);
        if (is_plot_cumtraj_)
            plotCumTrajectory(car);
        plt::pause(0.000001);
    }

    void Visualizer::plotCarBox(const CarState &state, const std::tuple<float, float, float> &size)
    {
        const float car_l = std::get<0>(size) + 6; // 增大可视尺寸
        const float car_wide = std::get<1>(size) + 6;

        std::vector<double> car_x, car_y;

        const double xtmp1 = std::cos(state.phi_) * car_l;
        const double xtmp2 = std::sin(state.phi_) * car_wide;
        const double ytmp1 = std::sin(state.phi_) * car_l;
        const double ytmp2 = -std::cos(state.phi_) * car_wide;

        car_x.push_back(state.x_ + xtmp1 + xtmp2);
        car_x.push_back(state.x_ + xtmp1 - xtmp2);
        car_x.push_back(state.x_ - xtmp1 - xtmp2);
        car_x.push_back(state.x_ - xtmp1 + xtmp2);
        car_x.push_back(state.x_ + xtmp1 + xtmp2);

        car_y.push_back(state.y_ + ytmp1 + ytmp2);
        car_y.push_back(state.y_ + ytmp1 - ytmp2);
        car_y.push_back(state.y_ - ytmp1 - ytmp2);
        car_y.push_back(state.y_ - ytmp1 + ytmp2);
        car_y.push_back(state.y_ + ytmp1 + ytmp2);

        // plt::plot(car_x, car_y, 'k', 'LineWidth', 1);

        plt::plot(car_x, car_y, "k");
        plt::plot(std::vector<double>(car_x.begin(), car_x.begin() + 1),
                  std::vector<double>(car_y.begin(), car_y.begin() + 1), "b");
        plt::xlim(-300, 300);
        plt::ylim(-300, 300);
    }

    void Visualizer::plotTrajectory(const Vehicle *car)
    {

        const auto traj = car->getTraj();                      // vector
        std::tuple<float, float, float> size = car->getSize(); // size:l,w,h
        for (const auto &little_state : traj)
        {
            plt::cla();
            plotCarBox(little_state, size);
            //plt::show();
            plt::pause(0.0000001);
        }
    }

    void Visualizer::plotCumTrajectory(const Vehicle *car)
    {
        std::vector<double> car_x, car_y;

        const auto &trajs = car->getCumTraj();

        for (const auto &state : trajs)
        {
            car_x.push_back(state.x_);
            car_y.push_back(state.y_);
        }
        plt::plot(car_x, car_y, "b");
    }
}