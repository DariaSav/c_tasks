#include <vector>
#include <string>
#include <any>
#include <memory>

#include "generators.h"

class TFactory {

	class TImpl;
	std::unique_ptr<const TImpl> Impl;
	

public:
	
	TFactory();
	~TFactory();
	std::unique_ptr<TRandomNumberGenerator> Create(
		const std::string& name, const TOptions &opts) const;

	std::vector<std::string> GetAvailableObjects() const;
};

