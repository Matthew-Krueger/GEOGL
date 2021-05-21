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

#include "Camera.hpp"
#include "../Application/Application.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace GEOGL{



    OrthographicCamera::OrthographicCamera() {
        GEOGL_PROFILE_FUNCTION();

        auto orthographicBounds = calculateBestOrthographicBounds(Application::get().getWindow().getDimensions());

        m_Projectionmatrix = glm::ortho(
                orthographicBounds.left,
                orthographicBounds.right,
                orthographicBounds.bottom,
                orthographicBounds.top,
                orthographicBounds.zNearClipPlane,
                orthographicBounds.zFarClipPlane
        );
        m_ViewMatrix = glm::mat4(1.0f);

        m_OrthographicBounds = orthographicBounds;
        m_ProjectionViewMatrix = m_Projectionmatrix * m_ViewMatrix;

    }

    OrthographicCamera::OrthographicCamera(const ProjectionBounds& bounds)
        : m_Projectionmatrix(
                glm::ortho(
                        bounds.left,
                        bounds.right,
                        bounds.bottom,
                        bounds.top,
                        bounds.zNearClipPlane,
                        bounds.zFarClipPlane
                )),
        m_ViewMatrix(1.0f){
        GEOGL_PROFILE_FUNCTION();

        m_OrthographicBounds = bounds;
        m_ProjectionViewMatrix = m_Projectionmatrix * m_ViewMatrix;

    }

    void OrthographicCamera::setProjection(const ProjectionBounds& orthographicBounds) {
        GEOGL_PROFILE_FUNCTION();

        m_OrthographicBounds = orthographicBounds;
        m_Projectionmatrix = glm::ortho(m_OrthographicBounds.left,
                                        m_OrthographicBounds.right,
                                        m_OrthographicBounds.bottom,
                                        m_OrthographicBounds.top,
                                        m_OrthographicBounds.zNearClipPlane,
                                        m_OrthographicBounds.zFarClipPlane);

        recalculateViewMatrix();

    }



    OrthographicCamera::ProjectionBounds OrthographicCamera::calculateBestOrthographicBounds(const glm::ivec2 &windowDimensions, const glm::vec2& clipPlanes) {
        GEOGL_PROFILE_FUNCTION();

        ProjectionBounds newBounds;

        float aspectRatio = (float)windowDimensions.x/(float)windowDimensions.y;

        newBounds.left = -aspectRatio;
        newBounds.right = aspectRatio;
        newBounds.bottom = -1;
        newBounds.top = 1;
        newBounds.zNearClipPlane = -1;
        newBounds.zFarClipPlane = 1;

        return newBounds;

    }

    void OrthographicCamera::recalculateViewMatrix() {
        GEOGL_PROFILE_FUNCTION();

        /* Translate the matrix by position */
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
        /* Now, rotate the matrix around the z axis */
        transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationZ), glm::vec3(0,0,1));

        /* now, set view matrix from the inverse of transform */
        m_ViewMatrix = glm::inverse(transform);

        /* calculate view projection matrix from these */
        m_ProjectionViewMatrix = m_Projectionmatrix * m_ViewMatrix;

    }

}