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



#include "CameraController.hpp"
#include "Input.hpp"

namespace GEOGL{

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio), m_OrthographicCamera({-m_AspectRatio * m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel}), m_RotationEnabled(rotation){

    }

    OrthographicCameraController::OrthographicCameraController(const glm::ivec2& windowDimensions, bool rotation)
    : m_AspectRatio((float)windowDimensions.x / (float)windowDimensions.y), m_OrthographicCamera({-m_AspectRatio * m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel}), m_RotationEnabled(rotation){

    }


    void OrthographicCameraController::onUpdate(TimeStep ts) {


        glm::vec3 deltaPosition(0.0f);

        /* If dpad or q or e is pressed, translate or rotate */
        if (Input::isKeyPressed(Key::A)) {
            deltaPosition.x -= m_CameraTranslationSpeed * ts;
        }
        if (Input::isKeyPressed(Key::D)) {
            deltaPosition.x += m_CameraTranslationSpeed * ts;
        }
        if (Input::isKeyPressed(Key::S)) {
            deltaPosition.y -= m_CameraTranslationSpeed * ts;
        }
        if (Input::isKeyPressed(Key::W)) {
            deltaPosition.y += m_CameraTranslationSpeed * ts;
        }

        if(m_RotationEnabled) {
            if (Input::isKeyPressed(Key::Q)) {
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            }
            if (Input::isKeyPressed(Key::E)) {
                m_CameraRotation += m_CameraRotationSpeed * ts;
            }
            m_OrthographicCamera.setRotationZ(m_CameraRotation);
        }
        /* Double the speed */
        if (Input::isKeyPressed(Key::LeftShift) || GEOGL::Input::isKeyPressed(GEOGL::Key::RightShift)) {
            deltaPosition *= glm::vec3(2.0f);
        }

        /* Now, update the camera position */
        m_CameraPosition += deltaPosition;

        /* if c is pressed, center everything */
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::C)){
            m_CameraPosition = glm::vec3(0.0f);
            m_CameraRotation = 0;
            m_OrthographicCamera.setRotationZ(m_CameraRotation);
            m_ZoomLevel = 1;
            m_OrthographicCamera.setProjection({-m_AspectRatio * m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel});
        }


        /* Now upload it to the camera */
        m_OrthographicCamera.setPosition(m_CameraPosition);

    }

    void OrthographicCameraController::onEvent(Event &e) {

        EventDispatcher dispatcher(e);

        dispatcher.dispatch<MouseScrolledEvent>(GEOGL_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(GEOGL_BIND_EVENT_FN(OrthographicCameraController::onWindowResize));

    }

    bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent &e) {

        m_ZoomLevel -= e.getYOffset() * m_ZoomFactor * (m_ZoomLevel*m_ZoomFadeFactor);

        /* Guard against high and low zoom levels */
        m_ZoomLevel = std::max(m_ZoomLevel, m_ZoomMin);
        m_ZoomLevel = std::min(m_ZoomLevel, m_ZoomMax);

        m_OrthographicCamera.setProjection({-m_AspectRatio * m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel});
        return false;

    }

    bool OrthographicCameraController::onWindowResize(WindowResizeEvent &e) {

        m_AspectRatio = (float)e.getWidth()/(float)e.getHeight();

        m_OrthographicCamera.setProjection({-m_AspectRatio * m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel});
        return false;

    }
}
