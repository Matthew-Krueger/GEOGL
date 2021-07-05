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


#ifndef GEOGL_CAMERA_HPP
#define GEOGL_CAMERA_HPP


namespace GEOGL{

    class GEOGL_API OrthographicCamera{
    public:

        /**
         * Represents the bounds of an orthographic camera
         */
        struct ProjectionBounds{
            float left;
            float right;
            float bottom;
            float top;
            float zNearClipPlane = -1.0;
            float zFarClipPlane = 1.0;

            inline float getWidth() const  { return right-left; };
            inline float getHeight() const { return top-bottom; };

        };

    public:
        /**
         * \brief Creates a new OrthographicCamera based on the information stored in the window.
         *
         * \note This is the preferred way to create an OrthographicCamera, as any update done using
         * OrthographicCamera::calculateBestOrthographicBounds() will be consistant with this. However, you may still
         * explicitly initialize an OrthographicCamera using the OrthographicCamera(OrthographicBounds&) Initialization
         *
         */
        OrthographicCamera();

        /**
         * \brief Creates a new Orthographic camera with the selected bounds
         * @param left The left bound
         * @param right The right bound
         * @param bottom The Bottom Bound
         * @param top The Top Bound
         */
        explicit OrthographicCamera(const ProjectionBounds& orthographicBounds);

        const glm::vec3& getPosition() const { return m_Position; };
        inline void setPosition(const glm::vec3& position){ m_Position = position; recalculateViewMatrix(); };

        float getRotationZ() const { return m_RotationZ; };
        inline void setRotationZ(float zRotation){ if(zRotation>360) { m_RotationZ = zRotation-360; }else if (zRotation < 0){ m_RotationZ = zRotation+360; }else{m_RotationZ = zRotation; } recalculateViewMatrix(); };

        inline const glm::mat4& getProjectionMatrix() const { return m_Projectionmatrix; };
        inline const glm::mat4& getViewMatrix() const { return m_ViewMatrix; };
        inline const glm::mat4& getProjectionViewMatrix() const { return m_ProjectionViewMatrix; };

        inline const ProjectionBounds& getProjectionBounds() const {return m_OrthographicBounds; };
        void setProjectionBounds(const ProjectionBounds& projectionBounds);

        static ProjectionBounds calculateBestOrthographicBounds(const glm::ivec2& windowDimensions, const glm::vec2& clipPlanes = {-1.0f, 1.0f});

    private:
        void recalculateViewMatrix();
    private:
        ProjectionBounds m_OrthographicBounds;

        glm::mat4 m_Projectionmatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionViewMatrix;

        glm::vec3 m_Position = {0.0f,0.0f,0.0f};
        float m_RotationZ = 0.0f;
    };

}

#endif //GEOGL_CAMERA_HPP
