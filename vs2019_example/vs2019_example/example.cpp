#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../../jomock/jomock.h"

#include <iostream>

using namespace ::std;
using namespace ::testing;

string func() {
    return "no mock";
}

class ClassTest {
public:
    static int staticFunc() 
    {
        return 1;
    }

    int nonStaticFunc() 
    {
        return 2;
    }

    static string parameterFunc(bool, char, string, const string&)
    {
        return "no mock";
    }

    static string referenceParameterFunc(bool& result, char&, string&, const string&)
    {
        return "no mock";
    }

    static void outputArgumentFunc(bool* result, int input)
    {
        *result = false;
    }
};

TEST(JoMock, GlobalFunction) 
{

    EXPECT_CALL(JOMOCK(func), JOMOCK_FUNC())
        .Times(Exactly(1))
        .WillOnce(Return("global mock"));

    EXPECT_EQ("global mock", func());


    EXPECT_CALL(JOMOCK(ClassTest::staticFunc), JOMOCK_FUNC())
        .Times(Exactly(1))
        .WillOnce(Return(1));

    EXPECT_EQ(ClassTest::staticFunc(), 1);
}
/**/
TEST(JoMock, StaticFunctionClass) 
{

    EXPECT_CALL(JOMOCK(ClassTest::staticFunc), JOMOCK_FUNC())
        .Times(Exactly(1))
        .WillOnce(Return(3));

    EXPECT_EQ(ClassTest::staticFunc(), 3);
}

TEST(JoMock, NonStaticFunctionClass) 
{

    ClassTest classTest;
    auto mocker = &JOMOCK(&ClassTest::nonStaticFunc);
    EXPECT_CALL(*mocker, JOMOCK_FUNC(&classTest))
        .Times(Exactly(1))
        .WillOnce(Return(4));
    EXPECT_EQ(classTest.nonStaticFunc(), 4);

    mocker->restore();
    EXPECT_EQ(classTest.nonStaticFunc(), 2);
}

TEST(JoMock, ParameterFunctionTest)
{
    auto funcBinded = bind(ClassTest::parameterFunc, true, 'c', "test", "");
    auto mocker = &JOMOCK(ClassTest::parameterFunc);
    EXPECT_CALL(*mocker, JOMOCK_FUNC(_, _, _, _))
        .Times(Exactly(1))
        .WillOnce(Return("mocked func"));
    EXPECT_EQ(funcBinded(), "mocked func");
}

TEST(JoMock, ReferenceParameterFunctionTest)
{
    bool b;
    char c;
    string s;
    const string cs;

    EXPECT_CALL(JOMOCK(ClassTest::referenceParameterFunc), JOMOCK_FUNC(_, _, _, _))
        .Times(Exactly(2))
        .WillRepeatedly(Return("mocked func"));

    EXPECT_EQ(ClassTest::referenceParameterFunc(ref(b), ref(c), ref(s), ref(cs)), "mocked func");
}

TEST(JoMock, OutputArgumentFunctionTest)
{
    bool result = false;
    bool tmp = true;

    ClassTest::outputArgumentFunc(&result, 10);
    EXPECT_EQ(result, false);

    EXPECT_CALL(JOMOCK(ClassTest::outputArgumentFunc), JOMOCK_FUNC(_, _))
        .Times(Exactly(1))
        .WillOnce(SetArgPointee<0>(tmp));

    ClassTest::outputArgumentFunc(&result, 10);
    EXPECT_EQ(result, true);
}

TEST(JoMock, LogacyLibraryTest)
{
    EXPECT_CALL(JOMOCK(atoi), JOMOCK_FUNC(_))
        .Times(Exactly(1))
        .WillOnce(Return(1));
    EXPECT_EQ(atoi("TEN"), 1);
}

int main(int argc, char* argv[])
{
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

    return 0;
}

