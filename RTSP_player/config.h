#pragma once
#include <iostream>
#include <vector>
#include "tinyxml.h"
#include <map> 
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

struct ConfigAttribute
{
	std::string strAttribute;
	std::string strValue;
	std::map<std::string, string> sub;
};

class Config
{
public:
	Config(std::string filename, std::string identifier);
	
	bool ReadConfigFile();
	int Config::getInteger(int iDefault, std::string strAttribute, std::string subAttribute);
	double getDouble(double dDefault, std::string strAttribute, std::string subAttribute);
	std::string Config::getString(string sDefault, std::string strAttribute, std::string subAttribute);
	bool Config::getBool(bool bDefault, std::string strAttribute, std::string subAttribute);
	std::map<std::string, ConfigAttribute> getMap() { return m_mapConfig; }
	std::string Uppercase(std::string strInput);
	bool WriteConfigFile();
	void setString(std::string sValue, std::string strAttribute, std::string subAttribute);
	void setInteger(int iValue, std::string strAttribute, std::string subAttribute);
	void setBool(bool bValue, std::string strAttribute, std::string subAttribute);
	void setDouble(double dValue, std::string strAttribute, std::string subAttribute);
private:
	std::string m_strFilename;
	std::string m_strIdentifier;
	std::map<std::string, ConfigAttribute> m_mapConfig;
};

Config::Config(std::string filename, std::string identifier) {
	m_strFilename = filename;
	m_strIdentifier = identifier;
}

bool Config::ReadConfigFile() {
	TiXmlDocument doc(m_strFilename.c_str());
	
	bool loadOkay = doc.LoadFile();

	if (!loadOkay)
	{
		std::cout << "Could not load test file '" << m_strFilename << "'. Error='" << doc.ErrorDesc() << "'. Using Default Data" << std::endl;
		return false;
	}

	TiXmlNode* parent = 0;

	parent = doc.FirstChild(m_strIdentifier.c_str());
	assert(parent);
	auto configElement = parent->ToElement();
	assert(configElement);
	for (auto child = configElement->FirstChild(); child; child = child->NextSibling())
	{
		auto itemElement = child->ToElement();
		assert(itemElement);
		//convert to uppercase
		
		ConfigAttribute element;
		std::string strElement = itemElement->Value();
		element.strAttribute = strElement;

		if (child->FirstChildElement() != NULL) {
			for (auto tempChild = child->FirstChild(); tempChild; tempChild = tempChild->NextSibling())
			{
				auto tempElement = tempChild->ToElement();
				std::string strTempElemnet = tempElement->Value();
				/*ConfigAttribute subelement;
				subelement.strAttribute = strTempElemnet;
				subelement.strValue = tempElement->GetText();*/

				/*cout << subelement.strAttribute << endl;
				cout << subelement.strValue << endl;*/

				element.sub[strTempElemnet]= tempElement->GetText();
			}
		}
		else {
			element.strValue = itemElement->GetText();
			/*cout << strElement << endl;
			cout << element.strValue << endl;*/
		}
		m_mapConfig[strElement] = element;
	}
	return true;
}
std::string Config::getString(string sDefault, std::string strAttribute, std::string subAttribute = "None") {
	if (subAttribute == "None") {
		if (m_mapConfig.find(strAttribute) != m_mapConfig.end())
		{
			string returnValue = m_mapConfig[strAttribute].strValue;
			return returnValue.c_str();
		}
		else {
			cout << "키가 존재하지 않음.";
			return sDefault;
		}
	}
	else {
		//size 있음
		if (m_mapConfig.find(strAttribute) != m_mapConfig.end())
		{
			//width 있음
			if (m_mapConfig[strAttribute].sub.find(subAttribute) != m_mapConfig[strAttribute].sub.end()) {
				string returnValue = m_mapConfig[strAttribute].sub[subAttribute];

				return returnValue.c_str();
			}
			else {
				cout << "키가 존재하지 않음.";
				return sDefault;
			}
		}
		else {
			cout << "키가 존재하지 않음.";
			return sDefault;
		}
	}
}

int Config::getInteger(int iDefault, std::string strAttribute, std::string subAttribute="None") {
	std::string strDefault;
	std::ostringstream strStream;

	strStream << iDefault;
	strDefault = strStream.str();

	std::string returnValue = getString(strDefault, strAttribute, subAttribute);
	return atoi(returnValue.c_str());
}

double Config::getDouble(double dDefault, std::string strAttribute, std::string subAttribute="None") {
	std::string strDefault;
	std::ostringstream strStream;

	strStream << dDefault;
	strDefault = strStream.str();

	std::string returnValue = getString(strDefault, strAttribute, subAttribute);
	return atof(returnValue.c_str());
}



bool Config::getBool(bool bDefault, std::string strAttribute, std::string subAttribute = "None") {
	std::string returnValue;
	if (bDefault)
		returnValue = getString("true", strAttribute, subAttribute);
	else 
		returnValue = getString("false", strAttribute, subAttribute);
	if (Uppercase(returnValue) == "TRUE")
		return true;
	else
		return false;
}



std::string Config::Uppercase(std::string strInput)
{
	std::transform(strInput.begin(), strInput.end(), strInput.begin(), toupper);
	return strInput;
}

bool Config::WriteConfigFile()
{
	TiXmlDocument doc(m_strFilename.c_str());
	TiXmlElement configData(m_strIdentifier.c_str());
	configData.SetAttribute("version", "1.0");

	std::map<std::string, ConfigAttribute>::iterator configIter;
	for (configIter = m_mapConfig.begin(); configIter != m_mapConfig.end(); configIter++)
	{
		ConfigAttribute item = configIter->second;
		if (item.sub.empty()) {
			std::string attribute = item.strAttribute;

			std::string value = item.strValue;

			TiXmlElement* pElem = new TiXmlElement(attribute.c_str());
			pElem->LinkEndChild(new TiXmlText(value.c_str()));
			configData.LinkEndChild(pElem);
		}
		else {
			std::string attribute = item.strAttribute;

			TiXmlElement* pElem = new TiXmlElement(attribute.c_str());
			configData.LinkEndChild(pElem);
			std::map<std::string, string>::iterator subIter;
			for (subIter = item.sub.begin(); subIter != item.sub.end(); subIter++) {
				std::string subAttr = subIter->first;
				std::string subValue = subIter->second;
				TiXmlElement* subElem = new TiXmlElement(subAttr.c_str());
				subElem->LinkEndChild(new TiXmlText(subValue.c_str()));
				pElem->LinkEndChild(subElem);
			}
		}
	}
	doc.LinkEndChild(configData.Clone());

	return doc.SaveFile();
}
void Config::setString(std::string sValue, std::string strAttribute, std::string subAttribute="None") {
	if (subAttribute == "None") {
		ConfigAttribute configattribue;
		configattribue.strAttribute = strAttribute;
		configattribue.strValue = sValue;
		m_mapConfig[strAttribute] = configattribue;
	}
	else {
		std::map<std::string, string> subConfigAttribute;
		if (!m_mapConfig[strAttribute].sub.empty()) {
			subConfigAttribute = m_mapConfig[strAttribute].sub;
		}
	
		subConfigAttribute[subAttribute] = sValue;
		ConfigAttribute configattribue;
		configattribue.strAttribute = strAttribute;
		configattribue.sub = subConfigAttribute;
		m_mapConfig[strAttribute] = configattribue;
		
	}
}
void Config::setInteger(int iValue, std::string strAttribute, std::string subAttribute="None") {
	std::string strDefault;
	std::ostringstream strStream;

	strStream << iValue;
	strDefault = strStream.str();

	setString(strDefault, strAttribute, subAttribute);
}

void Config::setBool(bool bValue, std::string strAttribute, std::string subAttribute="None") {
	if (bValue) {
		std::string strValue = "true";
		setString(strValue, strAttribute, subAttribute);
	}
	else {
		std::string strValue = "false";
		setString(strValue, strAttribute, subAttribute);
	}
}

void Config::setDouble(double dValue, std::string strAttribute, std::string subAttribute = "None") {
	std::string strDefault;
	std::ostringstream strStream;

	strStream << dValue;
	strDefault = strStream.str();

	setString(strDefault, strAttribute, subAttribute);
}