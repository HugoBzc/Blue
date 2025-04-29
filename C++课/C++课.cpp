#include <vector>
#include <cstdint>
#include <algorithm> // for std::copy
#include <iostream>
using namespace std;
    struct point
    {
        uint32_t a7;
        uint32_t a8;
        float a9;
        uint8_t a10;
        uint8_t a11;
        uint8_t a12[6];
    };


// 函数定义：根据 a7 和 a8 的数组构造结构体数组，其他字段固定
std::vector<point> create_point_array(
    const uint32_t a7_values[],
    const uint32_t a8_values[],
    size_t num_points,
    float fixed_a9,
    uint8_t fixed_a10,
    uint8_t fixed_a11,
    const uint8_t fixed_a12[6]) 
{
    std::vector<point> result;
    result.reserve(num_points); // 预分配空间

    for (size_t i = 0; i < num_points; ++i) 
    {
        point p;
        p.a7 = a7_values[i];
        p.a8 = a8_values[i];
        p.a9 = fixed_a9;
        p.a10 = fixed_a10;
        p.a11 = fixed_a11;
        std::copy(fixed_a12, fixed_a12 + 6, p.a12); // 复制数组
        result.push_back(p);
    }

    return result;
}

// 使用示例
int main() {
    // 定义可变字段的数组
    uint32_t a7_values[] = { 1, 12, 100 };
    uint32_t a8_values[] = { 2, 13, 200 };
    uint8_t fixed_a12[] = { 6,7,8,9,10,11 };

    // 动态构造数组（生成3个结构体）
    auto points = create_point_array(
        a7_values,
        a8_values,
        3, // 数量
        3.0f, // a9固定值
        4,    // a10固定值
        5,    // a11固定值
        fixed_a12
    );
    cout>>points[1]. a7
}