#ifndef __OpenViBEDesigner_CCommentEditorDialog_H__
#define __OpenViBEDesigner_CCommentEditorDialog_H__

#include "ovd_base.h"

namespace OpenViBEDesigner
{
	class CCommentEditorDialog
	{
	public:

		CCommentEditorDialog(const OpenViBE::Kernel::IKernelContext& rKernelContext, OpenViBE::Kernel::IComment& rComment, const char* sGUIFilename);
		virtual ~CCommentEditorDialog(void);

		OpenViBE::boolean run(void);

		// Callback for text formatting
		void applyTagCB(const char* sTagIn, const char* sTagOut);

		// help formatting pango
		void infoCB(void);
	protected:

		const OpenViBE::Kernel::IKernelContext& m_rKernelContext;
		OpenViBE::Kernel::IComment& m_rComment;
		OpenViBE::CString m_sGUIFilename;

	private:

		CCommentEditorDialog(void);

		::GtkBuilder* m_pInterface;
		::GtkWidget* m_pDialog;
		::GtkWidget* m_pInfoDialog;
		::GtkWidget* m_pDescription;
		::GtkTextBuffer* m_pDescriptionBuffer;
	};
};

#endif // __OpenViBEDesigner_CCommentEditorDialog_H__
