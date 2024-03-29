#pragma once
#include <tuple>
#include <memory>
#include <vector>


#include "DataStructs.h"


namespace test
{
	class RenderDataBuilder
	{
	public:
		explicit RenderDataBuilder(int capacity);
		void Add(const Vertex& vertex, int surface);
		void Clear();

		std::vector<std::shared_ptr<RenderData>> Get(const VertexBufferLayout& layout);
	private:
		std::vector<std::vector<int>> m_Indexes;
		std::vector<std::vector<Vertex>> m_Vertexes;
	};

}
