
#include <exception>
#include <string>

namespace smarthome {

    /**
     * Thrown when an abstract interface is being fetched but hasn't had a concrete implementation registered with the service factory.
     */
    class ConcreteImplementationNotRegistered : public std::exception {
        public:
            /**
             * @param abstractBaseName Abstract base class without a registered concrete implementation.
             */
            ConcreteImplementationNotRegistered(const std::string abstractBaseName) :
                _msg("A concrete implementation hasn't been registered for the abstract base " + abstractBaseName) { }

            const char* what() const noexcept {
                return _msg.c_str();
            }

        private:

            const std::string _msg;
    };

} // namespace smarthome