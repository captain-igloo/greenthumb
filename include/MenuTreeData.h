#ifndef MENUTREEDATA_H
#define MENUTREEDATA_H

#include <greentop/menu/Node.h>

#include <wx/treectrl.h>

namespace greenthumb {

class MenuTreeData : public wxTreeItemData {
    public:

        MenuTreeData();

        MenuTreeData(const greentop::menu::Node& node);

        const greentop::menu::Node node;
        bool valid = false;

    protected:
    private:

};

}

#endif // MENUTREEDATA_H
