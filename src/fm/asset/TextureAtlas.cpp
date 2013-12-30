/*
TextureAtlas.cpp
Copyright (C) 2010-2013 Marc GILLERON
This file is part of the zCraftFramework project.
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include "TextureAtlas.hpp"
#include "../sfml/sfml2_utils.hpp"
#include "../json/JsonBox.h"

namespace zn
{

TextureAtlas::TextureAtlas()
{
}

//------------------------------------------------------------------------------
bool TextureAtlas::loadFromFile(const std::string & filePath)
{
	return loadFromJSONFile(filePath);
}

//------------------------------------------------------------------------------
bool TextureAtlas::loadFromJSONFile(const std::string & filePath)
{
	std::ifstream ifs(filePath.c_str(), std::ios::in|std::ios::binary);
	if(!ifs.good())
	{
		std::cout << "E: TiledMap::loadFromJSONFile: couldn't open \"" + filePath + '"' << std::endl;
		return false;
	}

	std::cout << "Reading TextureAtlas..." << std::endl;

	// Parse stream

	JsonBox::Value doc;
	doc.loadFromStream(ifs);
	ifs.close();

	// Check document

	assert(doc.isObject());
	s32 version = doc["version"].getInt();
	if(version != 1)
	{
		std::cout << "E: TextureAtlas: unsupported JSON version (" << version << ")" << std::endl;
		return false;
	}

	// Globals

	std::string textureName = doc["texture"].getString();
	s32 prescale = doc["prescale"].getInt();
	s32 timescale = doc["timescale"].getInt();

	// Frames

	JsonBox::Array jframes = doc["frames"].getArray();
	for(auto it = jframes.begin(); it != jframes.end(); ++it)
	{
		Frame f;

		std::string name = (*it)["name"].getString();

		f.rect.left = prescale * (*it)["x"].getInt();
		f.rect.top = prescale * (*it)["y"].getInt();
		f.rect.width = prescale * (*it)["w"].getInt();
		f.rect.height = prescale * (*it)["h"].getInt();

		if(m_frames.find(name) != m_frames.end())
		{
			std::cout << "W: TextureAtlas: found duplicate frame \"" << name << "\" in " << filePath << std::endl;
		}

		m_frames[name] = f;
	}

	// Sequences

	JsonBox::Array jsequences = doc["sequences"].getArray();
	for(auto it = jsequences.begin(); it != jsequences.end(); ++it)
	{
		Sequence seq;

		std::string name = (*it)["name"].getString();

		JsonBox::Array jseqframes = (*it)["frames"].getArray();
		for(auto itf = jseqframes.begin(); itf != jseqframes.end(); ++itf)
		{
			SequenceFrame f;

			f.rect.left = prescale * (*itf)["x"].getInt();
			f.rect.top = prescale * (*itf)["y"].getInt();
			f.rect.width = prescale * (*itf)["w"].getInt();
			f.rect.height = prescale * (*itf)["h"].getInt();

			f.duration = timescale * (*itf)["d"].getInt();

			seq.frames.push_back(f);
		}

		if(m_sequences.find(name) != m_sequences.end())
		{
			std::cout << "W: TextureAtlas: found duplicate sequence \"" << name << "\" in " << filePath << std::endl;
		}

		m_sequences[name] = seq;
	}

	// Load texture

	std::string localDir = filePath.substr(0, filePath.find_last_of("/\\"));
	std::string texturePath = localDir + '/' + textureName;
	if(!m_texture.loadFromFile(texturePath))
	{
		std::cout << "E: TextureAtlas: couldn't load texture \"" << texturePath << '"' << std::endl;
		return false;
	}

	// TODO add JSON option for texture smooth
	m_texture.setSmooth(false);

	return true;
}

//------------------------------------------------------------------------------
const TextureAtlas::Frame * TextureAtlas::frame(std::string id) const
{
	auto it = m_frames.find(id);
	if(it != m_frames.end())
		return &it->second;
	else
		return nullptr;
}

//------------------------------------------------------------------------------
const TextureAtlas::Sequence * TextureAtlas::sequence(std::string id) const
{
	auto it = m_sequences.find(id);
	if(it != m_sequences.end())
		return &it->second;
	else
		return nullptr;
}

//------------------------------------------------------------------------------
void TextureAtlas::addFrame(std::string id, sf::IntRect rect)
{
#ifdef ZN_DEBUG
	if(frame(id) != nullptr)
		std::cout << "W: TextureAtlas::addFrame: overwrite previous ID (" << id << std::endl;
#endif
	m_frames[id].rect = rect;
}

//------------------------------------------------------------------------------
void TextureAtlas::addSequence(std::string id, const Sequence & seq)
{
#ifdef ZN_DEBUG
	if(sequence(id) != nullptr)
		std::cout << "W: TextureAtlas::addFrame: overwrite previous ID (" << id << std::endl;
#endif
	m_sequences[id] = seq;
}

//------------------------------------------------------------------------------
bool TextureAtlas::checkFrame(const sf::IntRect & rect)
{
	//cout << " " << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << " ";
	return checkTextureRect(m_texture, rect);
}

//------------------------------------------------------------------------------
const sf::Vector2i TextureAtlas::frameFromNumber(u32 i, const sf::Vector2i & tileSize) const
{
	return sf::Vector2i(
		i*tileSize.x % m_texture.getSize().x,
		i/(m_texture.getSize().x / tileSize.x) * tileSize.y
	);
}

} // namespace zn

