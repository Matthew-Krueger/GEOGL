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

#ifndef GEOGL_GRAPHICSCONTEXT_HPP
#define GEOGL_GRAPHICSCONTEXT_HPP

namespace GEOGL{

    /**
     * \brief Represents a Graphical context interface.
     */
    class GEOGL_API GraphicsContext{
    public:

        GraphicsContext() = default;
        /**
         * \brief Deletes any possibility of a copy constructor. Avoids weirdness.
         * @param context The graphics context
         */
        GraphicsContext(GraphicsContext& context) = delete;
        virtual ~GraphicsContext() = default;

        /**
         * \brief Clears the color of the screen.
         * \todo Maybe move this to Renderer?? I think that may fit slightly better.
         */
        virtual void clearColor() = 0;

        /**
         * \brief Sets the viewport of the window
         *
         * Sets the viewport of the window. Each vector taken is taken as reference. I believe that initializer lists
         * work as well.
         *
         * \note This is NOT a top left and bottom right corner function. It is top left and dimensions FROM THAT POINT.
         *
         * @param topLeftCorner The top left corner of the viewport
         * @param dimensions The dimensions of the viewport
         */
        virtual void setViewport(const glm::ivec2& topLeftCorner, const glm::ivec2& dimensions) = 0;

        /**
         * \brief Sets whether or not to run in vSync mode.
         *
         * \note This function takes a pointer as it will mutate the bool to the value set. This functionality is used
         * to deny the switch to vSync mode in the event that it is not supported for any reason.
         *
         * @param vSyncStatus A pointer to a boolean that contains the vsync set state.
         */
        virtual void setVSync(bool* vSyncStatus) = 0;

        /**
         * \brief Swaps the swapchain buffers to present the image to the screen.
         */
        virtual void swapBuffers() = 0;

    private:

    };

}

#endif //GEOGL_GRAPHICSCONTEXT_HPP
