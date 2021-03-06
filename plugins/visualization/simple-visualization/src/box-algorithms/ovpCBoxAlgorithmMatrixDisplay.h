#ifndef __OpenViBEPlugins_BoxAlgorithm_MatrixDisplay_H__
#define __OpenViBEPlugins_BoxAlgorithm_MatrixDisplay_H__

#include "../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <toolkit/ovtk_all.h>
#include <visualization-toolkit/ovviz_all.h>
#include <gtk/gtk.h>
#include <map>
#include <string>

#define OVP_ClassId_BoxAlgorithm_MatrixDisplay     OpenViBE::CIdentifier(0x54F0796D, 0x3EDE2CC0)
#define OVP_ClassId_BoxAlgorithm_MatrixDisplayDesc OpenViBE::CIdentifier(0x63AB4BA7, 0x022C1524)


namespace OpenViBEPlugins
{
	namespace SimpleVisualization
	{
		class CBoxAlgorithmMatrixDisplay : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_MatrixDisplay);

		protected:

			// we need an algorithm to decode the EBML stream (memory buffer) into a Streamed Matrix

			// for the TARGET
			OpenViBEToolkit::TStreamedMatrixDecoder<CBoxAlgorithmMatrixDisplay> m_MatrixDecoder;
			OpenViBE::Kernel::IAlgorithmProxy* m_pMatrixDecoder;
			OpenViBE::Kernel::TParameterHandler < const OpenViBE::IMemoryBuffer* > ip_pMemoryBuffer;
			OpenViBE::Kernel::TParameterHandler < OpenViBE::IMatrix* > op_pMatrix;

			// Outputs: visualization in a gtk window
			::GtkBuilder* m_pMainWidgetInterface;
			::GtkBuilder* m_pToolbarWidgetInterface;
			::GtkWidget* m_pMainWidget;
			::GtkWidget* m_pToolbarWidget;

			std::vector< std::pair <GtkWidget*,GdkColor> > m_vEventBoxCache;
			std::vector< std::pair <GtkLabel*, std::string> > m_vLabelCache;

			std::vector< std::pair <GtkLabel*, std::string> > m_vRowLabelCache;
			std::vector< std::pair <GtkLabel*, std::string> > m_vColumnLabelCache;

			OpenViBE::CMatrix m_MatrixInterpolatedColorGardient;
			OpenViBE::CMatrix m_MatrixColorGradient;
			OpenViBE::uint32  m_GradientSteps;
			OpenViBE::float64 m_f64MaxValue;
			OpenViBE::float64 m_f64MinValue;

			OpenViBE::boolean m_bSymetricMinMax;
			OpenViBE::boolean m_bRealTimeMinMax;

		public:

			OpenViBE::boolean m_bShowValues;
			OpenViBE::boolean m_bShowColors;

			virtual OpenViBE::boolean resetColors(void);
		private:
			OpenViBEVisualizationToolkit::IVisualizationContext* m_visualizationContext;
		};

		class CBoxAlgorithmMatrixDisplayDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Matrix Display"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Laurent Bonnet"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Display a streamed matrix"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("The streamed matrix can be visualized using a table of values and/or a color gradient."); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Visualization/Basic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-select-color"); }
			virtual OpenViBE::CString getSoftwareComponent(void) const   { return OpenViBE::CString("openvibe-designer"); }
			virtual OpenViBE::CString getAddedSoftwareVersion(void) const   { return OpenViBE::CString("0.0.0"); }
			virtual OpenViBE::CString getUpdatedSoftwareVersion(void) const { return OpenViBE::CString("0.0.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_MatrixDisplay; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SimpleVisualization::CBoxAlgorithmMatrixDisplay; }

			virtual OpenViBE::boolean hasFunctionality(OpenViBE::CIdentifier functionalityIdentifier) const
			{
				return functionalityIdentifier == OVD_Functionality_Visualization;
			}

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addSetting("Color gradient",    OV_TypeId_ColorGradient, "0:2,36,58; 50:100,100,100; 100:83,17,20");
				rBoxAlgorithmPrototype.addSetting("Steps",             OV_TypeId_Integer,       "100");
				rBoxAlgorithmPrototype.addSetting("Symetric min/max",  OV_TypeId_Boolean,       "false");
				rBoxAlgorithmPrototype.addSetting("Real time min/max", OV_TypeId_Boolean,       "false");
				rBoxAlgorithmPrototype.addInput  ("Matrix",            OV_TypeId_StreamedMatrix);
				// rBoxAlgorithmPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_IsUnstable);

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_MatrixDisplayDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_MatrixDisplay_H__
