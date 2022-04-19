#ifndef TESTFUNCTIONS_HPP
#define TESTFUNCTIONS_HPP

#define TEST_MODE 0
#ifndef TEST_MODE
#define TEST_MODE

namespace test {
    void showSizeDirectives(void) {
        std::cout << "main :" << this->mainDirectives.size() << std::endl;
        std::cout << "event :" << this->eventDirectives.size() << std::endl;
        std::cout << "http :" << this->httpDirectives.size() << std::endl;
    };
}

#endif // TEST_MODE

#endif // TESTFUNCTIONS_HPP