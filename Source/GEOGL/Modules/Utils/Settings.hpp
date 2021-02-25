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

#ifndef GEOGL_SETTINGS_HPP
#define GEOGL_SETTINGS_HPP

#include "../../Rendering/RendererAPI.hpp"

namespace GEOGL{

    /**
     * \brief An API for reading settings from JSON from disk
     */
    class GEOGL_API Settings{
    public:

        /**
         * \brief Constructs a blank settings object. Must call open to load a file.
         */
        Settings();
        ~Settings();

        inline RendererAPI::RenderingAPI getRenderingAPI() const { return static_cast<RendererAPI::RenderingAPI>(data["RenderingAPI"]); };
        inline void setRenderingAPI(RendererAPI::RenderingAPI api) { data["RenderingAPI"] = static_cast<uint32_t>(api); };

        /**
         * Opens the specified file and loads it into data.
         * @param filePath
         * @return TRUE if reading succeeded, FALSE otherwise.
         */
        bool open(std::string filePath);

        /**
         * Reloads the json from the disk. Useful for live patches.
         * @return If it successfully reloaded.
         */
        bool reload();

        /**
         * Flushes the current json to the disk.
         */
        void flush();

    public:
        /**
         * \brief The settings data
         */
        json data;

    private:
        /**
         * The filePath to the setting file.
         */
        std::string m_FilePath;

    };

}

#endif //GEOGL_SETTINGS_HPP
