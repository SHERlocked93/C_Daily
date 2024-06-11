#include <fstream>
#include <iostream>
#include <sstream>

template <typename... Args>
int getLineFromCin(const std::string& filePath, Args... args)
{
    std::ofstream ofs(filePath, std::fstream::app);
    if (ofs) {
        std::stringstream ss;
        (ss << ... << args); // 使用折叠表达式将所有参数写入ostringstream
        ofs << ss.str() << std::endl;
        ofs.close();
        return EXIT_SUCCESS;
    } else {
        std::cerr << "文件打开失败!\n";
        return EXIT_FAILURE;
    }
}

int main()
{
    getLineFromCin("./output.txt", "hello world", 666, 1111);
    return 0;
}
