#include "tire.h"

namespace scut_racing
{
    /**
     * @brief 设置轮胎参数
     * 
     * @param filename 
     * @param mass : 前/后轴质量
     */
    void Tire::setTireParam(string filename, double mass)
    {
        std::ifstream in_file(filename);
        if (!in_file)
        {
            std::cout << filename << ":文件不存在!!!" << std::endl;
            return;
        }

        double mu;
        in_file >> B_ >> C_ >> mu;
        D_ = 1.2 * 9.81 * mu * mass;

        // FIXME: 缺少数据数量不匹配检查
    }
} // namespace scut_racing
