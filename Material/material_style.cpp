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
#include "progress_style_animation.h"
#include "style_animation.h"

//#include <QStyleOptionTabV3>

MaterialStyle::MaterialStyle()
{
    setObjectName(QLatin1String("Fusion"));

    loadCustomFonts();
    groupBoxFont = QFont("Myriad Pro", 10);
    groupBoxFont.setBold(true);
    //    groupBoxFont.setStyleStrategy(QFont::NoAntialias);

    regularFont = QFont("Myriad Pro", 9);
    //    regularFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.7);
    //    regularFont.setWeight(5);
    //    regularFont.setPixelSize(2);
    //    regularFont.setStyleStrategy(QFont::NoAntialias);
}

void MaterialStyle::drawPrimitive(QStyle::PrimitiveElement elem, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (elem) {
    case PE_FrameGroupBox: {
        painter->setRenderHint(QPainter::Antialiasing);
        QFontMetrics fm(groupBoxFont);
        const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(option);
        QRect textRect = fm.boundingRect(groupBox->text);
        textRect.moveTo(0, 0);
        QPixmap bPix(":/images/images/group_box_bottom.png");
        int hExtra = 0;
        int indW = 0;
        QPixmap tPix;
        int glue = 0;
        if (groupBox->subControls & SC_GroupBoxLabel) {
            hExtra = 2 * GROUP_BOX_TEXT_V_MARGIN;
            tPix = QPixmap(":/images/images/group_box_top.png");
        }
        else if (groupBox->subControls & SC_GroupBoxCheckBox) {
            hExtra = 2 * GROUP_BOX_TEXT_V_MARGIN;
            tPix = QPixmap(":/images/images/group_box_top.png");
            indW = pixelMetric(PM_IndicatorWidth, option, widget);
        }
        else {
            hExtra = 0;
            QTransform tranf;
            tranf.rotate(180);
            tPix = bPix.transformed(tranf);
            glue = 4;
        }
        int yDiv = hExtra + qMax(textRect.height(), indW) + glue;
        QRect topFrame(0, 0, groupBox->rect.width(), yDiv);
        QRect bottomFrame(0, yDiv, groupBox->rect.width(), groupBox->rect.height() - yDiv - 1);
        if (groupBox->subControls & SC_GroupBoxLabel || groupBox->subControls & SC_GroupBoxCheckBox) {
            qDrawBorderPixmap(painter, topFrame, QMargins(4, 4, 4, 4), tPix);
            qDrawBorderPixmap(painter, bottomFrame, QMargins(4, 4, 4, 4), bPix);
        }
        else {
            QPixmap px(":/images/images/frame.png");
            qDrawBorderPixmap(painter, groupBox->rect, QMargins(4, 4, 4, 4), px);
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
        ////        QPen dashedPen(QColor());
        //        painter->setOpacity(0.2);
        ////        painter->fillRect(option->rect, Qt::blue);
        //        painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, -1), 3, 3);
        QPixmap px(":/images/images/frame.png");
        qDrawBorderPixmap(painter, option->rect, QMargins(4, 4, 4, 4), px);
        break;
    }
    case PE_Frame:{
        painter->setPen(COLOR_FRAME_BORDER);
        painter->setBrush(COLOR_PALETTE_WINDOW);
        painter->drawRect(option->rect.adjusted(0, 0, -1, -1));
    }
        break;
    case PE_FrameMenu:
        if (const QStyleOptionFrame *frame = qstyleoption_cast<const QStyleOptionFrame *>(option)) {
            //            if (elem == PE_FrameMenu || (frame->state & State_Sunken) || (frame->state & State_Raised)) {
            //                QPixmap px(":/images/images/frame.png");
            //                qDrawBorderPixmap(painter, frame->rect, QMargins(4, 4, 4, 4), px);
            //            } else {
            //                qDrawPlainRect(painter, frame->rect, COLOR_FRAME_BORDER, frame->lineWidth);
            //            }
            painter->setPen(COLOR_FRAME_BORDER);
            //TODO only for testing
            painter->setBrush(Qt::red);
            painter->drawRect(frame->rect.adjusted(0, 0, -1, -1));
        }
        break;
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
    case PE_FrameTabWidget: {
        //        painter->fillRect(option->rect, QColor(255, 0 ,0, 100));
        QPixmap px(":/images/images/tabwidget_frame.png");
        if (const QStyleOptionTabWidgetFrame *tWidget = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option)) {
            QTransform matrix;
            if (tWidget->shape == QTabBar::RoundedEast) {
                matrix.rotate(+90);
            } else if (tWidget->shape == QTabBar::RoundedSouth) {
                matrix.rotate(+180);
            } else if (tWidget->shape == QTabBar::RoundedWest) {
                matrix.rotate(+270);
            }
            QPixmap pxTransf = px.transformed(matrix);
            qDrawBorderPixmap(painter, option->rect, QMargins(4, 4, 4, 4), pxTransf);
        }

        //        painter->save();
        //        painter->fillRect(option->rect.adjusted(0, 0, -1, -1), tabFrameColor);
        //        if (const QStyleOptionTabWidgetFrame *twf = qstyleoption_cast<const QStyleOptionTabWidgetFrame *>(option)) {
        //            QColor borderColor = outline.lighter(110);
        //            QRect rect = option->rect.adjusted(0, 0, -1, -1);

        //            // Shadow outline
        //            if (twf->shape != QTabBar::RoundedSouth) {
        //                rect.adjust(0, 0, 0, -1);
        //                QColor alphaShadow(Qt::black);
        //                alphaShadow.setAlpha(15);
        //                painter->setPen(alphaShadow);
        //                painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());            painter->setPen(borderColor);
        //            }

        //            // outline
        //            painter->setPen(outline);
        //            painter->drawRect(rect);

        //            // Inner frame highlight
        //            painter->setPen(d->innerContrastLine());
        //            painter->drawRect(rect.adjusted(1, 1, -1, -1));

        //        }
        //        painter->restore();
    }
        break ;
    case PE_IndicatorTabClose: {
//        QStyleHintReturn *hr = 0;
        if ((option->state & State_Enabled) && (option->state & State_MouseOver))
            proxy()->drawPrimitive(PE_PanelButtonCommand, option, painter, widget);
        QPixmap px(":/images/images/tab_close.png");
        QCommonStyle::drawItemPixmap(painter, option->rect, Qt::AlignCenter, px);
//        if (option->state & State_MouseOver) {
//            painter->setPen(QPen(COLOR_TAB_NORMAL_UL));
//            painter->drawRect(option->rect.adjusted(0, 0, -1, -1));
//        }
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
        //        QRect r = option->rect;
        //        bool hasFocus = option->state & State_HasFocus;

        //        painter->save();
        //        QString pxStr = hasFocus ? ":/images/images/edit_frame_active.png" :
        //                                   ":/images/images/edit_frame.png";
        //        QPixmap pxm(pxStr);
        //        qDrawBorderPixmap(painter, r, QMargins(4, 4, 4, 4), pxm);
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
                    painter->fillRect(rect.center().x() + i, rect.center().y() + j, 2, 2, COLOR_TOOLBAR_BG.lighter(80));
                    painter->fillRect(rect.center().x() + i, rect.center().y() + j, 1, 1, COLOR_TOOLBAR_BG.darker(80));
                }
            }
        } else { //vertical toolbar
            for (int i = -6 ; i < 12 ; i += 3) {
                for (int j = -3 ; j < 2 ; j += 3) {
                    painter->fillRect(rect.center().x() + i, rect.center().y() + j, 2, 2, COLOR_TOOLBAR_BG.lighter(80));
                    painter->fillRect(rect.center().x() + i, rect.center().y() + j, 1, 1, COLOR_TOOLBAR_BG.darker(80));
                }
            }
        }
        break;
    }
    case PE_PanelButtonTool:
        painter->save();
        if ((option->state & State_Enabled || option->state & State_On) || !(option->state & State_AutoRaise)) {
            if (widget && widget->inherits("QDockWidgetTitleButton")) {
                if (option->state & State_MouseOver)
                    proxy()->drawPrimitive(PE_PanelButtonCommand, option, painter, widget);
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

            painter->setPen(Qt::red);

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

            painter->translate(0.5, 0.5);
            QString px1Str;
            QString px2Str;
            QString statusStr;
            if (selected) {
                statusStr = "sunken";
            } else if (hover) {
                statusStr = "hover";
            } else {
                statusStr = "normal";
            }

            if (onlyOne) {
                px1Str = QString(":/images/images/tab_%1_rounded.png").arg(statusStr);
                px2Str = QString(":/images/images/tab_%1_rounded.png").arg(statusStr);
            }
            else {
                if (firstTab) {
                    px1Str = QString(":/images/images/tab_%1_rounded.png").arg(statusStr);
                    px2Str = QString(":/images/images/tab_%1_squared.png").arg(statusStr);
                } else if (lastTab) {
                    px1Str = QString(":/images/images/tab_%1_squared.png").arg(statusStr);
                    px2Str = QString(":/images/images/tab_%1_rounded.png").arg(statusStr);
                } else {
                    px1Str = QString(":/images/images/tab_%1_squared.png").arg(statusStr);
                }
            }

            painter->translate(0.5, 0.5);//para corregir problema de East por bug de QPainter + hidpi
            QPixmap px1(px1Str);
            int test = 0;
            //            painter->setRenderHint(QPainter::Antialiasing);
            if (px2Str.isEmpty()) { //una intermedia
                qDrawBorderPixmap(painter, rect.adjusted(-1, test, -1, 0), QMargins(4, 4, 4, 4), px1);
                //                painter->drawPixmap(0, 0, px1);
            }
            else {
                QPixmap px2(px2Str);
                //                QRect lRect = rect.adjusted(0, 0, -rect.width() / 2, 0);
                int cosmeticOffset = 1;
                qDrawBorderPixmap(painter, rect.adjusted(-cosmeticOffset, test, -rect.width() / 2, 0), QMargins(4, 4, 4, 4), px1);
                if (!px2Str.contains("squared", Qt::CaseInsensitive)) { //la ultima
                    px2 = QPixmap::fromImage(QImage(px2Str).mirrored(true, false));
                    qDrawBorderPixmap(painter, rect.adjusted(rect.width() / 2, test, -1, 0),
                                      QMargins(4, 4, 4, 4), px2);
                }
                else {
                    qDrawBorderPixmap(painter, rect.adjusted(rect.width() / 2, test, -1, 0),
                                      QMargins(4, 4, 4, 4), px2);
                }
            }

            //            //###########temporal workaround
            //            if (tab->shape == QTabBar::RoundedEast) {
            //                qDebug() << "siiiiiiiiiiiii";
            //                painter->setPen(QPen(QColor(255, 0, 0, 50), 5));
            //                QPoint p1 = rect.topLeft();
            //                QPoint p2 = rect.topRight();
            //                p1.rx() += 4;
            ////                p1.ry() += 1;
            //                p2.rx() -= 4;
            ////                p2.ry() += 1;
            ////                painter->drawLine(p1, p2);
            //            }

            //draw the underline
            QColor uLineColor = selected ? COLOR_TAB_SELECTED_UL : COLOR_TAB_NORMAL_UL;
            painter->setPen(QPen(uLineColor, 2));
            QPoint p1 = rect.bottomLeft();
            p1.ry() -= 1;
            QPoint p2 = rect.bottomRight();
            p2.ry() -= 1;
            p2.rx() -= 1;
            painter->drawLine(p1, p2);

            //draw the separation
            if (!(lastTab || onlyOne)) {
                int sepMargin = 1;
                painter->setPen(QPen(COLOR_TAB_SEPARATOR, 1));
                painter->drawLine(rect.right() - 1, sepMargin,
                                  rect.right() - 1, rect.height() - sepMargin);
            }

            //            QRect drawRect = rect.adjusted(0, selected ? 0 : 2, 0, 3);
            //            painter->setPen(outlinePen);
            //            painter->save();
            //            painter->setClipRect(rect.adjusted(-1, -1, 1, selected ? -2 : -3));
            //            painter->setBrush(fillGradient);
            //            painter->drawRoundedRect(drawRect.adjusted(0, 0, -1, -1), 2.0, 2.0);
            //            painter->setBrush(Qt::NoBrush);
            //            painter->setPen(Qt::red);
            //            painter->drawRoundedRect(drawRect.adjusted(1, 1, -2, -1), 2.0, 2.0);
            //            painter->restore();

            //            if (selected) {
            //                painter->fillRect(rect.left() + 1, rect.bottom() - 1, rect.width() - 2, rect.bottom() - 1, tabFrameColor);
            //                painter->fillRect(QRect(rect.bottomRight() + QPoint(-2, -1), QSize(1, 1)), Qt::red);
            //                painter->fillRect(QRect(rect.bottomLeft() + QPoint(0, -1), QSize(1, 1)), Qt::red);
            //                painter->fillRect(QRect(rect.bottomRight() + QPoint(-1, -1), QSize(1, 1)), Qt::red);
            //            }
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


            //            }
            //Draw the Button
            //            QString btnPxmStr;
            //            if (sunken) {
            //                btnPxmStr = ":/images/images/combobox_button_sunken.png";
            //            } else if (hover) {
            //                btnPxmStr = ":/images/images/combobox_button_hover.png";
            //            } else {
            //                btnPxmStr = ":/images/images/combobox_button.png";
            //            }
            //            QPixmap btnPxm(btnPxmStr);
            //            qDrawBorderPixmap(painter, btnRect, QMargins(4, 4, 4, 4), btnPxm);

            QColor btnBgColor;
            if (sunken) {
                btnBgColor = COLOR_COMBOBOX_SUNKEN;
            } else if (hover) {
                btnBgColor = COLOR_COMBOBOX_HOVER;
            } else {
                btnBgColor = COLOR_PALETTE_WINDOW;
            }

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->translate(0.5, 0.5); //due to hdpi bug
            painter->setPen(!(sunken | hover) ? btnBgColor : COLOR_PALETTE_WINDOW);
            painter->setBrush(btnBgColor);
            painter->setClipRect(btnRect.adjusted(0, 0, 2, 0)); //a little wider
            painter->drawRoundedRect(btnRect.adjusted(-4, 1, 0, -2), 4, 4);

            painter->setPen(COLOR_FRAME_BORDER);
            painter->drawLine(btnRect.left() + 1, 1, btnRect.left() + 1, btnRect.bottom() - 1);
            if (sunken | hover) {
                painter->setPen(COLOR_PALETTE_WINDOW);
                painter->drawLine(btnRect.left() + 2, 2, btnRect.left() + 2, btnRect.bottom() - 2);
            }
            painter->restore();

            //Draw the arrow indicator
//            QTransform matrix;
//            matrix.rotate(180);
//            QPixmap indPxm(":/images/images/arrow_ind.png");
//            QRect arrowRect(0, 0, 6, 6);
//            arrowRect.moveCenter(btnRect.center());

//            painter->drawPixmap(arrowRect, indPxm.transformed(matrix, Qt::SmoothTransformation));
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
            QColor btnBg = hover ? COLOR_COMBOBOX_HOVER : COLOR_PALETTE_WINDOW;
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

//            QPixmap indPxm(":/images/images/arrow_ind.png");
//            QRect arrowRect(0, 0, 6, 6);
//            arrowRect.moveCenter(upRect.center());
//            painter->drawPixmap(arrowRect.adjusted(0, -2, 0, -2), indPxm);

//            arrowRect.moveCenter(downRect.center());
//            QTransform matrix;
//            matrix.rotate(180);
//            painter->drawPixmap(arrowRect, indPxm.transformed(matrix));
            drawArrow(painter, Qt::UpArrow, COLOR_INDICATOR_ARROW, upRect.adjusted(0, -2, 0, 0));
            drawArrow(painter, Qt::DownArrow, COLOR_INDICATOR_ARROW, downRect);

        }
        break;
//    case CC_ToolButton:
//        painter->fillRect(option->rect, Qt::blue);
//        break;
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
            int indWidth = groupBox->subControls & SC_GroupBoxCheckBox ?
                        pixelMetric(PM_IndicatorWidth, opt, widget) : 0;
            //            QRect indicatorRect(GROUP_BOX_TEXT_H_MARGIN, (rect.height() - indWidth) / 2, indWidth, indWidth);

            QFontMetrics fm(groupBoxFont);
            QRect textRect = fm.boundingRect(groupBox->text);
            int stolenByText = 0;
            int stolenByIcon = 0;
            int textHeight = 0;
            int indHeight = 0;
            if (groupBox->subControls & SC_GroupBoxLabel) {
                QFontMetrics fm(groupBoxFont);
                QRect textRect = fm.boundingRect(groupBox->text);
                textHeight = textRect.height();
                stolenByText += 2 * GROUP_BOX_TEXT_V_MARGIN + textHeight + GROUP_BOX_UNDERLINE_WIDTH;
            }
            else if (groupBox->subControls & SC_GroupBoxCheckBox) {
                indHeight = indWidth;
                stolenByIcon += 2 * GROUP_BOX_TEXT_V_MARGIN + indHeight;
                //                    stolen += GROUP_BOX_UNDERLINE_WIDTH;
            }
            int headerHeight = qMax(stolenByText, stolenByIcon);

            if (sc == SC_GroupBoxLabel) {
                int space = pixelMetric(PM_CheckBoxLabelSpacing, opt, widget);
                textRect.moveTo(GROUP_BOX_TEXT_H_MARGIN + indWidth + space, GROUP_BOX_TEXT_V_MARGIN);
                return textRect;
            }

            else if (sc == SC_GroupBoxFrame) {
                return rect;
            }
            else if (sc == SC_GroupBoxContents) {
                //                stolen += 2; //top border
                QRect cRect = rect.adjusted(3, headerHeight, -3, -3);
                return cRect;
            }
            else if (sc == SC_GroupBoxCheckBox) {
                QRect iRect(GROUP_BOX_TEXT_H_MARGIN, (headerHeight - GROUP_BOX_UNDERLINE_WIDTH - indWidth) / 2,
                            indWidth, indWidth);
                return iRect;
            }
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
        return 1;
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
        return 4;
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
            || qobject_cast<QAbstractSpinBox *>(widget))
        widget->setAttribute(Qt::WA_Hover, true);
}

void MaterialStyle::polish(QPalette &pal)
{
    pal.setBrush(QPalette::Window, COLOR_PALETTE_WINDOW);       //window background
    pal.setBrush(QPalette::Light, QColor(64, 148, 208));         //pushbutton normal start gradient
    pal.setBrush(QPalette::Dark, QColor(7, 58, 147));            //pushbutton normal end gradient
    pal.setBrush(QPalette::Midlight, QColor(132, 191, 247));     //pushbutton hover start gradient
    pal.setBrush(QPalette::Mid, QColor(43, 84, 163));            //pushbutton hover end gradient
    pal.setBrush(QPalette::Button, COLOR_PALETTE_WINDOW);       //button normal color
    pal.setBrush(QPalette::ButtonText, COLOR_BUTTON_TEXT);   //pushbutton text color
    pal.setBrush(QPalette::Shadow, QColor(0, 0, 0));             //shadow
    pal.setBrush(QPalette::BrightText, QColor(7, 30, 82));       //pushbutton hover text color
    pal.setBrush(QPalette::Disabled, QPalette::ButtonText, QColor(171, 171, 171));
    pal.setBrush(QPalette::WindowText, COLOR_WINDOW_TEXT);
    pal.setBrush(QPalette::Highlight, COLOR_PALETTE_HIGHLIGHT);
    pal.setBrush(QPalette::Text, COLOR_PALETTE_TEXT);
    pal.setBrush(QPalette::HighlightedText, COLOR_PALETTE_HL_TEXT);
}

void MaterialStyle::unpolish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget)
            || qobject_cast<QProgressBar *>(widget)
            || qobject_cast<QScrollBar *>(widget)
            || qobject_cast<QSplitterHandle *>(widget)
            || qobject_cast<QAbstractSlider *>(widget)
            || qobject_cast<QAbstractSpinBox *>(widget))
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
        return 1;

    case SH_ToolBox_SelectedPageTitleBold:
    case SH_ScrollView_FrameOnlyAroundContents:
    case SH_Menu_AllowActiveAndDisabled:
    case SH_MainWindow_SpaceBelowMenuBar:
    case SH_DialogButtonBox_ButtonsHaveIcons:
    case SH_MessageBox_CenterButtons:
    case SH_RubberBand_Mask:
        return 0;
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
    QRect r(0, 0, 8 + (horizontal ? 0 : 1), 8 + (horizontal ? 1 : 0));
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
    QRect btnRect = btn->rect.adjusted(focusSize, focusSize, -focusSize, -focusSize);
    QColor textColor;
    QColor textShadowColor;
    QLinearGradient grad(0, 0, 0, btnRect.height());
    //    painter->fillRect(btn->rect, Qt::red);
    bool drawTextShadow = false;
//    QPixmap borderPxm;

    if (btn->state & State_Enabled) {
        if (btn->state & State_HasFocus && btn->state & State_KeyboardFocusChange) {
            drawPrimitive(PE_FrameFocusRect, btn, painter, widget);
        }
        if (btn->state & (State_Sunken | State_On)) {
            grad.setColorAt(0.0, btn->palette.color(QPalette::Dark));
            grad.setColorAt(1.0, btn->palette.color(QPalette::Light));
            textColor = btn->palette.color(QPalette::ButtonText);
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
            textShadowColor = btn->palette.color(QPalette::ButtonText);
            textShadowColor.setAlpha(40 * 255 / 100);
//            borderPxm = QPixmap(":/images/images/push_button_hover.png");
            drawTextShadow = true;
//            borderColor = COLOR_PUSHBUTTON_BORDER_NORMAL;
        }
        else {
            grad.setColorAt(0.0, btn->palette.color(QPalette::Light));
            grad.setColorAt(1.0, btn->palette.color(QPalette::Dark));
            textColor = btn->palette.color(QPalette::ButtonText);
            textShadowColor = btn->palette.color(QPalette::Shadow);
            textShadowColor.setAlpha(25 * 255 / 100);
//            borderPxm = QPixmap(":/images/images/push_button_normal.png");
            drawTextShadow = true;
//            borderColor = COLOR_PUSHBUTTON_BORDER_NORMAL;
        }
        painter->setBrush(grad);
        painter->setPen(COLOR_PUSHBUTTON_BORDER_NORMAL);
        painter->drawPath(roundRectPath(btnRect.adjusted(1, 1, 0, 0)));
        if (drawGlowLine) {
            painter->save();
            painter->setClipRect(btnRect.adjusted(1, 1, -2, -(btnRect.height() - 4)));
//            painter->fillRect(btnRect, QColor(255, 0, 0, 100));
            painter->setBrush(Qt::NoBrush);
            painter->setPen(COLOR_PUSHBUTTON_GLOW);
            painter->drawRoundedRect(btnRect.adjusted(2, 2, -2, -1), 2, 2);
            painter->restore();
        }
    }
    else {
//        borderPxm = QPixmap(":/images/images/push_button_disabled.png");
        textColor = btn->palette.color(QPalette::Disabled, QPalette::ButtonText);
        painter->setPen(COLOR_PUSHBUTTON_BORDER_DISABLED);
        painter->setBrush(btn->palette.window());
        painter->drawRoundedRect(btnRect, 2, 2);
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
    QRect indRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxCheckBox, widget);
    if (groupBox->subControls & QStyle::SC_GroupBoxCheckBox) {
        //        QRect indRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxCheckBox, widget);
        //        QRect textRec
        QStyleOptionButton chk;
        chk.QStyleOption::operator=(*groupBox);
        //        chk.rect = QRect(0, 0, groupBox->rect.width(), textRect.height() + 2 * GROUP_BOX_TEXT_V_MARGIN);
        chk.rect = indRect;
        //        painter->translate(4, 0);
        drawPrimitive(PE_IndicatorCheckBox, &chk, painter, widget);
    }

    //Draw the text
    if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
        //        QFont font("Myriad Pro", 12);
        painter->setPen(COLOR_GROUPBOX_TEXT);
        int spacing = pixelMetric(PM_CheckBoxLabelSpacing, groupBox, widget);
        int xOffset = groupBox->subControls & QStyle::SC_GroupBoxCheckBox ?
                    spacing + pixelMetric(PM_IndicatorWidth) : 0;
        xOffset += GROUP_BOX_TEXT_H_MARGIN;
        QFontMetrics fm(groupBoxFont);
        QRect textRect = fm.boundingRect(groupBox->text);
        textRect.moveTo(0, 0);
        int yOffset = GROUP_BOX_TEXT_V_MARGIN /*+ textRect.height()*/;
        QRect okRect = textRect.adjusted(xOffset, yOffset, xOffset, yOffset);
        painter->setFont(groupBoxFont);
        painter->drawText(okRect, groupBox->text);

        //Draw the decoration

        int underlineY = 2 * GROUP_BOX_TEXT_V_MARGIN + textRect.height();
        painter->setPen(QPen(COLOR_FRAME_BORDER, GROUP_BOX_UNDERLINE_WIDTH));
        //-2 fills the gap in Windows
        painter->drawLine(-2, underlineY, okRect.x(), underlineY);

        painter->setPen(QPen(COLOR_GROUPBOX_TEXT, GROUP_BOX_UNDERLINE_WIDTH));
        painter->drawLine(okRect.x(), underlineY, okRect.width() + okRect.x(), underlineY);

        painter->setPen(QPen(COLOR_FRAME_BORDER, GROUP_BOX_UNDERLINE_WIDTH));
        painter->drawLine(okRect.width() + okRect.x(), underlineY, groupBox->rect.width(), underlineY);
    }
    else if (groupBox->subControls & QStyle::SC_GroupBoxCheckBox) {
        painter->setPen(QPen(COLOR_FRAME_BORDER, 2));
        int decY = indRect.height() + GROUP_BOX_UNDERLINE_WIDTH;
        painter->drawLine(0, decY, groupBox->rect.width(), decY);
    }

}

void MaterialStyle::drawSlider(const QStyleOptionSlider *slider, QPainter *painter, const QWidget *widget) const
{
    //    painter->fillRect(slider->rect, Qt::lightGray);
    QRect groove = subControlRect(CC_Slider, slider, SC_SliderGroove, widget);
    QRect handle = subControlRect(CC_Slider, slider, SC_SliderHandle, widget);

    if (slider->state & State_HasFocus && slider->state & State_KeyboardFocusChange) {
        drawPrimitive(PE_FrameFocusRect, slider, painter, widget);
    }

    bool horizontal = slider->orientation == Qt::Horizontal;
    bool showTicks = !(slider->tickPosition & QSlider::NoTicks);

    if (slider->subControls & QStyle::SC_SliderGroove) {
        QPixmap px(":/images/images/slider_frame.png");
        qDrawBorderPixmap(painter, groove, QMargins(2, 2, 2, 2), px);
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
    painter->drawRoundRect(clipRect.adjusted(2, 2, -2, -2), 2, 2);
    //    painter->fillRect(clipRect, gradient)
    //    painter->drawRect(clipRect.adjusted(1, 1, -1, -1));
    painter->restore();

    //    //draw the ticks
    //    if (showTicks) {
    //        int tickInterval = slider->tickInterval;

    //    }

    if (slider->subControls & QStyle::SC_SliderHandle) {
        QString pxStr = slider->state & State_Sunken ? ":/images/images/slider_button_sunken.png":
                                                       ":/images/images/slider_button_hover.png";
        if (!(slider->state & State_MouseOver)) {
            pxStr = ":/images/images/slider_frame.png";
        }
        QPixmap px(pxStr);
        qDrawBorderPixmap(painter, handle, QMargins(2, 2, 2, 2), px);
    }
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

QPixmap MaterialStyle::testPxFactory() const
{
    const char * const px[] = {
        "11 13 7 1",
        "  c None",
        ". c #D5CFCB",
        "+ c #8F8B88",
        "@ c #6C6A67",
        "# c #ABA6A3",
        "$ c #B5B0AC",
        "% c #A4A09D",
        "           ",
        "@         @",
        "@         @",
        "@         @",
        "@         @",
        "@         @",
        "@         @",
        "@         @",
        "@         @",
        "@         @",
        "+%       #+",
        " +@@@@@@@+ ",
        "           "};

    const char * const px11[] = {
        "15 15 13 1",
        "  c None",
        "@ c #808080",
        "# c #858585",
        "$ c #868686",
        "% c #999999",
        "^ c #c0c0c0",
        "& c #c1c1c1",
        "* c #cccccc",
        "- c #cfcfcf",
        "+ c #d6d6d6",
        "/ c #dcdcdc",
        "? c #f7f7f7",
        "_ c #f8f8f8",
        " _/-*******-/_ ",
        "?-&%#@@@@@#%&-?",
        "/^$+_     _+$^/",
        "-%+         +%-",
        "*#_         _#*",
        "*@           @*",
        "*@           @*",
        "*@           @*",
        "*@           @*",
        "*@           @*",
        "*#_         _#*",
        "-%+         +%-",
        "/^$+_     _+$^/",
        "?-&%#@@@@@#%&-?",
        " _/-*******-/_ "
    };

    const char * const px1[] = {
        "16 15 9 1",
        "  c None",
        ". c #f1f1f1",
        "* c #bfbfbf"
        "+ c #a0a0a0",
        "@ c #f0f0f0",
        "# c #a8a8a8",
        "$ c #d6d6d6",
        "% c #f8f8f8",
        "& c #a5a5a5",
        "               ",
        " .*+++++++++*. ",
        "@#$%       %$#@",
        "*$           $*",
        "&             &",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "&             &",
        "*$           $*",
        "@#$%       %$#@",
        " .*+++++++++*. "
    };

    const char * const px2[] = {
        "15 15 9 1",
        "  c None",
        ". c #f1f1f1",
        "* c #bfbfbf"
        "+ c #000000",
        "@ c #f0f0f0",
        "# c #a8a8a8",
        "$ c #d6d6d6",
        "% c #f8f8f8",
        "& c #a5a5a5",
        "+++++++++++++++",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+             +",
        "+++++++++++++++"
    };
    //    return QPixmap((const char **)px);
    return QPixmap(":/images/images/group_box_top.png");
}
