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


#include "SubTexture2D.hpp"

namespace GEOGL{


    SubTexture2D::SubTexture2D(const Ref <Texture2D> &textureAtlas, const glm::vec2 &minBound,
                               const glm::vec2 &maxBound) : m_Texture(textureAtlas){

        m_TextureCoords[0] = {minBound.x, minBound.y};
        m_TextureCoords[1] = {maxBound.x, minBound.y};
        m_TextureCoords[2] = {maxBound.x, maxBound.y};
        m_TextureCoords[3] = {minBound.x, maxBound.y};

    }

    Ref <SubTexture2D> SubTexture2D::createFromCoords(const Ref<Texture2D>& textureAtlas, const glm::vec2 &spritePosition, const glm::vec2& cellSize, const glm::vec2& spriteDimensions) {


        glm::vec2 minBounds = {
                (spritePosition.x * cellSize.x)/(float)textureAtlas->getWidth(),
                (spritePosition.y * cellSize.y)/(float)textureAtlas->getHeight()};
        glm::vec2 maxBounds = {
                ((spritePosition.x + spriteDimensions.x) * cellSize.x) / (float)textureAtlas->getWidth(),
                ((spritePosition.y + spriteDimensions.y) * cellSize.y) / (float)textureAtlas->getHeight()};


        return createRef<SubTexture2D>(textureAtlas, minBounds, maxBounds);

    }
}