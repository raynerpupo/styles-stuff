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

#if defined xabal
#include "constants_xabal.h"
#elif defined xauce
#include "constants_xauce.h"
#elif defined xavia
#include "constants_xavia.h"
#elif defined xedro
#include "constants_xedro.h"
#else
#include "constants_xilema.h"
#endif


#define BUTTON_RADIUS 2
#define BUTTON_ICON_TEXT_SPACING 3
#define GROUP_BOX_TOP_MARGIN 3
#define GROUP_BOX_TEXT_V_MARGIN 4
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

//Custom Colors, not palette handled
#define COLOR_GROUPBOX_HEADER QColor(255, 255, 255)
#define COLOR_FRAME_BORDER QColor(160, 160, 160)
#define COLOR_SLIDER_GROOVE_BG QColor(255, 255, 255)
#define COLOR_SLIDER_HANDLE_NORMAL_BG QColor(244, 244, 244)
#define COLOR_SLIDER_HANDLE_NORMAL_BORDER COLOR_FRAME_BORDER
#define COLOR_SLIDER_HANDLE_HOVER_BG QColor(222, 222, 222)
#define COLOR_SLIDER_HANDLE_INNER_BORDER QColor(244, 244, 244)
#define COLOR_TAB_NORMAL_UL COLOR_FRAME_BORDER
#define COLOR_TAB_SELECTED_TEXT COLOR_GROUPBOX_TEXT
#define COLOR_TAB_NORMAL_TEXT QColor(61, 61, 61)
#define COLOR_WINDOW_TEXT QColor(61, 61, 61)
#define COLOR_SCROLLBAR_GROOVE QColor(255, 255, 255)
#define COLOR_PALETTE_WINDOW QColor(244, 244, 244)
#define COLOR_COMBOBOX_HOVER QColor(222, 222, 222)
#define COLOR_BUTTON_TEXT QColor(255, 255, 255)
#define COLOR_PALETTE_BUTTON_TEXT QColor(61, 61, 61)
#define COLOR_BUTTON_SHADOW QColor(0, 0, 0, 200)
#define COLOR_BUTTON_FOCUS_FG QColor(200, 200, 200, 100)
#define COLOR_PROGRESSBAR_UND_BG QColor(214, 214, 214)
#define COLOR_MENU_BG QColor(255, 255, 255)
#define COLOR_PALETTE_HIGHLIGHT QColor(222, 222, 222)
#define COLOR_PALETTE_TEXT QColor(61, 61, 61)
#define COLOR_PALETTE_HL_TEXT QColor(50, 50, 50)
#define COLOR_TOOLBAR_BG QColor(235, 235, 235)
#define COLOR_PUSHBUTTON_BORDER_DISABLED QColor(204, 204, 204)
#define COLOR_INDICATORS_BG QColor(255, 255, 255)
#define COLOR_INDICATORS_FG QColor(160, 160, 160)
#define COLOR_FG_DISABLED QColor(204, 204, 204)
#define COLOR_DEFAULT_TEXT QColor(42, 42, 42)
#define COLOR_TOOLBOX_CONTENT_BG QColor(255, 255, 255)
#define COLOR_TOOLBOX_TAB_NORMAL QColor(244, 244, 244)
#define COLOR_TOOLBOX_TAB_SELECTED QColor(255, 255, 255)
#define COLOR_DOCK_TITLE_FG QColor(255, 255, 255)
#define COLOR_SPLITTER_BG QColor(200, 200, 200)
#define COLOR_SPLITTER_DECO_BG COLOR_DOCK_TITLE_BG
#define COLOR_PALETTE_ALTENATE_BASE QColor(235, 235, 235)

#endif // CONSTANTS_H
