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
#include <QResource>
#include <QFontDatabase>
#include <QDirIterator>
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
        QRect indRect = subElementRect(SE_CheckBoxIndicator, option, widget);
        indRect.moveTo(option->rect.topLeft());
        if (checkBox->state & QStyle::State_Off) {
            QPixmap pxm(":/images/images/check_box_state0.png");
            qDrawBorderPixmap(painter, indRect, QMargins(2, 2, 2, 2), pxm);
        }
        else if (checkBox->state & QStyle::State_On) {
            QPixmap pxm(":/images/images/check_box_state1.png");
            qDrawBorderPixmap(painter, indRect, QMargins(3, 3, 3, 3), pxm);
        }
        else {
            QPixmap pxm(":/images/images/check_box_state2.png");
            painter->drawPixmap(indRect.topLeft(), pxm);
        }
        break;
    }
    case PE_IndicatorRadioButton: {
        const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option);
        if (btn) {
            bool checked = btn->state & QStyle::State_On;
            QString iconRes = checked ? ":/images/images/qradiobutton_chacked.png" :
                                        ":/images/images/qradiobutton.png";
            QIcon icon(iconRes);
            QIcon::State st = btn->state & QStyle::State_Enabled ? QIcon::On : QIcon::Off;
            icon.paint(painter, btn->rect, Qt::AlignCenter, QIcon::Normal, st);
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
    case PE_Frame:
    case PE_FrameMenu:
        if (const QStyleOptionFrame *frame = qstyleoption_cast<const QStyleOptionFrame *>(option)) {
//            if (elem == PE_FrameMenu || (frame->state & State_Sunken) || (frame->state & State_Raised)) {
//                QPixmap px(":/images/images/frame.png");
//                qDrawBorderPixmap(painter, frame->rect, QMargins(4, 4, 4, 4), px);
//            } else {
//                qDrawPlainRect(painter, frame->rect, COLOR_FRAME_BORDER, frame->lineWidth);
//            }
            painter->setPen(COLOR_FRAME_BORDER);
            painter->drawRect(frame->rect.adjusted(0, 0, -1, -1));
            break;
        }
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
        QStyleHintReturn *hr = 0;
        QPixmap px(":/images/images/tab_close.png");
        QCommonStyle::drawItemPixmap(painter, option->rect, Qt::AlignCenter, px);
        if (option->state & State_MouseOver) {
            painter->setPen(QPen(COLOR_TAB_NORMAL_UL));
            painter->drawRect(option->rect.adjusted(0, 0, -1, -1));
        }
        break;
    }
    case PE_PanelScrollAreaCorner: {
        painter->save();
        painter->setPen(COLOR_FRAME_BORDER);
        painter->setBrush(option->palette.window().color().darker());
        painter->drawRect(option->rect);
        painter->restore();
    } break;
    default:
        QCommonStyle::drawPrimitive(elem, option, painter, widget);
        break;
    }
}

void MaterialStyle::drawControl(QStyle::ControlElement ce, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (ce) {
    case QStyle::CE_PushButton:
        if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option)) {
            drawPushButton(btn, painter, widget);
        }
        break;
    case QStyle::CE_RadioButton:
    case QStyle::CE_CheckBox: {
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

//            painter->setPen(Qt::red);
//            painter->setBrush(QColor(0, 0, 255, 100));
//            painter->drawRect(rect.adjusted(0, 0, -1, 0));
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
            painter->drawLine(p1, p2);

            //draw the separation
            if (!(lastTab || onlyOne)) {
                int sepMargin = 7;
                painter->setPen(QPen(COLOR_TAB_SEPARATOR, 1));
                painter->drawLine(rect.right(), sepMargin,
                                  rect.right(), rect.height() - sepMargin);
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
                proxy()->drawItemText(painter, editRect.adjusted(1, 0, -1, 0),
                                      visualAlignment(cb->direction, Qt::AlignLeft | Qt::AlignVCenter),
                                      cb->palette, cb->state & State_Enabled, cb->currentText,
                                      cb->editable ? QPalette::Text : QPalette::ButtonText);
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
        if (const QStyleOptionSlider *scrollBar = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
            bool horizontal = scrollBar->orientation == Qt::Horizontal;
            bool sunken = scrollBar->state & State_Sunken;

            QRect scrollBarSubLine = subControlRect(control, scrollBar, SC_ScrollBarSubLine, widget);
            QRect scrollBarAddLine = subControlRect(control, scrollBar, SC_ScrollBarAddLine, widget);
            QRect scrollBarSlider = subControlRect(control, scrollBar, SC_ScrollBarSlider, widget);
//            QRect scrollBarGroove = subControlRect(control, scrollBar, SC_ScrollBarGroove, widget);

            QRect rect = option->rect;
            QColor frameBorderColor = COLOR_FRAME_BORDER;

            QColor arrowColor = option->palette.foreground().color();
            arrowColor.setAlpha(220);


            // Paint groove
            if (scrollBar->subControls & SC_ScrollBarGroove) {
                painter->save();
                painter->fillRect(rect, COLOR_SCROLLBAR_GROOVE);
                painter->setPen(Qt::NoPen);

                painter->fillRect(rect.adjusted(3, 3, -2, -2), COLOR_SCROLL_GROOVE_DECO);

                painter->setPen(frameBorderColor);
                if (horizontal)
                    painter->drawLine(rect.topLeft(), rect.topRight());
                else
                    painter->drawLine(rect.topLeft(), rect.bottomLeft());
                painter->restore();
            }

            QRect pixmapRect = scrollBarSlider;

            // Paint slider
            if (scrollBar->subControls & SC_ScrollBarSlider) {
//                QRect pixmapRect = scrollBarSlider;
//                painter->setPen(QPen(Qt::green));
                if (option->state & State_Sunken && scrollBar->activeSubControls & SC_ScrollBarSlider) {
                    QPixmap px(":/images/images/slider_button_sunken.png");
                    qDrawBorderPixmap(painter, pixmapRect.adjusted(0, 0, horizontal ? 0 : 1, horizontal ? 1 : 0),
                                      QMargins(2, 2, 2, 2), px);
                }
                else if (option->state & State_MouseOver && scrollBar->activeSubControls & SC_ScrollBarSlider) {
                    QPixmap px(":/images/images/slider_button_hover.png");
                    qDrawBorderPixmap(painter, pixmapRect.adjusted(0, 0, horizontal ? 0 : 1, horizontal ? 1 : 0),
                                      QMargins(2, 2, 2, 2), px);
                }
                else { //normal
                    QPixmap px(":/images/images/slider_button_normal.png");
                    qDrawBorderPixmap(painter, pixmapRect.adjusted(0, 0, horizontal ? 0 : 1, horizontal ? 1 : 0),
                                      QMargins(2, 2, 2, 2), px);
                }


//                painter->drawRect(pixmapRect.adjusted(horizontal ? -1 : 0, horizontal ? 0 : -1, horizontal ? 0 : 1, horizontal ? 1 : 0));

//                painter->setPen(Qt::darkYellow);
//                painter->drawRect(scrollBarSlider.adjusted(horizontal ? 0 : 1, horizontal ? 1 : 0, -1, -1));
            }

            // The SubLine (up/left) buttons
            if (scrollBar->subControls & SC_ScrollBarSubLine) {
//                qDebug() << "SUBLINE";
                if ((scrollBar->activeSubControls & SC_ScrollBarSubLine) && sunken)
                    painter->setBrush(scrollBar->palette.window().color().darker(130));
                else if ((scrollBar->activeSubControls & SC_ScrollBarSubLine))
                    painter->setBrush(scrollBar->palette.window().color().darker(116));
                else
                    painter->setBrush(scrollBar->palette.window());

                painter->setPen(Qt::NoPen);
                painter->drawRect(scrollBarSubLine.adjusted(horizontal ? 0 : 1, horizontal ? 1 : 0, 0, 0));
                painter->setPen(QPen(frameBorderColor));
                if (option->state & State_Horizontal) {
                    if (option->direction == Qt::RightToLeft) {
                        pixmapRect.setLeft(scrollBarSubLine.left());
                        painter->drawLine(pixmapRect.topLeft(), pixmapRect.bottomLeft());
                    } else {
                        pixmapRect.setRight(scrollBarSubLine.right());
                        painter->drawLine(pixmapRect.topRight(), pixmapRect.bottomRight());
                    }
                } else {
                    pixmapRect.setBottom(scrollBarSubLine.bottom());
                    painter->drawLine(pixmapRect.bottomLeft(), pixmapRect.bottomRight());
                }

//                painter->setBrush(Qt::NoBrush);
//                painter->setPen(Qt::darkGray);
//                painter->drawRect(scrollBarSubLine.adjusted(horizontal ? 0 : 1, horizontal ? 1 : 0 ,  horizontal ? -2 : -1, horizontal ? -1 : -2));

                // Arrows
                int rotation = 0;
                if (option->state & State_Horizontal)
                    rotation = option->direction == Qt::LeftToRight ? -90 : 90;
                QRect upRect = scrollBarSubLine.translated(horizontal ? -2 : -1, 0);
//                QPixmap arrowPixmap = colorizedImage(QLatin1String(":/qt-project.org/styles/commonstyle/images/fusion_arrow.png"), arrowColor, rotation);
                QPixmap arrowPixmap(":/images/images/slider_frame.png");
                painter->drawPixmap(QRectF(upRect.center().x() - arrowPixmap.width() / 4.0  + 2.0,
                                          upRect.center().y() - arrowPixmap.height() / 4.0 + 1.0,
                                          arrowPixmap.width() / 2.0, arrowPixmap.height() / 2.0),
                                          arrowPixmap, QRectF(QPoint(0.0, 0.0), arrowPixmap.size()));
            }

            // The AddLine (down/right) button
            if (scrollBar->subControls & SC_ScrollBarAddLine) {
                if ((scrollBar->activeSubControls & SC_ScrollBarAddLine) && sunken)
                    painter->setBrush(scrollBar->palette.window().color().darker(130));
                else if ((scrollBar->activeSubControls & SC_ScrollBarAddLine))
                    painter->setBrush(scrollBar->palette.window().color().darker(116));
                else
                    painter->setBrush(scrollBar->palette.window());

                painter->setPen(Qt::NoPen);
                painter->drawRect(scrollBarAddLine.adjusted(horizontal ? 0 : 1, horizontal ? 1 : 0, 0, 0));
                painter->setPen(QPen(frameBorderColor, 1));
                if (option->state & State_Horizontal) {
                    if (option->direction == Qt::LeftToRight) {
                        pixmapRect.setLeft(scrollBarAddLine.left());
                        painter->drawLine(pixmapRect.topLeft(), pixmapRect.bottomLeft());
                    } else {
                        pixmapRect.setRight(scrollBarAddLine.right());
                        painter->drawLine(pixmapRect.topRight(), pixmapRect.bottomRight());
                    }
                } else {
                    pixmapRect.setTop(scrollBarAddLine.top());
                    painter->drawLine(pixmapRect.topLeft(), pixmapRect.topRight());
                }

//                painter->setPen(Qt::darkGray);
//                painter->setBrush(Qt::NoBrush);
//                painter->drawRect(scrollBarAddLine.adjusted(1, 1, -1, -1));

                int rotation = 180;
                if (option->state & State_Horizontal)
                    rotation = option->direction == Qt::LeftToRight ? 90 : -90;
                QRect downRect = scrollBarAddLine.translated(-1, 1);
//                QPixmap arrowPixmap = colorizedImage(QLatin1String(":/qt-project.org/styles/commonstyle/images/fusion_arrow.png"), arrowColor, rotation);
                QPixmap arrowPixmap(":/images/images/slider_frame.png");
                painter->drawPixmap(QRectF(downRect.center().x() - arrowPixmap.width() / 4.0 + 2.0,
                                           downRect.center().y() - arrowPixmap.height() / 4.0,
                                           arrowPixmap.width() / 2.0, arrowPixmap.height() / 2.0),
                                           arrowPixmap, QRectF(QPoint(0.0, 0.0), arrowPixmap.size()));
            }

        }
        painter->restore();
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
            || qobject_cast<QSlider *>(widget))
        widget->setAttribute(Qt::WA_Hover, true);
//    if (qobject_cast<QGroupBox *>(widget)) {
//        QFont font("Myriad Pro", 12);
//        font.setBold(true);
//        widget->setFont(font);
    //    }
}

void MaterialStyle::polish(QPalette &pal)
{
    pal.setBrush(QPalette::Window, QColor(244, 244, 244));       //window background
    pal.setBrush(QPalette::Light, QColor(64, 148, 208));         //pushbutton normal start gradient
    pal.setBrush(QPalette::Dark, QColor(7, 58, 147));            //pushbutton normal end gradient
    pal.setBrush(QPalette::Midlight, QColor(132, 191, 247));     //pushbutton hover start gradient
    pal.setBrush(QPalette::Mid, QColor(43, 84, 163));            //pushbutton hover end gradient
    pal.setBrush(QPalette::Button, QColor(244, 244, 244));       //button normal color
    pal.setBrush(QPalette::ButtonText, QColor(255, 255, 255));   //pushbutton text color
    pal.setBrush(QPalette::Shadow, QColor(0, 0, 0));             //shadow
    pal.setBrush(QPalette::BrightText, QColor(7, 30, 82));       //pushbutton hover text color
    pal.setBrush(QPalette::Disabled, QPalette::ButtonText, QColor(171, 171, 171));
    pal.setBrush(QPalette::WindowText, COLOR_WINDOW_TEXT);
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
        break;
    default:
        return QCommonStyle::styleHint(sh, opt, w, shret);
    }
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

void MaterialStyle::drawPushButton(const QStyleOptionButton *btn, QPainter *painter, const QWidget *widget) const
{
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing);
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
    QPixmap borderPxm;

    if (btn->state & State_Enabled) {
        if (btn->state & State_HasFocus && btn->state & State_KeyboardFocusChange) {
            drawPrimitive(PE_FrameFocusRect, btn, painter, widget);
        }
        if (btn->state & (State_Sunken | State_On)) {
            grad.setColorAt(0.0, btn->palette.color(QPalette::Dark));
            grad.setColorAt(1.0, btn->palette.color(QPalette::Light));
            textColor = btn->palette.color(QPalette::ButtonText);
//            textColor.setNamedColor("White");
            textShadowColor = btn->palette.color(QPalette::Shadow);
            textShadowColor.setAlpha(25 * 255 / 100);
            borderPxm = QPixmap(":/images/images/push_button_normal.png");
            drawTextShadow = true;
        }
        else if (btn->state & (State_MouseOver)) {
            grad.setColorAt(0.0, btn->palette.midlight().color());
            grad.setColorAt(1.0, btn->palette.mid().color());
            drawGlowLine = false;
            textColor = btn->palette.color(QPalette::BrightText);
            textShadowColor = btn->palette.color(QPalette::ButtonText);
            textShadowColor.setAlpha(40 * 255 / 100);
            borderPxm = QPixmap(":/images/images/push_button_hover.png");
            drawTextShadow = true;
        }
        else {
            grad.setColorAt(0.0, btn->palette.color(QPalette::Light));
            grad.setColorAt(1.0, btn->palette.color(QPalette::Dark));
            textColor = btn->palette.color(QPalette::ButtonText);
            textShadowColor = btn->palette.color(QPalette::Shadow);
            textShadowColor.setAlpha(25 * 255 / 100);
            borderPxm = QPixmap(":/images/images/push_button_normal.png");
            drawTextShadow = true;
        }
        painter->setBrush(grad);
        painter->setPen(Qt::NoPen);
        painter->drawPath(roundRectPath(btnRect.adjusted(1, 1, 0, 0)));
    }
    else {
        borderPxm = QPixmap(":/images/images/push_button_disabled.png");
        textColor = btn->palette.color(QPalette::Disabled, QPalette::ButtonText);
    }

    qDrawBorderPixmap(painter, btnRect, QMargins(4, 5, 4, 4), borderPxm);



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
        painter->drawText(textRect.adjusted(1, 3, 2, 2), btn->text);
    }
    painter->setPen(textColor);
    painter->drawText(textRect.adjusted(0, 2, 1, 1), btn->text);

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
