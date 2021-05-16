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

	void SceneHierarchy::Duplicate(Entity entity)
	{
		uint32_t id = entity.GetID();
		if (id == 0 || m_Nodes.find(id) == m_Nodes.end())
			return;

		auto* node = m_Nodes[id];

		node->Duplicate(entity.GetParent());
	}

	void SceneHierarchy::Delete(Entity entity)
	{
		uint32_t id = entity.GetID();
		if (id == 0 || m_Nodes.find(id) == m_Nodes.end())
			return;

		uint32_t parentID = entity.GetParent().GetID();

		auto* node = m_Nodes[id];
		m_Nodes.erase(id);

		auto* parentNode = m_Nodes[parentID];
		parentNode->m_Children.erase(entity.ToString());

		node->Delete();
		delete node;
	}

	void SceneHierarchy::Clear()
	{
		delete m_Tree;
		m_Nodes.clear();

		m_Tree = new SceneHierarchyNode(0);
		m_Nodes[0] = m_Tree;
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

	void SceneHierarchyNode::Duplicate(Entity parent)
	{
		using ParentComponent = Component::Core::ParentComponent;

		Entity duplicatedEntity = m_Entity.Copy();

		if (duplicatedEntity.HasComponent<ParentComponent>())
			duplicatedEntity.GetComponent<ParentComponent>().Parent = parent;
		
		for (auto& entry : m_Children)
			entry.second->Duplicate(duplicatedEntity);
	}

	void SceneHierarchyNode::Delete()
	{
		for (auto entry : m_Children)
			entry.second->Delete();

		m_Entity.Destroy();
	}

}