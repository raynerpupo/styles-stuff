#include "style_animation.h"
#include <QCoreApplication>
#include <qwidget.h>
#include <QDebug>

StyleAnimation::StyleAnimation(QObject *target) : QAbstractAnimation(target),
    _delay(0), _duration(-1), _startTime(QTime::currentTime())
{
}

StyleAnimation::~StyleAnimation()
{
}

QObject *StyleAnimation::target() const
{
    return parent();
}

int StyleAnimation::duration() const
{
    return _duration;
}

void StyleAnimation::setDuration(int duration)
{
    _duration = duration;
}

int StyleAnimation::delay() const
{
    return _delay;
}

void StyleAnimation::setDelay(int delay)
{
    _delay = delay;
}

QTime StyleAnimation::startTime() const
{
    return _startTime;
}

void StyleAnimation::setStartTime(const QTime &time)
{
    _startTime = time;
}

void StyleAnimation::updateTarget()
{
//    qDebug() << Q_FUNC_INFO;
    QEvent event(QEvent::StyleAnimationUpdate);
    event.setAccepted(false);
    QCoreApplication::sendEvent(target(), &event);
    if (!event.isAccepted())
        stop();
}

bool StyleAnimation::isUpdateNeeded() const
{
    return currentTime() > _delay;
}

void StyleAnimation::updateCurrentTime(int)
{
    if (target() && isUpdateNeeded())
        updateTarget();
}
