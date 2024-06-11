#include <fstream>
#include <iostream>
#include <sstream>

template <typename... Args>
int getLineFromCin(const std::string& filePath, Args... args)
{
    std::ofstream ofs(filePath, std::fstream::app);
    if (ofs) {
        std::stringstream ss;
        (ss << ... << args); // ʹ���۵����ʽ�����в���д��ostringstream
        ofs << ss.str() << std::endl;
        ofs.close();
        return EXIT_SUCCESS;
    } else {
        std::cerr << "�ļ���ʧ��!\n";
        return EXIT_FAILURE;
    }
}

int main()
{
    getLineFromCin("./output.txt", "hello world", 666, 1111);
    return 0;
}
