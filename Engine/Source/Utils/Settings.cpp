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

#include <fstream>
#include <utility>
#include <iomanip>
#include "Settings.hpp"

namespace GEOGL{


    Settings::Settings() =default;

    Settings::~Settings() {

        flush();

    }

    bool Settings::open(std::string filePath) {

        m_FilePath = std::move(filePath);

        return reload();


    }

    bool Settings::reload() {

        /* Try to load settings file */
        std::ifstream settingsFile(m_FilePath);

        /* Make sure it loaded, otherwise, return false */
        if(!settingsFile)
            return false;

        /* now, load the json */
        settingsFile >> data;

        return !data.empty();

    }

    void Settings::flush() {

        std::ofstream outputFile;
        outputFile.open(m_FilePath, std::ios::out | std::ios::trunc);
        GEOGL_CORE_ASSERT_NOSTRIP(outputFile, "Filed to save settings {}", m_FilePath);
        outputFile << std::setw(4) << data << std::endl;
        outputFile.flush();
        outputFile.close();
        bool reloadStatus = reload();
        GEOGL_CORE_ASSERT_NOSTRIP(reloadStatus, GEOGL_FORMAT("Failed to reload settings after saving, File: {}", m_FilePath));

    }
}
