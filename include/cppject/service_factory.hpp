#pragma once

#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <typeinfo>

#include <cppject/exception.hpp>

namespace smarthome {

    /**
     * Service class used for injection of concrete objects.
     *
     * This provides for mockability of dependencies.
     */
    class ServiceFactory {
        public:

            /**
             * Get an instance of the specified service.
             *
             * @tparam AbstractBase Abstract base class for which an instance should be returned.
             * @return Shared pointer to the abstract base.
             */
            template <typename AbstractBase>
            const std::shared_ptr<AbstractBase> Get() {

                if ( !_registered<AbstractBase>() )
                    throw ConcreteImplementationNotRegistered( _name<AbstractBase>() );

                const auto generalizedConstructor = _constructors.at( _key<AbstractBase>() );
                const auto constructor = std::any_cast< std::function<std::shared_ptr<AbstractBase>()> >(generalizedConstructor);
                return constructor();
            }

            /**
             * Register a type with the service factory.
             *
             * @tparam ConcreteImpl Concrete implementation to be injected.
             * @tparam AbstractBase Abstract base class for which a concrete implementation is registered.
             */
            template <typename ConcreteImpl, typename AbstractBase>
            void Inject() {

                std::function< std::shared_ptr<AbstractBase>() > constructor = [] {

                    return std::shared_ptr<AbstractBase> (
                        new ConcreteImpl()
                    );
                };

                const auto key = _key<AbstractBase>();
                _constructors.insert({ key, constructor });
            }

            /**
             * Get the singleton instance.
             *
             * @return Singleton instance of the service factory.
             */
            static std::shared_ptr<ServiceFactory> Instance() {

                if (_instance == nullptr)
                    _instance = std::shared_ptr<ServiceFactory>( new ServiceFactory() );
                return _instance;
            }

        private:
            ServiceFactory() { }

            /**
             * Get the key associated with an abstract base class.
             *
             * @tparam AbstractBase Abstract base class for which the key will be fetched.
             * @return The key associated with the specified abstract base class.
             */
            template <typename AbstractBase>
            const size_t _key() {
                return typeid(AbstractBase).hash_code();
            }

            /**
             * Get the name associated with the abstract base class.
             *
             * @tparam AbstractBase Abstract base class for which the name should be fetched.
             * @return Name of the abstract base class.
             */
            template <typename AbstractBase>
            const std::string _name() {
                const auto name = std::string( typeid(AbstractBase).name() );
                return name;
            }

            /**
             * Check if a concrete implementation is registered for the specified abstract base class.
             *
             * @tparam AbstractBase Abstract base class for which registration will be checked.
             * @return True if registered. False otherwise.
             */
            template <typename AbstractBase>
            const bool _registered() {

                const auto key = _key<AbstractBase>();
                const auto found = _constructors.find(key) != _constructors.end();
                return found;
            }

            /**
             * A data structure used to store functions that construct concrete instances.
             *
             * The use of std::any here allows abstraction of function signature differences so that objects of different types can be created.
             */
            std::unordered_map<size_t, std::any> _constructors { };

            /**
             * Singleton instance of the factory.
             */
            inline static std::shared_ptr<ServiceFactory> _instance = nullptr;
    };

} // namespace smarthome