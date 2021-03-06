#include <cstdio>

#include "visualization-toolkit/ovviz_all.h"
#include "ovvizCVisualizationContext.hpp"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;

namespace
{
	OpenViBEVisualizationToolkit::CVisualizationContextDesc visualizationContextDesc;
}

bool OpenViBEVisualizationToolkit::initialize(const IKernelContext& kernelContext)
{
	ITypeManager& typeManager = kernelContext.getTypeManager();
	IPluginManager& pluginManager = kernelContext.getPluginManager();

	typeManager.registerType(OV_TypeId_Color, "Color");
	typeManager.registerType(OV_TypeId_ColorGradient, "Color Gradient");

	pluginManager.registerPluginDesc(visualizationContextDesc);

	return true;
}

bool OpenViBEVisualizationToolkit::uninitialize(const IKernelContext& kernelContext)
{
	(void)kernelContext;
	return true;
}
