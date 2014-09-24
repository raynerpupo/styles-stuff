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
