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


#include "GameLayer.hpp"

static const int    s_MapWidth = 24,
                    s_MapHeight = 24;
static const char*  s_MapTiles =
        "WWWWWWWWWWWWWWWWWWWWWWWW"
        "WWWWWWWWWWWWWWWWWWWWWWWW"
        "WWWWWWWWWWWWDDWWWWWWWWWW"
        "WWWWWWWWWDDDDDDDDWWWWWWW"
        "WWWWDDDDDDDDDDDDDDDWWWWW"
        "WWWWWWWDDDDDDDDDDDDDWWWW"
        "WWWWWWWWWWDDDDDDDDDWWWWW"
        "WWWWWWWWDDDDDDDDDDDWWWWW"
        "WWWWWWDDDDDDDDDDDWWWWWWW"
        "WWWWWWWWDDDDDDDDDWWWWWWW"
        "WWWWWWDDDDDDDDDWWWWWWWWW"
        "WWWDDDDDDDDDDDDDDDDWWWWW"
        "WWWWWDDDDDDDDDDDDDDDWWWW"
        "WWWWWWDDDDDDDDDDDDDDWWWW"
        "WWWWWDDDDDDDDDDDDDDDWWWW"
        "WWWWDDDDDDDDDDDDDDDDDWWW"
        "WWWDDDDDDDDDDDDDDDDWWWWW"
        "WWWWDDDDDDDDDDDDDDDWWWWW"
        "WWWWWDDDDDDDDDDDDDDWWWWW"
        "WWWWWWDDDDDDDDDDDDDDWWWW"
        "WWWWWWWDDDDDDDDDDDDWWWWW"
        "WWWWWWWWWWWWDDDWWWWWWWWW"
        "WWWWWWWWWWWWWWWWWWWWWWWW"
        "WWWWWWWWWWWWWWWWWWWWWWWW";


namespace TwoDGame{

    void GameLayer::onAttach() {

        m_OrthographicCameraController = GEOGL::OrthographicCameraController(GEOGL::Application::get().getWindow().getDimensions());
        m_DebugName = "Game Layer - 2D Game Example";

        m_SpriteSheet = GEOGL::Texture2D::create("Example2DGameResources/Textures/Kenny-RPG-Pack/RPGpack_sheet_2X.png");
        m_TextureBarrel = GEOGL::SubTexture2D::createFromCoords(m_SpriteSheet, {9,2}, {128,128});
        m_TextureStairs = GEOGL::SubTexture2D::createFromCoords(m_SpriteSheet, {7,6}, {128,128});
        m_TextureTree = GEOGL::SubTexture2D::createFromCoords(m_SpriteSheet, {2,1}, {128,128}, {1,2});
        m_TextureError = GEOGL::Texture2D::create(1,1);
        uint32_t pinkColor = 0xFF7F00FF;
        m_TextureError->setData(&pinkColor, sizeof(pinkColor));


        m_TextureMap['D'] = GEOGL::SubTexture2D::createFromCoords(m_SpriteSheet, {6,11}, {128,128});
        m_TextureMap['W'] = GEOGL::SubTexture2D::createFromCoords(m_SpriteSheet, {11,11}, {128,128});


        m_OrthographicCameraController.setZoomLevel(5);

    }

    void GameLayer::onDetach() {

    }

    void GameLayer::onUpdate(GEOGL::TimeStep timeStep) {

        GEOGL::Renderer2D::resetStats();

        m_OrthographicCameraController.onUpdate(timeStep);

        GEOGL::Renderer2D::beginScene(m_OrthographicCameraController.getCamera());

        for(uint32_t y=0; y < s_MapHeight; ++y){
            for(uint32_t x=0; x < s_MapWidth; ++x){
                if(m_TextureMap.find(s_MapTiles[x+(y*s_MapWidth)]) != m_TextureMap.end()) {
                    GEOGL::Renderer2D::drawQuad({{(float) x - ((float) s_MapWidth / 2), ((float) s_MapHeight / 2) - (float)y, 0},{1,1}}, m_TextureMap[s_MapTiles[x+(y*s_MapWidth)]]);
                }else{
                    GEOGL::Renderer2D::drawQuad({{(float) x - ((float) s_MapWidth / 2), ((float) s_MapHeight / 2) - (float)y, 0},{1,1}}, m_TextureError);
                }
            }
        }

        GEOGL::Renderer2D::endScene();


    }

    void GameLayer::onImGuiRender(GEOGL::TimeStep timeStep) {


    }

    void GameLayer::onEvent(GEOGL::Event &event) {

        m_OrthographicCameraController.onEvent(event);

    }

}
