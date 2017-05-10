#pragma once

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>
#include <toolkit/ovtk_all.h>
#include <visualization-toolkit/ovviz_all.h>

#include "topographicMap2DDisplay/ovpCTopographicMapDatabase.h"
#include "topographicMap2DDisplay/ovpCTopographicMap2DView.h"

namespace OpenViBEPlugins
{
	namespace SimpleVisualization
	{
		class CBoxAlgorithmTopographicMap2DDisplay : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:
			CBoxAlgorithmTopographicMap2DDisplay(void);

			virtual void release(void) { delete this; }

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(
				OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean processClock(
				OpenViBE::Kernel::IMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_TopographicMap2DDisplay)

		protected:
			OpenViBEToolkit::TStreamedMatrixDecoder < CBoxAlgorithmTopographicMap2DDisplay >* m_pDecoder;
			OpenViBE::boolean m_bFirstBufferReceived;

			OpenViBE::Kernel::IAlgorithmProxy* m_pSphericalSplineInterpolation;
			CTopographicMapDatabase* m_pTopographicMapDatabase;
			CSignalDisplayDrawable* m_pTopographicMap2DView; //main object used for the display (contains all the GUI code)
		private:
			OpenViBEVisualizationToolkit::IVisualizationContext* m_visualizationContext;
		};

		class CBoxAlgorithmTopographicMap2DDisplayDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("2D topographic map"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Vincent Delannoy"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("This box demonstrates how to perform spherical spline interpolation"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Visualization/Topography"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("2.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString(GTK_STOCK_EXECUTE); }
			virtual OpenViBE::CString getSoftwareComponent(void) const   { return OpenViBE::CString("openvibe-designer"); }
			virtual OpenViBE::CString getAddedSoftwareVersion(void) const   { return OpenViBE::CString("0.0.0"); }
			virtual OpenViBE::CString getUpdatedSoftwareVersion(void) const { return OpenViBE::CString("0.0.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_TopographicMap2DDisplay; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SimpleVisualization::CBoxAlgorithmTopographicMap2DDisplay(); }

			virtual OpenViBE::boolean hasFunctionality(OpenViBE::CIdentifier functionalityIdentifier) const
			{
				return functionalityIdentifier == OVD_Functionality_Visualization;
			}

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addSetting("Interpolation type", OVP_TypeId_SphericalLinearInterpolationType, "1");
				rPrototype.addSetting("Delay (in s)", OV_TypeId_Float, "0");
				rPrototype.addInput("Signal", OV_TypeId_StreamedMatrix);
				rPrototype.addInput("Channel localization", OV_TypeId_ChannelLocalisation);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_TopographicMap2DDisplayDesc);
		};
	};
};

