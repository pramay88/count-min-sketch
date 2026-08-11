#include "cms.hpp"

#include <iostream>

int main()
{
    CountMinSketch cms(1000, 5);

    // Incoming request stream
    cms.update("192.168.1.10");
    cms.update("10.0.0.5");
    cms.update("192.168.1.10");
    cms.update("172.16.0.4");
    cms.update("192.168.1.10");

    std::cout << "192.168.1.10: " << cms.estimate("192.168.1.10") << '\n';

    std::cout << "10.0.0.5: " << cms.estimate("10.0.0.5") << '\n';

    std::cout << "172.16.0.4: " << cms.estimate("172.16.0.4") << '\n';

    return 0;
}