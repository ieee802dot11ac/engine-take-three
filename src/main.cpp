#include "obj/obj.hpp"
#include <iostream>

int main() {
    std::cout << "engine initing, commit " << GIT_COMMIT_HASH << std::endl;
    int i = 0;
    while (i++ < 10) {
        (new Object)->Reparent(gSceneRootNode.get());
    }
    gSceneRootNode->mChildObjs[6]->Reparent(gSceneRootNode->mChildObjs[4]);
    gSceneRootNode->Print(std::cout);
}
