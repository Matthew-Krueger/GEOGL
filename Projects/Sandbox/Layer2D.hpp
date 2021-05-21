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


#ifndef GEOGL_LAYER2D_HPP
#define GEOGL_LAYER2D_HPP

#include <GEOGL/Layers.hpp>
#include <GEOGL/IO.hpp>
#include <GEOGL/Renderer.hpp>

namespace SandboxApp{

    class Layer2D : public GEOGL::Layer{
    public:
        Layer2D();
        Layer2D(const Layer2D&) = delete;
        ~Layer2D() override = default;


        void onAttach() override;
        void onDetach() override;
        void onUpdate(GEOGL::TimeStep timeStep) override;
        void onImGuiRender(GEOGL::TimeStep timeStep) override;

        void onEvent(GEOGL::Event& event) override;

    private:

        /* Camera utils */
        GEOGL::OrthographicCameraController m_OrthographicCameraController;

        /* Textures */
        /* Cherno Logo (Copyright the Cherno) */
        GEOGL::Ref<GEOGL::Texture2D> m_ChernoLogo;
        GEOGL::Ref<GEOGL::Texture2D> m_Checkerboard;
        GEOGL::Ref<GEOGL::Texture2D> m_Sandman;

        /* Color Controllers */
        glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};

    };

}

#endif //GEOGL_LAYER2D_HPP
