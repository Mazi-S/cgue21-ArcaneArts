#pragma once
#include "Engine/Scene/Entity.h"

namespace Engine {

	class SceneHierarchyNode
	{
		friend class SceneHierarchy;

	public:
		SceneHierarchyNode(uint32_t id = 0, Entity entity = Entity())
			: m_ID(id), m_Entity(entity) { }

		~SceneHierarchyNode();

		bool operator<(SceneHierarchyNode other) const { return m_ID < other.m_ID; }

	private:
		uint32_t m_ID;
		Entity m_Entity;
		std::map<std::string, SceneHierarchyNode*> m_Children;
	};

	class SceneHierarchy
	{
	public:
		SceneHierarchy();
		~SceneHierarchy();

		void Add(Entity entity);

		void Draw(Entity& selection);

	private:
		void Draw(SceneHierarchyNode* node, Entity& selection);

		SceneHierarchyNode* m_Tree;
		std::map<uint32_t, SceneHierarchyNode*> m_Nodes;

	};

}