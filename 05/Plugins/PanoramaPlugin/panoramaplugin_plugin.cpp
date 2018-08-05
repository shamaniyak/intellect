#include "panoramaplugin_plugin.h"
#include "panoramamodel.h"
#include "cartographymap.h"

#include <qqml.h>

void PanoramaPluginPlugin::registerTypes(const char *uri)
{
	// @uri PanoramaPlugin
	//qmlRegisterType<PanoramaModel>(uri, 1, 0, "PanoramaModel");
	qmlRegisterType<CartographyMap>(uri, 1, 0, "Panorama");

}

