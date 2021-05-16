#include "egpch.h"
#include "SceneHierarchy.h"

#include "Engine/Scene/Components.h"

#include <imgui.h>
#include "ImGuiUtil.h"

namespace Engine {

	SceneHierarchy::SceneHierarchy()
		: m_Tree(new SceneHierarchyNode(0))
	{
		m_Nodes[0] = m_Tree;
	}

	SceneHierarchy::~SceneHierarchy()
	{
		delete m_Tree;
	}

	void SceneHierarchy::Add(Entity entity)
	{
		uint32_t id = entity.GetID();
		if (m_Nodes.find(id) != m_Nodes.end())
			return;

		SceneHierarchyNode* node = new SceneHierarchyNode(id, entity);
		m_Nodes[id] = node;

		const auto& parent = entity.GetParent();
		Add(parent);
		m_Nodes[parent.GetID()]->m_Children[entity.ToString()] = node;
	}

	void SceneHierarchy::Draw(Entity& selection)
	{
		for (auto& entry : m_Tree->m_Children)
			Draw(entry.second, selection);
	}

	void SceneHierarchy::Draw(SceneHierarchyNode* node, Entity& selection)
	{
		Entity entity = node->m_Entity;

		auto& tag = entity.GetComponent<Engine::Component::Core::TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (selection == entity) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity.GetID(), flags, tag.c_str());
		if (ImGui::IsItemClicked())
			selection = selection != entity ? entity : Entity();

		if (opened)
		{
			for (auto& entry : node->m_Children)
				Draw(entry.second, selection);

			ImGui::TreePop();
		}
	}

	SceneHierarchyNode::~SceneHierarchyNode()
	{
		for (auto entry : m_Children)
			delete entry.second;
	}

}