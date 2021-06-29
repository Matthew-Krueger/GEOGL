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


#ifndef GEOGL_POINTERS_HPP
#define GEOGL_POINTERS_HPP

namespace GEOGL{

    //template<typename T, typename ... Args>
    //make_shared(Args&& ... args){

    //}
    template<typename T>
    class GEOGL_API shared_ptr{

    public:
        template<typename ... Args>
        shared_ptr(Args&& ... args){
            m_Ptr = new T(std::forward<Args>(args)...);
            m_RefCount = new uint32_t;
            *m_RefCount = 1;
        }

        ~shared_ptr(){
            (*m_RefCount)--;

            /* If our refcount is zero, we can safely delete everything as it would otherwise become inaccessible */
            if(*m_RefCount == 0){
                delete m_Ptr;
                delete m_RefCount;
            }
        }

    private:
        uint32_t* m_RefCount;
        T* m_Ptr;
    };

}

#endif //GEOGL_POINTERS_HPP
