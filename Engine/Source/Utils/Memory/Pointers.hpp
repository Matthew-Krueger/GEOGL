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

    template<typename T>
    class GEOGL_API shared_ptr{

    public:
        template<typename ... Args>
        explicit shared_ptr<T>(Args&& ... args){
            m_Ptr = new T(std::forward<Args>(args)...);
            m_RefCount = new uint32_t;
            *m_RefCount = 1;
        }

        shared_ptr<T>(T* ptr = nullptr)
        {
            m_Ptr = ptr;
            m_RefCount = new uint32_t;
            if (ptr){
                (*m_RefCount)++;
            }
        }

        ~shared_ptr<T>(){
            if(m_RefCount){
                /* We exclusively own the object, go ahead and do normal math */
                (*m_RefCount)--;

                /* If our refcount is zero, we can safely delete everything as it would otherwise become inaccessible */
                if(*m_RefCount == 0){
                    if(m_Ptr) delete m_Ptr;
                    delete m_RefCount;
                }
            }else{
                /* ownership has been transferred out with move semantics */
                if(m_Ptr){
                    /* even though I have been avoiding exceptions, I am trying to prevent a circular dependency on my logs */
                    //throw std::exception("Pointer exists on object that was moved out of");
                }
            }

        }

        /* Copy Semantics */
        shared_ptr<T>(const shared_ptr<T>& obj) noexcept{
            m_Ptr = obj.m_Ptr;
            m_RefCount = obj.m_RefCount;
            (*m_RefCount)++;
        }

        shared_ptr<T>& operator=(const shared_ptr<T>& obj) noexcept{ // copy assignment

            // Assign incoming object's data to this object
            m_Ptr = obj.m_Ptr; // share the underlying pointer
            m_RefCount = obj.m_RefCount;
            (*m_RefCount)++;

            return *this;

        };

        /* Move Semantics */
        shared_ptr<T>(shared_ptr<T>&& dyingObj) noexcept{
            m_Ptr = dyingObj.m_Ptr; // share the underlying pointer
            m_RefCount = dyingObj.m_RefCount;

            dyingObj.m_Ptr = nullptr;
            dyingObj.m_RefCount = nullptr; // clean the dying object
        }

        shared_ptr<T>& operator=(shared_ptr<T>&& dyingObj) noexcept{

            m_Ptr = dyingObj.m_Ptr; // share the underlying pointer
            m_RefCount = dyingObj.m_RefCount;

            dyingObj.m_Ptr = nullptr;
            dyingObj.m_RefCount = nullptr; // clean the dying object

            return *this;

        }

        uint32_t getCount() const{

#ifndef NDEBUG
            if(!*m_RefCount)
                throw std::exception("Bad reference to a shared ptr refcount");
#endif

            return *m_RefCount;

        };

        T* get() const{ return m_Ptr; };
        T* operator->() const{ return m_Ptr; };
        T& operator*() const{ return *m_Ptr; };

    private:
        uint32_t* m_RefCount;
        T* m_Ptr;
    };

    template<typename T>
    class GEOGL_API unique_ptr{

    public:
        template<typename ... Args>
        explicit unique_ptr<T>(Args&& ... args){
            m_Ptr = new T(std::forward<Args>(args)...);
        }

        explicit unique_ptr<T>(T* ptr = nullptr){
            m_Ptr = ptr;
        }

        ~unique_ptr<T>(){
            if(m_Ptr) delete m_Ptr;
        }

        /* Copy Semantics */
        unique_ptr<T>(const unique_ptr<T>& obj) = delete;
        unique_ptr<T>& operator=(const unique_ptr<T>& obj) = delete;

        /* Move Semantics */
        unique_ptr<T>(unique_ptr<T>&& dyingObj) noexcept{
            m_Ptr = dyingObj.m_Ptr; // share the underlying pointer

            dyingObj.m_Ptr = nullptr;
        }

        unique_ptr<T>& operator=(unique_ptr<T>&& dyingObj) noexcept{

            m_Ptr = dyingObj.m_Ptr; // share the underlying pointer
            dyingObj.m_Ptr = nullptr; // clean the dying object

            return *this;

        }

        T* get() const{ return m_Ptr; };
        T* operator->() const{ return m_Ptr; };
        T& operator*() const{ return *m_Ptr; };

    private:
        T* m_Ptr;
    };

    template<typename T, typename ... Args>
    GEOGL::shared_ptr<T> make_shared(Args&& ... args){
        return GEOGL::shared_ptr<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    GEOGL::unique_ptr<T> make_unique(Args&& ... args){
        return GEOGL::unique_ptr<T>(std::forward<Args>(args)...);
    }

}



#endif //GEOGL_POINTERS_HPP
