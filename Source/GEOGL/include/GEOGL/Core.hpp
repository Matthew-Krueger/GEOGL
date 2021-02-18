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

#ifndef GEOGL_CORE_HPP
#define GEOGL_CORE_HPP

/* Dependencies */
//#include "../../Utils/Headers/Dependencies.hpp"
#include <GEOGL/Utils.h>

/* Application */
#include "../../Application/Application.hpp"

/* Loading and Callbacks */
#include "../../Utils/Callbacks.hpp"

/* Events */
#include "../../IO/Events/Event.hpp"
#include "../../IO/Events/ApplicationEvent.hpp"
#include "../../IO/Events/KeyEvent.hpp"
#include "../../IO/Events/MouseEvent.hpp"

/* Input Polling Section */
#include "../../IO/Input.hpp"
#include "../../Utils/InputCodes.hpp"

/* Layers API */
#include "../../Layers/Layer.hpp"
#include "../../ImGui/ImGuiLayer.hpp"
#include "../../Layers/LayerStack.hpp"

#endif //GEOGL_CORE_HPP
