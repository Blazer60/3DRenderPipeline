/**
 * @file Primitives.cpp
 * @brief [fill in this secton]
 * Project: RenderPipeline
 * Initial Version: 29/06/2021
 * @author Ryan Purse
 */

#include "Primitives.h"

namespace primitives
{
    PolygonalMesh cube()
    {
        // Positions
        const auto rtb = glm::vec3( 0.5f,  0.5f,  0.5f);
        const auto ltb = glm::vec3(-0.5f,  0.5f,  0.5f);
        const auto ltf = glm::vec3(-0.5f,  0.5f, -0.5f);
        const auto rtf = glm::vec3( 0.5f,  0.5f, -0.5f);
        const auto rbb = glm::vec3( 0.5f, -0.5f,  0.5f);
        const auto lbb = glm::vec3(-0.5f, -0.5f,  0.5f);
        const auto lbf = glm::vec3(-0.5f, -0.5f, -0.5f);
        const auto rbf = glm::vec3( 0.5f, -0.5f, -0.5f);

        // Normals
        const auto topFace      = glm::vec3(0.f, 1.f, 0.f);
        const auto backFace     = glm::vec3(0.f, 0.f, 1.f);
        const auto rightFace    = glm::vec3(1.f, 0.f, 0.f);
        const auto leftFace     = glm::vec3(-1.f, 0.f, 0.f);
        const auto frontFace    = glm::vec3(0.f, 0.f, -1.f);
        const auto bottomFace   = glm::vec3(0.f, -1.f, 0.f);

        // UV Coords.
        const auto bottomLeft   = glm::vec2(0.f, 0.f);
        const auto bottomRight  = glm::vec2(1.f, 0.f);
        const auto topRight     = glm::vec2(1.f, 1.f);
        const auto topLeft      = glm::vec2(0.f, 1.f);

        std::vector<Vertex> vertices {
             Vertex{ ltb, topFace,    bottomRight }, Vertex{ rtb, topFace,    bottomLeft }, Vertex{ rtf, topFace,    topLeft }, Vertex{ ltf, topFace,    topRight },  // Top Face
             Vertex{ rbb, backFace,   bottomRight }, Vertex{ rtb, backFace,   bottomLeft }, Vertex{ ltb, backFace,   topLeft }, Vertex{ lbb, backFace,   topRight },  // Back Face
             Vertex{ rtf, rightFace,  bottomRight }, Vertex{ rtb, rightFace,  bottomLeft }, Vertex{ rbb, rightFace,  topLeft }, Vertex{ rbf, rightFace,  topRight },  // Right Face
             Vertex{ ltf, leftFace,   bottomRight }, Vertex{ lbf, leftFace,   bottomLeft }, Vertex{ lbb, leftFace,   topLeft }, Vertex{ ltb, leftFace,   topRight },  // Left Face
             Vertex{ rbf, frontFace,  bottomRight }, Vertex{ lbf, frontFace,  bottomLeft }, Vertex{ ltf, frontFace,  topLeft }, Vertex{ rtf, frontFace,  topRight },  // Front Face
             Vertex{ lbb, bottomFace, bottomRight }, Vertex{ lbf, bottomFace, bottomLeft }, Vertex{ rbf, bottomFace, topLeft }, Vertex{ rbb, bottomFace, topRight },  // Bottom Face
        };

        std::vector<unsigned int> indices {
              0,  1,  2,  0,  2,  3,  // Top Face
              4,  5,  6,  4,  6,  7,  // Back Face
              8,  9, 10,  8, 10, 11,  // Right Face
             12, 13, 14, 12, 14, 15,  // Left Face
             16, 17, 18, 16, 18, 19,  // Front Face
             20, 21, 22, 20, 22, 23   // Bottom Face
        };

        return { vertices, indices };
    }

    PolygonalMesh inverseCube()
    {
        // Positions
        const auto rtb = glm::vec3( 0.5f,  0.5f,  0.5f);
        const auto ltb = glm::vec3(-0.5f,  0.5f,  0.5f);
        const auto ltf = glm::vec3(-0.5f,  0.5f, -0.5f);
        const auto rtf = glm::vec3( 0.5f,  0.5f, -0.5f);
        const auto rbb = glm::vec3( 0.5f, -0.5f,  0.5f);
        const auto lbb = glm::vec3(-0.5f, -0.5f,  0.5f);
        const auto lbf = glm::vec3(-0.5f, -0.5f, -0.5f);
        const auto rbf = glm::vec3( 0.5f, -0.5f, -0.5f);

        // Normals
        const auto topFace      = glm::vec3(0.f, -1.f, 0.f);
        const auto backFace     = glm::vec3(0.f, 0.f, -1.f);
        const auto rightFace    = glm::vec3(-1.f, 0.f, 0.f);
        const auto leftFace     = glm::vec3(1.f, 0.f, 0.f);
        const auto frontFace    = glm::vec3(0.f, 0.f, 1.f);
        const auto bottomFace   = glm::vec3(0.f, 1.f, 0.f);

        // UV Coords.
        const auto bottomLeft   = glm::vec2(0.f, 0.f);
        const auto bottomRight  = glm::vec2(1.f, 0.f);
        const auto topRight     = glm::vec2(1.f, 1.f);
        const auto topLeft      = glm::vec2(0.f, 1.f);

        std::vector<Vertex> vertices {
                Vertex{ rtb, topFace,    bottomLeft }, Vertex{ ltb, topFace,    bottomRight }, Vertex{ ltf, topFace,    topRight }, Vertex{ rtf, topFace,    topLeft },  // Top Face
                Vertex{ rtb, backFace,   bottomLeft }, Vertex{ rbb, backFace,   bottomRight }, Vertex{ lbb, backFace,   topRight }, Vertex{ ltb, backFace,   topLeft },  // Back Face
                Vertex{ rtb, rightFace,  bottomLeft }, Vertex{ rtf, rightFace,  bottomRight }, Vertex{ rbf, rightFace,  topRight }, Vertex{ rbb, rightFace,  topLeft },  // Right Face
                Vertex{ lbf, leftFace,   bottomLeft }, Vertex{ ltf, leftFace,   bottomRight }, Vertex{ ltb, leftFace,   topRight }, Vertex{ lbb, leftFace,   topLeft },  // Left Face
                Vertex{ lbf, frontFace,  bottomLeft }, Vertex{ rbf, frontFace,  bottomRight }, Vertex{ rtf, frontFace,  topRight }, Vertex{ ltf, frontFace,  topLeft },  // Front Face
                Vertex{ lbf, bottomFace, bottomLeft }, Vertex{ lbb, bottomFace, bottomRight }, Vertex{ rbb, bottomFace, topRight }, Vertex{ rbf, bottomFace, topLeft },  // Bottom Face
        };

        std::vector<unsigned int> indices {
                0,  1,  2,  0,  2,  3,  // Top Face
                4,  5,  6,  4,  6,  7,  // Back Face
                8,  9, 10,  8, 10, 11,  // Right Face
                12, 13, 14, 12, 14, 15,  // Left Face
                16, 17, 18, 16, 18, 19,  // Front Face
                20, 21, 22, 20, 22, 23   // Bottom Face
        };

        return { vertices, indices };
    }

    PolygonalMesh tri()
    {
        std::vector<Vertex> vertices {
            Vertex{ glm::vec3( 0.f,   0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f)},
            Vertex{ glm::vec3( 0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f)},
            Vertex{ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f)}
        };

        std::vector<unsigned int> indices {
            0, 2, 1
        };

        return { vertices, indices };
    }
}


