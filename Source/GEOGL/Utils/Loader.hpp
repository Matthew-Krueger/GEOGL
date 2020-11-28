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


#ifndef GEOGL_LOADER_HPP
#define GEOGL_LOADER_HPP

namespace GEOGL{

    class GEOGL_API Loader{

    public:
        /**
         * Loads a model into a RawModel
         * @param positions The positions to load
         * @return The shared_ptr to the new RawModel
         */
        static std::shared_ptr<GEOGL::RawModel> loadToVAO(const std::vector<float>& positions);

        /**
         * Deletes a VAO
         * @param vaoID The VAO to delete
         */
        static void deleteVAO(GLuint vaoID);

        /**
         * Deletes a VBO vector
         * @param vbos The vbos to delete
         */
        static void deleteVBO(std::vector<GLuint>& vbos);


    private:

        /**
         * Creates a vao
         * @return The VAOid
         */
        static GLuint createVAO();

        /**
         * Stores Float data in an attribute list
         * @param attributeNumber The attribute number to store it in
         * @param data the data to store
         * @return The VBO id
         */
        static GLuint storeDataInAttributeList(int attributeNumber, const std::vector<float> &data, int sizePerVertex);

        /**
         * Unbinds the current VAO
         */
        static void unbindVAO();

    };

}

#endif //GEOGL_LOADER_HPP
