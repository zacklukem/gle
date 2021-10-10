#ifndef GLE_MESHS_PRIMITIVES_HPP
#define GLE_MESHS_PRIMITIVES_HPP

#include <gle/common.hpp>
#include <gle/mesh.hpp>
#include <memory>

GLE_NAMESPACE_BEGIN

/// @brief Create a cube mesh with width 2
///
/// @return std::shared_ptr<Mesh>
inline std::shared_ptr<Mesh> make_cube_mesh();

/// @brief Create a ico sphere mesh with radius 1
///
/// @param subdivisions the number of ico subdivisions
/// @return std::shared_ptr<Mesh>
inline std::shared_ptr<Mesh> make_ico_sphere_mesh(int subdivisions = 0);

/// @brief Create a plane mesh
///
/// @param subdivisions the number of grid lines
/// @return std::shared_ptr<Mesh>
inline std::shared_ptr<Mesh> make_plane_mesh(int subdivisions = 1);

inline std::shared_ptr<Mesh> make_arrow();

GLE_NAMESPACE_END

#endif // GLE_MESHS_PRIMITIVES_HPP
