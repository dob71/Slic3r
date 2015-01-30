#include "Layer.hpp"
#include "ClipperUtils.hpp"
#include "Print.hpp"
#include "Surface.hpp"

namespace Slic3r {

LayerRegion::LayerRegion(Layer *layer, PrintRegion *region)
:   _layer(layer),
    _region(region)
{
}

LayerRegion::~LayerRegion()
{
}

Layer*
LayerRegion::layer()
{
    return this->_layer;
}

PrintRegion*
LayerRegion::region()
{
    return this->_region;
}

Flow
LayerRegion::flow(FlowRole role, bool bridge, double width) const
{
    return this->_region->flow(
        role,
        this->_layer->height,
        bridge,
        this->_layer->id() == 0,
        width,
        *this->_layer->object(),
        false
    );
}

void
LayerRegion::merge_slices()
{
    ExPolygons expp;
    // without safety offset, artifacts are generated (GH #2494)
    union_(this->slices, &expp, true);
    this->slices.surfaces.clear();
    this->slices.surfaces.reserve(expp.size());
    
    for (ExPolygons::const_iterator expoly = expp.begin(); expoly != expp.end(); ++expoly)
        this->slices.surfaces.push_back(Surface(stInternal, *expoly));
}

#ifdef SLIC3RXS
REGISTER_CLASS(LayerRegion, "Layer::Region");
#endif

}
