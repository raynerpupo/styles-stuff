#include "progress_style_animation.h"

ProgressStyleAnimation::ProgressStyleAnimation(int speed, QObject *target) :
    StyleAnimation(target), _speed(speed), _step(-1)
{
}

int ProgressStyleAnimation::animationStep() const
{
    return currentTime() / (1000.0 / _speed);
}

int ProgressStyleAnimation::progressStep(int width) const
{
    int step = animationStep();
    int progress = (step * width / _speed) % width;
    if (((step * width / _speed) % (2 * width)) >= width)
        progress = width - progress;
    return progress;
}

int ProgressStyleAnimation::speed() const
{
    return _speed;
}

void ProgressStyleAnimation::setSpeed(int speed)
{
    _speed = speed;
}

bool ProgressStyleAnimation::isUpdateNeeded() const
{
    if (StyleAnimation::isUpdateNeeded()) {
        int current = animationStep();
        if (_step == -1 || _step != current)
        {
            _step = current;
            return true;
        }
    }
    return false;
}
