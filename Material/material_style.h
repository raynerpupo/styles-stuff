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

#ifndef MATERIAL_STYLE_H
#define MATERIAL_STYLE_H

#include "constants.h"

#include <QCommonStyle>
#include <QStyleOptionTabV3>
#include <QFont>

class QStyleOptionButton;
class QStyleOptionGroupBox;
class QStyleOptionSlider;
//class QStyleOptionTabV3;

class MaterialStyle : public QCommonStyle
{
    Q_OBJECT
public:
    explicit MaterialStyle();
    void drawPrimitive(PrimitiveElement elem,
                       const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = 0) const;
    void drawControl(ControlElement ce, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget) const;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const;
    int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const;
    void polish(QWidget *widget);
    void polish(QPalette &pal);
    QRect subElementRect(SubElement subElem, const QStyleOption *opt, const QWidget *widget) const;
    int styleHint(StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret = 0) const;
signals:

public slots:

private:
    QPainterPath roundRectPath(const QRect &rect) const;
    QPainterPath roundTopBorder(const QRect &rect) const;

    QFont groupBoxFont;
    QFont regularFont;

    //controls painting
    void drawPushButton(const QStyleOptionButton *btn, QPainter *painter, const QWidget *widget) const;

    //complex controls painting
    void drawGroupBox(const QStyleOptionGroupBox *groupBox, QPainter *painter, const QWidget *widget) const;
    void drawSlider(const QStyleOptionSlider *slider, QPainter *painter, const QWidget *widget) const;

    void tabLayout(const QStyleOptionTabV3 *opt, const QWidget *widget, QRect *textRect, QRect *pixmapRect) const;

    //testing
    QPixmap testPxFactory() const;


};

#endif // MATERIAL_STYLE_H
