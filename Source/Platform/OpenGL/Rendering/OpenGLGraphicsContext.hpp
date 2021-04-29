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

#ifndef GEOGL_OPENGLGRAPHICSCONTEXT_HPP
#define GEOGL_OPENGLGRAPHICSCONTEXT_HPP

#include "../../../GEOGL/Rendering/GraphicsContext.hpp"

struct GLFWwindow;

namespace GEOGL::Platform::OpenGL{

    /**
     * \brief Implements the Graphics context for OpenGL
     */
    class GEOGL_API GraphicsContext : public GEOGL::GraphicsContext{
    public:
        GraphicsContext(GLFWwindow* windowHandle);
        ~GraphicsContext();

        void clearColor() override;

        void setViewport(const glm::vec2& topLeftCorner, const glm::vec2& dimensions) override;
        void setVSync(bool* vSyncStatus) override;
        void swapBuffers() override;

        void deInitGlad();

    private:
        GLFWwindow* m_WindowHandle;

    };

}

#endif //GEOGL_OPENGLGRAPHICSCONTEXT_HPP
