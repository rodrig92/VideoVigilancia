#ifndef FRAMES_H
#define FRAMES_H

#include <QAbstractVideoSurface>

class Frames : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit Frames(QObject *parent = 0);

signals:

public slots:

};

#endif // FRAMES_H
