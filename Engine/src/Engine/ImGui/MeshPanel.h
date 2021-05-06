#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Mesh.h"

namespace Engine {

	class MeshPanel {
	public:
		MeshPanel() = default;
		~MeshPanel() = default;

		void OnImGui();

	private:
		bool m_Active = true;

		Ref<Mesh> m_SelectionContext;

	private:
		void DrawMeshNode(Ref<Mesh>& mesh);
		void DrawMesh(Ref<Mesh>& mesh);

		void Save();
	};

}