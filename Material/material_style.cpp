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

MaterialStyle::MaterialStyle()
{
    setObjectName(QLatin1String("Fusion"));

    groupBoxFont = QFont("Myriad Pro", 11);
    groupBoxFont.setBold(true);
//    groupBoxFont.setStyleStrategy(QFont::NoAntialias);

    regularFont = QFont("Myriad Pro", 11);
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
//        qDebug() << "tH from primitive" <<textRect.height();
        QRect topFrame(0, 0, groupBox->rect.width(), yDiv);

        qDrawBorderPixmap(painter, topFrame, QMargins(4, 4, 4, 4), tPix);

        QRect bottomFrame(0, yDiv, groupBox->rect.width(), groupBox->rect.height() - yDiv - 1);
//        qDebug() << "bottom frame" << bottomFrame;
        qDrawBorderPixmap(painter, bottomFrame, QMargins(4, 4, 4, 4), bPix);
        break;
    }
    case PE_IndicatorCheckBox: {
        const QStyleOptionButton *checkBox = qstyleoption_cast<const QStyleOptionButton *>(option);
//        qDebug() << "cbrect" << checkBox->rect;

//        qDebug() << "opt rect" << option->rect;
        QRect indRect = subElementRect(SE_CheckBoxIndicator, option, widget);
        indRect.moveTo(option->rect.topLeft());
//        qDebug() << "indRect" << indRect;
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
////        painter->setRenderHint(QPainter::Antialiasing);
//        ///finish-him
//        painter->setPen(QPen(QColor(160,160,160), 1));
//        qDebug() << option->rect;
//        painter->drawEllipse(option->rect.adjusted(1, 1, -1, -1));
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
//        QPen dashedPen(QColor());
        painter->setOpacity(0.2);
//        painter->fillRect(option->rect, Qt::blue);
        painter->drawRoundedRect(option->rect.adjusted(0, 0, -1, -1), 3, 3);
        break;
    }
    case PE_Frame:
    case PE_FrameMenu:
        if (const QStyleOptionFrame *frame = qstyleoption_cast<const QStyleOptionFrame *>(option)) {
            if (elem == PE_FrameMenu || (frame->state & State_Sunken) || (frame->state & State_Raised)) {
//                qDebug() << "frame aaaaaaaaaaa";
                QPixmap px(":/images/images/frame.png");
                qDrawBorderPixmap(painter, frame->rect, QMargins(4, 4, 4, 4), px);
            } else {
//                qDebug() << "frame bbbbbbbbbbb";
                qDrawPlainRect(painter, frame->rect, QColor(160, 160, 160), frame->lineWidth);
            }
            break;
        }
    default:
//        qDebug() << elem;
        QCommonStyle::drawPrimitive(elem, option, painter, widget);
        break;
    }
}

void MaterialStyle::drawControl(QStyle::ControlElement ce, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
//    QCommonStyle::drawControl(ce, option, painter, widget);
//    qDebug() << Q_FUNC_INFO << option->rect;
    switch (ce) {
    case QStyle::CE_PushButton:
        if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(option)) {
            drawPushButton(btn, painter, widget);
        }
        break;
//    case QStyle::CE_CheckBoxLabel: {
////        qDebug() << "draw CE_CheckBoxLabel";
//        QFont font = regularFont;
////        painter->setRenderHint(QPainter::TextAntialiasing);
//        const QStyleOptionButton *check = qstyleoption_cast<const QStyleOptionButton *>(option);
////        font.setStyleStrategy(QFont::NoAntialias);
//        painter->setFont(font);
//        painter->setPen(QColor(61, 61, 61));
//        painter->drawText(check->rect, check->text);
//        break;
//    }
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
////        qDebug() << "draw CE_CheckBox for indicator";
//        const QStyleOptionButton *check = qstyleoption_cast<const QStyleOptionButton *>(option);
//        //draw primitive indicator
//        drawPrimitive(PE_IndicatorCheckBox, check, painter, widget);
//        //draw control CE_CheckBoxLabel
//        QStyleOptionButton subOpt = *check;
//        subOpt.rect = subElementRect(SE_CheckBoxContents, check, widget);
//        drawControl(CE_CheckBoxLabel, &subOpt, painter, widget);
//        break;
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
    }
        break;
    }


}

QRect MaterialStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget) const
{
    QRect rect = QCommonStyle::subControlRect(cc, opt, sc, widget);
    switch (cc) {
    case CC_GroupBox: {
//        qDebug() << "SC_GroupBoxLabel" << cc << sc;
        if (const QStyleOptionGroupBox *groupBox = qstyleoption_cast<const QStyleOptionGroupBox *>(opt)) {
            rect = opt->rect;
            int indWidth = groupBox->subControls & SC_GroupBoxCheckBox ?
                        pixelMetric(PM_IndicatorWidth, opt, widget) : 0;
//            QRect indicatorRect(GROUP_BOX_TEXT_H_MARGIN, (rect.height() - indWidth) / 2, indWidth, indWidth);

            QFontMetrics fm(groupBoxFont);
            QRect textRect = fm.boundingRect(groupBox->text);
            int stolen = 0;
            int textHeight = 0;
            int indHeight = 0;
            if (groupBox->subControls & SC_GroupBoxLabel) {
                QFontMetrics fm(groupBoxFont);
                QRect textRect = fm.boundingRect(groupBox->text);
                textHeight = textRect.height();
//                qDebug() << "th from screct" << textHeight;
                stolen += 2 * GROUP_BOX_TEXT_V_MARGIN;
                stolen += GROUP_BOX_UNDERLINE_WIDTH;
            }
            else if (groupBox->subControls & SC_GroupBoxCheckBox) {
                indHeight = indWidth;
                stolen += 2 * GROUP_BOX_TEXT_V_MARGIN;
//                    stolen += GROUP_BOX_UNDERLINE_WIDTH;
            }
            stolen += qMax(textHeight, indHeight);

            if (sc == SC_GroupBoxLabel) {

//                qDebug() << "req gb text rect";
                int space = pixelMetric(PM_CheckBoxLabelSpacing, opt, widget);

                textRect.moveTo(GROUP_BOX_TEXT_H_MARGIN + indWidth + space, GROUP_BOX_TEXT_V_MARGIN);
//                qDebug() << "text rectt" << textRect;
                return textRect;
            }

            else if (sc == SC_GroupBoxFrame) {
//                qDebug() << "req gb frame rect";
                return rect;
            }
            else if (sc == SC_GroupBoxContents) {
//                stolen += 2; //top border
                QRect cRect = rect.adjusted(3, stolen, -3, -3);

//                qDebug() << "conten rect" << cRect;
                return cRect;
//                qDebug() << "th indh" << textHeight << indHeight;



////                qDebug() << "req gb contents rect";
//                QFontMetrics fm(groupBoxFont);
//                QRect textRect = fm.boundingRect(groupBox->text);
//                int contentY = textRect.height() + 2 * GROUP_BOX_TEXT_V_MARGIN + GROUP_BOX_UNDERLINE_WIDTH;
////                qDebug() << "contentY" << contentY << rect;
//                return rect.adjusted(2, contentY, -3, -3 );
            }
            else if (sc == SC_GroupBoxCheckBox) {
//                qDebug() << "SC_GroupBoxCheckBox";
                QRect iRect(GROUP_BOX_TEXT_H_MARGIN, (stolen - indWidth) / 2,
                            indWidth, indWidth);
//                qDebug() << "irect" << iRect;
                return iRect;
            }
        }
        return rect;
    }
    default:
        return rect;
    }

}

int MaterialStyle::pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    switch (metric) {
    case PM_ButtonMargin:
        return 3;
    case PM_SliderTickmarkOffset:
        return 4;
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
        return 20;
    case PM_TabBarTabVSpace:
        return 12;
    case PM_TabBarTabOverlap:
        return 1;
    case PM_TabBarBaseOverlap:
        return 2;
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
            || qobject_cast<QComboBox *>(widget))
        widget->setAttribute(Qt::WA_Hover, true);
//    if (qobject_cast<QGroupBox *>(widget)) {
//        QFont font("Myriad Pro", 12);
//        font.setBold(true);
//        widget->setFont(font);
    //    }
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
    QRect btnRect = btn->rect.adjusted(0, 1, -BORDER_WIDTH, -BORDER_WIDTH);
    QColor textColor;
    QColor textShadowColor;
    QLinearGradient grad(0, 0, 0, btnRect.height());

    bool drawTextShadow = false;
    QPixmap borderPxm;
    if (btn->state & State_Enabled) {
        if (btn->state & (State_Sunken | State_On)) {
            //        qDebug() << "presed";
            grad.setColorAt(0.0, QColor(7, 58, 147));
            grad.setColorAt(1.0, QColor(64, 148, 208));
            textColor.setNamedColor("White");
            textShadowColor.setRgb(0, 0, 0, 25 * 255 / 100);
            borderPxm = QPixmap(":/images/images/push_button_normal.png");
            drawTextShadow = true;
        }
        else if (btn->state & (State_MouseOver)) {
            //        qDebug() << "over";
            grad.setColorAt(0.0, QColor(132, 191, 247));
            grad.setColorAt(1.0, QColor(43, 84, 163));
            drawGlowLine = false;
            textColor.setRgb(7, 30, 82);
            textShadowColor.setRgb(255, 255, 255, 40 * 255 / 100);
            borderPxm = QPixmap(":/images/images/push_button_hover.png");
            drawTextShadow = true;
        }
        else {
            //        qDebug() << "normal";
            grad.setColorAt(0.0, QColor(64, 148, 208));
            grad.setColorAt(1.0, QColor(7, 58, 147));
            textColor.setNamedColor("White");
            textShadowColor.setRgb(0, 0, 0, 25);
            borderPxm = QPixmap(":/images/images/push_button_normal.png");
            drawTextShadow = true;
        }
        painter->setBrush(grad);
        painter->setPen(Qt::NoPen);
        painter->drawPath(roundRectPath(btnRect.adjusted(1, 1, 0, 0)));
    }
    else {
        borderPxm = QPixmap(":/images/images/push_button_disabled.png");
        textColor.setRgb(171, 171, 171);
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
    QFont font("Myriad", 10);
    QFontMetrics fm(font);
    painter->setFont(font);
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

}

void MaterialStyle::drawGroupBox(const QStyleOptionGroupBox *groupBox, QPainter *painter, const QWidget *widget) const
{

    QFontMetrics fm(groupBoxFont);
    QRect textRect = fm.boundingRect(groupBox->text);
    //Draw the frame
    if (groupBox->subControls & QStyle::SC_GroupBoxFrame) {
        drawPrimitive(PE_FrameGroupBox, groupBox, painter, widget);
    }

    //Draw the indicator
    if (groupBox->subControls & QStyle::SC_GroupBoxCheckBox) {
//        QRect indRect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxCheckBox, widget);
//        QRect textRec
        QStyleOptionButton chk;
        chk.QStyleOption::operator=(*groupBox);
//        chk.rect = QRect(0, 0, groupBox->rect.width(), textRect.height() + 2 * GROUP_BOX_TEXT_V_MARGIN);
        chk.rect = subControlRect(CC_GroupBox, groupBox, SC_GroupBoxCheckBox, widget);
//        qDebug() << "chk.rect" << chk.rect;
//        painter->translate(4, 0);
        drawPrimitive(PE_IndicatorCheckBox, &chk, painter, widget);
    }

    //Draw the text
    if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty()) {
//        QFont font("Myriad Pro", 12);
        painter->setPen(QColor(0, 105, 170));
        int spacing = pixelMetric(PM_CheckBoxLabelSpacing, groupBox, widget);
        int xOffset = groupBox->subControls & QStyle::SC_GroupBoxCheckBox ?
                    spacing + pixelMetric(PM_IndicatorWidth) : 0;
        xOffset += GROUP_BOX_TEXT_H_MARGIN;
        QFontMetrics fm(groupBoxFont);
        QRect textRect = fm.boundingRect(groupBox->text);
        textRect.moveTo(0, 0);
//        qDebug() << "textRect" << textRect;
        int yOffset = GROUP_BOX_TEXT_V_MARGIN /*+ textRect.height()*/;
        QRect okRect = textRect.adjusted(xOffset, yOffset, xOffset, yOffset);
//        qDebug() << "x-y" << xOffset << yOffset;
//        qDebug() << "draw text" << groupBox->text << "on" << okRect;
        painter->setFont(groupBoxFont);
        painter->drawText(okRect, groupBox->text);

        //Draw the decoration

        int underlineY = 2 * GROUP_BOX_TEXT_V_MARGIN + textRect.height();
        painter->setPen(QPen(QColor(160, 160, 160), GROUP_BOX_UNDERLINE_WIDTH));
        //-2 fills the gap in Windows
        painter->drawLine(-2, underlineY, okRect.x(), underlineY);

        painter->setPen(QPen(QColor(1, 98, 177), GROUP_BOX_UNDERLINE_WIDTH));
        painter->drawLine(okRect.x(), underlineY, okRect.width() + okRect.x(), underlineY);

        painter->setPen(QPen(QColor(160, 160, 160), GROUP_BOX_UNDERLINE_WIDTH));
        painter->drawLine(okRect.width() + okRect.x(), underlineY, groupBox->rect.width(), underlineY);
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
