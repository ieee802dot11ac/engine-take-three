#include "obj.hpp"

std::unique_ptr<IObject> gSceneRootNode(new IObject("[ROOT NODE]"));

IObject::IObject() : mName("[unnamed]") {}

IObject::IObject(const char* name) : mName(name) {}

IObject::~IObject() {
    mChildObjs.clear();
}

void IObject::Print(std::ostream& stream) const {
    static int indent_lvl = 0;

    for (int _ = 0; _ < indent_lvl; _++) stream << '\t';
    stream << mName << " (" << ClassName() << ")\n";
    
    for (int _ = 0; _ < indent_lvl; _++) stream << '\t';
    if (!mChildObjs.empty()) {
        indent_lvl++;
        stream << "Children:\n";
        for (auto& child : mChildObjs) {
            child->Print(stream);
            stream << '\n';
        }
        indent_lvl--;
    } else {
        stream << "Leaf node." << std::endl;
    }
}
