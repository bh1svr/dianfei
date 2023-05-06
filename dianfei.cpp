#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    int min = 0,max = 10000;
    if (argc == 2)
    {
        max = std::stoi(argv[1]);
    }
    random_device seed;//硬件生成随机数种子
	ranlux48 engine(seed());//利用种子生成随机数引擎
    uniform_int_distribution<> distrib(min, max);//设置随机数范围，并为均匀分布
    int random = distrib(engine);
    if (argc == 2)
    {
        cout << random << endl;
    }
    else
    {
        cout << random / 100 << "." << setw(2) << setfill('0') << random % 100 << endl;//随机数
    }
}