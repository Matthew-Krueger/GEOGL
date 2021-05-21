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


#include "DebugLayer.hpp"
#include <ImGui/imgui.h>
#include <GEOGL/Core.hpp>

namespace SandboxApp{

    static uint64_t frameCount = 0;
    static float minFPS = std::numeric_limits<float>::max();
    static float maxFPS = 0; // this can be zero as we shouldn't have negative FPS
    static float totalFrameTime = 0;
    static float fps = 0;
    static double averageFPS = 0;
    static glm::ivec2 dimensions;

    void DebugLayer::onAttach() {
        GEOGL_PROFILE_FUNCTION();

        m_DebugName = "Debug Layer";

        frameCount = 0;
        minFPS = std::numeric_limits<float>::max();
        maxFPS = 0;
        totalFrameTime = 0;
        fps = 0;
        averageFPS = -1;
        dimensions = {0,0};

    }

    void DebugLayer::onDetach() {

        GEOGL_INFO("Called ondetach {}", m_DebugName);

    }

    void DebugLayer::onUpdate(GEOGL::TimeStep timeStep) {
        GEOGL_PROFILE_FUNCTION();

        /* update stats */
        totalFrameTime += timeStep;
        fps = 1.0f / (timeStep.getSeconds());
        averageFPS = 1.0/(totalFrameTime/(double)frameCount);
        minFPS = (fps < minFPS) ? fps : minFPS;
        maxFPS = (fps > maxFPS) ? fps : maxFPS;

        dimensions = GEOGL::Application::get().getWindow().getDimensions();

    }

    void DebugLayer::onImGuiRender(GEOGL::TimeStep timeStep) {
        GEOGL_PROFILE_FUNCTION();


        /* Only run the fps min and max afterwords to give time to stabalize */
        if(frameCount++ > 45) {


            ImGui::Begin("Debug Info");
            ImGui::SetWindowFontScale(1.25f);
            ImGui::Text("Window size %d x %d", dimensions.x, dimensions.y);
            ImGui::Text("Aspect Ratio %f", (float)dimensions.x/(float)dimensions.y);
            ImGui::Text("VSync Enabled: %s", (GEOGL::Application::get().getWindow().isVSync()) ? "TRUE" : "FALSE");
            ImGui::Text("FrameTime: %.2f ms", timeStep.getMilliseconds());
            ImGui::Text("FPS: %.2f", fps);
            ImGui::Text("Min FPS: %.2f", minFPS);
            ImGui::Text("Max FPS: %.2f", maxFPS);
            ImGui::Text("Average FPS: %.2f", averageFPS);
            ImGui::Text("Frame Count: %llu", frameCount);
            ImGui::Text("Total Frame Time: %.2f s", totalFrameTime);
            ImGui::Text("Total Memory In Use: %.2f MB", GEOGL::getMegabytesAllocated() - GEOGL::getMegabytesDeallocated());
            ImGui::Text("Total Memory Allocations: %zu", GEOGL::getNumberAllocations());
            ImGui::Text("Total Memory Allocated: %.2f MB", GEOGL::getMegabytesAllocated());
            ImGui::Text("Total Memory Deallocations: %zu", GEOGL::getNumberDeallocations());
            ImGui::Text("Total Memory Deallocated: %.2f MB", GEOGL::getMegabytesDeallocated());

            if(ImGui::Button("Clear FPS Information")){
                fps=0;
                minFPS = std::numeric_limits<float>::max();
                maxFPS = 0;
                averageFPS = 0;
                frameCount = 0;
                totalFrameTime = 0;
            }

            ImGui::End();
        }else{
            ImGui::Begin("Preparing Debug Info");
            ImGui::SetWindowFontScale(1.25f);
            ImGui::Text("Waiting for FPS to stabilize.");
            ImGui::End();
        }

    }

}