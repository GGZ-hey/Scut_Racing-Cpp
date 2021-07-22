#ifndef VISUAL_H
#define VISUAL_H

#include "matplotlibcpp.h"
#include "vehicle.h"

namespace scut_racing
{
	namespace plt = matplotlibcpp;

	/**
     * @brief Visualize the car
     * 
     */
	class Visualizer
	{
	public:
		Visualizer() {}
		Visualizer(int wide, int height) : size_w_(wide), size_h_(height) {}

		void visualCar(const Vehicle *car);			// 绘制车辆行驶路径以及车体
		void plotTrajectory(const Vehicle *car);	// 绘制轨迹(局部)
		void plotCumTrajectory(const Vehicle *car); // 绘制轨迹(全部)

		// setter
		void setPlotCumTrajFlag() { is_plot_cumtraj_ = 1; }
		void unsetPlotCumTrajFlag() { is_plot_cumtraj_ = 0; }

	private:
		int size_w_;
		int size_h_;

		bool is_plot_cumtraj_ = 0;													// 是否绘制全部轨迹
		void plotCarBox(const CarState &, const std::tuple<float, float, float> &); // 绘制车体
		void initFig();																// 画布初始化
	};
} // namespace scut_racing
#endif