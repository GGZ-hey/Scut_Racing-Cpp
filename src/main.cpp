/***********************
 ****** test file ******
 ***********************/
// #include "test_vehicle.h"
// #include "test_kb_input.h"

#include <iostream>

#include "vehicle.h"
#include "visual.h"
#include "keyboard_control.h"

using scut_racing::CarState;
using scut_racing::KeyBoardControl;
using scut_racing::RaceCar;
using scut_racing::Visualizer;

void showEmergencyInfo(const KeyBoardControl &controller)
{
    std::cout << "<------------- Now, we get EMERGENCY Flag --------------->" << std::endl;
    std::cout << " ------------- Control cmd:     D = " << controller.getD() << std::endl;
    std::cout << " ------------- Control cmd: Delta = " << controller.getDelta() << std::endl;
    std::cout << "<-------------------------------------------------------->" << std::endl;
}

int main(int argc, char *argv[])
{
    string car_file_name = "../config/car_config.txt";
    string ftire_file_name = "../config/ftire_config.txt";
    string rtire_file_name = "../config/rtire_config.txt";

    RaceCar scut_rc(car_file_name, ftire_file_name, rtire_file_name);

    // set initial state
    CarState init_state;
    init_state.x_ = 0;
    init_state.y_ = 0;
    init_state.phi_ = 0;
    init_state.vx_ = 25;
    init_state.vy_ = 0;
    init_state.r_ = 0;
    scut_rc.setInitialState(init_state);

    KeyBoardControl controller;
    Visualizer tv;

    while (controller.getCMDFromConsole())
    {
        // plot trajectory
        if (controller.isShowTraj())
        {
            // 绘制所行驶的轨迹
            tv.setPlotCumTrajFlag();
        }
        else
        {
            tv.unsetPlotCumTrajFlag();
        }

        if (controller.isEmergency()) // until velocity decrease to 0
            while (scut_rc.getState().vx_ > 0)
            {
                showEmergencyInfo(controller);
                scut_rc.step(controller);
                scut_rc.showPosition();
                tv.visualCar(&scut_rc);
            }
        else if (controller.isHorn())
        {
            scut_rc.horn();
        }
        else
        {
            scut_rc.step(controller);
        }

        // show position
        scut_rc.showPosition();
        tv.visualCar(&scut_rc);
    }

    return 0;
}