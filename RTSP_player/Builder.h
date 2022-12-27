#pragma once
#include "config.h"
#include <stdio.h>
#include <io.h>

class Builder
{
public:
	virtual Config* getConfig()=0;
};

class miniBuilder:public Builder
{
public:
	Config* getConfig() override { 
		std::string filename = "../Runtime/config/config.xml";
		std::string identifier = "config";
		Config* config = new Config(filename, identifier);
		if ((_access(filename.c_str(), 0) != -1)) {
			if (!config->ReadConfigFile())
				std::cout << "Failed to read config file." << std::endl;
		}
		else {
			config->setInteger(300, "size", "width");
			config->setInteger(400, "size", "height");
			config->setDouble(0.5, "threshold", "conf");
			config->setDouble(0.45, "threshold", "iou");
			config->setBool(true, "person");
			config->WriteConfigFile();
		}	
		return config;
	}
};

class bigBuilder :public Builder
{
public:
	Config* getConfig() override {
		std::string filename = "../Runtime/config/config.xml";
		std::string identifier = "config";
		Config* config = new Config(filename, identifier);
		if ((_access(filename.c_str(), 0) != -1)) {
			if (!config->ReadConfigFile())
				std::cout << "Failed to read config file." << std::endl;
		}
		else {
			config->setInteger(1500, "size", "width");
			config->setInteger(1000, "size", "height");
			config->setDouble(0.45, "threshold", "conf");
			config->setDouble(0.5, "threshold", "iou");
			config->setBool(false, "person");
			config->WriteConfigFile();
		}
		return config;
	}
};

class Director
{
public:
	Director(Builder* builder) : pBuilder(builder) {};
	~Director() { if (pBuilder) delete pBuilder; }

	Config* construct() {
		Config* pConfig = pBuilder->getConfig();
		return pConfig;
	}
private:
	Builder* pBuilder;
};

