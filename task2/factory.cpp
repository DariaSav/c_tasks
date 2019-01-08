#include <vector>
#include <map>
#include <string>
#include <any>
#include <memory>
#include <stdexcept>

#include "factory.h"
//#include "generators.h"

class TFactory::TImpl {
	class ICreator {
	public:
		virtual ~ICreator(){}
		virtual std::unique_ptr<TRandomNumberGenerator> Create(const TOptions &opts) const = 0;
	};
	using TCreatorPtr = std::shared_ptr<ICreator>;
	using TRegisteredCreators = std::map<std::string, TCreatorPtr>;
	TRegisteredCreators RegisteredCreators;

public:
	template <class TCurrentGenerator>
	class TCreator : public ICreator{
		std::unique_ptr<TRandomNumberGenerator> Create(const TOptions &opts) const override{
			return std::make_unique<TCurrentGenerator>(opts);
		}
	};

public:
	TImpl() { RegisterAll();}
	template <typename T>
	void RegisterCreator(const std::string& name) {
		RegisteredCreators[name] = std::make_shared<TCreator<T>>();
	}
	void RegisterAll() {
		RegisterCreator<Poisson>("poisson");
		RegisterCreator<Bernoulli>("bernoulli");
		RegisterCreator<Geometric>("geometric");
        	RegisterCreator<Discrete>("discrete");
	}

	std::unique_ptr<TRandomNumberGenerator> CreateObject(const std::string& n, const TOptions &opts) const {
		auto creator = RegisteredCreators.find(n);
		if (creator == RegisteredCreators.end()) {
			return nullptr;
		}
		
		try {
			return creator->second->Create(opts);
		} 
		catch (std::invalid_argument &err) {
			return nullptr;
		}
	}

public:

	std::vector<std::string> GetAvailableObjects () const {
	std::vector<std::string> result;
	for (const auto& creatorPair : RegisteredCreators) {
		result.push_back(creatorPair.first);
		}
		return result;
	}
};

std::unique_ptr<TRandomNumberGenerator> TFactory::Create(const std::string& n, const TOptions &opts) const {
	return Impl->CreateObject(n, opts);
}

TFactory::TFactory() : Impl(std::make_unique<TFactory::TImpl>()) {}
TFactory::~TFactory(){}
std::vector<std::string> TFactory::GetAvailableObjects() const {
	return Impl->GetAvailableObjects();
}

