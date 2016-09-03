#include <iostream>
#include <wx/stdpaths.h>

#include "ArtProvider.h"

namespace greenthumb {

wxBitmap ArtProvider::GetBitmap(IconId iconId) {

    wxBitmap bitmap;
    wxString filename("blank.png");

    switch (iconId) {

        case IconId::BLANK:
            filename = "blank.png";
            break;
        case IconId::CLOSE:
            filename = "window-close.png";
            break;
        case IconId::REFRESH:
            filename = "view-refresh.png";
            break;
        case IconId::VIEW_LIST:
            filename = "view-list.png";
            break;
        case IconId::GO_NEXT:
            filename = "go-next.png";
            break;
        case IconId::GO_PREVIOUS:
            filename = "go-previous.png";
            break;
        case IconId::GO_FIRST:
            filename = "go-first.png";
            break;
        case IconId::GO_LAST:
            filename = "go-last.png";
            break;
        case IconId::TICK_GREEN:
            filename = "tick-green.png";
            break;
        case IconId::TICK_GREY:
            filename = "tick-grey.png";
            break;
        case IconId::SERVER_GO:
            filename = "server-go.png";
            break;
        case IconId::GREENTHUMB:
            filename = "greenthumb.png";
            break;
        case IconId::CHART:
            filename = "chart.png";
            break;
    }

    wxStandardPaths sp = wxStandardPaths::Get();
    wxString bitmapFilename = sp.GetResourcesDir() + wxFILE_SEP_PATH + filename;

    if (!wxFileExists(bitmapFilename)) {
        wxString artDir("art");
        bitmapFilename = artDir + wxFILE_SEP_PATH + filename;
    }

    return wxBitmap(bitmapFilename, wxBITMAP_TYPE_PNG);

}

}
