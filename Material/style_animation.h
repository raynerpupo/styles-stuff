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

#ifndef STYLEA_NIMATION_H
#define STYLEA_NIMATION_H

#include <QAbstractAnimation>
#include <QDateTime>

class StyleAnimation : public QAbstractAnimation
{
    Q_OBJECT

public:
    StyleAnimation(QObject *target);
    virtual ~StyleAnimation();

    QObject *target() const;

    int duration() const;
    void setDuration(int duration);

    int delay() const;
    void setDelay(int delay);

    QTime startTime() const;
    void setStartTime(const QTime &time);

    void updateTarget();

protected:
    virtual bool isUpdateNeeded() const;
    virtual void updateCurrentTime(int time);

private:
    int _delay;
    int _duration;
    QTime _startTime;
};

#endif // STYLEA_NIMATION_H
