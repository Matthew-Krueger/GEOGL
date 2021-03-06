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


#ifndef GEOGL_OPENGLFRAMEBUFFER_HPP
#define GEOGL_OPENGLFRAMEBUFFER_HPP

#include "../../../GEOGL/Rendering/Framebuffer.hpp"

namespace GEOGL::Platform::OpenGL{

    class GEOGL_API Framebuffer : public GEOGL::Framebuffer{
    public:
        Framebuffer(const FramebufferSpecification& framebufferSpecification);
        virtual ~Framebuffer() override;

        void invalidate();

        void bind() const override;
        void unbind() const override;

        void resize(uint32_t width, uint32_t height) override;

        [[nodiscard]] inline uint32_t getColorAttachmentRendererID() const override {return m_ColorAttachment; };

        [[nodiscard]] const FramebufferSpecification& getFramebufferSpecification() const override;

    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
        FramebufferSpecification m_FramebufferSpecification;

    };

}

#endif //GEOGL_OPENGLFRAMEBUFFER_HPP
