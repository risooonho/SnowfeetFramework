/*
TextureAtlas.hpp
Copyright (C) 2010-2014 Marc GILLERON
This file is part of the SnowfeetFramework project.
*/

#ifndef HEADER_ZN_TEXTUREATLAS_HPP_INCLUDED
#define HEADER_ZN_TEXTUREATLAS_HPP_INCLUDED

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <fm/types.hpp>

namespace zn
{

/// \brief Holds coordinates of sub-rectangles inside a texture.
/// It's like an UV-mapping using rectangles for sprites.
/// It is not really tied to a texture, since it's only UV data.
/// (the texture can be changed and use the same atlas)
class ZN_API TextureAtlas
{
public:

	//------------------------------
	// Structures
	//------------------------------

	struct Frame
	{
		sf::IntRect rect;
		//sf::IntRect rect2;
	};

	struct SequenceFrame
	{
		sf::IntRect rect;
		//sf::IntRect rect2;
		u32 duration = 0;
	};

	struct Sequence
	{
		std::vector<SequenceFrame> frames;
	};

	//------------------------------
	// Methods
	//------------------------------

	TextureAtlas();

	bool loadFromFile(const std::string & filePath);
	bool loadFromJSONFile(const std::string & filePath);

	//inline const sf::Texture & texture() const { return m_texture; }
	const Frame * frame(std::string id) const;
	const Sequence * sequence(std::string id) const;
	//const sf::Vector2i frameFromNumber(u32 i, const sf::Vector2i & tileSize) const;

private:

	void addFrame(std::string id, sf::IntRect rect);
	void addSequence(std::string id, const Sequence & seq);
	//bool checkFrame(const sf::IntRect & rect);

	std::unordered_map<std::string,Frame> m_frames; // single frames
	std::unordered_map<std::string,Sequence> m_sequences; // grouped frames

	//sf::Texture m_texture;

};

} // namespace zn

#endif // HEADER_ZN_TEXTUREATLAS_HPP_INCLUDED



