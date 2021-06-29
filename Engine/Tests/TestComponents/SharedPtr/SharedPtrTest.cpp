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

#include <Catch/Catch2.hpp>
#include <GEOGL/Utils.hpp>

class Test1{
public:
    explicit Test1(std::string string): storedString(std::move(string)){
        std::cerr << "Created a test object\n";
    };

    ~Test1(){
        std::cerr << "Destroying a Test Object\n";
    };

private:
    std::string storedString;
    int storedInt;


};

void createTest1String(){

    std::cerr << "Testing creating and destructing a test object works\n";

    size_t bytesInUse = GEOGL::getBytesAllocated()-GEOGL::getBytesDeallocated();
    {
        GEOGL::shared_ptr<Test1> test("Hello");
    }
    size_t bytesInUseAfter = GEOGL::getBytesAllocated() - GEOGL::getBytesDeallocated();
    REQUIRE(bytesInUse == bytesInUseAfter);

}

TEST_CASE("Trying to create a GEOGL::shared_ptr.", "[SharedPtrTests]") {

    REQUIRE_NOTHROW(createTest1String());

}