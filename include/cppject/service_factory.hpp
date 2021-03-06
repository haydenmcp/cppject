#pragma once

#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <typeinfo>

#include <cppject/exception.hpp>

namespace cppject {

    /**
     * Service class used for injection of concrete objects.
     *
     * This provides for mockability of dependencies.
     */
    class ServiceFactory {
        public:

            using shared_ptr = std::shared_ptr<ServiceFactory>;

            /**
             * Get an instance of the specified service.
             *
             * @tparam AbstractBase Abstract base class for which an instance should be returned.
             * @return Instance of the concrete type.
             */
            template <typename AbstractBase>
            AbstractBase& Get() {

                return _instance<AbstractBase>();
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

                if (_factoryInstance == nullptr)
                    _factoryInstance = std::shared_ptr<ServiceFactory>( new ServiceFactory() );
                return _factoryInstance;
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
             * Get the instance associated with an abstract base.
             *
             * @tparam AbstractBase Abstract base class for which registration will be checked.
             * @return Instance of the specified type.
             */
            template <typename AbstractBase>
            AbstractBase& _instance() {

                if ( !_registered<AbstractBase>() )
                    throw ConcreteImplementationNotRegistered( _name<AbstractBase>() );

                const auto key = _key<AbstractBase>();
                if (!_created<AbstractBase>()) {

                    const auto generalizedConstructor = _constructors.at(key);
                    const auto constructor = std::any_cast< std::function<std::shared_ptr<AbstractBase>()> >(generalizedConstructor);
                    _instances.insert({key, constructor() });
                }

                auto generalizedInstance = _instances.at(key);
                return *std::any_cast< std::shared_ptr<AbstractBase> >( generalizedInstance );
            }

            /**
             * Check if a concrete implementation has already been created.
             *
             * @tparam AbstractBase Abstract base class for which registration will be checked.
             * @return True if created. False otherwise.
             */
            template <typename AbstractBase>
            const bool _created() {

                const auto key = _key<AbstractBase>();
                const auto found = _instances.find(key) != _instances.end();
                return found;
            }

            /**
             * A data structure used to store functions that construct concrete instances.
             *
             * The use of std::any here allows abstraction of function signature differences so that objects of different types can be created.
             */
            std::unordered_map<size_t, std::any> _constructors { };

            /**
             * Stores the object instances.
             */
            std::unordered_map<size_t, std::any> _instances { };

            /**
             * Singleton instance of the factory.
             */
            inline static std::shared_ptr<ServiceFactory> _factoryInstance = nullptr;
    };

} // namespace smarthome