#pragma once

#include <visualization-toolkit/ovvizIVisualizationTree.h>

#include <map>
#include <gtk/gtk.h>

namespace json
{
	class Object;
}
namespace OpenViBEDesigner
{
	class CVisualizationTree final : public OpenViBEVisualizationToolkit::IVisualizationTree
	{
	public:
		CVisualizationTree(const OpenViBE::Kernel::IKernelContext& kernelContext);
		virtual ~CVisualizationTree(void);

		bool init(const OpenViBE::Kernel::IScenario* scenario);

		bool getNextVisualizationWidgetIdentifier(OpenViBE::CIdentifier& identifier) const;
		bool getNextVisualizationWidgetIdentifier(OpenViBE::CIdentifier& identifier, OpenViBEVisualizationToolkit::EVisualizationWidgetType type) const;
		bool isVisualizationWidget(const OpenViBE::CIdentifier& identifier) const;
		OpenViBEVisualizationToolkit::IVisualizationWidget* getVisualizationWidget(const OpenViBE::CIdentifier& identifier) const;
		OpenViBEVisualizationToolkit::IVisualizationWidget* getVisualizationWidgetFromBoxIdentifier(const OpenViBE::CIdentifier& boxIdentifier) const;
		bool addVisualizationWidget(
		        OpenViBE::CIdentifier& identifier,
		        const OpenViBE::CString& name,
		        OpenViBEVisualizationToolkit::EVisualizationWidgetType type,
		        const OpenViBE::CIdentifier& parentIdentifier,
		        OpenViBE::uint32 parentIndex,
		        const OpenViBE::CIdentifier& boxIdentifier,
		        OpenViBE::uint32 childCount,
		        const OpenViBE::CIdentifier& suggestedIdentifier);
		bool getVisualizationWidgetIndex(const OpenViBE::CIdentifier& identifier, OpenViBE::uint32& index) const;
		bool unparentVisualizationWidget(const OpenViBE::CIdentifier& identifier, OpenViBE::uint32& index);
		bool parentVisualizationWidget(const OpenViBE::CIdentifier& identifier, const OpenViBE::CIdentifier& parentIdentifier, OpenViBE::uint32 index);
		bool destroyHierarchy(const OpenViBE::CIdentifier& identifier, bool destroyVisualizationBoxes);

		::GtkTreeView* createTreeViewWithModel(void);
		bool setTreeViewCB(OpenViBEVisualizationToolkit::ITreeViewCB* treeViewCB);

		bool reloadTree(void);

		bool getTreeSelection(::GtkTreeView* preeView, ::GtkTreeIter* iter);
		::GtkTreePath* getTreePath(::GtkTreeIter* treeIter) const;
		unsigned long getULongValueFromTreeIter(::GtkTreeIter* treeIter, OpenViBEVisualizationToolkit::EVisualizationTreeColumn visualizationTreeColumn) const;
		bool getStringValueFromTreeIter(::GtkTreeIter* treeIter, char*& string, OpenViBEVisualizationToolkit::EVisualizationTreeColumn visualizationTreeColumn) const;
		bool getPointerValueFromTreeIter(::GtkTreeIter* treeIter, void*& pointer, OpenViBEVisualizationToolkit::EVisualizationTreeColumn visualizationTreeColumn) const;
		bool getIdentifierFromTreeIter(::GtkTreeIter* iter, OpenViBE::CIdentifier& identifier, OpenViBEVisualizationToolkit::EVisualizationTreeColumn visualizationTreeColumn) const;

		bool findChildNodeFromRoot(::GtkTreeIter* iter, const char* label, OpenViBEVisualizationToolkit::EVisualizationTreeNode type);
		bool findChildNodeFromParent(::GtkTreeIter* iter, const char* label, OpenViBEVisualizationToolkit::EVisualizationTreeNode type);
		bool findChildNodeFromRoot(::GtkTreeIter* iter, void* widget);
		bool findChildNodeFromParent(::GtkTreeIter* iter, void* widget);
		bool findChildNodeFromRoot(::GtkTreeIter* iter, OpenViBE::CIdentifier identifier);
		bool findChildNodeFromParent(::GtkTreeIter* iter, OpenViBE::CIdentifier identifier);
		bool findParentNode(::GtkTreeIter* iter, OpenViBEVisualizationToolkit::EVisualizationTreeNode type);

		bool dragDataReceivedInWidgetCB(const OpenViBE::CIdentifier& sourceWidgetIdentifier, ::GtkWidget* destinationWidget);
		bool dragDataReceivedOutsideWidgetCB(const OpenViBE::CIdentifier& sourceWidgetIdentifier, ::GtkWidget* destinationWidget, OpenViBEVisualizationToolkit::EDragDataLocation location);

		bool setToolbar(const OpenViBE::CIdentifier& boxIdentifier, ::GtkWidget* toolbarWidget);
		bool setWidget(const OpenViBE::CIdentifier& boxIdentifier, ::GtkWidget* topmostWidget);

		OpenViBE::CString serialize() const;
		bool deserialize(const OpenViBE::CString& serializedVisualizationTree);

	private:
		json::Object serializeWidget(OpenViBEVisualizationToolkit::IVisualizationWidget& widget) const;

	private:

		bool _destroyHierarchy(const OpenViBE::CIdentifier& identifier, bool destroyVisualizationBoxes);
		OpenViBE::CIdentifier getUnusedIdentifier(const OpenViBE::CIdentifier& suggestedIdentifier) const;

		bool _findChildNodeFromParent(::GtkTreeIter* iter, const char* label, OpenViBEVisualizationToolkit::EVisualizationTreeNode type);
		bool _findChildNodeFromParent(::GtkTreeIter* iter, void* widget);
		bool _findChildNodeFromParent(::GtkTreeIter* iter, OpenViBE::CIdentifier identifier);

		bool loadVisualizationWidget(OpenViBEVisualizationToolkit::IVisualizationWidget* visualizationWidget, ::GtkTreeIter* parentIter);

	private:

		std::map<OpenViBE::CIdentifier, OpenViBEVisualizationToolkit::IVisualizationWidget*> m_VisualizationWidgets;
		OpenViBE::CIdentifier m_ScenarioIdentifier;
		const OpenViBE::Kernel::IKernelContext& m_KernelContext;
		const OpenViBE::Kernel::IScenario* m_Scenario;
		::GtkTreeStore* m_TreeStore;
		::GtkTreeIter m_InternalTreeNode;
		OpenViBEVisualizationToolkit::ITreeViewCB* m_TreeViewCB;
	};
}

