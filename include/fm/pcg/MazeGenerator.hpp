#ifndef ZN_MAZEGENERATOR_HPP_INCLUDED
#define ZN_MAZEGENERATOR_HPP_INCLUDED

#include <vector>

#include <fm/types.hpp>
#include <fm/util/Array2D.hpp>
#include <fm/util/math.hpp>
#include <fm/util/Direction.hpp>

#ifdef ZN_SFML
#include <SFML/Graphics.hpp>
#endif

namespace zn
{

/// \brief 2D orthonormal maze generator
class ZN_API MazeGenerator
{
public:

	static const u32 UNVISITED_BIT  = 1 << 6; // Note: choosed 6 because directions could be 3D
	static const u32 DISABLED_BIT   = 1 << 7;

	u32 corridorLengthMin = 1;
	u32 corridorLengthMax = 8;
	f32 loopChance = 0.f;

	/// \brief Output of the generator.
	/// Each cell of the grid is a bitmask :
	/// the first 4 lower bits represent which directions are available,
	/// the 6th tells if the cell is unvisited,
	/// the 7th tells if the cell is disabled (note: you can assign it before generation to have non-rectangular mazes).
	Array2D<u32> grid;

	MazeGenerator(u32 width, u32 height);

	/// \brief Generates a maze from one starting point.
	/// All corridors will be connected to this point,
	/// and no one will make any loop.
	/// \param seedX: Seed x.
	/// \param seedY: Seed y.
	void generate(u32 seedX, u32 seedY);

#ifdef ZN_SFML

	/// \brief Bakes a texture that visually represents the generated maze.
	/// Useful for debug purpose.
	void bakeTexture(sf::Image & tex);

#endif

private:

	/// \brief This function creates loops at random in the maze by joining corridors together.
	/// \param chance: Probability for a dead-end to join.
	void connectRandomNodes(f32 chance);

	std::vector<u32> unvisitedDirections(s32 x, s32 y);

};

} // namespace zn

#endif // ZN_MAZEGENERATOR_HPP_INCLUDED


