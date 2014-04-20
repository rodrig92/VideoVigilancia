#include "frames.h"

Frames::Frames (QObject* parent) : QAbstractVideoSurface (parent) { }

bool Frames::present (const QVideoFrame& f) {
    QVideoFrame frame = f;
    frame.map (QAbstractVideoBuffer::ReadOnly);
    QImage frameAsImage = QImage (frame.bits (), frame.width (),
                                 frame.height (), frame.bytesPerLine (),
                                 QVideoFrame::imageFormatFromPixelFormat (frame.pixelFormat ()));
    QImage img;
    img = frameAsImage.copy ();
    emit rep (img);
    frame.unmap ();
    return true;
}
