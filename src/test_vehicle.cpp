#include "test_vehicle.h"
#include <unistd.h>

void scut_racing::test_func()
{
    string car_file_name = "../config/car_config.txt";
    string ftire_file_name = "../config/ftire_config.txt";
    string rtire_file_name = "../config/rtire_config.txt";

    // 获取当前路径
    char *buffer;
    buffer = getcwd(NULL, 0);
    printf("current dir is ------------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %s\n", buffer);
    free(buffer);

    auto rc = scut_racing::RaceCar(car_file_name, ftire_file_name, rtire_file_name);
    auto cmd = scut_racing::KeyBoardControl();
    Visualizer tv(1200, 800);
    CarState init_state;
    init_state.x_ = 0;
    init_state.y_ = 0;
    init_state.phi_ = 0;
    init_state.vx_ = 30;
    init_state.vy_ = 0;
    init_state.r_ = 0;
    rc.setInitialState(init_state);
    for (int i = 0; i < 100; ++i)
    {
        cmd.setD(1);
        cmd.setDelta(0.1);
        rc.step(cmd);
        rc.showPosition();
        std::cout << std::endl;
        tv.visualCar(&rc);
    }

    cmd.setD(1);
    cmd.setDelta(0.5);
    rc.step(cmd);
    rc.showPosition();
    std::cout << std::endl;
    //tv.plotTrajectory(&rc);

    cmd.setD(-1);
    cmd.setDelta(-0.5);
    rc.step(cmd);
    rc.showPosition();
    std::cout << std::endl;
    //tv.plotTrajectory(&rc);

    //tv.visualCar(&rc);
    return;
}