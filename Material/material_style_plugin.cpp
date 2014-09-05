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

#include "material_style_plugin.h"
#include "material_style.h"

MaterialStylePlugin::MaterialStylePlugin(QObject *parent) :
    QStylePlugin(parent)
{
}

QStyle *MaterialStylePlugin::create(const QString &key)
{
    return new MaterialStyle;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Material, MaterialStylePlugin)
#endif // QT_VERSION < 0x050000
