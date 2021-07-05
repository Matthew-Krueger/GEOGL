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


#ifndef GEOGL_CAMERACONTROLLER_HPP
#define GEOGL_CAMERACONTROLLER_HPP

#include "../Rendering/Camera.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/MouseEvent.hpp"

namespace GEOGL{

    class GEOGL_API OrthographicCameraController{
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);
        OrthographicCameraController(const glm::ivec2& windowDimensions, bool rotation = false);

        inline const OrthographicCamera& getCamera() const { return m_OrthographicCamera; };

        void onUpdate(TimeStep ts);
        void onEvent(Event& e);

    private:
        bool onMouseScrolled(MouseScrolledEvent& e);
        bool onWindowResize(WindowResizeEvent& e);
    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_OrthographicCamera;

        bool m_RotationEnabled;
        float m_CameraRotation = 0.0f;
        glm::vec3 m_CameraPosition = {0.0f,0.0f,0.0f};

        float m_CameraRotationSpeed = 180.0f;

        /* Translation Controls */
        float m_CameraTranslationSpeed = 2.0f;
        float m_CameraTranslationFadeFactor = 0.35f;
        float m_CameraTranslationBias = 1.0f;

        /* Zoom Controls */
        float m_ZoomFactor = 0.5f;
        float m_ZoomFadeFactor = 0.5;
        float m_ZoomMin = 0.15;
        float m_ZoomMax = 250;

    };

}

#endif //GEOGL_CAMERACONTROLLER_HPP
