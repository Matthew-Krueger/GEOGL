/*******************************************************************************
 * Copyright (c) 2020 Matthew Krueger                                          *
 *                                                                             *
 * This software is provided 'as-is', without any express or implied           *
 * warranty. In no event will the authors be held liable for any damages       *
 * arising from the use of this software.                                      *
 *                                                                             *
 * Permission is granted to anyone to use this software for any purpose,       *
 * including commercial applications, and to alter it and redistribute it      *
 * freely, subject to the following restrictions:                              *
 *                                                                             *
 * 1. The origin of this software must not be misrepresented; you must not     *
 *    claim that you wrote the original software. If you use this software     *
 *    in a product, an acknowledgment in the product documentation would       *
 *    be appreciated but is not required.                                      *
 *                                                                             *
 * 2. Altered source versions must be plainly marked as such, and must not     *
 *    be misrepresented as being the original software.                        *
 *                                                                             *
 * 3. This notice may not be removed or altered from any source                *
 *    distribution.                                                            *
 *                                                                             *
 *******************************************************************************/

#ifndef GEOGL_RENDERER2D_HPP
#define GEOGL_RENDERER2D_HPP

#include "Camera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace GEOGL {

	class Renderer2D {
	public:

	    /**
	     * Defines the properties of a Quad to be rendered by the Renderer2D.
	     */
	    struct QuadProperties{

	        inline QuadProperties(  const glm::vec3& quadPosition = {0,0,0},
                                    const glm::vec2& quadSize = {1,1},
                                    const glm::vec4& quadColorTint = {1,1,1,1},
                                    float quadTilingFactor = 1)
                               : position(quadPosition), size(quadSize), colorTint(quadColorTint), tilingFactor(quadTilingFactor){};

	        glm::vec3 position      = {0,0,0};
	        glm::vec2 size          = {1,1};
	        glm::vec4 colorTint     = {1,1,1,1};
	        float tilingFactor      = 1;
	    };

	public:
	    static void init();
	    static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
        static void endScene();
        static void flush();

        // primitives rendering
        static void drawQuad(const QuadProperties& properties);
        static void drawQuad(const QuadProperties& properties, const Ref<Texture2D>& texture);

        static void drawRotatedQuad(const QuadProperties& properties, float rotation = 0);
        static void drawRotatedQuad(const QuadProperties& properties, const Ref<Texture2D>& texture, float rotation = 0);

	public:
        struct Statistics{
            uint32_t drawCalls;
            uint32_t quadCount;

            inline uint32_t getTotalVertexCount() {return quadCount*4; };
            inline uint32_t getTotalIndexCount() { return quadCount*6; };
        };


        static void resetStats();
        static Statistics getStatistics();


    };

}

#endif //GEOGL_RENDERER2D_HPP