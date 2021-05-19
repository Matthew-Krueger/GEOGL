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


#include "OpenGLTexture.hpp"
#include <STB/stb_image.h>
#include <utility>
#include <glad/glad.h>

namespace GEOGL::Platform::OpenGL {

    Texture2D::Texture2D(std::string filePath)
    : m_Path(std::move(filePath)){
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
        GEOGL_CORE_ASSERT_NOSTRIP(data, "Failed to load image {}", m_Path);
        m_Width = width;
        m_Height = height;


        GLenum internalFormat = GL_INVALID_ENUM, dataFormat = GL_INVALID_ENUM;
        switch(channels){
            case 3:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case 4:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            default:
                internalFormat = GL_INVALID_ENUM;
                dataFormat = GL_INVALID_ENUM;
                GEOGL_CORE_ERROR_NOSTRIP("Loading a format that is not supported!");
                break;
        }


        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, (GLsizei) m_Width, (GLsizei) m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, (GLsizei) m_Width, (GLsizei) m_Height, dataFormat, GL_UNSIGNED_BYTE, (void*) data);

        stbi_image_free(data);

    }


    Texture2D::~Texture2D() {

        glDeleteTextures(1, &m_RendererID);

    }

    void Texture2D::bind(uint32_t slotID) const {

        glBindTextureUnit(slotID, m_RendererID);

    }
}