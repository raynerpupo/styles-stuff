/****************************************************************************
* Copyright (C) 2011-2014 Rayner Pupo Gomez
*
* GNU Lesser General Public License Usage
* This file may be used under the terms of the GNU Lesser
* General Public License version 2.1 as published by the Free Software
* Foundation and appearing in the file LICENSE.LGPL included in the
* packaging of this file.  Please review the following information to
* ensure the GNU Lesser General Public License version 2.1 requirements
* will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BUTTON_RADIUS 2
//#define BORDER_WIDTH 1
#define BUTTON_ICON_TEXT_SPACING 3
//#define BUTTON_CONTENT_MARGINS 3
#define GROUP_BOX_TOP_MARGIN 3
#define GROUP_BOX_TEXT_V_MARGIN 4
//#define GROUP_BOX_TEXT_H_MARGIN 10
#define GROUP_BOX_UNDERLINE_WIDTH 2
#define GROUP_BOX_INDICATOR_MARGIN 4
#define GROUP_BOX_HEADER_MARGINS 3
#define COMBOBOX_BUTTON_WIDTH 25
#define SPINBOX_BUTTON_WIDTH COMBOBOX_BUTTON_WIDTH
#define SPLITTER_IND_WIDTH 30
#define windowsItemFrame 2
#define windowsItemHMargin 3
#define windowsItemVMargin 8
#define windowsRightBorder 15
#define menuArrowHMargin 6
#define menuCheckMarkWidth 12
#define menuRightBorder 15

//Custom Colors, not palette handled    xilema                    xavia
#define COLOR_GROUPBOX_TEXT QColor(0, 105, 170)//                 33,110,18
#define COLOR_GROUPBOX_HEADER QColor(255, 255, 255)
#define COLOR_FRAME_BORDER QColor(160, 160, 160)
#define COLOR_SLIDER_GROOVE_L QColor(80, 160, 205)//              110,200,90
#define COLOR_SLIDER_GROOVE_D QColor(25, 65, 150)//               30,120,25
#define COLOR_SLIDER_GROOVE_BG QColor(255, 255, 255)
#define COLOR_SLIDER_HANDLE_SUNKEN_BG QColor(190, 220, 240)//     196,229,189
#define COLOR_SLIDER_HANDLE_SUNKEN_BORDER QColor(1, 98, 177)//    77,155,62
#define COLOR_SLIDER_HANDLE_NORMAL_BG QColor(244, 244, 244)
#define COLOR_SLIDER_HANDLE_NORMAL_BORDER COLOR_FRAME_BORDER
#define COLOR_SLIDER_HANDLE_HOVER_BG QColor(222, 222, 222)
#define COLOR_SLIDER_HANDLE_INNER_BORDER QColor(244, 244, 244)
#define COLOR_TAB_SELECTED_UL QColor(1, 98, 177)//                77,155,62
#define COLOR_TAB_NORMAL_UL COLOR_FRAME_BORDER
#define COLOR_TAB_SELECTED_TEXT COLOR_GROUPBOX_TEXT
#define COLOR_TAB_NORMAL_TEXT QColor(61, 61, 61)
//#define COLOR_TAB_SEPARATOR COLOR_TAB_NORMAL_UL
#define COLOR_WINDOW_TEXT QColor(61, 61, 61)
#define COLOR_SCROLLBAR_GROOVE QColor(255, 255, 255)
//#define COLOR_SCROLL_GROOVE_DECO COLOR_SLIDER_GROOVE_L
#define COLOR_PALETTE_WINDOW QColor(244, 244, 244)
#define COLOR_COMBOBOX_SUNKEN QColor(190, 220, 240)//             196,229,189
#define COLOR_COMBOBOX_HOVER QColor(222, 222, 222)
#define COLOR_BUTTON_TEXT QColor(255, 255, 255)
#define COLOR_PALETTE_BUTTON_TEXT QColor(61, 61, 61)
#define COLOR_BUTTON_SHADOW QColor(0, 0, 0, 200)
#define COLOR_BUTTON_FOCUS_FG QColor(200, 200, 255, 100)//        200,255,200,100
#define COLOR_PROGRESSBAR_UND_BG QColor(214, 214, 214)
#define COLOR_MENU_BG QColor(255, 255, 255)
#define COLOR_PALETTE_HIGHLIGHT QColor(222, 222, 222)
#define COLOR_PALETTE_TEXT QColor(61, 61, 61)
#define COLOR_PALETTE_HL_TEXT QColor(50, 50, 50)
#define COLOR_INDICATOR_ARROW QColor(50, 100, 230)//              50,230,100*
#define COLOR_TOOLBAR_BG QColor(235, 235, 235)

#define COLOR_PUSHBUTTON_BORDER_NORMAL QColor(0, 60, 112)//       33,110,18
#define COLOR_PUSHBUTTON_BORDER_DISABLED QColor(204, 204, 204)
#define COLOR_PUSHBUTTON_GLOW QColor(138, 199, 253)//             184,226,175

#define COLOR_INDICATORS_BG QColor(255, 255, 255)
#define COLOR_INDICATORS_FG QColor(160, 160, 160)
#define COLOR_FG_DISABLED QColor(204, 204, 204)

#define COLOR_DEFAULT_TEXT QColor(42, 42, 42)

#define COLOR_TOOLBOX_CONTENT_BG QColor(255, 255, 255)
#define COLOR_TOOLBOX_TAB_NORMAL QColor(244, 244, 244)
//#define COLOR_TOOLBOX_TAB_HOVER QColor(222, 222, 222)
#define COLOR_TOOLBOX_TAB_SELECTED QColor(255, 255, 255)

#define COLOR_DOCK_TITLE_BG QColor(1, 98, 177)//                  77,155,62
#define COLOR_DOCK_TITLE_FG QColor(255, 255, 255)

//#define COLOR_SPLITTER_BG QColor(240, 255, 235)
#define COLOR_SPLITTER_BG QColor(200, 200, 200)
#define COLOR_SPLITTER_DECO_BG COLOR_DOCK_TITLE_BG

#define COLOR_HEADER QColor(190, 220, 240)//                      196,229,189
#define COLOR_PALETTE_ALTENATE_BASE QColor(235, 235, 235)

//#define COLOR_TITLEBAR_BG COLOR_DOCK_TITLE_BG
//#define COLOR_TITLEBAR_DISABLED_BG QColor()


//#include <QVariant>
//#include <QColor>


//#define SETBRANCH QString branch = property("branch").toString();


//static int getValue(const QString &propName, const QString &branch) {
//    QString bName = branch.isEmpty() ? "xilema" : branch.toLower();
//    if (propName == "BUTTON_RADIUS")
//        return 2;
//    else if (propName == "BUTTON_ICON_TEXT_SPACING")
//        return 3;
//    else if (propName == "GROUP_BOX_TOP_MARGIN")
//        return 3;
//    else if (propName == "GROUP_BOX_TEXT_V_MARGIN")
//        return 4;
//    else if (propName == "GROUP_BOX_UNDERLINE_WIDTH")
//        return 2;
//    else if (propName == "GROUP_BOX_INDICATOR_MARGIN")
//        return 4;
//    else if (propName == "GROUP_BOX_HEADER_MARGINS")
//        return 3;
//    else if (propName == "COMBOBOX_BUTTON_WIDTH")
//        return 25;
//    else if (propName == "SPINBOX_BUTTON_WIDTH")
//        return 25;
//    else if (propName == "SPLITTER_IND_WIDTH")
//        return 30;
//    else if (propName == "windowsItemFrame")
//        return 2;
//    else if (propName == "windowsItemHMargin")
//        return 3;
//    else if (propName == "windowsItemVMargin")
//        return 8;
//    else if (propName == "windowsRightBorder")
//        return 15;
//    else if (propName == "menuArrowHMargin")
//        return 6;
//    else if (propName == "menuCheckMarkWidth")
//        return 12;
//    else if (propName == "menuRightBorder")
//        return 15;
//}

//static QColor getColor(const QString &cName, const QString &branch) {
//    QString bName = branch.isEmpty() ? "xilema" : branch.toLower();
//    if (cName == "COLOR_GROUPBOX_HEADER")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_FRAME_BORDER" || cName == "COLOR_SLIDER_HANDLE_NORMAL_BORDER" || cName == "COLOR_TAB_NORMAL_UL")
//        return QColor(160, 160, 160);
//    else if (cName == "COLOR_SLIDER_GROOVE_BG")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_SLIDER_HANDLE_NORMAL_BG")
//        return QColor(244, 244, 244);
//    else if (cName == "COLOR_SLIDER_HANDLE_HOVER_BG")
//        return QColor(222, 222, 222);
//    else if (cName == "COLOR_SLIDER_HANDLE_INNER_BORDER")
//        return QColor(244, 244, 244);
//    else if (cName == "COLOR_TAB_NORMAL_TEXT")
//        return QColor(61, 61, 61);
//    else if (cName == "COLOR_WINDOW_TEXT")
//        return QColor(61, 61, 61);
//    else if (cName == "COLOR_SCROLLBAR_GROOVE")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_PALETTE_WINDOW")
//        return QColor(244, 244, 244);
//    else if (cName == "COLOR_COMBOBOX_HOVER")
//        return QColor(222, 222, 222);
//    else if (cName == "COLOR_BUTTON_TEXT")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_PALETTE_BUTTON_TEXT")
//        return QColor(61, 61, 61);
//    else if (cName == "COLOR_BUTTON_SHADOW")
//        return QColor(0, 0, 0, 200);
//    else if (cName == "COLOR_PROGRESSBAR_UND_BG")
//        return QColor(214, 214, 214);
//    else if (cName == "COLOR_MENU_BG")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_PALETTE_HIGHLIGHT")
//        return QColor(222, 222, 222);
//    else if (cName == "COLOR_PALETTE_TEXT")
//        return QColor(61, 61, 61);
//    else if (cName == "COLOR_PALETTE_HL_TEXT")
//        return QColor(50, 50, 50);
//    else if (cName == "COLOR_TOOLBAR_BG")
//        return QColor(235, 235, 235);
//    else if (cName == "COLOR_PUSHBUTTON_BORDER_DISABLED")
//        return QColor(204, 204, 204);
//    else if (cName == "COLOR_INDICATORS_BG")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_INDICATORS_FG")
//        return QColor(160, 160, 160);
//    else if (cName == "COLOR_FG_DISABLED")
//        return QColor(204, 204, 204);
//    else if (cName == "COLOR_DEFAULT_TEXT")
//        return QColor(42, 42, 42);
//    else if (cName == "COLOR_TOOLBOX_CONTENT_BG")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_TOOLBOX_TAB_NORMAL")
//        return QColor(244, 244, 244);
//    else if (cName == "COLOR_TOOLBOX_TAB_SELECTED")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_DOCK_TITLE_FG")
//        return QColor(255, 255, 255);
//    else if (cName == "COLOR_SPLITTER_BG")
//        return QColor(200, 200, 200);
//    else if (cName == "COLOR_PALETTE_ALTENATE_BASE")
//        return QColor(235, 235, 235);

//    if (bName == "xavia") {
//        if (cName == "COLOR_GROUPBOX_TEXT" || cName == "COLOR_TAB_SELECTED_TEXT")
//            return QColor(33, 110, 18);
//        else if (cName == "COLOR_SLIDER_GROOVE_L")
//            return QColor(110, 200, 90);
//        else if (cName == "COLOR_SLIDER_GROOVE_D")
//            return QColor(30, 120, 25);
//        else if (cName == "COLOR_SLIDER_HANDLE_SUNKEN_BG")
//            return QColor(196, 229, 189);
//        else if (cName == "COLOR_SLIDER_HANDLE_SUNKEN_BORDER")
//            return QColor(77, 155, 62);
//        else if (cName == "COLOR_TAB_SELECTED_UL")
//            return QColor(77, 155, 62);
//        else if (cName == "COLOR_COMBOBOX_SUNKEN")
//            return QColor(196, 229, 189);
//        else if (cName == "COLOR_BUTTON_FOCUS_FG")
//            return QColor(200, 255, 200, 100);
//        else if (cName == "COLOR_INDICATOR_ARROW")
//            return QColor(50, 230, 100);
//        else if (cName == "COLOR_PUSHBUTTON_BORDER_NORMAL")
//            return QColor(33, 110, 18);
//        else if (cName == "COLOR_PUSHBUTTON_GLOW")
//            return QColor(184, 226, 175);
//        else if (cName == "COLOR_DOCK_TITLE_BG")
//            return QColor(77, 155, 62);
//        else if (cName == "COLOR_HEADER")
//            return QColor(196, 229, 189);
//    }
//    else/* if (branch == "xilema")*/ {
//        if (cName == "COLOR_GROUPBOX_TEXT" || cName == "COLOR_TAB_SELECTED_TEXT")
//            return QColor(0, 105, 170);
//        else if (cName == "COLOR_SLIDER_GROOVE_L")
//            return QColor(80, 160, 205);
//        else if (cName == "COLOR_SLIDER_GROOVE_D")
//            return QColor(25, 65, 150);
//        else if (cName == "COLOR_SLIDER_HANDLE_SUNKEN_BG")
//            return QColor(190, 220, 240);
//        else if (cName == "COLOR_SLIDER_HANDLE_SUNKEN_BORDER")
//            return QColor(1, 98, 177);
//        else if (cName == "COLOR_TAB_SELECTED_UL")
//            return QColor(1, 98, 177);
//        else if (cName == "COLOR_COMBOBOX_SUNKEN")
//            return QColor(190, 220, 240);
//        else if (cName == "COLOR_BUTTON_FOCUS_FG")
//            return QColor(200, 200, 255, 100);
//        else if (cName == "COLOR_INDICATOR_ARROW")
//            return QColor(50, 100, 230);
//        else if (cName == "COLOR_PUSHBUTTON_BORDER_NORMAL")
//            return QColor(0, 60, 112);
//        else if (cName == "COLOR_PUSHBUTTON_GLOW")
//            return QColor(138, 199, 253);
//        else if (cName == "COLOR_DOCK_TITLE_BG")
//            return QColor(1, 98, 177);
//        else if (cName == "COLOR_HEADER")
//            return QColor(190, 220, 240);
//    }
//}

#endif // CONSTANTS_H
