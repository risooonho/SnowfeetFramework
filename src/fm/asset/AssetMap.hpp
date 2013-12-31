/*
AssetMap.hpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#ifndef HEADER_ZN_ASSETMAP_HPP_INCLUDED
#define HEADER_ZN_ASSETMAP_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <iostream>

#include "../util/stringutils.hpp"
#include "../config.hpp"
#include "../json/JsonBox.h"

namespace zn
{

// This generic class handles the loading of specific-type assets and indexes them in a map.
// assets with the same file path are not reloaded.
// The template parameter must be an asset class implementing functions defined in IAsset
// (such as bool loadFromFile(const std::string & filePath) )
template <class T>
class AssetMap
{
public:

	~AssetMap()
	{
		clear();
	};

	// Destroys all the assets contained in the map
	void clear()
	{
		for(auto it = m_map.begin(); it != m_map.end(); ++it)
			delete it->second;
		m_map.clear();
	}

	// Get the root folder (without the end slash)
	inline const std::string & rootFolder() const { return m_rootFolder; }

	// Sets the root folder that will be used for loading the assets of this type
	void setRootFolder(const std::string & rf)
	{
#ifdef ZN_DEBUG
		if(!m_map.empty())
			std::cout << "D: AssetMap: root folder changed while assets are already loaded." << std::endl;
#endif
		m_rootFolder = rf;
		// Remove the '/' if it is present
		if(!m_rootFolder.empty() && m_rootFolder[m_rootFolder.size()-1] == '/')
		{
			m_rootFolder = m_rootFolder.substr(0, m_rootFolder.size()-1);
		}
	}

	// Gets an asset from its name
	T * get(const std::string & id) const
	{
		auto it = m_map.find(id);
		if(it != m_map.end())
			return it->second;
		else
		{
#ifdef ZN_DEBUG
			std::cout << "D: AssetMap::get: not found " << id << std::endl;
#endif
			return nullptr;
		}
	}

	// Finds the name of an asset from its pointer.
	// Returns an empty string if the asset is not contained in the map.
	std::string findName(const T * asset) const
	{
		for(auto it = m_map.cbegin(); it != m_map.cend(); ++it)
		{
			if(it->second == asset)
				return it->first;
		}
		return "";
	}

	// Loads an asset from a file into the map with a name
	bool load(std::string filePath, const std::string & id)
	{
		if(id.empty())
		{
#ifdef ZN_DEBUG
			std::cout << "E: AssetMap::load: empty ID specified !" << std::endl;
#endif
			return false;
		}

		if(m_map.find(id) != m_map.end())
			return true; // Already loaded

		if(!m_rootFolder.empty())
			filePath = m_rootFolder + '/' + filePath;
#ifdef ZN_DEBUG
		std::cout << "D: AssetMap: loading " << filePath << std::endl;
#endif

		T * asset = new T();
		if(!loadAsset(asset, filePath))
		{
			delete asset;
			return false;
		}
		else
		{
			m_map[id] = asset;
			return true;
		}
	}

	// Loads a list of assets from its JSON object representation
	bool loadList(JsonBox::Value & obj)
	{
		if(!obj["root"].isNull())
		{
			setRootFolder(obj["root"].getString());
		}

		JsonBox::Array jlist = obj["list"].getArray();

		std::string name, src;

		for(auto it = jlist.begin(); it != jlist.end(); ++it)
		{
			src = (*it)["src"].getString();

			// Get the user-defined name of the asset
			// Note : JsonBox will not raise an error if the name is not specified,
			// it reacts as an std::map
			name = (*it)["name"].getString();

			// Then if the name is empty
			if(name.empty())
			{
				// We use the filename directly
				name = fileNameWithoutExtension(src);
			}

			if(!load(src, name))
			{
				return false;
			}
		}

		return true;
	}

	inline std::unordered_map<std::string,T*> begin() const { return m_map.begin(); }
	inline std::unordered_map<std::string,T*> end() const { return m_map.end(); }

private:

	// Internal asset loading routine.
	// If T has a different loading method, a template specialization may be required.
	inline bool loadAsset(T * asset, const std::string & filePath)
	{
		return asset->loadFromFile(filePath);
	}

	std::string m_rootFolder;
	std::unordered_map<std::string,T*> m_map;
};

#include <SFML/Audio.hpp>

// Specialization for sf::Music
template <>
inline bool AssetMap<sf::Music>::loadAsset(sf::Music * asset, const std::string & filePath)
{
	// Music is not really loaded, it is streamed
	return asset->openFromFile(filePath);
}

} // namespace zn

#endif // HEADER_ZN_ASSETMAP_HPP_INCLUDED


