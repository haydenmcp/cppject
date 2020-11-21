#include <gmock/gmock.h>
#include <exception>

#include <cppject/service_factory.hpp>

using namespace cppject;

class AbstractBase {
    public:
        virtual void SomeFunction() = 0;
};

class ConcreteImpl : public AbstractBase {
    public:
        void SomeFunction() { }
};

TEST(ServiceFactory, ShouldDieIfAnAttemptIsMadeToGetAnInstanceOfAnUnregisteredAbstractBaseClass) {
    auto serviceFactory = ServiceFactory::Instance();
    EXPECT_THROW(serviceFactory->Get<AbstractBase>(), ConcreteImplementationNotRegistered);
}

TEST(ServiceFactory, ShouldAllowUserToRegisterConcreteImplementations) {

    auto serviceFactory = ServiceFactory::Instance();
    try {
        serviceFactory->Inject<ConcreteImpl, AbstractBase>();
    } catch (...) {
        FAIL();
    }
}

TEST(ServiceFactory, ShouldAllowUserToFetchAConcreteImplementationUsingTheAbstractBaseClass) {

    auto serviceFactory = ServiceFactory::Instance();
    try {
        serviceFactory->Inject<ConcreteImpl, AbstractBase>();
        auto& concreteImplementation = serviceFactory->Get<AbstractBase>();
    } catch(const std::exception& e) {
        FAIL() << "Failed with exception: " << e.what();
    }
}