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


#include "Loader.hpp"
#include "../ModelComponents/RawModel.hpp"


namespace GEOGL{


    std::shared_ptr<GEOGL::RawModel> Loader::loadToVAO(const std::vector<float>& positions) {

        GLuint vaoID = createVAO();

        std::vector<GLuint> vbos;

        vbos.push_back(storeDataInAttributeList(0, positions, 3));

        unbindVAO();

        return std::make_shared<GEOGL::RawModel>(vaoID, vbos, positions.size());

    }

    GLuint Loader::createVAO() {
        GLuint result;
        glGenVertexArrays(1, &result);
        glBindVertexArray(result);
        return result;
    }

    GLuint Loader::storeDataInAttributeList(int attributeNumber, const std::vector<float>& data, int sizePerVertex) {

        GLuint vboID;
        glGenBuffers(1, &vboID);

        glBindBuffer(GL_ARRAY_BUFFER, vboID);

        glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(attributeNumber, sizePerVertex, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(attributeNumber);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vboID;

    }

    void Loader::unbindVAO() {
        glBindVertexArray(0);
    }

    void Loader::deleteVAO(GLuint vaoID) {

        glDeleteVertexArrays(1, &vaoID);

    }

    void Loader::deleteVBO(std::vector<GLuint>& vbos) {

        glDeleteBuffers(vbos.size(), vbos.data());

        vbos.clear();

    }
}