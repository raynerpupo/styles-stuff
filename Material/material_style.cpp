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

#include "material_style.h"
#include <QDebug>
#include <QStyleOptionButton>
#include <QPainter>
#include <qdrawutil.h>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QScrollBar>
#include <QProgressBar>
#include <QSplitterHandle>
#include <QResource>
#include <QFontDatabase>
#include <QDirIterator>
#include <QMainWindow>
#include <QToolBox>
#include <QTableWidget>
#include <QLayout>
#include <QPixmap>
#include <qmath.h>
#include <QtMath>

#include "progress_style_animation.h"
#include "style_animation.h"

static const qreal Q_PI   = qreal(3.14159265358979323846);   // pi
//#include <QStyleOptionTabV3>

MaterialStyle::MaterialStyle()
{
    setObjectName(QLatin1String("Fusion"));

    loadCustomFonts();
    groupBoxFont = QFont("Myriad Pro", 10);
    groupBoxFont.setBold(true);
    //    groupBoxFont.setStyleStrategy(QFont::NoAntialias);

    regularFont = QFont("Myriad Pro", 10);
    //    regularFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.7);
    //    regularFont.setWeight(5);
    //    regularFont.setPixelSize(2);
    //    regularFont.setStyleStrategy(QFont::NoAntialias);
}

void MaterialStyle::drawPrimitive(QStyle::PrimitiveElement elem, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (elem) {
    case PE_FrameGroupBox: {
        if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
            QRect fullRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxFrame, widget);
            QRect contentsRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxContents, widget);
//            QRect indRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxCheckBox, widget);
            QRect labelRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxLabel, widget);

            //Draw the border
            painter->save();
            prepareSmothPainter(painter);
            painter->setPen(COLOR_FRAME_BORDER);
//            painter->setPen(QPen(COLOR_FRAME_BORDER, 2));
            painter->setBrush(option->palette.window());
            painter->drawRoundedRect(fullRect.adjusted(0, 0, -1, -1), 4, 4);
            //Draw the header
            if (contentsRect.top() > 8) {
                painter->save();
                painter->setClipRect(0, 0, fullRect.width(), contentsRect.top());
                painter->setBrush(COLOR_GROUPBOX_HEADER);
                painter->drawRoundedRect(fullRect.adjusted(0, 0, -1, -1), 4, 4);
                //Draw the text underline
                if (groupBox->subControls & SC_GroupBoxLabel) {
                    painter->translate(-0.5, -0.5);
                    painter->setPen(QPen(COLOR_FRAME_BORDER, 2));
                    painter->drawLine(0, contentsRect.top() - 1, labelRect.left(), contentsRect.top() - 1);
                    painter->setPen(QPen(COLOR_GROUPBOX_TEXT, 2));
                    painter->drawLine(labelRect.left(), contentsRect.top() - 1, labelRect.right(), contentsRect.top() - 1);
                    painter->setPen(QPen(COLOR_FRAME_BORDER, 2));
                    painter->drawLine(labelRect.right(), contentsRect.top() - 1, fullRect.right(), contentsRect.top() - 1);
                } else if (groupBox->subControls & SC_GroupBoxCheckBox) {
                    painter->translate(-0.5, -0.5);
                    painter->setPen(QPen(COLOR_FRAME_BORDER, 2));
                    painter->drawLine(0, contentsRect.top() - 1, fullRect.right(), contentsRect.top() - 1);
                }
                painter->restore();
            }

            painter->restore();
        }
        break;
    }
    case PE_IndicatorCheckBox: {
        const QStyleOptionButton *checkBox = qstyleoption_cast<const QStyleOptionButton *>(option);
        bool enabled = checkBox->state & State_Enabled;
        QColor fgColor = enabled ? COLOR_INDICATORS_FG : COLOR_FG_DISABLED;
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->translate(0.5, 0.5);
        painter->setPen(fgColor);
        painter->setBrush(COLOR_INDICATORS_BG);
        QRect indRect = subElementRect(SE_CheckBoxIndicator, option, widget);
//        qDebug() << indRect;
        indRect.moveTo(option->rect.topLeft());
        QRect r = indRect.adjusted(2, 2, -2, -2);
//        r.moveTo(0, 0);

        painter->drawRoundedRect(indRect, 2, 2);

        if (checkBox->state & QStyle::State_Off) {
//            QPixmap pxm(":/images/images/check_box_state0.png");
//            qDrawBorderPixmap(painter, indRect, QMargins(2, 2, 2, 2), pxm);

        }
        else if (checkBox->state & QStyle::State_On) {
//            QPixmap pxm(":/images/images/check_box_state1.png");
//            qDrawBorderPixmap(painter, indRect, QMargins(3, 3, 3, 3), pxm);
            painter->fillRect(r, fgColor);
        }
        else {
//            QPixmap pxm(":/images/images/check_box_state2.png");
//            painter->drawPixmap(indRect.topLeft(), pxm);
            r.adjust(0, 0, 1, 1);
            QPainterPath path(r.topLeft());
            path.lineTo(r.bottomLeft());
            path.lineTo(r.bottomRight());
            path.closeSubpath();
            painter->setBrush(fgColor);
            painter->drawPath(path);
        }

        painter->restore();
        break;
    }
    case PE_IndicatorRadioButton: {
        const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option);
        if (btn) {
            bool checked = btn->state & QStyle::State_On;
            bool enabled = btn->state & State_Enabled;
//            QString iconRes = checked ? ":/images/images/qradiobutton_chacked.png" :
//                                        ":/images/images/qradiobutton.png";
//            QIcon icon(iconRes);
//            QIcon::State st = btn->state & QStyle::State_Enabled ? QIcon::On : QIcon::Off;
//            icon.paint(painter, btn->rect, Qt::AlignCenter, QIcon::Normal, st);
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->translate(0.5, 0.5);
            painter->setBrush(COLOR_INDICATORS_BG);
            painter->setPen(QPen(enabled ? COLOR_INDICATORS_FG : COLOR_FG_DISABLED, 1));
            painter->drawEllipse(btn->rect);

            if (checked) {
                painter->setBrush(painter->pen().color());
                painter->drawEllipse(btn->rect.adjusted(3, 3, -3, -3));
            }
            painter->restore();
        }
        break;
    }
    case PE_FrameFocusRect: {
        painter->save();
        prepareSmothPainter(painter);
        painter->setPen(QPen(COLOR_FRAME_BORDER, 1/*, Qt::DashLine, Qt::RoundCap*/));
        painter->setBrush(Qt::NoBrush);
        if (!widget->inherits("QAbstractItemView")) {
            painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, -1), 2, 2);
        }
        painter->restore();
        break;
    }
//    case PE_Frame:{
//        qDebug() << "pe_frame";
//        painter->setPen(COLOR_FRAME_BORDER);
//        painter->setBrush(option->palette.window());
//        painter->drawRect(option->rect.adjusted(0, 0, -1, -1));
//        painter->save();
//        prepareSmothPainter(painter);
//        painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, -1), 8, 8);
//        painter->restore();
//    }
//        break;
//    case PE_FrameMenu:
//        if (const QStyleOptionFrame *frame = qstyleoption_cast<const QStyleOptionFrame *>(option)) {
//            painter->setPen(COLOR_FRAME_BORDER);
//            //TODO only for testing
//            painter->setBrush(Qt::red);
//            painter->drawRect(frame->rect.adjusted(0, 0, -1, -1));
//        }
//        break;
    case PE_PanelMenuBar: {
        //TODO only for testing
        qDebug() << "PE_PanelMenuBar";
        painter->fillRect(option->rect, Qt::green);
    }
        break;
    case PE_PanelMenu: {
        //TODO only for testing
        painter->setPen(COLOR_FRAME_BORDER);
        painter->setBrush(COLOR_MENU_BG);
        painter->drawRect(option->rect.adjusted(0, 0, -1, -1));
    }
        break;
    case PE_FrameTabWidget:
        if (const QStyleOptionTabWidgetFrame *tWidget = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option)) {
        int x, y, x1, y1;
        option->rect.getCoords(&x, &y, &x1, &y1);
        int radius = 4;
        int diam = radius * 2;

        QPainterPath path;
        if (tWidget->shape == QTabBar::RoundedNorth) {
            path.moveTo(x, y);
            path.lineTo(x, y1 - radius);
            path.arcTo(QRect(x, y1 - diam, diam, diam), 180.0, +90.0);
            path.lineTo(x1 - radius, y1);
            path.arcTo(QRect(x1 - diam, y1 - diam, diam, diam), 270, 90);
            path.lineTo(x1, y);
            path.closeSubpath();
        } else if (tWidget->shape == QTabBar::RoundedEast) {
            path.moveTo(x1, y);
            path.lineTo(x + radius, y);
            path.arcTo(QRect(x, y, diam, diam), 90, 90);
            path.lineTo(x, y1 - radius);
            path.arcTo(QRect(x, y1 - diam, diam, diam), 180.0, +90.0);
            path.lineTo(x1, y1);
            path.closeSubpath();
        } else if (tWidget->shape == QTabBar::RoundedSouth) {
            path.moveTo(x1, y1);
            path.lineTo(x1, y + radius);
            path.arcTo(QRect(x1 - diam, y, diam, diam), 0, 90);
            path.lineTo(x + radius, y);
            path.arcTo(QRect(x, y, diam, diam), 90, 90);
            path.lineTo(x, y1);
            path.closeSubpath();
        } else {
            path.moveTo(x, y);
            path.lineTo(x, y1);
            path.lineTo(x1 - radius, y1);
            path.arcTo(QRect(x1 - diam, y1 - diam, diam, diam), 270, 90);
            path.lineTo(x1, y + radius);
            path.arcTo(QRect(x1 - diam, y, diam, diam), 0, 90);
            path.closeSubpath();
        }

        painter->save();
        prepareSmothPainter(painter);
        painter->setPen(COLOR_FRAME_BORDER);
        painter->drawPath(path);
        painter->restore();
    }
        break ;
    case PE_IndicatorTabClose: {
//        QStyleHintReturn *hr = 0;
        if ((option->state & State_Enabled) && (option->state & State_MouseOver))
            proxy()->drawPrimitive(PE_PanelButtonCommand, option, painter, widget);
//        QPixmap px(":/images/images/tab_close.png");
//        QCommonStyle::drawItemPixmap(painter, option->rect, Qt::AlignCenter, px);
        int indWidth = pixelMetric(PM_TabCloseIndicatorWidth, option, widget);
        int indHeight = pixelMetric(PM_TabCloseIndicatorHeight, option, widget);
        QRect indRect(0, 0, indWidth, indHeight);
        indRect.moveCenter(option->rect.center());
        painter->save();
        prepareSmothPainter(painter);
        painter->setPen(QPen(COLOR_FRAME_BORDER, 2));
//        painter->drawRect(indRect);
//        painter->setPen(Qt::blue);
        painter->drawLine(indRect.left(), indRect.top(), indRect.right() + 1, indRect.bottom() + 1);
        painter->drawLine(indRect.right() + 1, indRect.top(), indRect.left(), indRect.bottom() + 1);
        painter->restore();
        break;
    }
    case PE_PanelScrollAreaCorner: {
//        qDebug() << "PE_PanelScrollAreaCorner" << option->rect;
        painter->save();
        painter->setPen(COLOR_FRAME_BORDER);
        painter->setBrush(option->palette.window().color().darker());
        painter->drawRect(option->rect);
        painter->restore();
//        painter->fillRect(QRect(0, 0, 200, 200), Qt::red);
    }
        break;
    case PE_FrameLineEdit: {
        QRect r = option->rect;
        bool hasFocus = option->state & State_HasFocus;

        painter->save();

        painter->setRenderHint(QPainter::Antialiasing, true);
        //  ### highdpi painter bug.
        painter->translate(0.5, 0.5);

        // Draw Outline
        painter->setPen( QPen(hasFocus ? COLOR_TAB_SELECTED_UL : COLOR_FRAME_BORDER));
        painter->setBrush(option->palette.base());
        painter->drawRoundedRect(r.adjusted(0, 0, -1, -1), 4, 4);

        //        if (hasFocus) {
        //            QColor softHighlight = Qt::red;
        //            softHighlight.setAlpha(40);
        //            painter->setPen(softHighlight);
        //            painter->drawRoundedRect(r.adjusted(1, 1, -2, -2), 1.7, 1.7);
        //        }
        // Draw inner shadow
        //        painter->setPen(Qt::blue);
        //        painter->drawLine(QPoint(r.left() + 2, r.top() + 1), QPoint(r.right() - 2, r.top() + 1));

        painter->restore();
    }
        break;
    case PE_IndicatorButtonDropDown:
    case PE_PanelButtonCommand: {
        //TODO hacerlo de verdad
        bool isDefault = false;
//        bool isFlat = false;
        bool hover = option->state & State_MouseOver;
        bool isDown = (option->state & State_Sunken) || (option->state & State_On);
        bool autoRaise = option->state & State_AutoRaise;
        QRect r = option->rect;

        if (const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton*>(option)) {
            isDefault = (button->features & QStyleOptionButton::DefaultButton) && (button->state & State_Enabled);
        }
        if (isDown) {
            painter->setBrush(COLOR_COMBOBOX_SUNKEN);
        }
        else if (hover || !autoRaise){
            painter->setBrush(option->palette.window());
        }
        else break;
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->translate(0.5, 0.5);
        painter->setPen(COLOR_FRAME_BORDER);
        painter->drawRoundedRect(r.adjusted(0, 0, -1, -1), 2, 2);
        painter->restore();
    }
        break;
    case PE_IndicatorArrowUp:
    case PE_IndicatorArrowDown:
    case PE_IndicatorArrowRight:
    case PE_IndicatorArrowLeft:
    {
//        QRect r = option->rect;
//        QPainterPath path;


//        painter->save();
//        painter->setRenderHint(QPainter::Antialiasing);
//        painter->translate(0.5, 0.5);
//        painter->setPen(Qt::NoPen);
//        painter->setBrush(COLOR_TAB_SELECTED_UL);
        Qt::ArrowType aType;
        if (option->rect.width() <= 1 || option->rect.height() <= 1)
            break;
//        int rotation = 0;
        switch (elem) {
        case PE_IndicatorArrowUp:
            aType = Qt::UpArrow;
            break;
        case PE_IndicatorArrowDown:
            aType = Qt::DownArrow;
            break;
        case PE_IndicatorArrowRight:
            aType = Qt::RightArrow;
            break;
        case PE_IndicatorArrowLeft:
            aType = Qt::LeftArrow;
            break;
        default:
            break;
        }
        drawArrow(painter, aType, COLOR_TAB_SELECTED_UL, option->rect);
//        painter->drawPath(path);
//        painter->restore();
    }
        break;
    case PE_IndicatorToolBarSeparator:
    {
        QRect rect = option->rect;
        const int margin = 4;
        painter->setPen(COLOR_FRAME_BORDER);
        if (option->state & State_Horizontal) {
            const int offset = rect.width()/2;
            painter->drawLine(rect.bottomLeft().x() + offset,
                              rect.bottomLeft().y() - margin,
                              rect.topLeft().x() + offset,
                              rect.topLeft().y() + margin);
        } else { //Draw vertical separator
            const int offset = rect.height()/2;
            painter->drawLine(rect.topLeft().x() + margin ,
                              rect.topLeft().y() + offset,
                              rect.topRight().x() - margin,
                              rect.topRight().y() + offset);
        }
    }
        break;
    case PE_IndicatorToolBarHandle:
    {
        //draw grips
        QRect rect = option->rect;
        if (option->state & State_Horizontal) {
            for (int i = -3 ; i < 2 ; i += 3) {
                for (int j = -8 ; j < 10 ; j += 3) {
                    painter->fillRect(rect.center().x() + i, rect.center().y() + j, 2, 2, COLOR_TOOLBAR_BG.lighter(105));
                    painter->fillRect(rect.center().x() + i, rect.center().y() + j, 1, 1, COLOR_TOOLBAR_BG.darker(120));
                }
            }
        } else { //vertical toolbar
            for (int i = -6 ; i < 12 ; i += 3) {
                for (int j = -3 ; j < 2 ; j += 3) {
                    painter->fillRect(rect.center().x() + i, rect.center().y() + j, 2, 2, COLOR_TOOLBAR_BG.lighter(105));
                    painter->fillRect(rect.center().x() + i, rect.center().y() + j, 1, 1, COLOR_TOOLBAR_BG.darker(120));
                }
            }
        }
        break;
    }
    case PE_PanelButtonTool:
        painter->save();
        if ((option->state & State_Enabled || option->state & State_On) || !(option->state & State_AutoRaise)) {
            if (widget && widget->inherits("QDockWidgetTitleButton")) {
                if (option->state & State_MouseOver) {
                    if (const QStyleOptionToolButton *btn = qstyleoption_cast<const QStyleOptionToolButton *>(option)) {
                        QStyleOptionToolButton opt = *btn;
                        QPalette pal = option->palette;
                        pal.setBrush(QPalette::Window, Qt::NoBrush);
                        opt.palette = pal;
                        proxy()->drawPrimitive(PE_PanelButtonCommand, &opt, painter, widget);
                    }
                }
            } else {
//                bool test = option->state & State_MouseOver;
//                qDebug() << test;
                proxy()->drawPrimitive(PE_PanelButtonCommand, option, painter, widget);
            }
        }
        painter->restore();
        break;
//    case PE_PanelToolBar:
//        qDebug() << "PE_PanelToolBar";
//        painter->fillRect(option->rect, Qt::blue);
//        break;
//    case :
//        painter->fillRect(option->rect, Qt::blue);
//        break;
    case PE_FrameWindow:
        painter->setBrush(option->palette.window());
        painter->setPen(COLOR_FRAME_BORDER);
        painter->save();
        prepareSmothPainter(painter);
//        painter->setClipRect(option->rect);
        painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, -1), 4, 4);
        painter->restore();
        break;
    case PE_FrameDockWidget: {
        QRect titleRect = subElementRect(SE_DockWidgetTitleBarText, option, widget);
        painter->setPen(COLOR_FRAME_BORDER);
        painter->setBrush(option->palette.window());
        painter->drawRect(option->rect.adjusted(0, 0, -1, -1));
//        painter->setBrush(Qt::blue);
////        qDebug() << "rrrr" << titleRect;
//        painter->drawRect(titleRect);
    }
        break;
    case PE_IndicatorDockWidgetResizeHandle:
    {
        QStyleOption dockWidgetHandle = *option;
        bool horizontal = option->state & State_Horizontal;
        if (horizontal)
            dockWidgetHandle.state &= ~State_Horizontal;
        else
            dockWidgetHandle.state |= State_Horizontal;
        proxy()->drawControl(CE_Splitter, &dockWidgetHandle, painter, widget);
    }
        break;
    case PE_IndicatorBranch:
        if (!(option->state & State_Children))
            break;
        qDebug() << option->rect;
        if (option->state & State_Open)
            drawPrimitive(PE_IndicatorArrowDown, option, painter, widget);
        else
            drawPrimitive(PE_IndicatorArrowRight, option, painter, widget);
        break;
    default:
        QCommonStyle::drawPrimitive(elem, option, painter, widget);
        break;
    }
}

void MaterialStyle::drawControl(QStyle::ControlElement ce, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (ce) {
    case CE_PushButton:
        if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option)) {
            drawPushButton(btn, painter, widget);
        }
        break;
    case CE_RadioButton:
    case CE_CheckBox: {
        if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option)) {
            bool isRadio = (ce == CE_RadioButton);
            QStyleOptionButton subopt = *btn;
            subopt.rect = subElementRect(isRadio ? SE_RadioButtonIndicator
                                                 : SE_CheckBoxIndicator, btn, widget);
            drawPrimitive(isRadio ? PE_IndicatorRadioButton : PE_IndicatorCheckBox,
                          &subopt, painter, widget);
            subopt.rect = subElementRect(isRadio ? SE_RadioButtonContents
                                                 : SE_CheckBoxContents, btn, widget);
            drawControl(isRadio ? CE_RadioButtonLabel : CE_CheckBoxLabel, &subopt, painter, widget);
            if (btn->state & State_HasFocus) {
                QStyleOptionFocusRect fropt;
                fropt.QStyleOption::operator=(*btn);
                fropt.rect = subElementRect(isRadio ? SE_RadioButtonFocusRect
                                                    : SE_CheckBoxFocusRect, btn, widget);
                drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
            }
        }
    }
        break;
    case CE_RadioButtonLabel:
    case CE_CheckBoxLabel:
        if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option)) {
            uint alignment = visualAlignment(btn->direction, Qt::AlignLeft | Qt::AlignVCenter);

            if (!styleHint(SH_UnderlineShortcut, btn, widget))
                alignment |= Qt::TextHideMnemonic;
            QPixmap pix;
            QRect textRect = btn->rect;
            if (!btn->icon.isNull()) {
                pix = btn->icon.pixmap(btn->iconSize, btn->state & State_Enabled ? QIcon::Normal : QIcon::Disabled);
                proxy()->drawItemPixmap(painter, btn->rect, alignment, pix);
                if (btn->direction == Qt::RightToLeft)
                    textRect.setRight(textRect.right() - btn->iconSize.width() - 4);
                else
                    textRect.setLeft(textRect.left() + btn->iconSize.width() + 4);
            }
            if (!btn->text.isEmpty()){
                drawItemText(painter, textRect, alignment | Qt::TextShowMnemonic,
                             btn->palette, btn->state & State_Enabled, btn->text, QPalette::WindowText);
            }
        }
        break;
    case CE_TabBarTab: {
        //        painter->fillRect(option->rect, QColor(0, 255, 0, 100));
        painter->save();
        QRect rect = option->rect;
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
            bool rtlHorTabs = (tab->direction == Qt::RightToLeft
                               && (tab->shape == QTabBar::RoundedNorth
                                   || tab->shape == QTabBar::RoundedSouth));
            bool selected = tab->state & State_Selected;
            bool lastTab = ((!rtlHorTabs && tab->position == QStyleOptionTab::End)
                            || (rtlHorTabs
                                && tab->position == QStyleOptionTab::Beginning));
            bool firstTab = ((!rtlHorTabs && tab->position == QStyleOptionTab::Beginning)
                             || (rtlHorTabs
                                 && tab->position == QStyleOptionTab::End));
            bool onlyOne = tab->position == QStyleOptionTab::OnlyOneTab;
            bool hover = tab->state & State_MouseOver;
            int tabOverlap = pixelMetric(PM_TabBarTabOverlap, option, widget);
            rect = option->rect.adjusted(0, 0, (onlyOne || lastTab) ? 0 : tabOverlap, 0);

            QRect r2(rect);
            int x1 = r2.left();
            int x2 = r2.right();
            int y1 = r2.top();
            int y2 = r2.bottom();

            QTransform rotMatrix;
            bool flip = false;
            //            painter->setPen(Qt::blue);

            switch (tab->shape) {
            case QTabBar::RoundedNorth:
                break;
            case QTabBar::RoundedSouth:
                rotMatrix.rotate(180);
                rotMatrix.translate(0, -rect.height() + 1);
                rotMatrix.scale(-1, 1);
                painter->setTransform(rotMatrix, true);
                break;
            case QTabBar::RoundedWest:
                rotMatrix.rotate(180 + 90);
                rotMatrix.scale(-1, 1);
                flip = true;
                painter->setTransform(rotMatrix, true);
                break;
            case QTabBar::RoundedEast:
                rotMatrix.rotate(90);
                rotMatrix.translate(0, - rect.width() + 1);
                flip = true;
                painter->setTransform(rotMatrix, true);
                break;
            default:
                painter->restore();
                QCommonStyle::drawControl(ce, tab, painter, widget);
                return;
            }

            if (flip) {
                QRect tmp = rect;
                rect = QRect(tmp.y(), tmp.x(), tmp.height(), tmp.width());
                int temp = x1;
                x1 = y1;
                y1 = temp;
                temp = x2;
                x2 = y2;
                y2 = temp;
            }

            prepareSmothPainter(painter);
            painter->setPen(COLOR_FRAME_BORDER);
            painter->setBrush(selected ? COLOR_GROUPBOX_HEADER : hover ? option->palette.highlight() : option->palette.window());
            QPainterPath path;

            int radius = 4;
            int diam = radius * 2;
            if (firstTab || onlyOne) {
                path.moveTo(x2, y2);
                path.lineTo(x1, y2);
                path.lineTo(x1, y1 + radius);
                path.arcTo(QRect(x1, y1, diam, diam), 180, -90);
            }
            if (!(lastTab || onlyOne)) {
                if (!firstTab) {
                    path.moveTo(x2, y2);
                    path.lineTo(x1, y2);
                    path.lineTo(x1, y1);
                }
                path.lineTo(x2, y1);
            }
            if (lastTab || onlyOne) {
                if (!(firstTab || onlyOne)) {
                    path.moveTo(x2, y2);
                    path.lineTo(x1, y2);
                    path.lineTo(x1, y1);
                }
                path.lineTo(x2 - radius, y1);
                path.arcTo(QRect(x2 - diam, y1, diam, diam), 90, -90);
                path.lineTo(x2, y2);
            }
            painter->drawPath(path);

            int yOffset = (tab->shape == QTabBar::RoundedEast || tab->shape == QTabBar::RoundedSouth) ? 1 : 0;
            painter->setPen(QPen(selected ? COLOR_TAB_SELECTED_UL : COLOR_TAB_NORMAL_UL, 2));
            painter->translate(-0.5, -0.5);
            painter->drawLine(x1 + 1, y2 - yOffset, x2 + 1, y2 - yOffset);
        }

        painter->restore();

        drawControl(CE_TabBarTabLabel, option, painter, widget);
        break;
    }
    case CE_ComboBoxLabel://combopaint
        if (const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
            QRect editRect = proxy()->subControlRect(CC_ComboBox, cb, SC_ComboBoxEditField, widget);
            painter->save();
            painter->setClipRect(editRect);
            if (!cb->currentIcon.isNull()) {
                //                qDebug() << "COMBO ICON";
                QIcon::Mode mode = cb->state & State_Enabled ? QIcon::Normal
                                                             : QIcon::Disabled;
                QPixmap pixmap = cb->currentIcon.pixmap(cb->iconSize, mode);
                QRect iconRect(editRect);
                iconRect.setWidth(cb->iconSize.width() + 4);
                iconRect = alignedRect(cb->direction,
                                       Qt::AlignLeft | Qt::AlignVCenter,
                                       iconRect.size(), editRect);
                if (cb->editable)
                    painter->fillRect(iconRect, cb->palette.brush(QPalette::Base));
                proxy()->drawItemPixmap(painter, iconRect, Qt::AlignCenter, pixmap);

                if (cb->direction == Qt::RightToLeft)
                    editRect.translate(-4 - cb->iconSize.width(), 0);
                else
                    editRect.translate(cb->iconSize.width() + 4, 0);
            }
            if (!cb->currentText.isEmpty() && !cb->editable) {
                //                qDebug() << "COMBO text" << cb->palette.text();
                proxy()->drawItemText(painter, editRect.adjusted(1, 0, -1, 0),
                                      visualAlignment(cb->direction, Qt::AlignLeft | Qt::AlignVCenter),
                                      cb->palette, cb->state & State_Enabled, cb->currentText,
                                      QPalette::Text);
            }
            painter->restore();
        }
        break;
    case CE_TabBarTabShape: {
        painter->fillRect(option->rect, QColor(0, 0, 255, 100));
        break;
    }
    case CE_TabBarTabLabel: {
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
            QStyleOptionTabV3 tabV2(*tab);
            QRect tr = tabV2.rect;
            bool verticalTabs = tabV2.shape == QTabBar::RoundedEast
                    || tabV2.shape == QTabBar::RoundedWest
                    || tabV2.shape == QTabBar::TriangularEast
                    || tabV2.shape == QTabBar::TriangularWest;

            int alignment = Qt::AlignCenter | Qt::TextShowMnemonic;
            if (!proxy()->styleHint(SH_UnderlineShortcut, option, widget))
                alignment |= Qt::TextHideMnemonic;

            if (verticalTabs) {
                painter->save();
                int newX, newY, newRot;
                if (tabV2.shape == QTabBar::RoundedEast || tabV2.shape == QTabBar::TriangularEast) {
                    newX = tr.width() + tr.x();
                    newY = tr.y();
                    newRot = 90;
                } else {
                    newX = tr.x();
                    newY = tr.y() + tr.height();
                    newRot = -90;
                }
                QTransform m = QTransform::fromTranslate(newX, newY);
                m.rotate(newRot);
                painter->setTransform(m, true);
            }
            QRect iconRect;
            tabLayout(&tabV2, widget, &tr, &iconRect);
            tr = subElementRect(SE_TabBarTabText, option, widget); //we compute tr twice because the style may override subElementRect

            if (!tabV2.icon.isNull()) {
                QPixmap tabIcon = tabV2.icon.pixmap(tabV2.iconSize,
                                                    (tabV2.state & State_Enabled) ? QIcon::Normal
                                                                                  : QIcon::Disabled,
                                                    (tabV2.state & State_Selected) ? QIcon::On
                                                                                   : QIcon::Off);
                painter->drawPixmap(iconRect.x(), iconRect.y(), tabIcon);
            }

            QPalette pal = tab->palette;
            bool selected = tabV2.state & State_Selected;
            pal.setColor(QPalette::WindowText, selected ? COLOR_TAB_SELECTED_TEXT :
                                                          COLOR_TAB_NORMAL_TEXT);
            painter->setFont(groupBoxFont);
            drawItemText(painter, tr, alignment, pal, tab->state & State_Enabled, tab->text, QPalette::WindowText);
            if (verticalTabs)
                painter->restore();

            if (tabV2.state & State_HasFocus) {
                const int OFFSET = 1 + pixelMetric(PM_DefaultFrameWidth);

                int x1, x2;
                x1 = tabV2.rect.left();
                x2 = tabV2.rect.right() - 1;

                QStyleOptionFocusRect fropt;
                fropt.QStyleOption::operator=(*tab);
                fropt.rect.setRect(x1 + 1 + OFFSET, tabV2.rect.y() + OFFSET,
                                   x2 - x1 - 2*OFFSET, tabV2.rect.height() - 2*OFFSET);
                drawPrimitive(PE_FrameFocusRect, &fropt, painter, widget);
            }
        }
        break;
    }
    case CE_ProgressBarGroove:{
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->translate(0.5, 0.5);
        painter->setPen(COLOR_FRAME_BORDER);
        painter->setBrush(COLOR_PALETTE_WINDOW);
        painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, -1), 4, 4);
        painter->restore();
    }
        break;
    case CE_ProgressBarContents: {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->translate(0.5, 0.5);
        QRect rect = option->rect;

        if (const QStyleOptionProgressBar *bar = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            bool vertical = (bar->orientation == Qt::Vertical);
            rect = subElementRect(SE_ProgressBarContents, bar, widget);
//            bool inverted = bar->invertedAppearance;
            int minimum = bar->minimum;
            int maximum = bar->maximum;
            bool indeterminate = (minimum == 0 && maximum == 0);
            bool complete = bar->progress == maximum;
            int progress = bar->progress;
            if (minimum < maximum) {
                //parar la animacion si hay
                stopAnimation(option->styleObject);

                QLinearGradient gradient;
                if (!vertical) {
                    gradient.setStart(0, 0);
                    gradient.setFinalStop(rect.right(), 0);
                } else {
                    gradient.setStart(0, rect.bottom());
                    gradient.setFinalStop(0, 0);
                }
                gradient.setColorAt(0, COLOR_SLIDER_GROOVE_L);
                gradient.setColorAt(1, COLOR_SLIDER_GROOVE_D);

                painter->setBrush(gradient);
                painter->setPen(Qt::NoPen);

                painter->save();
                rect.adjust(2, 2, -2, -3);
                int totalLenght = vertical ? rect.height() : rect.width();
                int currentPos = progress * totalLenght / (maximum - minimum);
                QRect clipRect = rect.adjusted(-1, -1, 0, 0);
                if (vertical) {
                    clipRect.setHeight(currentPos);
                    clipRect.moveTop(totalLenght - currentPos + 1);
                } else {
                    clipRect.setWidth(currentPos);
                }
                painter->setClipRect(clipRect);
                painter->drawRoundedRect(rect.adjusted(0, 0, -1, 0), 4, 4);
                painter->restore();

            }
            else { //animacion fula
                //pintando el fondo
                painter->setBrush(COLOR_PROGRESSBAR_UND_BG);
                painter->setPen(Qt::NoPen);
                QRect cRect = rect;
                cRect.adjust(2, 2, -3, -3);
                painter->drawRoundedRect(cRect, 4, 4);
                painter->setClipRect(cRect);

                //pintando las lineas
                painter->setPen(QPen(QColor(90, 90, 90, 80), 8));
                int step = 0;
                if (ProgressStyleAnimation *anim = qobject_cast<ProgressStyleAnimation*>(animation(bar->styleObject))) {
                    step = anim->animationStep() % 22;
                } else {
                    startAnimation(new ProgressStyleAnimation(30, bar->styleObject));
                }
                for (int x = rect.left() - rect.height(); x < rect.right() ; x += 22)
                    painter->drawLine(x + step, rect.bottom() + 1,
                                      x + rect.height() + step, rect.top() - 2);
                }
            }
        }
        painter->restore();    
        break;
    case CE_ProgressBarLabel: {
        if (const QStyleOptionProgressBar *bar = qstyleoption_cast<const QStyleOptionProgressBar *>(option)) {
            bool vertical = (bar->orientation == Qt::Vertical);
            QRect textRect = subElementRect(SE_ProgressBarLabel, bar, widget);
            if (vertical) {
                textRect = QRect(textRect.left(), textRect.top(), textRect.height(), textRect.width());
                QTransform matrix;
                matrix.rotate(90);
//                qDebug() << textRect;
                matrix.translate(0, -textRect.height());
                painter->setTransform(matrix);
            }
            QString text = bar->text;
            if (bar->minimum == 0 && bar->maximum == 0) {
                text = tr("Loading...");
            }
            else if (bar->progress == bar->maximum) {
                text = tr("Complete!");
            }
            painter->save();
            painter->setRenderHint(QPainter::TextAntialiasing);
            QFont shadowFont = regularFont;
//            shadowFont.setWeight(80);
            painter->setFont(shadowFont);
            painter->setPen(QPen(COLOR_BUTTON_SHADOW, 4));
            painter->drawText(textRect.adjusted(2, 2, 0, 0), text, QTextOption(Qt::AlignAbsolute | Qt::AlignHCenter | Qt::AlignVCenter));
            painter->setPen(COLOR_BUTTON_TEXT.darker(110));
            painter->setFont(regularFont);
            painter->drawText(textRect, text, QTextOption(Qt::AlignAbsolute | Qt::AlignHCenter | Qt::AlignVCenter));
            painter->restore();
        }
    }
        break;
    case CE_MenuItem:
        painter->save();
        if (const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
            QColor highlight = option->palette.highlight().color();
//            QColor highlightOutline = highlight.darker(150);
            if (menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
                int w = 0;
                if (!menuItem->text.isEmpty()) {
                    painter->setFont(menuItem->font);
                    proxy()->drawItemText(painter, menuItem->rect.adjusted(5, 0, -5, 0), Qt::AlignLeft | Qt::AlignVCenter,
                                          menuItem->palette, menuItem->state & State_Enabled, menuItem->text,
                                          QPalette::Text);
                    w = menuItem->fontMetrics.width(menuItem->text) + 5;
                }
                painter->setPen(COLOR_FRAME_BORDER);
                bool reverse = menuItem->direction == Qt::RightToLeft;
                painter->drawLine(menuItem->rect.left() + 5 + (reverse ? 0 : w), menuItem->rect.center().y(),
                                  menuItem->rect.right() - 5 - (reverse ? w : 0), menuItem->rect.center().y());
                painter->restore();
                break;
            }
            bool selected = menuItem->state & State_Selected && menuItem->state & State_Enabled;
            if (selected) {
                QRect r = option->rect;
                painter->fillRect(r.adjusted(2, 1, -2, -1), highlight);
//                painter->setPen(Qt::NoPen);
//                const QLine lines[4] = {
//                    QLine(QPoint(r.left() + 1, r.bottom()), QPoint(r.right() - 1, r.bottom())),
//                    QLine(QPoint(r.left() + 1, r.top()), QPoint(r.right() - 1, r.top())),
//                    QLine(QPoint(r.left(), r.top()), QPoint(r.left(), r.bottom())),
//                    QLine(QPoint(r.right() , r.top()), QPoint(r.right(), r.bottom())),
//                };
//                painter->drawLines(lines, 4);
            }
            bool checkable = menuItem->checkType != QStyleOptionMenuItem::NotCheckable;
            bool checked = menuItem->checked;
            bool sunken = menuItem->state & State_Sunken;
            bool enabled = menuItem->state & State_Enabled;

            bool ignoreCheckMark = false;
            int checkcol = qMax(menuItem->maxIconWidth, 20);

            if (qobject_cast<const QComboBox*>(widget))
                ignoreCheckMark = true; //ignore the checkmarks provided by the QComboMenuDelegate

            bool act = menuItem->state & State_Selected;
//            bool hover = menuItem->state & State_MouseOver;
            if (!ignoreCheckMark) {
                // Check
                QRect checkRect(option->rect.left() + 7, option->rect.center().y() - 6, 14, 14);
                checkRect = visualRect(menuItem->direction, menuItem->rect, checkRect);
                if (checkable) {
                    if (menuItem->checkType & QStyleOptionMenuItem::Exclusive) {
                        // Radio button
                        if (checked || sunken) {
                            painter->setRenderHint(QPainter::Antialiasing);
                            painter->setPen(Qt::NoPen);

                            QPalette::ColorRole textRole = !enabled ? QPalette::Text:
                                                                      selected ? QPalette::HighlightedText : QPalette::ButtonText;
                            painter->setBrush(option->palette.brush( option->palette.currentColorGroup(), textRole));
                            painter->drawEllipse(checkRect.adjusted(4, 4, -4, -4));
                        }
                    } else {
                        // Check box
                        if (menuItem->icon.isNull()) {
                            QStyleOptionButton box;
                            box.QStyleOption::operator=(*option);
                            box.rect = checkRect;
                            box.state = checked ? State_On : State_Off;
                            proxy()->drawPrimitive(PE_IndicatorCheckBox, &box, painter, widget);
                        }
                    }
                }
            } else { //ignore checkmark
                if (menuItem->icon.isNull())
                    checkcol = 0;
                else
                    checkcol = menuItem->maxIconWidth;
            }

            // Text and icon, ripped from windows style
            bool dis = !enabled;
            const QStyleOption *opt = option;
            const QStyleOptionMenuItem *menuitem = menuItem;

            QPainter *p = painter;
            QRect vCheckRect = visualRect(opt->direction, menuitem->rect,
                                          QRect(menuitem->rect.x() + 4, menuitem->rect.y(),
                                                checkcol, menuitem->rect.height()));
            if (!menuItem->icon.isNull()) {
                QIcon::Mode mode = dis ? QIcon::Disabled : QIcon::Normal;
                if (act && !dis)
                    mode = QIcon::Active;
                QPixmap pixmap;

                int smallIconSize = proxy()->pixelMetric(PM_SmallIconSize, option, widget);
                QSize iconSize(smallIconSize, smallIconSize);
                if (const QComboBox *combo = qobject_cast<const QComboBox*>(widget))
                    iconSize = combo->iconSize();
                if (checked)
                    pixmap = menuItem->icon.pixmap(iconSize, mode, QIcon::On);
                else
                    pixmap = menuItem->icon.pixmap(iconSize, mode);

                int pixw = pixmap.width();
                int pixh = pixmap.height();

                QRect pmr(0, 0, pixw, pixh);
                pmr.moveCenter(vCheckRect.center());
                painter->setPen(menuItem->palette.text().color());
                if (checkable && checked) {
                    QStyleOption opt = *option;
                    if (act) {
                        QColor activeColor = (option->palette.highlight().color().lighter(120));
                        opt.palette.setBrush(QPalette::Button, activeColor);
                    }
                    opt.state |= State_Sunken;
                    opt.rect = vCheckRect;
                    proxy()->drawPrimitive(PE_PanelButtonCommand, &opt, painter, widget);
                }
                painter->drawPixmap(pmr.topLeft(), pixmap);
            }
            if (selected) {
                painter->setPen(menuItem->palette.highlightedText().color());
            } else {
                painter->setPen(menuItem->palette.text().color());
            }
            int x, y, w, h;
            menuitem->rect.getRect(&x, &y, &w, &h);
            int tab = menuitem->tabWidth;
            QColor discol;
            if (dis) {
                discol = menuitem->palette.text().color();
                p->setPen(discol);
            }
            int xm = windowsItemFrame + checkcol + windowsItemHMargin + 2;
            int xpos = menuitem->rect.x() + xm;

            QRect textRect(xpos, y + windowsItemVMargin, w - xm - windowsRightBorder - tab + 1, h - 2 * windowsItemVMargin);
            QRect vTextRect = visualRect(opt->direction, menuitem->rect, textRect);
            QString s = menuitem->text;
            if (!s.isEmpty()) {                     // draw text
                p->save();
                int t = s.indexOf(QLatin1Char('\t'));
                int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
                if (!styleHint(SH_UnderlineShortcut, menuitem, widget))
                    text_flags |= Qt::TextHideMnemonic;
                text_flags |= Qt::AlignLeft;
                if (t >= 0) {
                    QRect vShortcutRect = visualRect(opt->direction, menuitem->rect,
                                                     QRect(textRect.topRight(), QPoint(menuitem->rect.right(), textRect.bottom())));
                    if (dis && !act && proxy()->styleHint(SH_EtchDisabledText, option, widget)) {
                        p->setPen(menuitem->palette.light().color());
                        p->drawText(vShortcutRect.adjusted(1, 1, 1, 1), text_flags, s.mid(t + 1));
                        p->setPen(discol);
                    }
                    p->drawText(vShortcutRect, text_flags, s.mid(t + 1));
                    s = s.left(t);
                }
                QFont font = menuitem->font;
                font.setPointSizeF(QFontInfo(menuItem->font).pointSizeF());

                if (menuitem->menuItemType == QStyleOptionMenuItem::DefaultItem)
                    font.setBold(true);

                p->setFont(font);
                if (dis && !act && proxy()->styleHint(SH_EtchDisabledText, option, widget)) {
                    p->setPen(menuitem->palette.light().color());
                    p->drawText(vTextRect.adjusted(1, 1, 1, 1), text_flags, s.left(t));
                    p->setPen(discol);
                }
                p->drawText(vTextRect, text_flags, s.left(t));
                p->restore();
            }

            // Arrow
            if (menuItem->menuItemType == QStyleOptionMenuItem::SubMenu) {// draw sub menu arrow
                int dim = (menuItem->rect.height() - 4) / 2;
                PrimitiveElement arrow;
                arrow = option->direction == Qt::RightToLeft ? PE_IndicatorArrowLeft : PE_IndicatorArrowRight;
                int xpos = menuItem->rect.left() + menuItem->rect.width() - 3 - dim;
                QRect  vSubMenuRect = visualRect(option->direction, menuItem->rect,
                                                 QRect(xpos, menuItem->rect.top() + menuItem->rect.height() / 2 - dim / 2, dim, dim));
                QStyleOptionMenuItem newMI = *menuItem;
                newMI.rect = vSubMenuRect;
                newMI.state = !enabled ? State_None : State_Enabled;
                if (selected)
                    newMI.palette.setColor(QPalette::Foreground,
                                           newMI.palette.highlightedText().color());
                proxy()->drawPrimitive(arrow, &newMI, painter, widget);
            }
        }
        painter->restore();
        break;
    case CE_MenuBarItem:
        painter->save();
        if (const QStyleOptionMenuItem *mbi = qstyleoption_cast<const QStyleOptionMenuItem *>(option))
        {
            QRect rect = mbi->rect;
            QStyleOptionMenuItem item = *mbi;
            item.rect = mbi->rect.adjusted(0, 1, 0, -3);
            QColor highlightOutline = option->palette.highlight().color().darker(125);
//            painter->fillRect(rect, option->palette.window());

//            QCommonStyle::drawControl(ce, &item, painter, widget);

            bool act = mbi->state & State_Selected && mbi->state & State_Sunken;
            bool dis = !(mbi->state & State_Enabled);

            QRect r = option->rect;
            if (act) {
                painter->setBrush(option->palette.highlight().color());
                painter->setPen(QPen(highlightOutline));
                painter->drawRect(r.adjusted(0, 0, -1, -1));

                //                painter->drawRoundedRect(r.adjusted(1, 1, -1, -1), 2, 2);

                //draw text
            }
            QPalette::ColorRole textRole = dis ? QPalette::Text : QPalette::HighlightedText;
            uint alignment = Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
            if (!styleHint(SH_UnderlineShortcut, mbi, widget))
                alignment |= Qt::TextHideMnemonic;
//                QPalette pal = mbi->palette;
//                pal.setBrush(QPalette::ButtonText, Qt::green);
//                mbi->palette = pal;
//                mbi->palette.setBrush(QPalette::ButtonText, Qt::green);
            proxy()->drawItemText(painter, item.rect, alignment, mbi->palette, mbi->state & State_Enabled, mbi->text, textRole);
        }
        painter->setRenderHint(QPainter::Antialiasing);
        painter->translate(0.5, 0.5);
        painter->setPen(COLOR_FRAME_BORDER.lighter(130));
        painter->drawLine(option->rect.left(), option->rect.bottom(),
                          option->rect.right(), option->rect.bottom());
        painter->restore();
        break;
    case CE_ToolBar:
        if (const QStyleOptionToolBar *toolBar = qstyleoption_cast<const QStyleOptionToolBar *>(option)) {
            painter->fillRect(toolBar->rect, COLOR_TOOLBAR_BG);
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->translate(0.5, 0.5);
            QRect rect = toolBar->rect;
            if (option->state & State_Horizontal) {
                if (toolBar->toolBarArea == Qt::TopToolBarArea) {
                    painter->setPen(COLOR_FRAME_BORDER.lighter());
                    painter->drawLine(rect.topLeft(), rect.topRight());
                    painter->setPen(COLOR_FRAME_BORDER);
                    painter->drawLine(rect.left(), rect.bottom(),
                                      rect.right(), rect.bottom());
                }
                else {
                    painter->setPen(COLOR_FRAME_BORDER.lighter());
                    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
                    painter->setPen(COLOR_FRAME_BORDER);
                    painter->drawLine(rect.left(), rect.top(),
                                      rect.right(), rect.top());
                }
            }
            else {
                if (toolBar->toolBarArea == Qt::LeftToolBarArea) {
                    painter->setPen(COLOR_FRAME_BORDER.lighter());
                    painter->drawLine(rect.topLeft(), rect.topRight());
                    painter->setPen(COLOR_FRAME_BORDER);
                    painter->drawLine(rect.right(), rect.top(),
                                      rect.right(), rect.bottom());
                }
                else {
                    painter->setPen(COLOR_FRAME_BORDER.lighter());
                    painter->drawLine(rect.topRight(), rect.bottomRight());
                    painter->setPen(COLOR_FRAME_BORDER);
                    painter->drawLine(rect.left(), rect.top(),
                                      rect.left(), rect.bottom());
                }
            }

            painter->restore();
        }
        break;
    case CE_MenuBarEmptyArea:
        painter->save();
    {
        QRect rect = option->rect;
        painter->fillRect(rect, option->palette.window());
        QColor shadow = COLOR_FRAME_BORDER.lighter(130);
        painter->setPen(QPen(shadow));
        painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());
    }
        painter->restore();
        break;
    case CE_RubberBand:
        if (qstyleoption_cast<const QStyleOptionRubberBand *>(option)) {
            QColor highlight = option->palette.color(QPalette::Active, QPalette::Highlight);
            painter->save();
            QColor penColor = highlight.darker(120);
            penColor.setAlpha(180);
            painter->setPen(penColor);
            QColor dimHighlight(qMin(highlight.red()/2 + 110, 255),
                                qMin(highlight.green()/2 + 110, 255),
                                qMin(highlight.blue()/2 + 110, 255));
            dimHighlight.setAlpha(widget && widget->isTopLevel() ? 255 : 80);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->translate(0.5, 0.5);
            painter->setBrush(dimHighlight);
            painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, -1), 1, 1);
            QColor innerLine = Qt::white;
            innerLine.setAlpha(40);
            painter->setPen(innerLine);
            painter->drawRoundedRect(option->rect.adjusted(1, 1, -2, -2), 1, 1);
            painter->restore();
            return;
        }
    case CE_ToolBoxTab:
        if (const QStyleOptionToolBox *tb = qstyleoption_cast<const QStyleOptionToolBox *>(option)) {
            proxy()->drawControl(CE_ToolBoxTabShape, tb, painter, widget);
            proxy()->drawControl(CE_ToolBoxTabLabel, tb, painter, widget);
        }
    break;
    case CE_ToolBoxTabShape:
        if (const QStyleOptionToolBox *tab = qstyleoption_cast<const QStyleOptionToolBox *> (option)) {
//            painter->setPen(Qt::black);
//            painter->setBrush(QColor(255, 255, 0, 100));
//            painter->drawRect(tab->rect.adjusted(0, 0, -1, -1));
            painter->save();
            prepareSmothPainter(painter);
//            painter->drawRoundedRect(tab->rect.adjusted(0, 0, -1, -1), 4, 4);

            bool begining = tab->position == (QStyleOptionToolBox::Beginning);
            bool ending = tab->position == QStyleOptionToolBox::End;
            bool selected = tab->state & State_Selected;
            bool prevSelected = tab->selectedPosition & QStyleOptionToolBox::PreviousIsSelected;
            bool hover = tab->state & State_MouseOver; //no trabaja bien

            painter->setPen(COLOR_SLIDER_HANDLE_INNER_BORDER);
            painter->setBrush(selected ? COLOR_TOOLBOX_TAB_SELECTED/*Qt::red*/ : COLOR_TOOLBOX_TAB_NORMAL/*Qt::blue*/);

            int x1, x2, y1, y2;
            tab->rect.getCoords(&x1, &y1, &x2, &y2);
//            x1++;
//            y1++;
//            x2--;
//            y2--;
            int radius = 4;
            int diam = radius * 2;
            int offset = selected ? 2 : 1;
            QPainterPath path;
//            qDebug() << "tab pos" << tab->position;
            if (begining) {
//                qDebug() << "begining";
//                path.moveTo(x1, y2);
//                path.lineTo(x1, y1 + radius);
//                path.arcTo(QRect(x1, y1, diam, diam), 180, -90);
//                path.lineTo(x2 - radius, y1);
//                path.arcTo(QRect(x2 - diam, y1, diam, diam), 90, -90);
//                path.lineTo(x2, y2);
//                path.closeSubpath();
                path.moveTo(x2, y2 - offset);
                path.lineTo(x2, y1 + radius);
                path.arcTo(QRect(x2 - diam, y1, diam, diam), 0, +90);
                path.lineTo(x1 + radius, y1);
                path.arcTo(QRect(x1, y1, diam, diam), 90, +90);
                path.lineTo(x1, y2 - offset);
                path.closeSubpath();
            }
            else if (!ending || (ending && selected)) {
                path.moveTo(x1, y2 - offset);
                path.lineTo(x1, y1);
                path.lineTo(x2, y1);
                path.lineTo(x2, y2 - offset);
                path.closeSubpath();
            } else {
                if (!selected) {
                    path.moveTo(x1, y2 - radius);
                    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180, 90);
                    path.lineTo(x2 - radius, y2);
                    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270, 90);
                } else {
                    path.moveTo(x1, y2);
                    path.lineTo(x2, y2);
                }
                path.lineTo(x2, y1);
                path.lineTo(x1, y1);
                path.closeSubpath();
//                qDebug() << "repintando";
            }
            painter->drawPath(path);
//            if (!ending) {
                if (selected) {
                    painter->translate(-0.5, -0.5);
                    painter->setPen(QPen(COLOR_TAB_SELECTED_UL, 2));
                } else {
                    painter->setPen(COLOR_FRAME_BORDER);
                }
                if (!(!selected && ending))
                painter->drawLine(x1, y2, x2, y2);

//            }
            if (prevSelected) {
                painter->setPen(COLOR_FRAME_BORDER);
                painter->drawLine(x1, y1, x2, y1);
            }

            painter->restore();
        }
        break;
    case CE_ToolBoxTabLabel:
        if (const QStyleOptionToolBox *tb = qstyleoption_cast<const QStyleOptionToolBox *>(option)) {
            bool enabled = tb->state & State_Enabled;
            bool selected = tb->state & State_Selected;
            QPixmap pm = tb->icon.pixmap(proxy()->pixelMetric(QStyle::PM_SmallIconSize, tb, widget),
                                         enabled ? QIcon::Normal : QIcon::Disabled);
            if (pm.isNull()) {
                pm = generatedIconPixmap(selected ? QIcon::Normal : QIcon::Disabled, pm, tb);
            }

            QRect cr = subElementRect(QStyle::SE_ToolBoxTabContents, tb, widget);
            QRect tr, ir;
            int ih = 0;
            if (pm.isNull()) {
                tr = cr;
                tr.adjust(4, 0, -8, 0);
            } else {
                int iw = pm.width() / pm.devicePixelRatio() + 4;
                ih = pm.height()/ pm.devicePixelRatio();
                ir = QRect(cr.left() + 4, cr.top(), iw + 2, ih);
                tr = QRect(ir.right(), cr.top(), cr.width() - ir.right() - 4, cr.height());
            }

            if (selected && proxy()->styleHint(QStyle::SH_ToolBox_SelectedPageTitleBold, tb, widget)) {
                QFont f(regularFont);
                f.setBold(true);
                painter->setFont(f);
            }

            QString txt = tb->fontMetrics.elidedText(tb->text, Qt::ElideRight, tr.width());

            if (ih)
                painter->drawPixmap(ir.left(), (tb->rect.height() - ih) / 2, pm);

            int alignment = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic;
            if (!proxy()->styleHint(QStyle::SH_UnderlineShortcut, tb, widget))
                alignment |= Qt::TextHideMnemonic;
            proxy()->drawItemText(painter, tr, alignment, tb->palette, enabled, txt, QPalette::Text);

            if (!txt.isEmpty() && option->state & State_HasFocus) {
                QStyleOptionFocusRect opt;
                opt.rect = tr;
                opt.palette = tb->palette;
                opt.state = QStyle::State_None;
                proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &opt, painter, widget);
            }
        }
        break;
    case CE_ShapedFrame:
        painter->save();
        painter->setPen(COLOR_FRAME_BORDER);
        painter->setBrush(option->palette.window());
        prepareSmothPainter(painter);
        if (qobject_cast<const QToolBox *>(option->styleObject)) {
            painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, -1), 4, 4);
        } else {
            if (const QStyleOptionFrameV3 *f = qstyleoption_cast<const QStyleOptionFrameV3 *>(option)) {
                int frameShape  = f->frameShape;
                int frameShadow = QFrame::Plain;
                if (f->state & QStyle::State_Sunken) {
                    frameShadow = QFrame::Sunken;
                } else if (f->state & QStyle::State_Raised) {
                    frameShadow = QFrame::Raised;
                }

                int lw = f->lineWidth;
                int mlw = f->midLineWidth;
                QPalette::ColorRole foregroundRole = QPalette::WindowText;
                if (widget)
                    foregroundRole = widget->foregroundRole();
                painter->setPen(COLOR_FRAME_BORDER);

                switch (frameShape) {
                case QFrame::Box:
                case QFrame::StyledPanel:
                case QFrame::Panel:
                case QFrame::WinPanel:
                    painter->drawRect(option->rect.adjusted(0, 0, -1, -1));
                    break;
                case QFrame::HLine:
                case QFrame::VLine: {
                    QPoint p1, p2;
                    if (frameShape == QFrame::HLine) {
                        p1 = QPoint(option->rect.x(), option->rect.y() + option->rect.height() / 2);
                        p2 = QPoint(option->rect.x() + option->rect.width(), p1.y());
                    } else {
                        p1 = QPoint(option->rect.x() + option->rect.width() / 2, option->rect.y());
                        p2 = QPoint(p1.x(), p1.y() + option->rect.height());
                    }

//                    QPen oldPen = painter->pen();
//                    painter->setPen(QPen(option->palette.brush(foregroundRole), lw));
                    painter->drawLine(p1, p2);
//                    painter->setPen(oldPen);

                    break;
                }
                }
            }
//
//            painter->drawRect(option->rect.adjusted(0, 0 ,-1, -1));
//            QCommonStyle::drawControl(ce, option, painter, widget);
        }
        painter->restore();
        break;
    case CE_DockWidgetTitle:
        painter->save();
        if (const QStyleOptionDockWidget *dwOpt = qstyleoption_cast<const QStyleOptionDockWidget *>(option)) {
            bool verticalTitleBar = dwOpt->verticalTitleBar;
            painter->fillRect(dwOpt->rect, COLOR_DOCK_TITLE_BG);
//            qDebug() << "dwtit";
            QRect titleRect = subElementRect(SE_DockWidgetTitleBarText, option, widget);
            if (verticalTitleBar) {
                QRect rect = dwOpt->rect;
                QRect r = rect;

                QSize s = r.size();
                s.transpose();
                r.setSize(s);
                titleRect = QRect(r.left() + rect.bottom()
                                  - titleRect.bottom(),
                                  r.top() + titleRect.left() - rect.left(),
                                  titleRect.height(), titleRect.width());

                painter->translate(r.left(), r.top() + r.width());
                painter->rotate(-90);
                painter->translate(-r.left(), -r.top());
            }

            if (!dwOpt->title.isEmpty()) {
//                qDebug() << "paint dw tit" << dwOpt->title << titleRect;
                QString titleText
                        = painter->fontMetrics().elidedText(dwOpt->title,
                                                            Qt::ElideRight, titleRect.width());
                QPalette pal = dwOpt->palette;
                pal.setBrush(QPalette::ButtonText, COLOR_BUTTON_TEXT);
                proxy()->drawItemText(painter,
                                      titleRect,
                                      Qt::AlignLeft | Qt::AlignVCenter | Qt::TextShowMnemonic, pal,
                                      dwOpt->state & State_Enabled, titleText,
                                      QPalette::ButtonText);
            }
        }
        painter->restore();
        break;
    case CE_Splitter:
    {
        // Don't draw handle for single pixel splitters
        if (option->rect.width() > 1 && option->rect.height() > 1) {
            //draw grips
            QRect r = option->rect;
            painter->fillRect(r, COLOR_SPLITTER_BG);
            if (option->state & State_Horizontal) {
                QRect indRect(r.left(), r.top() + (r.height() - SPLITTER_IND_WIDTH) / 2, r.width(), SPLITTER_IND_WIDTH);
                painter->fillRect(indRect, COLOR_SPLITTER_DECO_BG);
                drawArrow(painter, Qt::RightArrow, Qt::white, indRect.adjusted(2, 2, -2, -2));
            } else {
                QRect indRect(r.left() + (r.width() - SPLITTER_IND_WIDTH) / 2, r.top(), SPLITTER_IND_WIDTH, r.height());
                painter->fillRect(indRect, COLOR_SPLITTER_DECO_BG);
                drawArrow(painter, Qt::UpArrow, Qt::white, indRect.adjusted(2, 2, -2, -2));
            }
        }
        break;
    }
    case CE_HeaderSection:
        painter->save();
        // Draws the header in tables.
        if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option)) {
//            QString pixmapName = QStyleHelper::uniqueName(QLatin1String("headersection"), option, option->rect.size());
//            pixmapName += QString::number(- int(header->position));
//            pixmapName += QString::number(- int(header->orientation));

//            QPixmap cache;
//            if (!QPixmapCache::find(pixmapName, cache)) {
//                cache = styleCachePixmap(rect.size());
//                cache.fill(Qt::transparent);
//                QRect pixmapRect(0, 0, rect.width(), rect.height());
//                QPainter cachePainter(&cache);
//                QColor buttonColor = d->buttonColor(option->palette);
//                QColor gradientStopColor;
//                QColor gradientStartColor = buttonColor.lighter(104);
//                gradientStopColor = buttonColor.darker(102);
//                QLinearGradient gradient(pixmapRect.topLeft(), pixmapRect.bottomLeft());

//                if (option->palette.background().gradient()) {
//                    gradient.setStops(option->palette.background().gradient()->stops());
//                } else {
//                    QColor midColor1 = mergedColors(gradientStartColor, gradientStopColor, 60);
//                    QColor midColor2 = mergedColors(gradientStartColor, gradientStopColor, 40);
//                    gradient.setColorAt(0, gradientStartColor);
//                    gradient.setColorAt(0.5, midColor1);
//                    gradient.setColorAt(0.501, midColor2);
//                    gradient.setColorAt(0.92, gradientStopColor);
//                    gradient.setColorAt(1, gradientStopColor.darker(104));
//                }
//                cachePainter.fillRect(pixmapRect, gradient);
//                cachePainter.setPen(d->innerContrastLine());
//                cachePainter.setBrush(Qt::NoBrush);
//                cachePainter.drawLine(pixmapRect.topLeft(), pixmapRect.topRight());
//                cachePainter.setPen(d->outline(option->palette));
//                cachePainter.drawLine(pixmapRect.bottomLeft(), pixmapRect.bottomRight());

//                if (header->orientation == Qt::Horizontal &&
//                        header->position != QStyleOptionHeader::End &&
//                        header->position != QStyleOptionHeader::OnlyOneSection) {
//                    cachePainter.setPen(QColor(0, 0, 0, 40));
//                    cachePainter.drawLine(pixmapRect.topRight(), pixmapRect.bottomRight() + QPoint(0, -1));
//                    cachePainter.setPen(d->innerContrastLine());
//                    cachePainter.drawLine(pixmapRect.topRight() + QPoint(-1, 0), pixmapRect.bottomRight() + QPoint(-1, -1));
//                } else if (header->orientation == Qt::Vertical) {
//                    cachePainter.setPen(d->outline(option->palette));
//                    cachePainter.drawLine(pixmapRect.topRight(), pixmapRect.bottomRight());
//                }
//                cachePainter.end();
//                QPixmapCache::insert(pixmapName, cache);
//            }
//            painter->drawPixmap(rect.topLeft(), cache);
            painter->setBrush(COLOR_HEADER);
            painter->setPen(COLOR_FRAME_BORDER);
            QRect r = option->rect;
            painter->drawRect(r.adjusted(-1, -1, -1, -1));
            bool selected = header->state & QStyle::State_On;
            if (selected) {
//                painter->setRenderHint(QPainter::Antialiasing);
                painter->setPen(QPen(COLOR_TAB_SELECTED_UL, 2));
                QPoint endPoint = r.bottomRight();
                endPoint.rx() --;
                painter->drawLine(r.bottomLeft(), endPoint);
            }
        }
        painter->restore();
        break;
    case CE_HeaderLabel:
        if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option)) {
            QRect rect = header->rect;
            if (!header->icon.isNull()) {
                QPixmap pixmap
                    = header->icon.pixmap(proxy()->pixelMetric(PM_SmallIconSize), (header->state & State_Enabled) ? QIcon::Normal : QIcon::Disabled);
                int pixw = pixmap.width() / pixmap.devicePixelRatio();

                QRect aligned = alignedRect(header->direction, QFlag(header->iconAlignment), pixmap.size() / pixmap.devicePixelRatio(), rect);
                QRect inter = aligned.intersected(rect);
                painter->drawPixmap(inter.x(), inter.y() - 1, pixmap,
                              inter.x() - aligned.x(), inter.y() - aligned.y(),
                              aligned.width() * pixmap.devicePixelRatio(),
                              pixmap.height() * pixmap.devicePixelRatio());

                if (header->direction == Qt::LeftToRight)
                    rect.setLeft(rect.left() + pixw + 2);
                else
                    rect.setRight(rect.right() - pixw - 2);
            }
//            if (header->state & QStyle::State_On) {
//                QFont fnt = painter->font();
//                fnt.setBold(true);
//                painter->setFont(fnt);
//            }
            proxy()->drawItemText(painter, rect.adjusted(0, -2, 0, -2), header->textAlignment, header->palette,
                         (header->state & State_Enabled), header->text, QPalette::ButtonText);
        }
        break;
//    case CE_ItemViewItem:

//        break;
    default:
        QCommonStyle::drawControl(ce, option, painter, widget);
        break;
    }
}

void MaterialStyle::drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    switch (control) {
    case CC_GroupBox: {
        if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(option)) {
            drawGroupBox(groupBox, painter, widget);
        }
        break;
    }
    case CC_Slider: {
        if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
            drawSlider(slider, painter, widget);
        }
        break;
    }
    case CC_ScrollBar:
        painter->save();
//        painter->setRenderHint(QPainter::Antialiasing);
//        painter->translate(0.5, 0.5);
        if (const QStyleOptionSlider *scrollBar = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
            bool wasActive = false;
            qreal expandScale = 1.0;
            qreal expandOffset = -1.0;
            bool transient = proxy()->styleHint(SH_ScrollBar_Transient, option, widget);
            bool horizontal = scrollBar->orientation == Qt::Horizontal;
            bool sunken = scrollBar->state & State_Sunken;

            QRect scrollBarSubLine = proxy()->subControlRect(control, scrollBar, SC_ScrollBarSubLine, widget);
            QRect scrollBarAddLine = proxy()->subControlRect(control, scrollBar, SC_ScrollBarAddLine, widget);
            QRect scrollBarSlider = proxy()->subControlRect(control, scrollBar, SC_ScrollBarSlider, widget);
            QRect scrollBarGroove = proxy()->subControlRect(control, scrollBar, SC_ScrollBarGroove, widget);

            QRect rect = option->rect;
            QColor alphaOutline = COLOR_FRAME_BORDER;
            alphaOutline.setAlpha(180);

            QColor arrowColor = COLOR_INDICATOR_ARROW;
            arrowColor.setAlpha(220);

            const QColor bgColor = option->palette.color(QPalette::Base);
            const bool isDarkBg = bgColor.red() < 128 && bgColor.green() < 128 && bgColor.blue() < 128;

            if (transient) {
                if (horizontal) {
                    rect.setY(rect.y() + 4.5 - expandOffset);
                    scrollBarSlider.setY(scrollBarSlider.y() + 4.5 - expandOffset);
                    scrollBarGroove.setY(scrollBarGroove.y() + 4.5 - expandOffset);

                    rect.setHeight(rect.height() * expandScale);
                    scrollBarGroove.setHeight(scrollBarGroove.height() * expandScale);
                } else {
                    rect.setX(rect.x() + 4.5 - expandOffset);
                    scrollBarSlider.setX(scrollBarSlider.x() + 4.5 - expandOffset);
                    scrollBarGroove.setX(scrollBarGroove.x() + 4.5 - expandOffset);

                    rect.setWidth(rect.width() * expandScale);
                    scrollBarGroove.setWidth(scrollBarGroove.width() * expandScale);
                }
            }

            // Paint groove
            if ((!transient || scrollBar->activeSubControls || wasActive) && scrollBar->subControls & SC_ScrollBarGroove) {
                painter->save();
                if (transient)
                    painter->setOpacity(0.8);
                painter->fillRect(rect, COLOR_SCROLLBAR_GROOVE);
                painter->setPen(Qt::NoPen);
                if (transient)
                    painter->setOpacity(0.4);
                painter->setPen(alphaOutline);
                if (horizontal)
                    painter->drawLine(rect.topLeft(), rect.topRight());
                else
                    painter->drawLine(rect.topLeft(), rect.bottomLeft());

                QColor subtleEdge = alphaOutline;
                subtleEdge.setAlpha(40);
                painter->setPen(Qt::NoPen);
                painter->setBrush(Qt::NoBrush);
                painter->setClipRect(scrollBarGroove.adjusted(1, 0, -1, -3));
                painter->drawRect(scrollBarGroove.adjusted(1, 0, -1, -1));
                painter->restore();
            }

            QRect pixmapRect = scrollBarSlider;

            // Paint slider
            if (scrollBar->subControls & SC_ScrollBarSlider) {
                if (transient) {
                    QRect rect = scrollBarSlider.adjusted(horizontal ? 1 : 2, horizontal ? 2 : 1, -1, -1);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(isDarkBg ? Qt::lightGray : Qt::darkGray);
                    int r = qMin(rect.width(), rect.height()) / 2;

                    painter->save();
                    painter->setRenderHint(QPainter::Antialiasing, true);
                    painter->drawRoundedRect(rect, r, r);
                    painter->restore();
                } else {
                    QRect pixmapRect = scrollBarSlider;
                    painter->setPen(QPen(alphaOutline));
                    if (option->state & State_Sunken && scrollBar->activeSubControls & SC_ScrollBarSlider)
                        painter->setBrush(COLOR_TAB_SELECTED_UL);
                    else if (option->state & State_MouseOver && scrollBar->activeSubControls & SC_ScrollBarSlider)
                        painter->setBrush(scrollBar->palette.highlight().color().darker(120));
                    else
                        painter->setBrush(scrollBar->palette.window().color().darker(120));

                    painter->drawRect(pixmapRect.adjusted(horizontal ? -1 : 0, horizontal ? 0 : -1, horizontal ? 0 : 1, horizontal ? 1 : 0));

                    painter->setPen(COLOR_FRAME_BORDER);
//                    painter->drawRect(scrollBarSlider.adjusted(horizontal ? 0 : 1, horizontal ? 1 : 0, -1, -1));
                }
            }

            // The SubLine (up/left) buttons
            if (!transient && scrollBar->subControls & SC_ScrollBarSubLine) {
                if ((scrollBar->activeSubControls & SC_ScrollBarSubLine) && sunken)
                    painter->setBrush(COLOR_TAB_SELECTED_UL);
                else if ((scrollBar->activeSubControls & SC_ScrollBarSubLine))
                    painter->setBrush(scrollBar->palette.highlight().color().darker(120));
                else
                    painter->setBrush(scrollBar->palette.window().color().darker(120));

                painter->setPen(Qt::NoPen);
                painter->drawRect(scrollBarSubLine.adjusted(horizontal ? 0 : 1, horizontal ? 1 : 0, 0, 0));
                painter->setPen(QPen(alphaOutline));
                if (option->state & State_Horizontal) {
                    if (option->direction == Qt::RightToLeft) {
                        pixmapRect.setLeft(scrollBarSubLine.left());
                        painter->drawLine(pixmapRect.topLeft(), pixmapRect.bottomLeft());
                    } else {
                        pixmapRect.setRight(scrollBarSubLine.right());
                        painter->drawLine(pixmapRect.topRight(), pixmapRect.bottomRight());
                    }
                    drawArrow(painter, Qt::LeftArrow, COLOR_INDICATOR_ARROW, scrollBarSubLine.adjusted(2, 4, -4, -3));
                } else {
                    pixmapRect.setBottom(scrollBarSubLine.bottom());
                    painter->drawLine(pixmapRect.bottomLeft(), pixmapRect.bottomRight());
                    drawArrow(painter, Qt::UpArrow, COLOR_INDICATOR_ARROW, scrollBarSubLine.adjusted(4, 2, -3, -4));
                }

//                painter->setBrush(Qt::NoBrush);
//                painter->setPen(COLOR_FRAME_BORDER);
//                painter->drawRect(scrollBarSubLine.adjusted(horizontal ? 0 : 1, horizontal ? 1 : 0 ,  horizontal ? -2 : -1, horizontal ? -1 : -2));

            }

            // The AddLine (down/right) button
            if (!transient && scrollBar->subControls & SC_ScrollBarAddLine) {
                if ((scrollBar->activeSubControls & SC_ScrollBarAddLine) && sunken)
                    painter->setBrush(COLOR_TAB_SELECTED_UL);
                else if ((scrollBar->activeSubControls & SC_ScrollBarAddLine))
                    painter->setBrush(scrollBar->palette.highlight().color().darker(120));
                else
                    painter->setBrush(scrollBar->palette.window().color().darker(120));

                painter->setPen(Qt::NoPen);
                painter->drawRect(scrollBarAddLine.adjusted(horizontal ? 0 : 1, horizontal ? 1 : 0, 0, 0));
                painter->setPen(QPen(alphaOutline, 1));
                if (option->state & State_Horizontal) {
                    if (option->direction == Qt::LeftToRight) {
                        pixmapRect.setLeft(scrollBarAddLine.left());
                        painter->drawLine(pixmapRect.topLeft(), pixmapRect.bottomLeft());
                    } else {
                        pixmapRect.setRight(scrollBarAddLine.right());
                        painter->drawLine(pixmapRect.topRight(), pixmapRect.bottomRight());
                    }
                    drawArrow(painter, Qt::RightArrow, COLOR_INDICATOR_ARROW, scrollBarAddLine.adjusted(4, 4, -3, -3));
                } else {
                    pixmapRect.setTop(scrollBarAddLine.top());
                    painter->drawLine(pixmapRect.topLeft(), pixmapRect.topRight());
                    drawArrow(painter, Qt::DownArrow, COLOR_INDICATOR_ARROW, scrollBarAddLine.adjusted(4, 4, -3, -3));
                }

                painter->setPen(COLOR_FRAME_BORDER);
                painter->setBrush(Qt::NoBrush);
                painter->drawRect(scrollBarAddLine.adjusted(0, 0, -1, horizontal ? 0 : -1));
            }

        }
        painter->restore();
        break;;
    case CC_ComboBox:
        painter->save();
        if (const QStyleOptionComboBox *comboBox = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
//            bool hasFocus = option->state & State_HasFocus && option->state & State_KeyboardFocusChange;
            bool sunken = comboBox->state & State_On;
//            bool isEnabled = comboBox->state & State_Enabled;
            bool hover = comboBox->state & State_MouseOver;

            QRect btnRect = subControlRect(CC_ComboBox, comboBox, SC_ComboBoxArrow, widget);

            //            if (comboBox->editable) {
            //Draw the edit area like a normal QLineEdit
            QStyleOptionFrame  buttonOption;
            buttonOption.QStyleOption::operator=(*comboBox);
            //                buttonOption.rect = rect;
            buttonOption.state = (comboBox->state & (State_Enabled | State_MouseOver | State_HasFocus))
                    | State_KeyboardFocusChange; // Always show hig

            if (sunken) {
                buttonOption.state |= State_Sunken;
                buttonOption.state &= ~State_MouseOver;
            }
            proxy()->drawPrimitive(PE_FrameLineEdit, &buttonOption, painter, widget);




            QColor btnBgColor;
            if (sunken) {
                btnBgColor = COLOR_COMBOBOX_SUNKEN;
            } else if (hover) {
                btnBgColor = COLOR_COMBOBOX_HOVER;
            } else {
                btnBgColor = option->palette.window().color();
            }

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->translate(0.5, 0.5); //due to hdpi bug
            painter->setPen(!(sunken | hover) ? btnBgColor : option->palette.window().color());
            painter->setBrush(btnBgColor);
            painter->setClipRect(btnRect.adjusted(0, 0, 2, 0)); //a little wider
            painter->drawRoundedRect(btnRect.adjusted(-4, 1, 0, -2), 4, 4);

            painter->setPen(COLOR_FRAME_BORDER);
            painter->drawLine(btnRect.left() + 1, 1, btnRect.left() + 1, btnRect.bottom() - 1);
            if (sunken | hover) {
                painter->setPen(option->palette.window().color());
                painter->drawLine(btnRect.left() + 2, 2, btnRect.left() + 2, btnRect.bottom() - 2);
            }
            painter->restore();

            drawArrow(painter, Qt::DownArrow, COLOR_INDICATOR_ARROW, btnRect);
        }
        painter->restore();
        break;
    case CC_SpinBox:
        if (const QStyleOptionSpinBox *spinBox = qstyleoption_cast<const QStyleOptionSpinBox *>(option)) {
            bool isEnabled = (spinBox->state & State_Enabled);
            bool hover = isEnabled && (spinBox->state & State_MouseOver);
            bool sunken = (spinBox->state & State_Sunken);
            bool upIsActive = (spinBox->activeSubControls == SC_SpinBoxUp);
            bool downIsActive = (spinBox->activeSubControls == SC_SpinBoxDown);
//            bool hasFocus = (option->state & State_HasFocus);

            QRect upRect = proxy()->subControlRect(CC_SpinBox, spinBox, SC_SpinBoxUp, widget);
            QRect downRect = proxy()->subControlRect(CC_SpinBox, spinBox, SC_SpinBoxDown, widget);

            QStyleOptionFrame  editOption;
            editOption.QStyleOption::operator=(*spinBox);
            editOption.state = (spinBox->state & (State_Enabled | State_MouseOver | State_HasFocus))
                    | State_KeyboardFocusChange; // Always show hig

            if (sunken) {
                editOption.state |= State_Sunken;
                editOption.state &= ~State_MouseOver;
            }
            proxy()->drawPrimitive(PE_FrameLineEdit, &editOption, painter, widget);

            painter->setRenderHint(QPainter::Antialiasing);
            painter->translate(-0.5, -0.5);
//            painter->setOpacity(0.4);
//            qDebug() << "uprect" << upRect;
            QColor btnBg = hover ? COLOR_COMBOBOX_HOVER : option->palette.window().color();
            painter->setPen(Qt::NoPen);
            painter->save();
            painter->translate(-0.5, -0.5);
            painter->setBrush((upIsActive && sunken) ? COLOR_COMBOBOX_SUNKEN : btnBg);
            painter->setClipRect(upRect.adjusted(1, 0, 1, 0));
            painter->drawRoundedRect(upRect.adjusted(-4, 1, 1, 4), 4, 4);
            painter->restore();
            painter->save();
            painter->translate(-0.5, -0.5);
            painter->setBrush((downIsActive && sunken) ? COLOR_COMBOBOX_SUNKEN : btnBg);
            painter->setClipRect(downRect.adjusted(1, 0, 1, 1));
            painter->drawRoundedRect(downRect.adjusted(-4, -4, 1, 1), 4, 4);
            painter->restore();

            painter->setPen(COLOR_FRAME_BORDER);
            painter->drawLine(upRect.left() + 1, 1, downRect.left() + 1, spinBox->rect.bottom());
            painter->drawLine(upRect.left() + 1, upRect.bottom() + 1, spinBox->rect.right(), upRect.bottom() + 1);

            drawArrow(painter, Qt::UpArrow, COLOR_INDICATOR_ARROW, upRect.adjusted(0, -2, 0, 0));
            drawArrow(painter, Qt::DownArrow, COLOR_INDICATOR_ARROW, downRect);

        }
        break;
    case CC_ToolButton:
        if (const QStyleOptionToolButton *btn = qstyleoption_cast<const QStyleOptionToolButton *>(option)) {
            QStyleOptionToolButton other;
//            other.QStyleOption::operator=(*btn);
            other = *btn;
            QPalette pal = other.palette;
            pal.setBrush(QPalette::ButtonText, COLOR_DEFAULT_TEXT);
            other.palette = pal;
            QCommonStyle::drawComplexControl(control, &other, painter, widget);
        }
        break;
    case CC_Dial:
        if (const QStyleOptionSlider *dial = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
//            QCommonStyle::drawComplexControl(control, option, painter, widget);
            QPalette pal = option->palette;
            QColor buttonColor = pal.button().color();
            const int width = option->rect.width();
            const int height = option->rect.height();
            const bool enabled = option->state & QStyle::State_Enabled;
            qreal r = qMin(width, height) / 2;
            r -= r/50;
            const qreal penSize = r/20.0;

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
//            painter->translate(0.5, 0.5);

            // Draw notches
            if (option->subControls & QStyle::SC_DialTickmarks) {
                painter->setPen(option->palette.dark().color().darker(120));
                painter->drawLines(calcLines(dial));
            }

            // Cache dial background
//            BEGIN_STYLE_PIXMAPCACHE(QString::fromLatin1("qdial"));
            painter->setRenderHint(QPainter::Antialiasing);

            const qreal d_ = r / 6;
            const qreal dx = option->rect.x() + d_ + (width - 2 * r) / 2 + 1;
            const qreal dy = option->rect.y() + d_ + (height - 2 * r) / 2 + 1;

            QRectF br = QRectF(dx + 0.5, dy + 0.5,
                               int(r * 2 - 2 * d_ - 2),
                               int(r * 2 - 2 * d_ - 2));
            buttonColor.setHsv(buttonColor .hue(),
                               qMin(140, buttonColor .saturation()),
                               qMax(180, buttonColor.value()));
//            QColor shadowColor(0, 0, 0, 20);

            if (enabled) {
                // Drop shadow
//                qreal shadowSize = qMax(1.0, penSize/2.0);
//                QRectF shadowRect= br.adjusted(-2*shadowSize, -2*shadowSize,
//                                               2*shadowSize, 2*shadowSize);
//                QRadialGradient shadowGradient(shadowRect.center().x(),
//                                               shadowRect.center().y(), shadowRect.width()/2.0,
//                                               shadowRect.center().x(), shadowRect.center().y());
//                shadowGradient.setColorAt(qreal(0.91), QColor(0, 0, 0, 40));
//                shadowGradient.setColorAt(qreal(1.0), Qt::transparent);
                painter->setBrush(Qt::white);
                painter->setPen(COLOR_FRAME_BORDER);
                painter->drawEllipse(br);
//                painter->translate(shadowSize, shadowSize);
//                painter->drawEllipse(shadowRect);
//                painter->translate(-shadowSize, -shadowSize);

                // Main gradient
//                QRadialGradient gradient(br.center().x() - br.width()/3, dy,
//                                         br.width()*1.3, br.center().x(),
//                                         br.center().y() - br.height()/2);
//                gradient.setColorAt(0, buttonColor.lighter(110));
//                gradient.setColorAt(qreal(0.5), buttonColor);
//                gradient.setColorAt(qreal(0.501), buttonColor.darker(102));
//                gradient.setColorAt(1, buttonColor.darker(115));
//                painter->setBrush(gradient);
            } else {
                painter->setBrush(Qt::NoBrush);
            }

            painter->setPen(QPen(buttonColor.darker(280)));
//            painter->drawEllipse(br);
            painter->setBrush(Qt::NoBrush);
            painter->setPen(buttonColor.lighter(110));
//            painter->drawEllipse(br.adjusted(1, 1, -1, -1));

            if (option->state & QStyle::State_HasFocus) {
                QColor highlight = pal.highlight().color();
                highlight.setHsv(highlight.hue(),
                                 qMin(160, highlight.saturation()),
                                 qMax(230, highlight.value()));
                highlight.setAlpha(127);
                painter->setPen(QPen(highlight, 2.0));
                painter->setBrush(Qt::NoBrush);
                painter->drawEllipse(br.adjusted(-2, -2, 2, 2));
            }
//            const int width = dial->rect.width();
//            const int height = dial->rect.height();
//            const int r = qMin(width, height) / 2;
            qreal offset = 0.70;
            const int currentSliderPosition = dial->upsideDown ? dial->sliderPosition : (dial->maximum - dial->sliderPosition);
            qreal a = 0;
            if (dial->maximum == dial->minimum)
                a = Q_PI / 2;
            else if (dial->dialWrapping)
                a = Q_PI * 3 / 2 - (currentSliderPosition - dial->minimum) * 2 * Q_PI
                    / (dial->maximum - dial->minimum);
            else
                a = (Q_PI * 8 - (currentSliderPosition - dial->minimum) * 10 * Q_PI
                    / (dial->maximum - dial->minimum)) / 6;
            qreal xc = width / 2.0;
            qreal yc = height / 2.0;
            qreal len = r - calcBigLineSize(r) - 3;
            qreal back = offset * len;
//            QPointF pos(QPointF(xc + back * qCos(a), yc - back * qSin(a)));

            QPointF dp(QPointF(xc + back * qCos(a), yc - back * qSin(a)));
            buttonColor = buttonColor.lighter(104);
            buttonColor.setAlphaF(qreal(0.8));
            const qreal ds = r/qreal(7.0);
            QRectF dialRect(dp.x() - ds, dp.y() - ds, 2*ds, 2*ds);
//            QRadialGradient dialGradient(dialRect.center().x() + dialRect.width()/2,
//                                         dialRect.center().y() + dialRect.width(),
//                                         dialRect.width()*2,
//                                         dialRect.center().x(), dialRect.center().y());
//            dialGradient.setColorAt(1, buttonColor.darker(140));
//            dialGradient.setColorAt(qreal(0.4), buttonColor.darker(120));
//            dialGradient.setColorAt(0, buttonColor.darker(110));
            if (penSize > 3.0) {
                painter->setPen(QPen(QColor(0, 0, 0, 25), penSize));
                painter->drawLine(calcRadialPos(dial, qreal(0.90)), calcRadialPos(dial, qreal(0.96)));
            }

            painter->setBrush(COLOR_TAB_SELECTED_UL);
//            painter->setPen(QColor(255, 255, 255, 150));
//            painter->drawEllipse(dialRect.adjusted(-1, -1, 1, 1));
//            painter->setPen(QColor(0, 0, 0, 80));
            painter->drawEllipse(dialRect);
            painter->restore();
        }
        break;
    case CC_TitleBar:
        if (const QStyleOptionTitleBar *titleBar = qstyleoption_cast<const QStyleOptionTitleBar *>(option)) {
            bool active = (titleBar->titleBarState & State_Active);

            painter->save();
            prepareSmothPainter(painter);
            painter->setPen(COLOR_FRAME_BORDER);
            painter->setBrush(COLOR_GROUPBOX_HEADER/*.darker()*/);
            painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, 40), 4, 4);

            painter->translate(-0.5, -0.5);
//            QString text = titleBar->text;
            QRect textRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarLabel, widget);
            //draw the text
            QColor fgColor = active? COLOR_TAB_SELECTED_TEXT :
                                  COLOR_TAB_NORMAL_TEXT;
            painter->setPen(fgColor);
            QString title = painter->fontMetrics().elidedText(titleBar->text, Qt::ElideRight, textRect.width() - 14);
            painter->drawText(textRect.adjusted(1, 1, 1, 1), title, QTextOption(Qt::AlignLeft | Qt::AlignVCenter));

            QRect actTextRect = painter->fontMetrics().boundingRect(title);
            actTextRect.moveLeft(textRect.left() + 1);

            //draw the text decoration
            QRect rect = option->rect;
            painter->setPen(QPen(COLOR_TAB_NORMAL_UL, 2));
            if (!active) {
                painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            }
            else {
                painter->drawLine(rect.left(), rect.bottom(), actTextRect.left(), rect.bottom());
                painter->setPen(QPen(COLOR_TAB_SELECTED_UL, 2));
                painter->drawLine(actTextRect.left(), rect.bottom(), actTextRect.right(), rect.bottom());
                painter->setPen(QPen(COLOR_TAB_NORMAL_UL, 2));
                painter->drawLine(actTextRect.right(), rect.bottom(), rect.right(), rect.bottom());
            }
            //draw the min button
            if ((titleBar->subControls & SC_TitleBarMinButton) && (titleBar->titleBarFlags & Qt::WindowMinimizeButtonHint) &&
                    !(titleBar->titleBarState& Qt::WindowMinimized)) {
                QRect minButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarMinButton, widget);
                bool hover = (titleBar->activeSubControls & SC_TitleBarMinButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarMaxButton) && (titleBar->state & State_Sunken);
                if (minButtonRect.isValid()) {
                    drawMdiButton(SC_TitleBarMinButton, painter, fgColor, fgColor.lighter(250), minButtonRect, hover, sunken);
                }
            }
            //draw the max button
            if ((titleBar->subControls & SC_TitleBarMaxButton) && (titleBar->titleBarFlags & Qt::WindowMaximizeButtonHint) &&
                    !(titleBar->titleBarState & Qt::WindowMaximized)) {
                QRect maxButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarMaxButton, widget);
                bool hover = (titleBar->activeSubControls & SC_TitleBarMaxButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarMaxButton) && (titleBar->state & State_Sunken);
                if (maxButtonRect.isValid()) {
                    drawMdiButton(SC_TitleBarMaxButton, painter, fgColor, fgColor.lighter(250), maxButtonRect, hover, sunken);
                }
            }
            // draw the close button
            if ((titleBar->subControls & SC_TitleBarCloseButton) && (titleBar->titleBarFlags & Qt::WindowSystemMenuHint)) {
                QRect closeButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarCloseButton, widget);
                bool hover = (titleBar->activeSubControls & SC_TitleBarCloseButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarCloseButton) && (titleBar->state & State_Sunken);
                if (closeButtonRect.isValid()) {
                    drawMdiButton(SC_TitleBarCloseButton, painter, fgColor, fgColor.lighter(250), closeButtonRect, hover, sunken);
                }
            }
            // draw the normalize button
            if ((titleBar->subControls & SC_TitleBarNormalButton) &&
                    (((titleBar->titleBarFlags & Qt::WindowMinimizeButtonHint) &&
                      (titleBar->titleBarState & Qt::WindowMinimized)) ||
                     ((titleBar->titleBarFlags & Qt::WindowMaximizeButtonHint) &&
                      (titleBar->titleBarState & Qt::WindowMaximized)))) {
                QRect normalButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarNormalButton, widget);
                bool hover = (titleBar->activeSubControls & SC_TitleBarNormalButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarNormalButton) && (titleBar->state & State_Sunken);
                if (normalButtonRect.isValid()) {
                    drawMdiButton(SC_TitleBarNormalButton, painter, fgColor, fgColor.lighter(250), normalButtonRect, hover, sunken);
                }
            }
            // draw the context help button
            if (titleBar->subControls & SC_TitleBarContextHelpButton
                    && (titleBar->titleBarFlags & Qt::WindowContextHelpButtonHint)) {
                QRect contextHelpButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarContextHelpButton, widget);
                bool hover = (titleBar->activeSubControls & SC_TitleBarContextHelpButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarContextHelpButton) && (titleBar->state & State_Sunken);
                if (contextHelpButtonRect.isValid()) {
                    drawMdiButton(SC_TitleBarContextHelpButton, painter, fgColor, fgColor.lighter(250), contextHelpButtonRect, hover, sunken);
                }
            }

            // draw the shade button
            if (titleBar->subControls & SC_TitleBarShadeButton && (titleBar->titleBarFlags & Qt::WindowShadeButtonHint)) {
                QRect shadeButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarShadeButton, widget);
                bool hover = (titleBar->activeSubControls & SC_TitleBarShadeButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarShadeButton) && (titleBar->state & State_Sunken);
                if (shadeButtonRect.isValid()) {
                    drawMdiButton(SC_TitleBarShadeButton, painter, fgColor, fgColor.lighter(250), shadeButtonRect, hover, sunken);
                }
            }

            // draw the unshade button
            if (titleBar->subControls & SC_TitleBarUnshadeButton && (titleBar->titleBarFlags & Qt::WindowShadeButtonHint)) {
                QRect unshadeButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarUnshadeButton, widget);
                bool hover = (titleBar->activeSubControls & SC_TitleBarUnshadeButton) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarUnshadeButton) && (titleBar->state & State_Sunken);
                if (unshadeButtonRect.isValid()) {
                    drawMdiButton(SC_TitleBarUnshadeButton, painter, fgColor, fgColor.lighter(250), unshadeButtonRect, hover, sunken);
                }
            }
            // draw the sys button
            if ((titleBar->subControls & SC_TitleBarSysMenu) && (titleBar->titleBarFlags & Qt::WindowSystemMenuHint)) {
                QRect iconRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarSysMenu, widget);
                bool hover = (titleBar->activeSubControls & SC_TitleBarSysMenu) && (titleBar->state & State_MouseOver);
                bool sunken = (titleBar->activeSubControls & SC_TitleBarSysMenu) && (titleBar->state & State_Sunken);
                if (iconRect.isValid()) {
                    drawMdiButton(SC_TitleBarSysMenu, painter, fgColor, fgColor.lighter(250), iconRect, hover, sunken);
                }
            }

//            painter->fillRect(textRect, Qt::yellow);

            /*
        if (const QStyleOptionTitleBar *titleBar = qstyleoption_cast<const QStyleOptionTitleBar *>(option)) {
            const int buttonMargin = 5;
            bool active = (titleBar->titleBarState & State_Active);
            QRect fullRect = titleBar->rect;
            QPalette palette = option->palette;
            QColor highlight = option->palette.highlight().color();

            QColor titleBarFrameBorder(active ? COLOR_DOCK_TITLE_BG: );
            QColor titleBarHighlight(active ? highlight.lighter(120): palette.background().color().lighter(120));
            QColor textColor(active ? 0xffffff : 0xff000000);
            QColor textAlphaColor(active ? 0xffffff : 0xff000000 );

            {
                // Fill title bar gradient
                QColor titlebarColor = QColor(active ? highlight: palette.background().color());
                QLinearGradient gradient(option->rect.center().x(), option->rect.top(),
                                         option->rect.center().x(), option->rect.bottom());

                gradient.setColorAt(0, titlebarColor.lighter(114));
                gradient.setColorAt(0.5, titlebarColor.lighter(102));
                gradient.setColorAt(0.51, titlebarColor.darker(104));
                gradient.setColorAt(1, titlebarColor);
                painter->fillRect(option->rect.adjusted(1, 1, -1, 0), gradient);

                // Frame and rounded corners
                painter->setPen(titleBarFrameBorder);

                // top outline
                painter->drawLine(fullRect.left() + 5, fullRect.top(), fullRect.right() - 5, fullRect.top());
                painter->drawLine(fullRect.left(), fullRect.top() + 4, fullRect.left(), fullRect.bottom());
                const QPoint points[5] = {
                    QPoint(fullRect.left() + 4, fullRect.top() + 1),
                    QPoint(fullRect.left() + 3, fullRect.top() + 1),
                    QPoint(fullRect.left() + 2, fullRect.top() + 2),
                    QPoint(fullRect.left() + 1, fullRect.top() + 3),
                    QPoint(fullRect.left() + 1, fullRect.top() + 4)
                };
                painter->drawPoints(points, 5);

                painter->drawLine(fullRect.right(), fullRect.top() + 4, fullRect.right(), fullRect.bottom());
                const QPoint points2[5] = {
                    QPoint(fullRect.right() - 3, fullRect.top() + 1),
                    QPoint(fullRect.right() - 4, fullRect.top() + 1),
                    QPoint(fullRect.right() - 2, fullRect.top() + 2),
                    QPoint(fullRect.right() - 1, fullRect.top() + 3),
                    QPoint(fullRect.right() - 1, fullRect.top() + 4)
                };
                painter->drawPoints(points2, 5);

                // draw bottomline
                painter->drawLine(fullRect.right(), fullRect.bottom(), fullRect.left(), fullRect.bottom());

                // top highlight
                painter->setPen(titleBarHighlight);
                painter->drawLine(fullRect.left() + 6, fullRect.top() + 1, fullRect.right() - 6, fullRect.top() + 1);
            }
            // draw title
            QRect textRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarLabel, widget);
            painter->setPen(active? (titleBar->palette.text().color().lighter(120)) :
                                    titleBar->palette.text().color() );
            // Note workspace also does elliding but it does not use the correct font
            QString title = painter->fontMetrics().elidedText(titleBar->text, Qt::ElideRight, textRect.width() - 14);
            painter->drawText(textRect.adjusted(1, 1, 1, 1), title, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
            painter->setPen(Qt::white);
            if (active)
                painter->drawText(textRect, title, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
            // min button
            if ((titleBar->subControls & SC_TitleBarMinButton) && (titleBar->titleBarFlags & Qt::WindowMinimizeButtonHint) &&
                    !(titleBar->titleBarState& Qt::WindowMinimized)) {
                QRect minButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarMinButton, widget);
                if (minButtonRect.isValid()) {
                    bool hover = (titleBar->activeSubControls & SC_TitleBarMinButton) && (titleBar->state & State_MouseOver);
                    bool sunken = (titleBar->activeSubControls & SC_TitleBarMinButton) && (titleBar->state & State_Sunken);
                    qt_fusion_draw_mdibutton(painter, titleBar, minButtonRect, hover, sunken);
                    QRect minButtonIconRect = minButtonRect.adjusted(buttonMargin ,buttonMargin , -buttonMargin, -buttonMargin);
                    painter->setPen(textColor);
                    painter->drawLine(minButtonIconRect.center().x() - 2, minButtonIconRect.center().y() + 3,
                                      minButtonIconRect.center().x() + 3, minButtonIconRect.center().y() + 3);
                    painter->drawLine(minButtonIconRect.center().x() - 2, minButtonIconRect.center().y() + 4,
                                      minButtonIconRect.center().x() + 3, minButtonIconRect.center().y() + 4);
                    painter->setPen(textAlphaColor);
                    painter->drawLine(minButtonIconRect.center().x() - 3, minButtonIconRect.center().y() + 3,
                                      minButtonIconRect.center().x() - 3, minButtonIconRect.center().y() + 4);
                    painter->drawLine(minButtonIconRect.center().x() + 4, minButtonIconRect.center().y() + 3,
                                      minButtonIconRect.center().x() + 4, minButtonIconRect.center().y() + 4);
                }
            }
            // max button
            if ((titleBar->subControls & SC_TitleBarMaxButton) && (titleBar->titleBarFlags & Qt::WindowMaximizeButtonHint) &&
                    !(titleBar->titleBarState & Qt::WindowMaximized)) {
                QRect maxButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarMaxButton, widget);
                if (maxButtonRect.isValid()) {
                    bool hover = (titleBar->activeSubControls & SC_TitleBarMaxButton) && (titleBar->state & State_MouseOver);
                    bool sunken = (titleBar->activeSubControls & SC_TitleBarMaxButton) && (titleBar->state & State_Sunken);
                    qt_fusion_draw_mdibutton(painter, titleBar, maxButtonRect, hover, sunken);

                    QRect maxButtonIconRect = maxButtonRect.adjusted(buttonMargin, buttonMargin, -buttonMargin, -buttonMargin);

                    painter->setPen(textColor);
                    painter->drawRect(maxButtonIconRect.adjusted(0, 0, -1, -1));
                    painter->drawLine(maxButtonIconRect.left() + 1, maxButtonIconRect.top() + 1,
                                      maxButtonIconRect.right() - 1, maxButtonIconRect.top() + 1);
                    painter->setPen(textAlphaColor);
                    const QPoint points[4] = {
                        maxButtonIconRect.topLeft(),
                        maxButtonIconRect.topRight(),
                        maxButtonIconRect.bottomLeft(),
                        maxButtonIconRect.bottomRight()
                    };
                    painter->drawPoints(points, 4);
                }
            }

            // close button
            if ((titleBar->subControls & SC_TitleBarCloseButton) && (titleBar->titleBarFlags & Qt::WindowSystemMenuHint)) {
                QRect closeButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarCloseButton, widget);
                if (closeButtonRect.isValid()) {
                    bool hover = (titleBar->activeSubControls & SC_TitleBarCloseButton) && (titleBar->state & State_MouseOver);
                    bool sunken = (titleBar->activeSubControls & SC_TitleBarCloseButton) && (titleBar->state & State_Sunken);
                    qt_fusion_draw_mdibutton(painter, titleBar, closeButtonRect, hover, sunken);
                    QRect closeIconRect = closeButtonRect.adjusted(buttonMargin, buttonMargin, -buttonMargin, -buttonMargin);
                    painter->setPen(textAlphaColor);
                    const QLine lines[4] = {
                        QLine(closeIconRect.left() + 1, closeIconRect.top(),
                        closeIconRect.right(), closeIconRect.bottom() - 1),
                        QLine(closeIconRect.left(), closeIconRect.top() + 1,
                        closeIconRect.right() - 1, closeIconRect.bottom()),
                        QLine(closeIconRect.right() - 1, closeIconRect.top(),
                        closeIconRect.left(), closeIconRect.bottom() - 1),
                        QLine(closeIconRect.right(), closeIconRect.top() + 1,
                        closeIconRect.left() + 1, closeIconRect.bottom())
                    };
                    painter->drawLines(lines, 4);
                    const QPoint points[4] = {
                        closeIconRect.topLeft(),
                        closeIconRect.topRight(),
                        closeIconRect.bottomLeft(),
                        closeIconRect.bottomRight()
                    };
                    painter->drawPoints(points, 4);

                    painter->setPen(textColor);
                    painter->drawLine(closeIconRect.left() + 1, closeIconRect.top() + 1,
                                      closeIconRect.right() - 1, closeIconRect.bottom() - 1);
                    painter->drawLine(closeIconRect.left() + 1, closeIconRect.bottom() - 1,
                                      closeIconRect.right() - 1, closeIconRect.top() + 1);
                }
            }

            // normalize button
            if ((titleBar->subControls & SC_TitleBarNormalButton) &&
                    (((titleBar->titleBarFlags & Qt::WindowMinimizeButtonHint) &&
                      (titleBar->titleBarState & Qt::WindowMinimized)) ||
                     ((titleBar->titleBarFlags & Qt::WindowMaximizeButtonHint) &&
                      (titleBar->titleBarState & Qt::WindowMaximized)))) {
                QRect normalButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarNormalButton, widget);
                if (normalButtonRect.isValid()) {

                    bool hover = (titleBar->activeSubControls & SC_TitleBarNormalButton) && (titleBar->state & State_MouseOver);
                    bool sunken = (titleBar->activeSubControls & SC_TitleBarNormalButton) && (titleBar->state & State_Sunken);
                    QRect normalButtonIconRect = normalButtonRect.adjusted(buttonMargin, buttonMargin, -buttonMargin, -buttonMargin);
                    qt_fusion_draw_mdibutton(painter, titleBar, normalButtonRect, hover, sunken);

                    QRect frontWindowRect = normalButtonIconRect.adjusted(0, 3, -3, 0);
                    painter->setPen(textColor);
                    painter->drawRect(frontWindowRect.adjusted(0, 0, -1, -1));
                    painter->drawLine(frontWindowRect.left() + 1, frontWindowRect.top() + 1,
                                      frontWindowRect.right() - 1, frontWindowRect.top() + 1);
                    painter->setPen(textAlphaColor);
                    const QPoint points[4] = {
                        frontWindowRect.topLeft(),
                        frontWindowRect.topRight(),
                        frontWindowRect.bottomLeft(),
                        frontWindowRect.bottomRight()
                    };
                    painter->drawPoints(points, 4);

                    QRect backWindowRect = normalButtonIconRect.adjusted(3, 0, 0, -3);
                    QRegion clipRegion = backWindowRect;
                    clipRegion -= frontWindowRect;
                    painter->save();
                    painter->setClipRegion(clipRegion);
                    painter->setPen(textColor);
                    painter->drawRect(backWindowRect.adjusted(0, 0, -1, -1));
                    painter->drawLine(backWindowRect.left() + 1, backWindowRect.top() + 1,
                                      backWindowRect.right() - 1, backWindowRect.top() + 1);
                    painter->setPen(textAlphaColor);
                    const QPoint points2[4] = {
                        backWindowRect.topLeft(),
                        backWindowRect.topRight(),
                        backWindowRect.bottomLeft(),
                        backWindowRect.bottomRight()
                    };
                    painter->drawPoints(points2, 4);
                    painter->restore();
                }
            }

            // context help button
            if (titleBar->subControls & SC_TitleBarContextHelpButton
                    && (titleBar->titleBarFlags & Qt::WindowContextHelpButtonHint)) {
                QRect contextHelpButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarContextHelpButton, widget);
                if (contextHelpButtonRect.isValid()) {
                    bool hover = (titleBar->activeSubControls & SC_TitleBarContextHelpButton) && (titleBar->state & State_MouseOver);
                    bool sunken = (titleBar->activeSubControls & SC_TitleBarContextHelpButton) && (titleBar->state & State_Sunken);
                    qt_fusion_draw_mdibutton(painter, titleBar, contextHelpButtonRect, hover, sunken);
                    QImage image(qt_titlebar_context_help);
                    QColor alpha = textColor;
                    alpha.setAlpha(128);
                    image.setColor(1, textColor.rgba());
                    image.setColor(2, alpha.rgba());
                    painter->setRenderHint(QPainter::SmoothPixmapTransform);
                    painter->drawImage(contextHelpButtonRect.adjusted(4, 4, -4, -4), image);
                }
            }

            // shade button
            if (titleBar->subControls & SC_TitleBarShadeButton && (titleBar->titleBarFlags & Qt::WindowShadeButtonHint)) {
                QRect shadeButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarShadeButton, widget);
                if (shadeButtonRect.isValid()) {
                    bool hover = (titleBar->activeSubControls & SC_TitleBarShadeButton) && (titleBar->state & State_MouseOver);
                    bool sunken = (titleBar->activeSubControls & SC_TitleBarShadeButton) && (titleBar->state & State_Sunken);
                    qt_fusion_draw_mdibutton(painter, titleBar, shadeButtonRect, hover, sunken);
                    QPixmap arrow = colorizedImage(QLatin1String(":/qt-project.org/styles/commonstyle/images/fusion_arrow.png"), textColor);
                    painter->drawPixmap(shadeButtonRect.adjusted(5, 7, -5, -7), arrow);
                }
            }

            // unshade button
            if (titleBar->subControls & SC_TitleBarUnshadeButton && (titleBar->titleBarFlags & Qt::WindowShadeButtonHint)) {
                QRect unshadeButtonRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarUnshadeButton, widget);
                if (unshadeButtonRect.isValid()) {
                    bool hover = (titleBar->activeSubControls & SC_TitleBarUnshadeButton) && (titleBar->state & State_MouseOver);
                    bool sunken = (titleBar->activeSubControls & SC_TitleBarUnshadeButton) && (titleBar->state & State_Sunken);
                    qt_fusion_draw_mdibutton(painter, titleBar, unshadeButtonRect, hover, sunken);
                    QPixmap arrow = colorizedImage(QLatin1String(":/qt-project.org/styles/commonstyle/images/fusion_arrow.png"), textColor, 180);
                    painter->drawPixmap(unshadeButtonRect.adjusted(5, 7, -5, -7), arrow);
                }
            }

            if ((titleBar->subControls & SC_TitleBarSysMenu) && (titleBar->titleBarFlags & Qt::WindowSystemMenuHint)) {
                QRect iconRect = proxy()->subControlRect(CC_TitleBar, titleBar, SC_TitleBarSysMenu, widget);
                if (iconRect.isValid()) {
                    if (!titleBar->icon.isNull()) {
                        titleBar->icon.paint(painter, iconRect);
                    } else {
                        QStyleOption tool(0);
                        tool.palette = titleBar->palette;
                        QPixmap pm = standardIcon(SP_TitleBarMenuButton, &tool, widget).pixmap(16, 16);
                        tool.rect = iconRect;
                        painter->save();
                        proxy()->drawItemPixmap(painter, iconRect, Qt::AlignCenter, pm);
                        painter->restore();
                    }
                }
            }
        }
        */
        painter->restore();
        }
        break;
    default:
        QCommonStyle::drawComplexControl(control, option, painter, widget);
        break;
    }


}

QRect MaterialStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget) const
{
    QRect rect = QCommonStyle::subControlRect(cc, opt, sc, widget);
    //    QRect rect;
    switch (cc) {
    case CC_GroupBox: {
        if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(opt)) {
            rect = opt->rect;

            if (sc == SC_GroupBoxFrame) {
                return groupBox->rect;
            }

            QFontMetrics fm(groupBoxFont);
            QSize textSize = fm.boundingRect(groupBox->text).size();
            int checkIndicatorWidth = pixelMetric(PM_IndicatorWidth, opt, widget);
            int headerHeight = 2 * GROUP_BOX_HEADER_MARGINS + qMax(textSize.height(), checkIndicatorWidth);

            if (sc == SC_GroupBoxCheckBox) {
                QRect indRect(0, 0, checkIndicatorWidth, checkIndicatorWidth);
                indRect.moveLeft(GROUP_BOX_INDICATOR_MARGIN);
                indRect.moveTop((headerHeight - checkIndicatorWidth) / 2);
                return (groupBox->subControls & SC_GroupBoxCheckBox) ? indRect : QRect();
            }
            if (sc == SC_GroupBoxLabel) {
                QRect textRect;
                textRect.setSize(textSize);
                textRect.moveLeft(GROUP_BOX_TEXT_V_MARGIN +
                                  ((groupBox->subControls & SC_GroupBoxCheckBox) ? GROUP_BOX_INDICATOR_MARGIN + checkIndicatorWidth :
                                                                                  0));
                textRect.moveTop((headerHeight - textSize.height()) / 2);
                return textRect;
            }
//            if (sc == SC_GroupBoxContents) {
                return groupBox->rect.adjusted(1, (groupBox->subControls & (SC_GroupBoxCheckBox | SC_GroupBoxLabel)) ? headerHeight + GROUP_BOX_UNDERLINE_WIDTH + 1 : 1,
                                               -1, -1);

        }
        break;
    }
    case CC_Slider:
        if (const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>(opt)) {
            //            int tickSize = proxy()->pixelMetric(PM_SliderTickmarkOffset, opt, widget);
            switch (sc) {
            case SC_SliderHandle: {
                if (slider->orientation == Qt::Horizontal) {
                    rect.setHeight(pixelMetric(PM_SliderControlThickness));
                    rect.setWidth(pixelMetric(PM_SliderLength));
                    int ry = (slider->rect.height() - rect.height()) / 2;
                    //                    int centerY = slider->rect.center().y() - rect.height() / 2;
                    //                    if (slider->tickPosition & QSlider::TicksAbove)
                    //                        centerY += tickSize;
                    //                    if (slider->tickPosition & QSlider::TicksBelow)
                    //                        centerY -= tickSize;
                    rect.moveTop(ry);
                } else {
                    rect.setWidth(pixelMetric(PM_SliderControlThickness));
                    rect.setHeight(pixelMetric(PM_SliderLength));
                    int rx = (slider->rect.width() - rect.width()) / 2;
                    //                    int centerX = slider->rect.center().x() - rect.width() / 2;
                    //                    if (slider->tickPosition & QSlider::TicksAbove)
                    //                        centerX += tickSize;
                    //                    if (slider->tickPosition & QSlider::TicksBelow)
                    //                        centerX -= tickSize;
                    rect.moveLeft(rx);
                }
            }
                break;
            case SC_SliderGroove: {
                //                QPoint grooveCenter = slider->rect.center();
                if (slider->orientation == Qt::Horizontal) {
                    rect.setHeight(10);
                    rect.moveTop((slider->rect.height() - 10) / 2);
                    //                    if (slider->tickPosition & QSlider::TicksAbove)
                    //                        grooveCenter.ry() += tickSize;
                    //                    if (slider->tickPosition & QSlider::TicksBelow)
                    //                        grooveCenter.ry() -= tickSize;
                } else {
                    rect.setWidth(10);
                    rect.moveLeft((slider->rect.width() - 10) / 2);
                    //                    if (slider->tickPosition & QSlider::TicksAbove)
                    //                        grooveCenter.rx() += tickSize;
                    //                    if (slider->tickPosition & QSlider::TicksBelow)
                    //                        grooveCenter.rx() -= tickSize;
                }
                break;
            }
            default:
                break;
            }
        }
        break;
    case CC_ComboBox:
        switch (sc) {
        case SC_ComboBoxArrow:
            rect = visualRect(opt->direction, opt->rect, rect);
            rect.setRect(rect.right() - COMBOBOX_BUTTON_WIDTH, rect.top() - 2,
                         (COMBOBOX_BUTTON_WIDTH + 1), rect.height() + 4);
            rect = visualRect(opt->direction, opt->rect, rect);
            break;
        case SC_ComboBoxEditField:
            int frameWidth = 2;
            rect = visualRect(opt->direction, opt->rect, rect);
            rect.setRect(opt->rect.left() + frameWidth, opt->rect.top() + frameWidth,
                         opt->rect.width() - (COMBOBOX_BUTTON_WIDTH + 1) - 2 * frameWidth,
                         opt->rect.height() - 2 * frameWidth);
            if (const QStyleOptionComboBox *box = qstyleoption_cast<const QStyleOptionComboBox *>(opt)) {
                if (!box->editable) {
                    rect.adjust(2, 0, 0, 0);
                    if (box->state & (State_Sunken | State_On))
                        rect.translate(1, 1);
                }
            }
            rect = visualRect(opt->direction, opt->rect, rect);
            break;
        }
    case CC_SpinBox:
        if (const QStyleOptionSpinBox *spinbox = qstyleoption_cast<const QStyleOptionSpinBox *>(opt)) {
//            QSize bs;
//            int center = spinbox->rect.height() / 2;
//            int fw = spinbox->frame ? proxy()->pixelMetric(PM_SpinBoxFrameWidth, spinbox, widget) : 0;
//            int y = fw;
//            bs.setHeight(qMax(8, spinbox->rect.height()/2 - y));
//            bs.setWidth(14);
//            int x, lx, rx;
//            x = spinbox->rect.width() - y - bs.width() + 2;
//            lx = fw;
//            rx = x - fw;
//            switch (sc) {
//            case SC_SpinBoxUp:
//                if (spinbox->buttonSymbols == QAbstractSpinBox::NoButtons)
//                    return QRect();
//                rect = QRect(x, fw, bs.width(), center - fw);
//                break;
//            case SC_SpinBoxDown:
//                if (spinbox->buttonSymbols == QAbstractSpinBox::NoButtons)
//                    return QRect();

//                rect = QRect(x, center, bs.width(), spinbox->rect.bottom() - center - fw + 1);
//                break;
//            case SC_SpinBoxEditField:
//                if (spinbox->buttonSymbols == QAbstractSpinBox::NoButtons) {
//                    rect = QRect(lx, fw, spinbox->rect.width() - 2*fw, spinbox->rect.height() - 2*fw);
//                } else {
//                    rect = QRect(lx, fw, rx - qMax(fw - 1, 0), spinbox->rect.height() - 2*fw);
//                }
//                break;
//            case SC_SpinBoxFrame:
//                rect = spinbox->rect;
//            default:
//                break;
//            }
            int bH = (spinbox->rect.height() - 2) / 2;
            int bW = SPINBOX_BUTTON_WIDTH;
            switch (sc) {
            case SC_SpinBoxUp:
                rect.setX(spinbox->rect.width() - bW - 1);
                rect.setY(1);
                rect.setSize(QSize(bW, bH));
                break;
            case SC_SpinBoxDown:
                rect.setX(spinbox->rect.width() - bW - 1);
                rect.setY(1 + bH);
                rect.setSize(QSize(bW, bH));
                break;
            case SC_SpinBoxEditField:
                rect.setRect(3, 1, spinbox->rect.width() - bW - 4, spinbox->rect.height() - 2);
            default:
                break;
            }

            rect = visualRect(spinbox->direction, spinbox->rect, rect);
        }
        break;
    case CC_TitleBar:
        if (const QStyleOptionTitleBar *tb = qstyleoption_cast<const QStyleOptionTitleBar *>(opt)) {
            QRect &ret = rect;
            const int indent = 3;
            const int controlTopMargin = 3;
            const int controlBottomMargin = 3;
            const int controlWidthMargin = 2;
            const int controlHeight = tb->rect.height() - controlTopMargin - controlBottomMargin ;
            const int delta = controlHeight + controlWidthMargin;
            int offset = 0;

            bool isMinimized = tb->titleBarState & Qt::WindowMinimized;
            bool isMaximized = tb->titleBarState & Qt::WindowMaximized;

            switch (sc) {
            case SC_TitleBarLabel:
                if (tb->titleBarFlags & (Qt::WindowTitleHint | Qt::WindowSystemMenuHint)) {
                    const int textVMargin = 2;
                    ret = tb->rect.adjusted(0, 1, 0, 0);
                    if (tb->titleBarFlags & Qt::WindowSystemMenuHint)
                        ret.adjust(delta + textVMargin, 0, -delta, 0);
                    if (tb->titleBarFlags & Qt::WindowMinimizeButtonHint)
                        ret.adjust(0, 0, -delta, 0);
                    if (tb->titleBarFlags & Qt::WindowMaximizeButtonHint)
                        ret.adjust(0, 0, -delta, 0);
                    if (tb->titleBarFlags & Qt::WindowShadeButtonHint)
                        ret.adjust(0, 0, -delta, 0);
                    if (tb->titleBarFlags & Qt::WindowContextHelpButtonHint)
                        ret.adjust(0, 0, -delta, 0);
                }
                break;
            case SC_TitleBarContextHelpButton:
                if (tb->titleBarFlags & Qt::WindowContextHelpButtonHint)
                    offset += delta;
            case SC_TitleBarMinButton:
                if (!isMinimized && (tb->titleBarFlags & Qt::WindowMinimizeButtonHint))
                    offset += delta;
                else if (sc == SC_TitleBarMinButton)
                    break;
            case SC_TitleBarNormalButton:
                if (isMinimized && (tb->titleBarFlags & Qt::WindowMinimizeButtonHint))
                    offset += delta;
                else if (isMaximized && (tb->titleBarFlags & Qt::WindowMaximizeButtonHint))
                    offset += delta;
                else if (sc == SC_TitleBarNormalButton)
                    break;
            case SC_TitleBarMaxButton:
                if (!isMaximized && (tb->titleBarFlags & Qt::WindowMaximizeButtonHint))
                    offset += delta;
                else if (sc == SC_TitleBarMaxButton)
                    break;
            case SC_TitleBarShadeButton:
                if (!isMinimized && (tb->titleBarFlags & Qt::WindowShadeButtonHint))
                    offset += delta;
                else if (sc == SC_TitleBarShadeButton)
                    break;
            case SC_TitleBarUnshadeButton:
                if (isMinimized && (tb->titleBarFlags & Qt::WindowShadeButtonHint))
                    offset += delta;
                else if (sc == SC_TitleBarUnshadeButton)
                    break;
            case SC_TitleBarCloseButton:
                if (tb->titleBarFlags & Qt::WindowSystemMenuHint)
                    offset += delta;
                else if (sc == SC_TitleBarCloseButton)
                    break;
                ret.setRect(tb->rect.right() - indent - offset, tb->rect.top() + controlTopMargin,
                            controlHeight, controlHeight);
                break;
            case SC_TitleBarSysMenu:
                if (tb->titleBarFlags & Qt::WindowSystemMenuHint) {
                    ret.setRect(tb->rect.left() + controlWidthMargin + indent, tb->rect.top() + controlTopMargin,
                                controlHeight, controlHeight);
                }
                break;
            default:
                break;
            }
            ret = visualRect(tb->direction, tb->rect, ret);
        }
        break;
    default:
        return rect;
    }
    return rect;
}

int MaterialStyle::pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    switch (metric) {
    case PM_ButtonMargin:
        return 3;
    case PM_FocusFrameHMargin:
    case PM_FocusFrameVMargin:
        return 1;
    case PM_SliderTickmarkOffset:
        return 4;
    case PM_SliderControlThickness:
        return 12;
    case PM_HeaderMargin:
        return 2;
    case PM_ToolTipLabelFrameWidth:
        return 2;
    case PM_ButtonDefaultIndicator:
        return 0;
    case PM_ButtonShiftHorizontal:
    case PM_ButtonShiftVertical:
        return 0;
    case PM_MessageBoxIconSize:
        return 48;
    case PM_ListViewIconSize:
        return 24;
    case PM_DialogButtonsSeparator:
    case PM_ScrollBarSliderMin:
        return 26;
    case PM_TitleBarHeight:
        return 24;
    case PM_ScrollBarExtent:
        return 14;
    case PM_SliderThickness:
        return 15;
    case PM_SliderLength:
        return 15;
    case PM_DockWidgetTitleMargin:
        return 2;
    case PM_DefaultFrameWidth:
        return 1;
    case PM_SpinBoxFrameWidth:
        return 3;
    case PM_MenuVMargin:
    case PM_MenuHMargin:
        return 0;
    case PM_MenuPanelWidth:
        return 0;
    case PM_MenuBarItemSpacing:
        return 6;
    case PM_MenuBarVMargin:
        return 0;
    case PM_MenuBarHMargin:
        return 0;
    case PM_MenuBarPanelWidth:
        return 0;
    case PM_ToolBarHandleExtent:
        return 9;
    case PM_ToolBarItemSpacing:
        return 1;
    case PM_ToolBarFrameWidth:
        return 2;
    case PM_ToolBarItemMargin:
        return 2;
    case PM_SmallIconSize:
        return 16;
    case PM_ButtonIconSize:
        return 16;
    case PM_DockWidgetTitleBarButtonMargin:
        return 2;
    case PM_MaximumDragDistance:
        return -1;
    case PM_TabCloseIndicatorWidth:
    case PM_TabCloseIndicatorHeight:
        return 5;
    case PM_TabBarTabVSpace:
        return 12;
    case PM_TabBarTabOverlap:
        return 0;
    case PM_TabBarBaseOverlap:
        return 0;
    case PM_SubMenuOverlap:
        return -1;
    case PM_DockWidgetHandleExtent:
    case PM_SplitterWidth:
        return 4;
    case PM_IndicatorHeight:
    case PM_IndicatorWidth:
    case PM_ExclusiveIndicatorHeight:
    case PM_ExclusiveIndicatorWidth:
        return 12;
    case PM_ScrollView_ScrollBarSpacing:
        return 0;
    case PM_ScrollView_ScrollBarOverlap:
        if (proxy()->styleHint(SH_ScrollBar_Transient, option, widget))
            return proxy()->pixelMetric(PM_ScrollBarExtent, option, widget);
        return 0;
    default:
        break;
    }
    return QCommonStyle::pixelMetric(metric, option, widget);
}

void MaterialStyle::polish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget)
            || qobject_cast<QProgressBar *>(widget)
            || qobject_cast<QScrollBar *>(widget)
            || qobject_cast<QSplitterHandle *>(widget)
            || qobject_cast<QAbstractSlider *>(widget)
            || qobject_cast<QAbstractSpinBox *>(widget)
            || qobject_cast<QTabBar *>(widget)
            || (widget->inherits("QDockSeparator"))
            || (widget->inherits("QDockWidgetSeparator"))) {
        widget->setAttribute(Qt::WA_Hover, true);
        widget->setAttribute(Qt::WA_OpaquePaintEvent, false);
    }

    if (QToolBox *tb = qobject_cast<QToolBox *>(widget)) {
        tb->setFrameStyle(QFrame::StyledPanel);
//        tb->setFrameShape(QFrame::StyledPanel);
        QPalette pal = tb->palette();
        pal.setBrush(QPalette::Button, COLOR_TOOLBOX_CONTENT_BG);
        tb->setPalette(pal);
        tb->setAttribute(Qt::WA_Hover, true);
        tb->setAttribute(Qt::WA_OpaquePaintEvent, false);
        tb->layout()->setSpacing(0);
        connect(tb, SIGNAL(currentChanged(int)), tb, SLOT(repaint()));
//        tb->setBackgroundRole(QPalette::NoRole);
        return;
    }
//    if (QTableWidget *tw = qobject_cast<QTableWidget *>(widget)) {
//        QPalette pal = tw->palette();
//        pal.setBrush(QPalette::AlternateBase, COLOR_PALETTE_ALTENATE_BASE);
//        tw->setPalette(pal);
//        tw->setAlternatingRowColors(true);
//    }
    if (QAbstractItemView *iv = qobject_cast<QAbstractItemView *>(widget)) {
        iv->setAlternatingRowColors(true);
    }
}

void MaterialStyle::polish(QPalette &pal)
{
    pal.setBrush(QPalette::Window, COLOR_PALETTE_WINDOW);       //window background
    pal.setBrush(QPalette::Light, QColor(64, 148, 208));         //pushbutton normal start gradient
    pal.setBrush(QPalette::Dark, QColor(7, 58, 147));            //pushbutton normal end gradient
    pal.setBrush(QPalette::Midlight, QColor(132, 191, 247));     //pushbutton hover start gradient
    pal.setBrush(QPalette::Mid, QColor(43, 84, 163));            //pushbutton hover end gradient
//    pal.setBrush(QPalette::Button, COLOR_PALETTE_HIGHLIGHT);       //button normal color
    pal.setBrush(QPalette::ButtonText, COLOR_PALETTE_BUTTON_TEXT);   //pushbutton text color
    pal.setBrush(QPalette::Shadow, QColor(0, 0, 0));             //shadow
    pal.setBrush(QPalette::BrightText, QColor(7, 30, 82));       //pushbutton hover text color
    pal.setBrush(QPalette::Disabled, QPalette::ButtonText, QColor(171, 171, 171));
    pal.setBrush(QPalette::WindowText, COLOR_WINDOW_TEXT);
    pal.setBrush(QPalette::Highlight, COLOR_PALETTE_HIGHLIGHT);
    pal.setBrush(QPalette::Text, COLOR_PALETTE_TEXT);
    pal.setBrush(QPalette::HighlightedText, COLOR_PALETTE_HL_TEXT);
    pal.setBrush(QPalette::AlternateBase, COLOR_PALETTE_ALTENATE_BASE);
}

void MaterialStyle::unpolish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget)
            || qobject_cast<QProgressBar *>(widget)
            || qobject_cast<QScrollBar *>(widget)
            || qobject_cast<QSplitterHandle *>(widget)
            || qobject_cast<QAbstractSlider *>(widget)
            || qobject_cast<QAbstractSpinBox *>(widget)
            || qobject_cast<QTabBar *>(widget)
            || (widget->inherits("QDockSeparator"))
            || (widget->inherits("QDockWidgetSeparator")))
        widget->setAttribute(Qt::WA_Hover, false);
}

QRect MaterialStyle::subElementRect(QStyle::SubElement subElem, const QStyleOption *opt, const QWidget *widget) const
{
    QRect r;
    switch (subElem) {
    case SE_CheckBoxIndicator: {
        const QStyleOptionButton *checkBox = qstyleoption_cast<const QStyleOptionButton *>(opt);
        int chkWidth = pixelMetric(PM_IndicatorWidth, opt, widget);
        QRect indRect(0, 0, chkWidth, chkWidth);
        indRect.moveTo(0, (checkBox->rect.height() - chkWidth) / 2);
        return indRect;
    }
    case SE_CheckBoxContents: {
        //        const QStyleOptionButton *checkBox = qstyleoption_cast<const QStyleOptionButton *>(opt);
        //        int chkWidth = pixelMetric(PM_IndicatorWidth, opt, widget);
        //        int space = pixelMetric(PM_CheckBoxLabelSpacing, opt, widget);
        //        QFontMetrics fm(regularFont);
        //        QRect textRect = fm.boundingRect(checkBox->text);
        //        textRect.moveTo(chkWidth + space, (checkBox->rect.height() - textRect.height()) / 2);
        //        return textRect;
        QRect ir = visualRect(opt->direction, opt->rect,
                              subElementRect(SE_CheckBoxIndicator, opt, widget));
        int spacing = proxy()->pixelMetric(PM_CheckBoxLabelSpacing, opt, widget);
        r.setRect(ir.right() + spacing, opt->rect.y(), opt->rect.width() - ir.width() - spacing,
                  opt->rect.height());
        r = visualRect(opt->direction, opt->rect, r);
    }
        break;
    case SE_ProgressBarLabel:
    case SE_ProgressBarContents:
    case SE_ProgressBarGroove:
        return opt->rect;
//    case SE_DockWidgetTitleBarText: {
//        if (const QStyleOptionDockWidget *titlebar = qstyleoption_cast<const QStyleOptionDockWidget*>(opt)) {
//            bool verticalTitleBar = titlebar->verticalTitleBar;
//            if (verticalTitleBar) {
//                r.adjust(0, 0, 0, -4);
//            } else {
//                if (opt->direction == Qt::LeftToRight)
//                    r.adjust(4, 0, 0, 0);
//                else
//                    r.adjust(0, 0, -4, 0);
//            }
//        }

//        break;
//    }
    default:
        return QCommonStyle::subElementRect(subElem, opt, widget);
    }
    return r;
}

int MaterialStyle::styleHint(QStyle::StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const
{
    switch (sh) {
    case SH_ScrollBar_Transient:
        return 0;
    case SH_Slider_SnapToValue:
    case SH_PrintDialog_RightAlignButtons:
    case SH_FontDialog_SelectAssociatedText:
    case SH_MenuBar_AltKeyNavigation:
    case SH_ComboBox_ListMouseTracking:
    case SH_ScrollBar_StopMouseOverSlider:
    case SH_ScrollBar_MiddleClickAbsolutePosition:
    case SH_EtchDisabledText:
    case SH_TitleBar_AutoRaise:
    case SH_TitleBar_NoBorder:
    case SH_ItemView_ShowDecorationSelected:
    case SH_ItemView_ArrowKeysNavigateIntoChildren:
    case SH_ItemView_ChangeHighlightOnFocus:
    case SH_MenuBar_MouseTracking:
    case SH_Menu_MouseTracking:
    case SH_Menu_SupportsSections:
    case SH_FocusFrame_AboveWidget:
        return 1;

    case SH_ToolBox_SelectedPageTitleBold:
    case SH_ScrollView_FrameOnlyAroundContents:
    case SH_Menu_AllowActiveAndDisabled:
    case SH_MainWindow_SpaceBelowMenuBar:
    case SH_DialogButtonBox_ButtonsHaveIcons:
    case SH_MessageBox_CenterButtons:
    case SH_RubberBand_Mask:
        return 0;
    case SH_WindowFrame_Mask:
        if (QStyleHintReturnMask *mask = qstyleoption_cast<QStyleHintReturnMask *>(shret)) {
            //left rounded corner
            mask->region = opt->rect;
            mask->region -= QRect(opt->rect.left(), opt->rect.top(), 5, 1);
            mask->region -= QRect(opt->rect.left(), opt->rect.top() + 1, 3, 1);
            mask->region -= QRect(opt->rect.left(), opt->rect.top() + 2, 2, 1);
            mask->region -= QRect(opt->rect.left(), opt->rect.top() + 3, 1, 2);

            //right rounded corner
            mask->region -= QRect(opt->rect.right() - 4, opt->rect.top(), 5, 1);
            mask->region -= QRect(opt->rect.right() - 2, opt->rect.top() + 1, 3, 1);
            mask->region -= QRect(opt->rect.right() - 1, opt->rect.top() + 2, 2, 1);
            mask->region -= QRect(opt->rect.right() , opt->rect.top() + 3, 1, 2);

            //bottom left
            mask->region -= QRect(opt->rect.left(), opt->rect.bottom(), 5, 1);
            mask->region -= QRect(opt->rect.left(), opt->rect.bottom() - 1, 3, 1);
            mask->region -= QRect(opt->rect.left(), opt->rect.bottom() - 2, 2, 1);
            mask->region -= QRect(opt->rect.left(), opt->rect.bottom() - 4, 1, 2);

            //bottom right
            mask->region -= QRect(opt->rect.right() - 4, opt->rect.bottom(), 5, 1);
            mask->region -= QRect(opt->rect.right() - 2, opt->rect.bottom() - 1, 3, 1);
            mask->region -= QRect(opt->rect.right() - 1, opt->rect.bottom() - 2, 2, 1);
            mask->region -= QRect(opt->rect.right(), opt->rect.bottom() - 4, 1, 2);
            return 1;
        }
    case SH_Table_GridLineColor:
        return COLOR_FRAME_BORDER.rgb();
    default:
        return QCommonStyle::styleHint(sh, opt, w, shret);
    }
}

QSize MaterialStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    QSize newSize = QCommonStyle::sizeFromContents(type, option, size, widget);
    switch (type) {
    case CT_PushButton:
        if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option)) {
            if (!btn->text.isEmpty() && newSize.width() < 80)
                newSize.setWidth(80);
            if (!btn->icon.isNull() && btn->iconSize.height() > 16)
                newSize -= QSize(0, 2);
        }
        break;
    case CT_GroupBox:
        if (option) {
            int topMargin = qMax(pixelMetric(PM_ExclusiveIndicatorHeight), option->fontMetrics.height()) + GROUP_BOX_TOP_MARGIN;
            newSize += QSize(10, topMargin); // Add some space below the groupbox
        }
        break;
    case CT_RadioButton:
    case CT_CheckBox:
        newSize += QSize(0, 1);
        break;
    case CT_ToolButton:
        newSize += QSize(2, 2);
        break;
    case CT_SpinBox:
        newSize += QSize(0, -3);
        break;
    case CT_ComboBox:
        newSize += QSize(2, 4);
        break;
    case CT_LineEdit:
        newSize += QSize(0, 4);
        break;
    case CT_MenuBarItem:
        newSize += QSize(8, 5);
        break;
    case CT_MenuItem:
        if (const QStyleOptionMenuItem *menuItem = qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
            int w = newSize.width();
            int maxpmw = menuItem->maxIconWidth;
            int tabSpacing = 20;
            if (menuItem->text.contains(QLatin1Char('\t')))
                w += tabSpacing;
            else if (menuItem->menuItemType == QStyleOptionMenuItem::SubMenu)
                w += 2 * menuArrowHMargin;
            else if (menuItem->menuItemType == QStyleOptionMenuItem::DefaultItem) {
                QFontMetrics fm(menuItem->font);
                QFont fontBold = menuItem->font;
                fontBold.setBold(true);
                QFontMetrics fmBold(fontBold);
                w += fmBold.width(menuItem->text) - fm.width(menuItem->text);
            }
            int checkcol = qMax<int>(maxpmw, menuCheckMarkWidth); // Windows always shows a check column
            w += checkcol;
            w += int(menuRightBorder) + 10;
            newSize.setWidth(w);
            if (menuItem->menuItemType == QStyleOptionMenuItem::Separator) {
                if (!menuItem->text.isEmpty()) {
                    newSize.setHeight(menuItem->fontMetrics.height());
                }
            }
            else if (!menuItem->icon.isNull()) {
                if (const QComboBox *combo = qobject_cast<const QComboBox*>(widget)) {
                    newSize.setHeight(qMax(combo->iconSize().height() + 2, newSize.height()));
                }
            }
            newSize.setWidth(newSize.width() + 12);
            newSize.setWidth(qMax(newSize.width(), 120));
        }
        break;
    case CT_SizeGrip:
        newSize += QSize(4, 4);
        break;
    case CT_MdiControls:
        if (const QStyleOptionComplex *styleOpt = qstyleoption_cast<const QStyleOptionComplex *>(option)) {
            int width = 0;
            if (styleOpt->subControls & SC_MdiMinButton)
                width += 19 + 1;
            if (styleOpt->subControls & SC_MdiNormalButton)
                width += 19 + 1;
            if (styleOpt->subControls & SC_MdiCloseButton)
                width += 19 + 1;
            newSize = QSize(width, 19);
        } else {
            newSize = QSize(60, 19);
        }
        break;
    default:
        break;
    }
    return newSize;
}

QPixmap MaterialStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const
{
    if (const QStyleOptionToolBox *tb = qstyleoption_cast<const QStyleOptionToolBox *>(opt)) {
        int iw = pixelMetric(PM_SmallIconSize, opt);
        QImage img(iw, iw, QImage::Format_ARGB32);
        img.fill(Qt::transparent);
        QPainter p(&img);
        prepareSmothPainter(&p);
        bool enabled = iconMode == QIcon::Normal;
        p.setPen(Qt::NoPen);
        p.setBrush(enabled ? COLOR_TAB_SELECTED_UL : COLOR_FG_DISABLED);
        p.drawEllipse(QRect(0, 0, iw - 1, iw - 1));
        drawArrow(&p, enabled ? Qt::DownArrow : Qt::RightArrow, Qt::white, QRect(0, 0, iw - 1, iw - 1));
        QPixmap px;
        px.convertFromImage(img);
        return px;
    }
    return QCommonStyle::generatedIconPixmap(iconMode, pixmap, opt);
}

QPixmap MaterialStyle::standardPixmap(QStyle::StandardPixmap sp, const QStyleOption *option, const QWidget *widget) const
{
//    qDebug() << sp;
    switch (sp) {
    case SP_TitleBarCloseButton:
    case SP_DockWidgetCloseButton: {
        int sz = 11;
        QImage img(sz, sz, QImage::Format_ARGB32);
        img.fill(Qt::transparent);
        QPainter painter(&img);

        QRect imgRect(0, 0, sz, sz);
        QRect indRect(0, 0, sz - 6, sz - 6);
        indRect.moveCenter(imgRect.center());
        painter.save();
        prepareSmothPainter(&painter);
        painter.setPen(QPen(COLOR_DOCK_TITLE_FG, 2));
        painter.drawLine(indRect.left() - 1, indRect.top() - 1, indRect.right() + 1, indRect.bottom() + 1);
        painter.drawLine(indRect.right() + 1, indRect.top() - 1, indRect.left() - 1, indRect.bottom() + 1);
        painter.restore();
        return QPixmap::fromImage(img);
    }
    case SP_TitleBarNormalButton: {
        int sz = 11;
        QImage img(sz, sz, QImage::Format_ARGB32);
        img.fill(Qt::transparent);
        QPainter painter(&img);

        QRect imgRect(0, 0, sz, sz);
        QRect indRect(0, 0, sz - 6, sz - 6);
        indRect.moveCenter(imgRect.center());
        painter.save();
        prepareSmothPainter(&painter);
        painter.setPen(QPen(COLOR_DOCK_TITLE_FG, 1));
        QRect topRect(sz / 2 - 1, sz / 5 - 1, sz / 2, sz / 2);
        QRect bottomRect(sz / 5 - 1, sz / 2 - 1, sz / 2, sz / 2);
        painter.drawRect(topRect);
        painter.setBrush(COLOR_DOCK_TITLE_FG);
        painter.drawRect(bottomRect);
//        QRegion region(indRect);
////        region.
////        painter.setClipRegion(region.subtracted(QRegion(bottomRect)));
        painter.restore();
        return QPixmap::fromImage(img);
    }
    default:
        return QCommonStyle::standardPixmap(sp, option, widget);
    }

}

void MaterialStyle::removeAnimation()
{
    QObject *animation = sender();
    if (animation)
        animations.remove(animation->parent());
}

QPainterPath MaterialStyle::roundRectPath(const QRect &rect) const
{
    int radius = BUTTON_RADIUS;
    int diam = 2 * radius;

    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);

    QPainterPath path;
    path.moveTo(x2, y1 + radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
    path.lineTo(x1 + radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
    path.lineTo(x1, y2 - radius);
    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
    path.lineTo(x1 + radius, y2);
    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
    path.closeSubpath();
    return path;
}

QPainterPath MaterialStyle::roundTopBorder(const QRect &rect) const
{
    int radius = BUTTON_RADIUS;
    int diam = 2 * radius;

    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);

    QPainterPath path;
    path.moveTo(x2, y1 + radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
    path.lineTo(x1 + radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
    //    path.lineTo(x1, y2 - radius);
    //    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
    //    path.lineTo(x1 + radius, y2);
    //    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
    //    path.closeSubpath();
    return path;
}

void MaterialStyle::stopAnimation(const QObject *target) const
{
    StyleAnimation *animation = animations.take(target);
    if (animation) {
        animation->stop();
        delete animation;
    }
}

StyleAnimation *MaterialStyle::animation(const QObject *target) const
{
    return animations.value(target);
}

void MaterialStyle::startAnimation(StyleAnimation *animation) const
{
    stopAnimation(animation->target());
    connect(animation, SIGNAL(destroyed()), SLOT(removeAnimation()), Qt::UniqueConnection);
    animations.insert(animation->target(), animation);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MaterialStyle::drawArrow(QPainter *painter, Qt::ArrowType type, const QColor &color, const QRect &rect) const
{
    QPainterPath path;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(0.5, 0.5);
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    bool horizontal = (type == Qt::RightArrow || type == Qt::LeftArrow);
    QRect r(0, 0, 6 + (horizontal ? 0 : 1), 6 + (horizontal ? 1 : 0));
    r.moveCenter(rect.center());
    switch (type) {
    case Qt::UpArrow:
        path.moveTo(r.bottomLeft());
        path.lineTo(r.left() + r.width() / 2, r.top());
        path.lineTo(r.bottomRight());
        path.lineTo(r.bottomLeft());
        break;
    case Qt::DownArrow:
        path.moveTo(r.topLeft());
        path.lineTo(r.left() + r.width() / 2, r.bottom());
        path.lineTo(r.topRight());
        path.lineTo(r.topLeft());
        break;
    case Qt::RightArrow:
        path.moveTo(r.topLeft());
        path.lineTo(r.right(), r.top() + r.height() / 2);
        path.lineTo(r.bottomLeft());
        path.lineTo(r.topLeft());
        break;
    case Qt::LeftArrow:
        path.moveTo(r.topRight());
        path.lineTo(r.bottomRight());
        path.lineTo(r.left(), r.top() + r.height() / 2);
        path.lineTo(r.topRight());
        break;
    default:
        break;
    }
    painter->drawPath(path);
    painter->restore();
}

void MaterialStyle::prepareSmothPainter(QPainter *painter, bool fixHdpi) const
{
    painter->setRenderHint(QPainter::Antialiasing);
    if (fixHdpi) {
        painter->translate(0.5, 0.5);
    }
}

QPolygonF MaterialStyle::calcLines(const QStyleOptionSlider *dial) const
{
    QPolygonF poly;
    int width = dial->rect.width();
    int height = dial->rect.height();
    qreal r = qMin(width, height) / 2;
    int bigLineSize = r / 6;
    if (bigLineSize < 4)
        bigLineSize = 4;
    if (bigLineSize > r / 2)
        bigLineSize = r / 2;

    qreal xc = width / 2 + 0.5;
    qreal yc = height / 2 + 0.5;
    const int ns = dial->tickInterval;
    if (!ns) // Invalid values may be set by Qt Designer.
        return poly;
    int notches = (dial->maximum + ns - 1 - dial->minimum) / ns;
    if (notches <= 0)
        return poly;
    if (dial->maximum < dial->minimum || dial->maximum - dial->minimum > 1000) {
        int maximum = dial->minimum + 1000;
        notches = (maximum + ns - 1 - dial->minimum) / ns;
    }

    poly.resize(2 + 2 * notches);
    int smallLineSize = bigLineSize / 2;
    for (int i = 0; i <= notches; ++i) {
        qreal angle = dial->dialWrapping ? Q_PI * 3 / 2 - i * 2 * Q_PI / notches
                  : (Q_PI * 8 - i * 10 * Q_PI / notches) / 6;
        qreal s = qSin(angle);
        qreal c = qCos(angle);
        if (i == 0 || (((ns * i) % (dial->pageStep ? dial->pageStep : 1)) == 0)) {
            poly[2 * i] = QPointF(xc + (r - bigLineSize) * c,
                                  yc - (r - bigLineSize) * s);
            poly[2 * i + 1] = QPointF(xc + r * c, yc - r * s);
        } else {
            poly[2 * i] = QPointF(xc + (r - 1 - smallLineSize) * c,
                                  yc - (r - 1 - smallLineSize) * s);
            poly[2 * i + 1] = QPointF(xc + (r - 1) * c, yc -(r - 1) * s);
        }
    }
    return poly;
}

void MaterialStyle::drawPushButton(const QStyleOptionButton *btn, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(widget)
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(0.5, 0.5);
    //Draw the background
    bool drawGlowLine = true;
    painter->setPen(QPen(Qt::black, 1));
    int focusSize = pixelMetric(PM_FocusFrameHMargin, btn, widget);
    QRect btnRect = btn->rect/*.adjusted(focusSize, focusSize, -focusSize, -focusSize)*/;
    QColor textColor;
    QColor textShadowColor;
    QLinearGradient grad(0, 0, 0, btnRect.height());
    //    painter->fillRect(btn->rect, Qt::red);
    bool drawTextShadow = false;
//    QPixmap borderPxm;

    if (btn->state & State_Enabled) {
        if (btn->state & (State_Sunken | State_On)) {
            grad.setColorAt(0.0, btn->palette.color(QPalette::Dark));
            grad.setColorAt(1.0, btn->palette.color(QPalette::Light));
            textColor = COLOR_BUTTON_TEXT;
            textShadowColor = btn->palette.color(QPalette::Shadow);
            textShadowColor.setAlpha(25 * 255 / 100);
//            borderPxm = QPixmap(":/images/images/push_button_normal.png");
            drawTextShadow = true;
//            borderColor = COLOR_PUSHBUTTON_BORDER_NORMAL;
        }
        else if (btn->state & (State_MouseOver)) {
            grad.setColorAt(0.0, btn->palette.midlight().color());
            grad.setColorAt(1.0, btn->palette.mid().color());
            drawGlowLine = false;
            textColor = btn->palette.color(QPalette::BrightText);
            textShadowColor = COLOR_BUTTON_TEXT;
            textShadowColor.setAlpha(40 * 255 / 100);
//            borderPxm = QPixmap(":/images/images/push_button_hover.png");
            drawTextShadow = true;
//            borderColor = COLOR_PUSHBUTTON_BORDER_NORMAL;
        }
        else {
            grad.setColorAt(0.0, btn->palette.color(QPalette::Light));
            grad.setColorAt(1.0, btn->palette.color(QPalette::Dark));
            textColor = COLOR_BUTTON_TEXT;
            textShadowColor = btn->palette.color(QPalette::Shadow);
            textShadowColor.setAlpha(25 * 255 / 100);
//            borderPxm = QPixmap(":/images/images/push_button_normal.png");
            drawTextShadow = true;
//            borderColor = COLOR_PUSHBUTTON_BORDER_NORMAL;
        }
        painter->setBrush(grad);
        painter->setPen(COLOR_PUSHBUTTON_BORDER_NORMAL);
        painter->drawPath(roundRectPath(btnRect.adjusted(0, 0, -1, -1)));
        if (drawGlowLine) {
            painter->save();
            painter->setClipRect(btnRect.adjusted(0, 0, -2, -(btnRect.height() - 3)));
//            painter->fillRect(btnRect, QColor(255, 0, 0, 100));
            painter->setBrush(Qt::NoBrush);
            painter->setPen(COLOR_PUSHBUTTON_GLOW);
            painter->drawRoundedRect(btnRect.adjusted(1, 1, -3, -1), 2, 2);
            painter->restore();
        }        
        if (btn->state & State_HasFocus && btn->state & State_KeyboardFocusChange) {
            painter->setPen(QPen(COLOR_BUTTON_FOCUS_FG, 3));
            painter->drawPath(roundRectPath(btnRect/*.adjusted(1, 1, 0, 0)*/));
        }
    }
    else {
//        borderPxm = QPixmap(":/images/images/push_button_disabled.png");
        textColor = btn->palette.color(QPalette::Disabled, QPalette::ButtonText);
        painter->setPen(COLOR_PUSHBUTTON_BORDER_DISABLED);
        painter->setBrush(btn->palette.window());
        painter->drawRoundedRect(btnRect.adjusted(0, 0, -1, -1), 2, 2);
    }

//    qDrawBorderPixmap(painter, btnRect, QMargins(4, 5, 4, 4), borderPxm);



    //Draw the text and icon if any
    int pxW = 0;
    int pxH = 0;
    QPixmap pxm;
    if (!btn->icon.isNull()) {
        QIcon icon(btn->icon);
        QIcon::Mode mode = btn->state & State_Enabled ? QIcon::Normal
                                                      : QIcon::Disabled;
        if (mode == QIcon::Normal && btn->state & State_HasFocus)
            mode = QIcon::Active;
        QIcon::State state = QIcon::Off;
        if (btn->state & State_On)
            state = QIcon::On;
        pxm = icon.pixmap(btn->iconSize, mode, state);
        pxW = pxm.width();
        pxH = pxm.height();
    }
    //    QFont font("Myriad", 10);
    QFontMetrics fm(regularFont);
    painter->setFont(regularFont);
    QRect textRect = fm.boundingRect(btn->text);
    int wReq = pxW + BUTTON_ICON_TEXT_SPACING + textRect.width() + 4;
    int iconX = 0;
    int iconY = qAbs(btn->rect.height() - pxH) / 2;
    int textX = 0;
    if (btn->rect.width() > wReq) { //place in center
        iconX = (btn->rect.width() - wReq) / 2;
    }
    else {
        int margin = pixelMetric(PM_ButtonMargin, btn, widget);
        iconX = margin;
        textRect.setWidth(btn->rect.width() - (margin * 2) - pxW - 6);
    }
    textX = iconX + pxW + BUTTON_ICON_TEXT_SPACING;
    painter->drawPixmap(iconX, iconY, pxm);
    textRect.moveTo(textX, (btn->rect.height() - textRect.height()) / 2);
    textRect.setWidth(btn->rect.width() - 4);
    if (drawTextShadow) {
        painter->setPen(textShadowColor);
        painter->drawText(textRect.adjusted(1, 1, 2, 2), btn->text);
    }
    painter->setPen(textColor);
    painter->drawText(textRect.adjusted(0, 0, 1, 1), btn->text);

    painter->restore();
}

void MaterialStyle::drawGroupBox(const QStyleOptionGroupBox *groupBox, QPainter *painter, const QWidget *widget) const
{

    //    QFontMetrics fm(groupBoxFont);
    //    QRect textRect = fm.boundingRect(groupBox->text);
    //Draw the frame
    if (groupBox->subControls & QStyle::SC_GroupBoxFrame) {
        drawPrimitive(PE_FrameGroupBox, groupBox, painter, widget);
    }
    //Draw the indicator
    if (groupBox->subControls & QStyle::SC_GroupBoxCheckBox) {
        QRect indRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxCheckBox, widget);
        QStyleOptionButton chk;
        chk.QStyleOption::operator=(*groupBox);
        chk.rect = indRect;
        drawPrimitive(PE_IndicatorCheckBox, &chk, painter, widget);
    }
    //Draw the text
    if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
        QRect textRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxLabel, widget);
        painter->setFont(groupBoxFont);
        painter->setPen(COLOR_GROUPBOX_TEXT);
        painter->drawText(textRect.adjusted(0, 1, 0, 0), groupBox->text);
    }

//    //Draw the text
//    if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
//        //        QFont font("Myriad Pro", 12);
//        painter->setPen(COLOR_GROUPBOX_TEXT);
//        int spacing = pixelMetric(PM_CheckBoxLabelSpacing, groupBox, widget);
//        int xOffset = groupBox->subControls & QStyle::SC_GroupBoxCheckBox ?
//                    spacing + pixelMetric(PM_IndicatorWidth) : 0;
//        xOffset += GROUP_BOX_TEXT_H_MARGIN;
//        QFontMetrics fm(groupBoxFont);
//        QRect textRect = fm.boundingRect(groupBox->text);
//        textRect.moveTo(0, 0);
//        int yOffset = GROUP_BOX_TEXT_V_MARGIN /*+ textRect.height()*/;
//        QRect okRect = textRect.adjusted(xOffset, yOffset, xOffset, yOffset);
//        painter->setFont(groupBoxFont);
//        painter->drawText(okRect, groupBox->text);

//        //Draw the decoration

//        int underlineY = 2 * GROUP_BOX_TEXT_V_MARGIN + textRect.height();
//        painter->setPen(QPen(COLOR_FRAME_BORDER, GROUP_BOX_UNDERLINE_WIDTH));
//        //-2 fills the gap in Windows
//        painter->drawLine(-2, underlineY, okRect.x(), underlineY);

//        painter->setPen(QPen(COLOR_GROUPBOX_TEXT, GROUP_BOX_UNDERLINE_WIDTH));
//        painter->drawLine(okRect.x(), underlineY, okRect.width() + okRect.x(), underlineY);

//        painter->setPen(QPen(COLOR_FRAME_BORDER, GROUP_BOX_UNDERLINE_WIDTH));
//        painter->drawLine(okRect.width() + okRect.x(), underlineY, groupBox->rect.width(), underlineY);
//    }
//    else if (groupBox->subControls & QStyle::SC_GroupBoxCheckBox) {
//        painter->setPen(QPen(COLOR_FRAME_BORDER, 2));
//        int decY = indRect.height() + GROUP_BOX_UNDERLINE_WIDTH;
//        painter->drawLine(0, decY, groupBox->rect.width(), decY);
//    }

}

void MaterialStyle::drawSlider(const QStyleOptionSlider *slider, QPainter *painter, const QWidget *widget) const
{
    //    painter->fillRect(slider->rect, Qt::lightGray);
    QRect groove = subControlRect(CC_Slider, slider, SC_SliderGroove, widget);
    QRect handle = subControlRect(CC_Slider, slider, SC_SliderHandle, widget);

    if (slider->state & State_HasFocus && slider->state & State_KeyboardFocusChange) {
        drawPrimitive(PE_FrameFocusRect, slider, painter, widget);
    }
    painter->save();
    prepareSmothPainter(painter);
    bool horizontal = slider->orientation == Qt::Horizontal;
//    bool showTicks = !(slider->tickPosition & QSlider::NoTicks);

//    qDebug() << slider->rect << groove << handle;
    if (slider->subControls & QStyle::SC_SliderGroove) {
//        QPixmap px(":/images/images/slider_frame.png");
//        qDrawBorderPixmap(painter, groove, QMargins(2, 2, 2, 2), px);
        painter->setPen(COLOR_FRAME_BORDER);
        painter->setBrush(COLOR_SLIDER_GROOVE_BG);
        painter->drawRoundedRect(groove.adjusted(0, 0,horizontal ? -1 : 0, horizontal ? 0 : -1), 2, 2);
    }
    QRect clipRect = groove;
    QLinearGradient gradient;
    if (horizontal) {
        gradient.setStart(0, 0);
        gradient.setFinalStop(groove.right(), 0);
        if (slider->upsideDown) {
            clipRect.setX(handle.x() + handle.width() / 2);
        }
        else {
            clipRect.setWidth(handle.x() + handle.width() / 2);
        }
    }
    else {
        gradient.setStart(0, groove.bottom());
        gradient.setFinalStop(0, 0);
        if (slider->upsideDown) {
            clipRect.setY(handle.y() + handle.height() / 2);
        }
        else {
            clipRect.setHeight(handle.y() + handle.height() / 2);
        }
    }
    gradient.setColorAt(0, COLOR_SLIDER_GROOVE_L);
    gradient.setColorAt(1, COLOR_SLIDER_GROOVE_D);

    painter->save();
    painter->setBrush(gradient);
    painter->setPen(Qt::NoPen);
    painter->drawRoundRect(clipRect.adjusted(2, 2, horizontal ? -3 : -2, horizontal ? -2 : -3), 2, 2);
    //    painter->fillRect(clipRect, gradient)
    //    painter->drawRect(clipRect.adjusted(1, 1, -1, -1));
    painter->restore();

    //    //draw the ticks
    //    if (showTicks) {
    //        int tickInterval = slider->tickInterval;

    //    }

    if (slider->subControls & QStyle::SC_SliderHandle) {
//        QString pxStr = slider->state & State_Sunken ? ":/images/images/slider_button_sunken.png":
//                                                       ":/images/images/slider_button_hover.png";
//        if (!(slider->state & State_MouseOver)) {
//            pxStr = ":/images/images/slider_frame.png";
//        }
//        QPixmap px(pxStr);
//        qDrawBorderPixmap(painter, handle, QMargins(2, 2, 2, 2), px);
        QColor borderColor;
        QColor bgColor;
        bool drawInnerBorder = false;
        if (slider->state & State_Sunken) {
            borderColor = COLOR_SLIDER_HANDLE_SUNKEN_BORDER;
            bgColor = COLOR_SLIDER_HANDLE_SUNKEN_BG;
            drawInnerBorder = true;
        }
        else if (slider->state & State_MouseOver) {
            borderColor = COLOR_SLIDER_HANDLE_NORMAL_BORDER;
            bgColor = COLOR_SLIDER_HANDLE_HOVER_BG;
            drawInnerBorder = true;
        }
        else {
            borderColor = COLOR_SLIDER_HANDLE_NORMAL_BORDER;
            bgColor = COLOR_SLIDER_HANDLE_NORMAL_BG;
        }
        painter->setPen(borderColor);
        painter->setBrush(bgColor);
        handle.adjust(0, 0, horizontal ? -1 : 0, horizontal ? 0 : -1);
        painter->drawRoundedRect(handle, 2, 2);
        if (drawInnerBorder) {
            painter->setPen(COLOR_SLIDER_HANDLE_INNER_BORDER);
            painter->setBrush(Qt::NoBrush);
            painter->drawRoundedRect(handle.adjusted(1, 1, -1, -1), 2, 2);
        }
    }
    painter->restore();
}

void MaterialStyle::tabLayout(const QStyleOptionTabV3 *opt, const QWidget *widget, QRect *textRect, QRect *iconRect) const
{
    Q_ASSERT(textRect);
    Q_ASSERT(iconRect);
    QRect tr = opt->rect;
    bool verticalTabs = opt->shape == QTabBar::RoundedEast
            || opt->shape == QTabBar::RoundedWest
            || opt->shape == QTabBar::TriangularEast
            || opt->shape == QTabBar::TriangularWest;
    if (verticalTabs)
        tr.setRect(0, 0, tr.height(), tr.width()); //0, 0 as we will have a translate transform

    int verticalShift = pixelMetric(QStyle::PM_TabBarTabShiftVertical, opt, widget);
    int horizontalShift = pixelMetric(QStyle::PM_TabBarTabShiftHorizontal, opt, widget);
    int hpadding = pixelMetric(QStyle::PM_TabBarTabHSpace, opt, widget) / 2;
    int vpadding = pixelMetric(QStyle::PM_TabBarTabVSpace, opt, widget) / 2;
    if (opt->shape == QTabBar::RoundedSouth || opt->shape == QTabBar::TriangularSouth)
        verticalShift = -verticalShift;
    tr.adjust(hpadding, verticalShift - vpadding, horizontalShift - hpadding, vpadding);
    bool selected = opt->state & QStyle::State_Selected;
    if (selected) {
        tr.setTop(tr.top() - verticalShift);
        tr.setRight(tr.right() - horizontalShift);
    }

    // left widget
    if (!opt->leftButtonSize.isEmpty()) {
        tr.setLeft(tr.left() + 4 +
                   (verticalTabs ? opt->leftButtonSize.height() : opt->leftButtonSize.width()));
    }
    // right widget
    if (!opt->rightButtonSize.isEmpty()) {
        tr.setRight(tr.right() - 4 -
                    (verticalTabs ? opt->rightButtonSize.height() : opt->rightButtonSize.width()));
    }

    // icon
    if (!opt->icon.isNull()) {
        QSize iconSize = opt->iconSize;
        if (!iconSize.isValid()) {
            int iconExtent = pixelMetric(QStyle::PM_SmallIconSize);
            iconSize = QSize(iconExtent, iconExtent);
        }
        QSize tabIconSize = opt->icon.actualSize(iconSize,
                                                 (opt->state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled,
                                                 (opt->state & QStyle::State_Selected) ? QIcon::On : QIcon::Off  );
        // High-dpi icons do not need adjustmet; make sure tabIconSize is not larger than iconSize
        tabIconSize = QSize(qMin(tabIconSize.width(), iconSize.width()), qMin(tabIconSize.height(), iconSize.width()));

        *iconRect = QRect(tr.left(), tr.center().y() - tabIconSize.height() / 2,
                          tabIconSize.width(), tabIconSize .height());
        if (!verticalTabs)
            *iconRect = visualRect(opt->direction, opt->rect, *iconRect);
        tr.setLeft(tr.left() + tabIconSize.width() + 4);
    }

    if (!verticalTabs)
        tr = visualRect(opt->direction, opt->rect, tr);

    *textRect = tr;
}

void MaterialStyle::loadCustomFonts()
{
    QDirIterator it(":/fonts/Fonts/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFontDatabase::addApplicationFont(it.next());
    }
}

int MaterialStyle::calcBigLineSize(int radius) const
{
    int bigLineSize = radius / 6;
    if (bigLineSize < 4)
        bigLineSize = 4;
    if (bigLineSize > radius / 2)
        bigLineSize = radius / 2;
    return bigLineSize;
}

QPointF MaterialStyle::calcRadialPos(const QStyleOptionSlider *dial, qreal offset) const
{
    const int width = dial->rect.width();
    const int height = dial->rect.height();
    const int r = qMin(width, height) / 2;
    const int currentSliderPosition = dial->upsideDown ? dial->sliderPosition : (dial->maximum - dial->sliderPosition);
    qreal a = 0;
    if (dial->maximum == dial->minimum)
        a = Q_PI / 2;
    else if (dial->dialWrapping)
        a = Q_PI * 3 / 2 - (currentSliderPosition - dial->minimum) * 2 * Q_PI
            / (dial->maximum - dial->minimum);
    else
        a = (Q_PI * 8 - (currentSliderPosition - dial->minimum) * 10 * Q_PI
            / (dial->maximum - dial->minimum)) / 6;
    qreal xc = width / 2.0;
    qreal yc = height / 2.0;
    qreal len = r - calcBigLineSize(r) - 3;
    qreal back = offset * len;
    QPointF pos(QPointF(xc + back * qCos(a), yc - back * qSin(a)));
    return pos;
}

void MaterialStyle::drawMdiButton(QStyle::SubControl sc, QPainter *painter, const QColor &fg, const QColor &bgColor, const QRect &rect, bool hover, bool sunken) const
{
    painter->save();
    prepareSmothPainter(painter);
    painter->setPen(fg);
    if (hover) {
        painter->setBrush(bgColor);
        if (sunken) {
            painter->setBrush(bgColor.darker(130));
        }
        painter->drawRoundedRect(rect.adjusted(0, 0, -1, -1), 2, 2);
    }

    QRect cRect = rect.adjusted(4, 4, -4, -4);
    painter->translate(-0.5, -0.5);
    painter->fillRect(cRect, QColor(255, 0, 0, 100));
    switch (sc) {
    case SC_TitleBarMinButton:
        painter->setPen(QPen(fg, 2));
        painter->drawLine(cRect.left() + 1, cRect.bottom(), cRect.right(), cRect.bottom());
        break;
    case SC_TitleBarMaxButton:
        painter->setPen(QPen(fg, 2));
        painter->drawRect(cRect.adjusted(1, 1, -1, -1));
        break;
    case SC_TitleBarCloseButton:
        painter->setPen(QPen(fg, 2));
        painter->drawLine(cRect.left() + 1, cRect.top() + 1, cRect.right(), cRect.bottom());
        painter->drawLine(cRect.right(), cRect.top() + 1, cRect.left() + 1, cRect.bottom());
        break;
    case SC_TitleBarNormalButton: {
        painter->setPen(QPen(fg, 2));
        int rw = cRect.width();
        int rh = cRect.height();
        int rx = cRect.left();
        int ry = cRect.top();
        QRect topRect(rx + rw / 2 - 1, ry + 1, rw / 2, rh / 2);
        QRect bottomRect(rx + 1, ry + rh / 2 - 1, rw / 2, rh / 2);
        painter->drawRect(topRect);
        painter->drawRect(bottomRect);
    }
        break;
    case SC_TitleBarContextHelpButton:
        explosion;
        break;
    default:
        break;
    }
    painter->restore();
}

QPixmap MaterialStyle::testPxFactory(const QString &pxName) const
{
//    return "";
}

static const char * const qt_titlebar_context_help[] = {
    "10 10 3 1",
    "  c None",
    "# c #000000",
    "+ c #444444",
    "  +####+  ",
    " ###  ### ",
    " ##    ## ",
    "     +##+ ",
    "    +##   ",
    "    ##    ",
    "    ##    ",
    "          ",
    "    ##    ",
    "    ##    "};
