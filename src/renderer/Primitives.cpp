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

        // Tangents, bi-tangents and texturesIds are not set.
        std::vector<Vertex> vertices {
             Vertex{ ltb, bottomRight, topFace,     }, Vertex{ rtb, bottomLeft, topFace,     }, Vertex{ rtf, topLeft, topFace,     }, Vertex{ ltf, topRight, topFace,     },  // Top Face
             Vertex{ rbb, bottomRight, backFace,    }, Vertex{ rtb, bottomLeft, backFace,    }, Vertex{ ltb, topLeft, backFace,    }, Vertex{ lbb, topRight, backFace,    },  // Back Face
             Vertex{ rtf, bottomRight, rightFace,   }, Vertex{ rtb, bottomLeft, rightFace,   }, Vertex{ rbb, topLeft, rightFace,   }, Vertex{ rbf, topRight, rightFace,   },  // Right Face
             Vertex{ ltf, bottomRight, leftFace,    }, Vertex{ lbf, bottomLeft, leftFace,    }, Vertex{ lbb, topLeft, leftFace,    }, Vertex{ ltb, topRight, leftFace,    },  // Left Face
             Vertex{ rbf, bottomRight, frontFace,   }, Vertex{ lbf, bottomLeft, frontFace,   }, Vertex{ ltf, topLeft, frontFace,   }, Vertex{ rtf, topRight, frontFace,   },  // Front Face
             Vertex{ lbb, bottomRight, bottomFace,  }, Vertex{ lbf, bottomLeft, bottomFace,  }, Vertex{ rbf, topLeft, bottomFace,  }, Vertex{ rbb, topRight, bottomFace,  },  // Bottom Face
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
                Vertex{ rtb, bottomLeft, topFace,     }, Vertex{ ltb, bottomRight, topFace,     }, Vertex{ ltf, topRight, topFace,     }, Vertex{ rtf, topLeft, topFace,     },  // Top Face
                Vertex{ rtb, bottomLeft, backFace,    }, Vertex{ rbb, bottomRight, backFace,    }, Vertex{ lbb, topRight, backFace,    }, Vertex{ ltb, topLeft, backFace,    },  // Back Face
                Vertex{ rtb, bottomLeft, rightFace,   }, Vertex{ rtf, bottomRight, rightFace,   }, Vertex{ rbf, topRight, rightFace,   }, Vertex{ rbb, topLeft, rightFace,   },  // Right Face
                Vertex{ lbf, bottomLeft, leftFace,    }, Vertex{ ltf, bottomRight, leftFace,    }, Vertex{ ltb, topRight, leftFace,    }, Vertex{ lbb, topLeft, leftFace,    },  // Left Face
                Vertex{ lbf, bottomLeft, frontFace,   }, Vertex{ rbf, bottomRight, frontFace,   }, Vertex{ rtf, topRight, frontFace,   }, Vertex{ ltf, topLeft, frontFace,   },  // Front Face
                Vertex{ lbf, bottomLeft, bottomFace,  }, Vertex{ lbb, bottomRight, bottomFace,  }, Vertex{ rbb, topRight, bottomFace,  }, Vertex{ rbf, topLeft, bottomFace,  },  // Bottom Face
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


