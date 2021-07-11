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


#ifndef GEOGL_FRAMEBUFFER_HPP
#define GEOGL_FRAMEBUFFER_HPP

namespace GEOGL{

    struct FramebufferSpecification{
        uint32_t width{}, height{};
        uint32_t samples = 1;

        bool swapChainTarget = false;
    };

    class GEOGL_API Framebuffer{
    public:
        virtual ~Framebuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t getColorAttachmentRendererID() const = 0;

        //virtual FramebufferSpecification& getFramebufferSpecification() = 0;
        [[nodiscard]] virtual const FramebufferSpecification& getFramebufferSpecification() const = 0;

        static Ref<Framebuffer> create(const FramebufferSpecification& framebufferSpecification);

    };

}

#endif //GEOGL_FRAMEBUFFER_HPP
