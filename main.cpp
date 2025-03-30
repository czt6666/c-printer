#include <iostream>
#include <vector>

int main()
{
    // 1. 创建一个空的 vector
    std::vector<int> vec;

    // 2. 向 vector 中添加元素
    vec.push_back(10); // 在末尾添加元素 10
    vec.push_back(20); // 在末尾添加元素 20

    // 3. 访问 vector 中的元素
    std::cout << "第一个元素: " << vec[0] << std::endl;    // 使用下标访问元素
    std::cout << "第二个元素: " << vec.at(1) << std::endl; // 使用 at 方法访问元素

    // 4. 获取 vector 的大小
    std::cout << "vector 的大小: " << vec.size() << std::endl;

    // 5. 遍历 vector
    for (int i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // 范围 for 循环遍历
    for (const auto &element : vec)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    // 6. 删除 vector 中的元素
    vec.pop_back(); // 删除最后一个元素
    std::cout << "删除最后一个元素后，vector 的大小: " << vec.size() << std::endl;

    return 0;
}