#include "RenderDataBuilder.h"

namespace test
{
	RenderDataBuilder::RenderDataBuilder(int capacity)
	{
		for (int i = 0; i < capacity; i++)
		{
			m_Indexes.emplace_back();
			m_Vertexes.emplace_back();
		}
	}

	void RenderDataBuilder::Add(const Vertex& vertex, int surface)
	{
		//auto iterator = std::find(m_Vertexes.begin(), m_Vertexes.end(), vertex);

		//int newIndexToIndexBuffer;
		//if (iterator == m_Vertexes.end())
		//{
		//	newIndexToIndexBuffer = m_Vertexes.size();
		//	m_Vertexes.push_back(vertex);
		//}
		//else
		//{
		//	newIndexToIndexBuffer = std::distance(m_Vertexes.begin(), iterator);
		//}

		//m_Indexes.push_back(newIndexToIndexBuffer);
		auto& indexes = m_Indexes[surface];
		auto& vertexes = m_Vertexes[surface];
		indexes.push_back(vertexes.size());
		vertexes.push_back(vertex);
	}


	void RenderDataBuilder::Clear()
	{
		m_Vertexes.clear();
		m_Indexes.clear();
	}

	std::unique_ptr<RenderData> RenderDataBuilder::Get(
		const VertexBufferLayout& layout)
	{
		auto vao = std::make_unique<VertexArray>();
		std::vector<Vertex>& vertices = m_Vertexes[0];

		auto vertex = vertices.data();
		size_t i1 = vertices.size() * sizeof(Vertex);

		auto vbo = std::make_unique<VertexBuffer>(vertex, i1);
		vao->AddBuffer(*vbo, layout);

		std::vector<int>& m_index = m_Indexes[0];
		int* data = m_index.data();
		size_t size = m_index.size();

		auto ibo = std::make_unique<IndexBuffer>(data, size);
		std::unique_ptr<RenderData> result = std::make_unique<RenderData>(std::move(ibo), std::move(vao), std::move(vbo));

		return result;
	}
}
