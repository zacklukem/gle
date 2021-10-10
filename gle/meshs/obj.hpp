#ifndef GLE_MESHS_OBJ_HPP
#define GLE_MESHS_OBJ_HPP

#include <gle/common.hpp>
#include <gle/mesh.hpp>
#include <istream>
#include <memory>

GLE_NAMESPACE_BEGIN

/// @brief Load an obj file to a mesh from a file path
///
/// @param source
/// @return std::shared_ptr<Mesh>
inline std::shared_ptr<Mesh> load_obj_from_file(const std::string &file);

/// @brief Load an obj file to a mesh
///
/// @param source
/// @return std::shared_ptr<Mesh>
inline std::shared_ptr<Mesh> load_obj(const std::string &source);

/// @brief Load an obj file to a mesh
///
/// @param source
/// @return std::shared_ptr<Mesh>
inline std::shared_ptr<Mesh> load_obj(std::istream &source);

GLE_NAMESPACE_END

#endif // GLE_MESHS_OBJ_HPP
