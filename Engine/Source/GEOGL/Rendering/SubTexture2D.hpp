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


#ifndef GEOGL_SUBTEXTURE2D_HPP
#define GEOGL_SUBTEXTURE2D_HPP

#include "Texture.hpp"

namespace GEOGL{

    class GEOGL_API SubTexture2D{
    public:
        SubTexture2D(const Ref<Texture2D>& textureAtlas, const glm::vec2& minBound, const glm::vec2& maxBound);
        virtual ~SubTexture2D() = default;

        [[nodiscard]] const Ref<Texture2D>& getTexture() const { return m_Texture; };
        [[nodiscard]] const glm::vec2* getTextureCoords() const {return m_TextureCoords; };

        static Ref<SubTexture2D> createFromCoords(const Ref <Texture2D>& textureAtlas, const glm::vec2& spritePosition, const glm::vec2& cellSize, const glm::vec2& spriteDimensions = {1,1});

    private:
        Ref<Texture2D> m_Texture;

        glm::vec2 m_TextureCoords[4]{};
    };

}

#endif //GEOGL_SUBTEXTURE2D_HPP
