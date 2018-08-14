#ifndef PANORAMAMODEL_H
#define PANORAMAMODEL_H

#include <QQuickItem>

class PanoramaModel : public QQuickItem
{
	Q_OBJECT
	Q_DISABLE_COPY(PanoramaModel)

public:
	PanoramaModel(QQuickItem *parent = nullptr);
	~PanoramaModel();
};

#endif // PANORAMAMODEL_H
