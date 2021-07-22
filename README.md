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
- 代码使用doxygen插件格式化.

## 参考文献
[1] [Liniger A ,  Domahidi A ,  Morari M . Optimization‐based autonomous racing of 1:43 scale RC cars[J]. Optimal Control Applications and Methods, 2015, 36(5).](https://onlinelibrary.wiley.com/doi/full/10.1002/oca.2123)