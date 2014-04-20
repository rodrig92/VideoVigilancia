#ifndef FRAMES_H
#define FRAMES_H

#include <QAbstractVideoSurface>

class Frames : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    explicit Frames (QObject* parent = 0);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats
    (QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const
    {
        QList<QVideoFrame::PixelFormat> formats;
        formats << QVideoFrame::Format_ARGB32;
        formats << QVideoFrame::Format_ARGB32_Premultiplied;
        formats << QVideoFrame::Format_RGB32;
        formats << QVideoFrame::Format_RGB24;
        formats << QVideoFrame::Format_RGB565;
        formats << QVideoFrame::Format_RGB555;
        return formats;
    }

    bool present (const QVideoFrame& f);

signals:
    void rep (const QImage& image);
};

#endif // FRAMES_H
