#include <cstdlib>
#include <iostream>
int main()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    std::cerr << "Hello windows\n";
#elif __APPLE__
    std::cerr << "Hello macos\n";
#elif __linux__
    std::cerr << "Hello linux\n";
#endif
    return EXIT_SUCCESS;
}
