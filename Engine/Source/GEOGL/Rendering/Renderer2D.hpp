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

namespace GEOGL {

	class Renderer2D {
	public:
	    static void init();
	    static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
        static void endScene();

        // primitives rendering
        static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	};

}

#endif //GEOGL_RENDERER2D_HPP