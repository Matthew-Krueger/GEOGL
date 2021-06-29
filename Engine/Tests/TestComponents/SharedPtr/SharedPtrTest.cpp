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
    };

    ~Test1() = default;

    const std::string& getStoredString(){
        return storedString;
    }

private:
    std::string storedString;

};

/* Note, test case does not work individually, for some reason. My allocation tracking code is weird */
TEST_CASE("Trying to create a GEOGL::shared_ptr.", "[SharedPtrTests]") {

    std::cerr << "Testing creating and destructing a test object works\n";

    size_t bytesInUse = GEOGL::getBytesAllocated()-GEOGL::getBytesDeallocated();
    {
        GEOGL::shared_ptr<Test1> test("Hello");
        /* don't test require that the starting bytes in use are different for current if in release mode, or
         * if track memory allocations is off. Otherwise it could get weird */
#if GEOGL_TRACK_MEMORY_ALLOC_FLAG
        REQUIRE(bytesInUse!=0);
        REQUIRE(bytesInUse<GEOGL::getBytesAllocated());
        REQUIRE(bytesInUse!=GEOGL::getBytesAllocated());
#endif
    }
    size_t bytesInUseAfter = GEOGL::getBytesAllocated() - GEOGL::getBytesDeallocated();
    REQUIRE(bytesInUse == bytesInUseAfter);

}

TEST_CASE("DeReferencing overloads", "[SharedPtrTests]") {

    std::cerr << "Testing DeReferencing overloads\n";

    SECTION("Testing with string Test1") {
        std::string testString = "Test1";
        GEOGL::shared_ptr<Test1> test(testString);

        REQUIRE(testString == (*test).getStoredString());
        REQUIRE(testString == test->getStoredString());
        REQUIRE(test.get() != nullptr);

    }

    SECTION("Testing with string TheQuickBrownFoxJumpedOverTheLazyDog") {
        std::string testString = "TheQuickBrownFoxJumpedOverTheLazyDog";
        GEOGL::shared_ptr<Test1> test(testString);

        REQUIRE(testString == (*test).getStoredString());
        REQUIRE(testString == test->getStoredString());
        REQUIRE(test.get() != nullptr);

    }

    SECTION("Testing with string The Big Sun in The $ky1234") {

        std::string testString = "The Big Sun in The $ky1234";
        GEOGL::shared_ptr<Test1> test(testString);

        REQUIRE(testString == (*test).getStoredString());
        REQUIRE(testString == test->getStoredString());
        REQUIRE(test.get() != nullptr);

    }

}

TEST_CASE("Copy Assignment", "[SharedPtrTests]") {

    std::cerr << "Testing CopyAssignment\n";

    SECTION("Testing with string ABC123DoReMiABC123") {
        GEOGL::shared_ptr<Test1> test("ABC123DoReMiABC123");
        GEOGL::shared_ptr<Test1> test2 = test;

        REQUIRE(test.getCount() == test2.getCount());
        REQUIRE(test.get() == test2.get());
        REQUIRE(test.get() != nullptr);
        REQUIRE(test2.get() != nullptr); /* even though if test test.get != nullptr, and test2.get() == test1.get() it should be identical, no harm in checking */

        /* Test it both ways to ensure there is no difference in accessing the pointer */
        REQUIRE((*test).getStoredString() == (*test2).getStoredString());
        REQUIRE(test->getStoredString() == test2->getStoredString());
        REQUIRE((*test).getStoredString() == test2->getStoredString());
        REQUIRE(test->getStoredString() == (*test2).getStoredString());

    }

    SECTION("Testing with string Its as Easy as 123") {

        GEOGL::shared_ptr<Test1> test("Its as Easy as 123");
        GEOGL::shared_ptr<Test1> test2 = test;

        REQUIRE(test.getCount() == test2.getCount());
        REQUIRE(test.get() == test2.get());
        REQUIRE(test.get() != nullptr);
        REQUIRE(test2.get() != nullptr); /* even though if test test.get != nullptr, and test2.get() == test1.get() it should be identical, no harm in checking */

        /* Test it both ways to ensure there is no difference in accessing the pointer */
        REQUIRE((*test).getStoredString() == (*test2).getStoredString());
        REQUIRE(test->getStoredString() == test2->getStoredString());
        REQUIRE((*test).getStoredString() == test2->getStoredString());
        REQUIRE(test->getStoredString() == (*test2).getStoredString());

    }

    SECTION("Testing with string Staring Michael Jackson") {

        GEOGL::shared_ptr<Test1> test("Staring Michael Jackson");
        GEOGL::shared_ptr<Test1> test2 = test;

        REQUIRE(test.getCount() == test2.getCount());
        REQUIRE(test.get() == test2.get());
        REQUIRE(test.get() != nullptr);
        REQUIRE(test2.get() != nullptr); /* even though if test test.get != nullptr, and test2.get() == test1.get() it should be identical, no harm in checking */

        /* Test it both ways to ensure there is no difference in accessing the pointer */
        REQUIRE((*test).getStoredString() == (*test2).getStoredString());
        REQUIRE(test->getStoredString() == test2->getStoredString());
        REQUIRE((*test).getStoredString() == test2->getStoredString());
        REQUIRE(test->getStoredString() == (*test2).getStoredString());

    }

}