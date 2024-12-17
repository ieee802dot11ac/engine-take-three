#include "iface/obj.hpp"
#include <iostream>

int main() {
    std::cout << "hai! :3" << std::endl;
    int i = 0;
    while (i++ < 10) {
        gSceneRootNode->mChildObjs.push_back(std::shared_ptr<IObject>(new IObject));
    }
    gSceneRootNode->Print(std::cout);
}
