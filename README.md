# <center> 小车控制C++例程  
## 简介  
&emsp;&emsp;本项目为华南理工大学赛车队培训期间所编写的C++例程，为华南理工大学无人车队所使用。目的在于培养无人系统队员们养成良好的C++编码规范以及软件设计思想.  

&emsp;&emsp;希望各位队友们能仔细研究并改进里面的代码，不断扩大该项目，为后面进入无人系统的队友提供更多的帮助. 

&emsp;&emsp;该项目依赖以下库:  
> &emsp;Eigen  
> &emsp;[matplotlib-cpp](https://github.com/lava/matplotlib-cpp.git)  

该项目还依赖**sox**，运行前请首先:  
> &emsp;sudo apt-get install sox  
> &emsp;sudo apt-get install libsox-fmt-all  

经验证，该项目代码能在Ubuntu 16.04上运行  
  
附加说明:  
- 数据配置放置于[config文件夹](config)内，详情可查阅[config/README.md](config/README.md).  
- 代码使用**doxygen**插件格式化.  
## 怎么使用  
&emsp;&emsp;首先需要配置Eigen依赖，如果已经安装ROS，则Eigen依赖已经安装好,此处不详细说明。  

&emsp;&emsp;其次是matplotlib-cpp的配置，由于依赖于旧版的matplotlib-cpp，链接放在[这里](https://github.com/GGZ-hey/matplotlib-cpp-py2version)，也可以这样操作，在当前目录:  
> mkdir External  
> cd External  
> git clone https://github.com/GGZ-hey/matplotlib-cpp-py2version.git  
> mv matplotlib-cpp-py2version matplotlib  

接着需要编译:  
> cd ..  
> mkdir build && cd build && cmake ..  
> make install  

你将会产生bin文件夹在工作目录，运行即可  
> cd ../bin  
> ./run_me

## 功能
> w    : 上    
> s    : 下    
> a    : 左  
> d    : 右  
> SPACE: 紧急制动  
> p    : 绘制轨迹  
> h    : 鸣喇叭  
> q    : 退出  
## 参考文献  
[1] [Liniger A ,  Domahidi A ,  Morari M . Optimization‐based autonomous racing of 1:43 scale RC cars[J]. Optimal Control Applications and Methods, 2015, 36(5).](https://onlinelibrary.wiley.com/doi/full/10.1002/oca.2123)