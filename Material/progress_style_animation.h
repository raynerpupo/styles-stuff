#ifndef PROGRESS_STYLE_ANIMATION_H
#define PROGRESS_STYLE_ANIMATION_H

#include "style_animation.h"

class ProgressStyleAnimation : public StyleAnimation
{
    Q_OBJECT

public:
    ProgressStyleAnimation(int speed, QObject *target);

    int animationStep() const;
    int progressStep(int width) const;

    int speed() const;
    void setSpeed(int speed);

protected:
    bool isUpdateNeeded() const;

private:
    int _speed;
    mutable int _step;
};

#endif // PROGRESS_STYLE_ANIMATION_H
