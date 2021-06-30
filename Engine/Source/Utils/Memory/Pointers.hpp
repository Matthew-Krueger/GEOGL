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
        explicit shared_ptr(Args&& ... args){
            m_Ptr = new T(std::forward<Args>(args)...);
            m_RefCount = new uint32_t;
            *m_RefCount = 1;
        }

        ~shared_ptr(){
            (*m_RefCount)--;

            /* If our refcount is zero, we can safely delete everything as it would otherwise become inaccessible */
            if(*m_RefCount == 0){
                if(m_Ptr) delete m_Ptr;
                delete m_RefCount;
            }
        }

        uint32_t getCount() const{ return *m_RefCount; };

        /* Copy Semantics */
        shared_ptr(const shared_ptr& obj){
            m_Ptr = obj.m_Ptr;
            m_RefCount = obj.m_RefCount;
            (*m_RefCount)++;
        }

        shared_ptr& operator=(const shared_ptr& obj){ // copy assignment

            // Assign incoming object's data to this object
            m_Ptr = obj.m_Ptr; // share the underlying pointer
            m_RefCount = obj.m_RefCount;
            (*m_RefCount)++;

        };

        /* Move Semantics */
        shared_ptr(shared_ptr&& dyingObj) // move constructor
        {
            m_Ptr = dyingObj.m_Ptr; // share the underlying pointer
            m_RefCount = dyingObj.m_RefCount;

            dyingObj.m_Ptr = dyingObj.m_RefCount = nullptr; // clean the dying object
        }

        shared_ptr& operator=(shared_ptr && dyingObj) // move assignment
        {

            m_Ptr = dyingObj.m_Ptr; // share the underlying pointer
            m_RefCount = dyingObj.m_RefCount;

            dyingObj.m_Ptr = dyingObj.m_RefCount = nullptr; // clean the dying object

        }

        T* get() const{ return m_Ptr; };
        T* operator->() const{ return m_Ptr; };
        T& operator*() const{ return *m_Ptr; };

    private:
        uint32_t* m_RefCount;
        T* m_Ptr;
    };

}

#endif //GEOGL_POINTERS_HPP
