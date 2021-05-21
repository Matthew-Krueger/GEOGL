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

    Texture2D::Texture2D(uint32_t width, uint32_t height) :
        m_Width(width), m_Height(height), m_Path("No Path, Loaded by Data"){
        GEOGL_PROFILE_FUNCTION();

        m_InternalFormat = GL_RGBA8;
        m_Format= GL_RGBA;

        {
            GEOGL_PROFILE_SCOPE("Create Texture and set Format, width, and height");
            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, m_InternalFormat, (GLsizei) m_Width, (GLsizei) m_Height);
        }

        {
            GEOGL_PROFILE_SCOPE("Set Texture parameters");
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

    }

    Texture2D::Texture2D(std::string filePath)
    : m_Path(std::move(filePath)){
        GEOGL_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_uc *data = nullptr;
        {
            GEOGL_PROFILE_SCOPE("Load image from file");
            stbi_set_flip_vertically_on_load(1);
            data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
            GEOGL_CORE_ASSERT_NOSTRIP(data, "Failed to load image {}", m_Path);
            m_Width = width;
            m_Height = height;
        }

        m_InternalFormat = GL_INVALID_ENUM;
        m_Format = GL_INVALID_ENUM;
        switch(channels){
            case 3:
                m_InternalFormat = GL_RGB8;
                m_Format = GL_RGB;
                break;
            case 4:
                m_InternalFormat = GL_RGBA8;
                m_Format = GL_RGBA;
                break;
            default:
                m_InternalFormat = GL_INVALID_ENUM;
                m_Format = GL_INVALID_ENUM;
                GEOGL_CORE_ERROR_NOSTRIP("Loading a format that is not supported!");
                break;
        }

        {
            GEOGL_PROFILE_SCOPE("Create Texture and set Format, width, and height");
            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureStorage2D(m_RendererID, 1, m_InternalFormat, (GLsizei) m_Width, (GLsizei) m_Height);
        }

        {
            GEOGL_PROFILE_SCOPE("Set Texture parameters");
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        {
            GEOGL_PROFILE_SCOPE("Substitute Texture into GPU Memory");
            glTextureSubImage2D(m_RendererID, 0, 0, 0, (GLsizei) m_Width, (GLsizei) m_Height, m_Format,
                                GL_UNSIGNED_BYTE, (void *) data);
        }

        {
            GEOGL_PROFILE_SCOPE("Free the image from memory");
            stbi_image_free(data);
        }
    }


    Texture2D::~Texture2D() {
        GEOGL_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);

    }

    void Texture2D::setData(void *data, uint32_t size) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        uint32_t bpp = m_Format == GL_RGBA ? 4 : 3;
        GEOGL_CORE_ASSERT(size == m_Width * m_Height * bpp, "The size of the data must be the entire texture.");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, (GLsizei) m_Width, (GLsizei) m_Height, m_Format, GL_UNSIGNED_BYTE, (void*) data);

    }

    void Texture2D::bind(uint32_t slotID) const {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glBindTextureUnit(slotID, m_RendererID);

    }


}