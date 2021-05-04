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

#include "../../GEOGL/Rendering/Texture.hpp"

#ifndef GEOGL_OPENGLTEXTURE_HPP
#define GEOGL_OPENGLTEXTURE_HPP

namespace GEOGL::Platform::OpenGL{

    class Texture2D : public GEOGL::Texture2D{
    public:
        explicit Texture2D(std::string  filePath);
        ~Texture2D();

        [[nodiscard]] inline uint32_t getWidth() const override { return m_Width; };
        [[nodiscard]] inline uint32_t getHeight() const override { return m_Height; };

        /**
         * Binds the texture for rendering
         */
        void bind(uint32_t slotID) const override;

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
    };

}

#endif //GEOGL_OPENGLTEXTURE_HPP
