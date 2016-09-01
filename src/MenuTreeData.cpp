#include "MenuTreeData.h"

namespace greenthumb {

MenuTreeData::MenuTreeData() {
}

MenuTreeData::MenuTreeData(const greentop::menu::Node& node) : node(node), valid(true) {

}

}
