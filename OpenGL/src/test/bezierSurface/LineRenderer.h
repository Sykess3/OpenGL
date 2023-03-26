#pragma once
#include <array>
#include "Renderer.h"
#include <memory>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

class LineRenderer {
public:
    static void Draw(glm::vec3 start, glm::vec3 end) {
        std::array<float, 6>vertices = {
             start.x, start.y, start.z,
             end.x, end.y, end.z,

        };
        auto vao= std::make_unique<VertexArray>();
        auto vbo = std::make_unique<VertexBuffer>(vertices.data(), sizeof(float) * 6);
        VertexBufferLayout layout;
        layout.Push<float>(3);
        vao->AddBuffer(*vbo, layout);

        vao->Bind();
        vbo->Bind();

        GlCall(glDrawArrays(GL_LINES, 0, 2));
    }
};
