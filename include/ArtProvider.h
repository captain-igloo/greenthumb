#ifndef ARTPROVIDER_H
#define ARTPROVIDER_H

#include <wx/bitmap.h>

namespace greenthumb {

class ArtProvider {

    public:

        enum class IconId {
            CLOSE, REFRESH, BLANK, VIEW_LIST, GO_NEXT, GO_PREVIOUS, GO_FIRST, GO_LAST, TICK_GREEN,
            TICK_GREY, SERVER_GO, GREENTHUMB, CHART
        };

        static wxBitmap GetBitmap(IconId iconId);

};

}

#endif // ARTPROVIDER_H
