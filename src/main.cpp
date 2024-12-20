#include "obj/obj.hpp"
#include <iostream>

int main() {
    std::cout << "engine initing, commit " << GIT_COMMIT_HASH << std::endl;
    int i = 0;
    while (i++ < 10) {
        gSceneRootNode->mChildObjs.push_back(std::shared_ptr<Object>(new Object));
    }
    gSceneRootNode->Print(std::cout);
}
