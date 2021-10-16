#ifdef GLE_COMPILE_TESTS
#  include <doctest.h>
#endif
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <toml++/toml.h>
#include <unordered_map>

class SceneCompiler {
private:
  const toml::table &table;

  enum Type { VEC3, FLOAT, STRING, TEXTURE };

  static std::optional<Type> get_type(const std::string &ty) {
    if (ty == "vec3") return Type::VEC3;
    if (ty == "float") return Type::FLOAT;
    if (ty == "string") return Type::STRING;
    if (ty == "texture") return Type::TEXTURE;
    return std::nullopt;
  }

  struct ShaderInfo {
    using material_param = std::pair<Type, std::string>;
    std::vector<material_param> material_params;
    std::string material_constructor;
  };

  struct MaterialInfo {
    std::string shader_name;
  };

  std::unordered_map<std::string, ShaderInfo> shaders;
  std::unordered_map<std::string, MaterialInfo> materials;

public:
  SceneCompiler(const toml::table &table) : table(table) {}

  [[noreturn]] void err(const toml::node &node, const std::string &msg) {
    std::stringstream stream;
    stream << node.source().path << ":" << node.source().begin.line << ":"
           << node.source().begin.column << ": " << msg;
    throw std::runtime_error(stream.str());
  }

  [[noreturn]] void err(const toml::node *node, const std::string &msg) {
    err(*node, msg);
  }

  [[noreturn]] void err(const std::string &msg) {
    throw std::runtime_error(msg);
  }

  std::optional<double> parse_number(const toml::node *node) {
    if (node == nullptr) return std::nullopt;
    if (!node->is_number()) err(node, "expected a number");

    const auto &val = node->value<double>();
    return val.value();
  }

  std::optional<std::string> node_to_string(const toml::node *node) {
    if (node == nullptr) return std::nullopt;

    if (node->is_number()) {
      return std::to_string(parse_number(node).value());
    } else if (node->is_string()) {
      return node->value<std::string>();
    }

    return std::nullopt;
  }

  std::optional<std::string> parse_vec(const toml::node *node, size_t size) {
    if (node == nullptr) return std::nullopt;

    if (node->is_number()) {
      try {
        auto num = parse_number(node).value();
        std::stringstream out;
        out << "glm::vec" << size << "(";
        for (size_t i = 0; i < size; i++) {
          out << std::to_string(num);
          if (i != size - 1) out << ", ";
        }
        out << ")";
        return out.str();
      } catch (const std::runtime_error &) {
        err(node, "expected a number or array");
      }
    }

    if (!node->is_array()) err(node, "vector type must be an array or number");
    auto arr = node->as_array();

    if (arr->size() != size) err(node, "vector size is invalid");

    std::stringstream out;
    out << "glm::vec" << size << "(";

    for (size_t i = 0; i < arr->size(); i++) {
      auto value = parse_number(arr->get(i));
      if (!value.has_value()) return std::nullopt;
      out << value.value();

      if (i != arr->size() - 1) out << ", ";
    }

    out << ")";
    return out.str();
  }

  std::string to_cpp_ident(const std::string &str) {
    std::string out(str);
    for (size_t i = 0; i < str.size(); i++) {
      if (str[i] == '-') {
        out[i] = '_';
      }
    }
    return out;
  }

  std::optional<std::string> parse_reference(const toml::node *node,
                                             const char *parent) {
    if (node == nullptr) return std::nullopt;

    if (!node->is_string()) err(node, "reference must be a string");
    auto ref_name = node->as_string()->get();

    auto dot_pos = ref_name.find_first_of('.');
    const auto &parent_name = ref_name.substr(0, dot_pos);
    if (parent_name != parent) err(node, "invalid reference parent");

    const auto &after = ref_name.substr(dot_pos + 1);
    if (after.find_first_of('.') != std::string::npos)
      err(node, "reference must only have one '.'");

    const auto &val = table[parent_name][after];
    if (!val) err(node, "invalid reference");

    return to_cpp_ident(after) + '_' + parent_name;
  }

  std::optional<std::string> parse_type(const toml::node *node, Type type) {
    switch (type) {
    case Type::VEC3:
      return parse_vec(node, 3);
    case Type::FLOAT: {
      auto num = parse_number(node);
      if (num.has_value()) return std::to_string(num.value());
      return std::nullopt;
    }
    case Type::TEXTURE:
      return parse_reference(node, "texture");
    default:
      return std::nullopt;
    }
  }

  std::string parse_textures() {
    if (!table.contains("texture")) return "";

    auto textures_ukn = table.get("texture");
    if (!textures_ukn->is_table()) err(textures_ukn, "Expected a table");
    const auto &textures = *textures_ukn->as_table();

    std::stringstream out;
    for (const auto &[key, tex] : textures) {
      if (!tex.is_table()) err(tex, "texture must be a table");

      auto varname = to_cpp_ident(key) + "_texture";

      const auto &texture = *tex.as_table();

      auto file = texture.get_as<std::string>("file");
      if (file == nullptr) err(tex, "Texture must contain a string file field");

      out << "auto &" << varname
          << " = scene->make_texture<gle::ImageTexture>(\"" << file->get()
          << "\");\n";
    }
    return out.str();
  }

  std::string parse_meshs() {
    if (!table.contains("mesh")) return "";

    auto meshs_ukn = table.get("mesh");
    if (!meshs_ukn->is_table()) err(meshs_ukn, "Expected a table");
    const auto &meshs = *meshs_ukn->as_table();

    std::stringstream out;
    for (const auto &[key, mesh_v] : meshs) {
      if (!mesh_v.is_table()) err(mesh_v, "mesh must be a table");

      auto varname = to_cpp_ident(key) + "_mesh";

      auto mesh = mesh_v.as_table();

      out << "auto &" << varname << " = scene->mesh(";
      if (mesh->contains("file")) {
        auto file = mesh->get_as<std::string>("file");
        out << "gle::load_obj_from_file(\"" << file->get() << "\")";
      } else if (mesh->contains("gen")) {
        auto gen = mesh->get_as<std::string>("gen");

        auto args_v = mesh->get("args");
        std::stringstream args_s;
        if (args_v != nullptr) {
          if (!args_v->is_array()) err(args_v, "gen args must be an array");
          auto args = args_v->as_array();

          for (size_t i = 0; i < args->size(); i++) {
            auto arg = args->get(i);
            args_s << node_to_string(arg).value();

            if (i != args->size() - 1) args_s << ", ";
          }
        }
        out << gen->get() << "(" << args_s.str() << ")";
      }
      out << ");\n";
    }

    return out.str();
  }

  std::string parse_shaders() {
    if (!table.contains("shader")) return "";

    auto shaders_ukn = table.get("shader");
    if (!shaders_ukn->is_table()) err(shaders_ukn, "Expected a table");
    const auto &shaders = *shaders_ukn->as_table();

    std::stringstream out;
    for (const auto &[key, shdr] : shaders) {
      if (!shdr.is_table()) err(shdr, "shader must be a table");
      const auto &shader = *shdr.as_table();

      auto varname = to_cpp_ident(key) + "_shader";

      auto constructor_p = shader.get_as<std::string>("constructor");
      if (constructor_p == nullptr) err(shdr, "shader must have constructor");
      const auto &constructor = constructor_p->get();

      out << "auto &" << varname << " = scene->make_shader<" << constructor
          << ">();\n";

      auto material_p = shader.get_as<std::string>("material");
      if (material_p == nullptr)
        err(shdr, "shader must have material constructor");
      const auto &material = material_p->get();

      auto material_args_p = shader.get_as<toml::array>("material_args");
      if (material_args_p == nullptr)
        err(shdr, "shader must have material args");

      std::vector<ShaderInfo::material_param> mat_params;
      for (const auto &arg : *material_args_p) {
        auto arg_p = arg.as<std::string>();
        if (arg_p == nullptr) err(arg, "must be string");
        const auto &arg_s = arg_p->get();

        auto space_index = arg_s.find_last_of(' ');
        if (space_index == std::string::npos)
          err(arg, "must be formatted as: '<type> <name>'");

        const auto &arg_ty_o = get_type(arg_s.substr(0, space_index));
        if (!arg_ty_o.has_value()) err(arg, "invalid type name");

        const auto &arg_ty = arg_ty_o.value();
        const auto &arg_name = arg_s.substr(space_index + 1);

        mat_params.push_back(std::make_pair(arg_ty, arg_name));
      }
      this->shaders[varname] = {mat_params, material};
    }
    return out.str();
  }

  std::string parse_materials() {
    if (!table.contains("material")) return "";

    auto materials_ukn = table.get("material");
    if (!materials_ukn->is_table()) err(materials_ukn, "Expected a table");
    const auto &materials = *materials_ukn->as_table();

    std::stringstream out;
    for (const auto &[key, mat] : materials) {
      if (!mat.is_table()) err(mat, "material must be a table");

      auto varname = to_cpp_ident(key) + "_material";

      const auto &material = *mat.as_table();

      auto shader_o = parse_reference(material.get("shader"), "shader");
      if (!shader_o.has_value()) err(mat, "Material must have shader key");
      auto shader = shader_o.value();

      const auto &shader_info = this->shaders[shader];

      out << "auto &" << varname << " = scene->make_material<"
          << shader_info.material_constructor << ">(";

      for (size_t i = 0; i < shader_info.material_params.size(); i++) {
        const auto &material_param = shader_info.material_params[i];
        const auto &param = parse_type(material.get(material_param.second),
                                       material_param.first);
        if (!param.has_value())
          err(mat, "material is missing required parameter");
        out << param.value();
        if (i != shader_info.material_params.size() - 1) out << ", ";
      }
      out << ");\n";

      this->materials[varname] = {shader};
    }
    return out.str();
  }

  std::string parse_lights() {
    if (!table.contains("light")) return "";

    auto lights_ukn = table.get("light");
    if (!lights_ukn->is_table()) err(lights_ukn, "Expected a table");
    const auto &lights = *lights_ukn->as_table();

    std::stringstream out;
    for (const auto &[key, lht] : lights) {
      if (!lht.is_table()) err(lht, "light must be a table");

      auto varname = to_cpp_ident(key) + "_light";

      const auto &light = *lht.as_table();

      auto type_p = light.get_as<std::string>("type");
      auto type_s = type_p == nullptr ? "directional" : type_p->get();
      auto type = "";
      if (type_s == "directional")
        type = "gle::DIRECTIONAL_LIGHT";
      else if (type_s == "point")
        type = "gle::POINT_LIGHT";
      else
        err(type_p, "invalid light type");
      const auto &origin =
          parse_vec(light.get("origin"), 3).value_or("glm::vec3(0, 0, 0)");
      const auto &direction =
          parse_vec(light.get("direction"), 3).value_or("glm::vec3(-1)");
      const auto &color =
          parse_vec(light.get("color"), 3).value_or("glm::vec3(1)");
      auto strength = parse_number(light.get("strength")).value_or(1.0);

      out << "auto &" << varname << " = scene->make_light(" << type << ", "
          << origin << ", " << direction << ", " << color << ", " << strength
          << ");\n";
    }
    return out.str();
  }

  std::string parse_objects() {
    if (!table.contains("object")) return "";

    auto objects_ukn = table.get("object");
    if (!objects_ukn->is_table()) err(objects_ukn, "Expected a table");
    const auto &objects = *objects_ukn->as_table();

    std::stringstream out;
    for (const auto &[key, obj] : objects) {
      if (!obj.is_table()) err(obj, "object must be a table");

      auto varname = to_cpp_ident(key) + "_object";

      const auto &object = *obj.as_table();

      const auto &material_o =
          parse_reference(object.get("material"), "material");
      if (!material_o.has_value()) err(obj, "object must have material");
      auto &material = material_o.value();
      auto shader = materials.at(material).shader_name;

      const auto &mesh_o = parse_reference(object.get("mesh"), "mesh");
      if (!mesh_o.has_value()) err(obj, "object must have mesh");
      const auto &mesh = mesh_o.value();

      const auto &position =
          parse_vec(object.get("position"), 3).value_or("glm::vec3(0)");
      const auto &rotation =
          parse_vec(object.get("rotation"), 3).value_or("glm::vec3(0)");
      const auto &scale =
          parse_vec(object.get("scale"), 3).value_or("glm::vec3(1)");

      out << "auto &" << varname << " = scene->make_object(" << shader << ", "
          << material << ", " << mesh << ", " << position << ", " << rotation
          << ", " << scale << ");\n";
    }
    return out.str();
  }

  std::string parse_camera() {
    if (!table.contains("camera")) return "";

    auto camera_ukn = table.get("camera");
    if (!camera_ukn->is_table()) err(camera_ukn, "Expected a table");
    const auto &camera = *camera_ukn->as_table();

    auto position =
        parse_vec(camera.get("position"), 3).value_or("glm::vec3(0)");
    auto direction =
        parse_vec(camera.get("direction"), 3).value_or("glm::vec3(0)");
    auto aspect =
        std::to_string(parse_number(camera.get("aspect")).value_or(1.5));
    auto fov =
        std::to_string(parse_number(camera.get("fov")).value_or(M_PI / 4));
    auto z_near =
        std::to_string(parse_number(camera.get("z_near")).value_or(0.1));
    auto z_far =
        std::to_string(parse_number(camera.get("z_far")).value_or(100));

    return "scene->make_camera(" + position + ", glm::vec3(0, 1, 0), " +
           direction + ", " + aspect + ", " + fov + ", " + z_near + ", " +
           z_far + ");\n";
  }

  std::string parse() {
    std::stringstream out;
    out << "// Generated file.\n";
    out << "#include <memory>\n";
    out << "#include <glm/glm.hpp>\n";
    out << "#include <gle/gle.hpp>\n";
    out << R"(#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-variable"
#elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-variable"
#endif
)";
    out << "std::unique_ptr<gle::Scene> make_scene() {\n";
    out << "auto scene = std::make_unique<gle::Scene>();\n";
    out << "// SHADERS\n";
    out << parse_shaders() << "\n// TEXTURES\n";
    out << parse_textures() << "\n// MATERIALS\n";
    out << parse_materials() << "\n// MESHS\n";
    out << parse_meshs() << "\n// OBJECTS\n";
    out << parse_objects() << "\n// LIGHTS\n";
    out << parse_lights() << "\n// CAMERA\n";
    out << parse_camera() << "\n";
    out << "return scene;\n";
    out << "}\n";
    out << R"(#if defined(__clang__)
#  pragma clang diagnostic pop
#elif defined(__GNUC__)
#  pragma GCC diagnostic pop
#endif
)";
    size_t h = std::hash<std::string>{}(out.str());
    std::stringstream guarded;
    guarded << "#ifndef _" << std::hex << h << "\n";
    guarded << "#define _" << std::hex << h << "\n";
    guarded << out.str();
    guarded << "\n#endif";
    return guarded.str();
  }
};

#ifndef GLE_COMPILE_TESTS
void print_usage() {
  std::cout << "usage: glescene <toml-scene-file> [-o file]" << std::endl;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_usage();
    return 1;
  }
  auto input = std::string(argv[1]);
  if (input == "--help" || input == "-h") {
    print_usage();
    return 0;
  }
  auto final_dot = input.find_last_of('.');
  auto basename = input.substr(0, final_dot);
  auto extension = input.substr(final_dot + 1, std::string::npos);
  if (extension != "toml") {
    std::cout << input << " is not a toml file!" << std::endl;
    return 1;
  }
  auto output = basename + ".scene.hpp";
  if (argc > 3 && std::string(argv[2]) == "-o") output = argv[3];
  try {
    auto table = toml::parse_file(input);
    auto sc = SceneCompiler(table);
    std::ofstream out(output, std::ios_base::out | std::ios_base::trunc);
    out << sc.parse();
    out.close();
  } catch (const toml::parse_error &err) {
    std::cerr << "Parsing failed:\n" << err << std::endl;
    return 1;
  } catch (const std::runtime_error &err) {
    std::cerr << "Failed:\n" << err.what() << std::endl;
    return 1;
  }
  return 0;
}
#endif

// TESTS
#ifdef GLE_COMPILE_TESTS
TEST_CASE("parse_number") {
  auto tbl = toml::parse(R"(
    integer = 32
    double = 45.67
    not_number = "hi"
  )");
  auto sc = SceneCompiler(tbl);
  SUBCASE("parses integers") {
    CHECK(sc.parse_number(tbl.get("integer")).value() == 32.0);
  }
  SUBCASE("parses doubles") {
    CHECK(sc.parse_number(tbl.get("double")) == 45.67);
  }
  SUBCASE("none if non existant") {
    CHECK(!sc.parse_number(tbl.get("nope")).has_value());
  }
  SUBCASE("errors if not a number") {
    CHECK_THROWS_WITH_AS(sc.parse_number(tbl.get("not_number")),
                         "0x0:4:18: expected a number", std::runtime_error);
  }
}

TEST_CASE("parse_vec") {
  auto tbl = toml::parse(R"(
    vec4 = [0, 0, 0, 0]
    vec3 = [0, 0, 0]
    vec2 = [0, 0]
    vec3_all = 1.23
    vec2_str = [0, "hi"]
  )");
  auto sc = SceneCompiler(tbl);
  SUBCASE("parses vec4") {
    CHECK(sc.parse_vec(tbl.get("vec4"), 4) == "glm::vec4(0, 0, 0, 0)");
  }
  SUBCASE("parses vec3") {
    CHECK(sc.parse_vec(tbl.get("vec3"), 3) == "glm::vec3(0, 0, 0)");
  }
  SUBCASE("parses vec2") {
    CHECK(sc.parse_vec(tbl.get("vec2"), 2) == "glm::vec2(0, 0)");
  }
  SUBCASE("fills vec on scalar") {
    CHECK(sc.parse_vec(tbl.get("vec3_all"), 3).value() ==
          "glm::vec3(1.230000, 1.230000, 1.230000)");
  }
  SUBCASE("fails on invalid size") {
    CHECK_THROWS_WITH_AS(sc.parse_vec(tbl.get("vec2"), 3),
                         "0x0:4:12: vector size is invalid",
                         std::runtime_error);
  }
  SUBCASE("fails on non-number data") {
    CHECK_THROWS_WITH_AS(sc.parse_vec(tbl.get("vec2_str"), 2),
                         "0x0:6:20: expected a number", std::runtime_error);
  }
  SUBCASE("fails on non-existant data") {
    CHECK(!sc.parse_vec(tbl.get("vec2_str_nope"), 2).has_value());
  }
}

TEST_CASE("parse_textures") {
  SUBCASE("parses textures") {
    auto tbl = toml::parse(R"(
      [texture.stone_color]
      file = "examples/1K/COL_1K.jpg"

      [texture.stone_normal]
      file = "examples/1K/NRM_1K.jpg"
    )");
    auto sc = SceneCompiler(tbl);
    CHECK(
        sc.parse_textures() ==
        "auto &stone_color_texture = "
        "scene->make_texture<gle::ImageTexture>(\"examples/1K/COL_1K.jpg\");\n"
        "auto &stone_normal_texture = "
        "scene->make_texture<gle::ImageTexture>(\"examples/1K/"
        "NRM_1K.jpg\");\n");
  }
  SUBCASE("fails if textures have no file") {
    auto tbl = toml::parse(R"(
      [texture.stone_color]
      file = "examples/1K/COL_1K.jpg"

      [texture.stone_normal]
    )");
    auto sc = SceneCompiler(tbl);
    CHECK_THROWS_WITH_AS(sc.parse_textures(),
                         "0x0:5:7: Texture must contain a string file field",
                         std::runtime_error);
  }
  SUBCASE("fails if textures have invalid file") {
    auto tbl = toml::parse(R"(
      [texture.stone_color]
      file = "examples/1K/COL_1K.jpg"

      [texture.stone_normal]
      file = 23
    )");
    auto sc = SceneCompiler(tbl);
    CHECK_THROWS_WITH_AS(sc.parse_textures(),
                         "0x0:5:7: Texture must contain a string file field",
                         std::runtime_error);
  }
  SUBCASE("fails if texture is invalid") {
    auto tbl = toml::parse(R"(
      texture = "hi"
    )");
    auto sc = SceneCompiler(tbl);
    CHECK_THROWS_WITH_AS(sc.parse_textures(), "0x0:2:17: Expected a table",
                         std::runtime_error);
  }
}

TEST_CASE("parse_meshs") {
  auto tbl = toml::parse(R"(
    [mesh.tea_pot]
    file = "examples/teacup.obj"

    [mesh.plane]
    gen = "gle::make_plane_mesh"
    args = [5]

    [mesh.sphere]
    gen = "gle::make_sphere_mesh"
    args = [3]
  )");
  auto sc = SceneCompiler(tbl);
  CHECK(sc.parse_meshs() ==
        "auto &plane_mesh = scene->mesh(gle::make_plane_mesh(5.000000));\n"
        "auto &sphere_mesh = scene->mesh(gle::make_sphere_mesh(3.000000));\n"
        "auto &tea_pot_mesh = "
        "scene->mesh(gle::load_obj_from_file(\"examples/teacup.obj\"));\n");
}

TEST_CASE("parse_reference") {
  auto tbl = toml::parse(R"(
    ref = "texture.stone_normal"
    bad_ref = "tex.stone_normal"
    bad_ref2 = "texture.not_me"

    [texture.stone_color]
    file = "examples/1K/COL_1K.jpg"

    [texture.stone_normal]
    file = "examples/1K/NRM_1K.jpg"
  )");
  auto sc = SceneCompiler(tbl);
  SUBCASE("parses reference") {
    CHECK(sc.parse_reference(tbl.get("ref"), "texture").value() ==
          "stone_normal_texture");
  }
  SUBCASE("fails on bad parent reference") {
    CHECK_THROWS_WITH_AS(sc.parse_reference(tbl.get("bad_ref"), "texture"),
                         "0x0:3:15: invalid reference parent",
                         std::runtime_error);
  }
  SUBCASE("fails on bad child reference") {
    CHECK_THROWS_WITH_AS(sc.parse_reference(tbl.get("bad_ref2"), "texture"),
                         "0x0:4:16: invalid reference", std::runtime_error);
  }
}

TEST_CASE("parse_shaders") {
  auto tbl = toml::parse(R"(
    [shader.standard]
    constructor = "gle::StandardShader"
    material = "gle::StandardMaterial"
    material_args = ["texture albedo", "texture normal", "texture displacement",
                    "float displacement_scale", "float specular", "float diffuse"]

    [shader.solid_color]
    constructor = "gle::SolidColorShader"
    material = "gle::SolidColorMaterial"
    material_args = ["vec3 albedo", "float specular", "float diffuse"]
  )");
  auto sc = SceneCompiler(tbl);
  CHECK(sc.parse_shaders() ==
        "auto &solid_color_shader = "
        "scene->make_shader<gle::SolidColorShader>();\n"
        "auto &standard_shader = scene->make_shader<gle::StandardShader>();\n");
}

TEST_CASE("parse_materials") {
  auto tbl = toml::parse(R"(
    [shader.standard]
    constructor = "gle::StandardShader"
    material = "gle::StandardMaterial"
    material_args = ["texture albedo", "texture normal", "texture displacement",
                    "float displacement_scale", "float specular", "float diffuse"]

    [shader.solid_color]
    constructor = "gle::SolidColorShader"
    material = "gle::SolidColorMaterial"
    material_args = ["vec3 albedo", "float specular", "float diffuse"]

    [texture.stone_color]
    file = "examples/1K/COL_1K.jpg"

    [texture.stone_normal]
    file = "examples/1K/NRM_1K.jpg"

    [texture.stone_disp]
    file = "examples/1K/DISP_1K.jpg"

    [material.stone]
    shader = "shader.standard"
    albedo = "texture.stone_color"
    normal = "texture.stone_normal"
    displacement = "texture.stone_disp"
    displacement_scale = 0.03
    specular = 1.0
    diffuse = 1.0

    [material.tea_pot]
    shader = "shader.solid_color"
    albedo = [1, 1, 1]
    specular = 1.0
    diffuse = 1.0
  )");
  auto sc = SceneCompiler(tbl);
  sc.parse_shaders();
  sc.parse_textures();
  CHECK(sc.parse_materials() ==
        "auto &stone_material = "
        "scene->make_material<gle::StandardMaterial>(stone_color_texture, "
        "stone_normal_texture, "
        "stone_disp_texture, 0.030000, 1.000000, 1.000000);\n"
        "auto &tea_pot_material = "
        "scene->make_material<gle::SolidColorMaterial>(glm::vec3(1, 1, 1), "
        "1.000000, 1.000000);\n");
}

TEST_CASE("parse_lights") {
  auto tbl = toml::parse(R"(
    [light.main]
    type = "directional"
    origin = [0, 0, 0]
    direction = [-1, -1, -1]
    color = [1, 1, 1]
    strength = 1.0
  )");
  auto sc = SceneCompiler(tbl);
  CHECK(sc.parse_lights() ==
        "auto &main_light = scene->make_light(gle::DIRECTIONAL_LIGHT, "
        "glm::vec3(0, 0, 0), glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), 1);\n");
}

TEST_CASE("parse_objects") {
  auto tbl = toml::parse(R"(
    [shader.standard]
    constructor = "gle::StandardShader"
    material = "gle::StandardMaterial"
    material_args = ["texture albedo", "texture normal", "texture displacement",
                    "float displacement_scale", "float specular", "float diffuse"]

    [shader.solid_color]
    constructor = "gle::SolidColorShader"
    material = "gle::SolidColorMaterial"
    material_args = ["vec3 albedo", "float specular", "float diffuse"]

    [texture.stone_color]
    file = "examples/1K/COL_1K.jpg"

    [texture.stone_normal]
    file = "examples/1K/NRM_1K.jpg"

    [texture.stone_disp]
    file = "examples/1K/DISP_1K.jpg"

    [material.stone]
    shader = "shader.standard"
    albedo = "texture.stone_color"
    normal = "texture.stone_normal"
    displacement = "texture.stone_disp"
    displacement_scale = 0.03
    specular = 1.0
    diffuse = 1.0

    [material.tea_pot]
    shader = "shader.solid_color"
    albedo = [1, 1, 1]
    specular = 1.0
    diffuse = 1.0

    [mesh.tea_pot]
    file = "examples/teacup.obj"

    [mesh.plane]
    gen = "gle::make_plane_mesh"
    args = [5]

    [mesh.sphere]
    gen = "gle::make_sphere_mesh"
    args = [3]

    [object.sphere]
    material = "material.stone"
    mesh = "mesh.sphere"
    position = [-1, 1, 1]
    scale = 0.7

    [object.ground]
    material = "material.stone"
    mesh = "mesh.plane"
    position = [-10, 0, -10]
    scale = [20, 1, 20]

    [object.tea_pot]
    material = "material.tea_pot"
    mesh = "mesh.tea_pot"
    position = [-3, 0, -2]
    rotation = [45, 0, 23]
  )");
  auto sc = SceneCompiler(tbl);
  sc.parse_shaders();
  sc.parse_textures();
  sc.parse_materials();
  sc.parse_meshs();
  CHECK(sc.parse_objects() ==
        "auto &ground_object = scene->make_object(standard_shader, "
        "stone_material, plane_mesh, glm::vec3(-10, 0, -10), glm::vec3(0), "
        "glm::vec3(20, 1, 20));\n"
        "auto &sphere_object = scene->make_object(standard_shader, "
        "stone_material, sphere_mesh, glm::vec3(-1, 1, 1), glm::vec3(0), "
        "glm::vec3(0.700000, 0.700000, 0.700000));\n"
        "auto &tea_pot_object = scene->make_object(solid_color_shader, "
        "tea_pot_material, tea_pot_mesh, glm::vec3(-3, 0, -2), glm::vec3(45, "
        "0, 23), glm::vec3(1));\n");
}

TEST_CASE("parse_camera") {
  auto tbl = toml::parse(R"(
    [camera]
    position = [10, 5, 10]
    direction = [-10, -5, -10]
    aspect = 1.5
    fov = 45
    z_near = 0.1
    z_far = 100
  )");
  auto sc = SceneCompiler(tbl);
  CHECK(sc.parse_camera() == "scene->make_camera(glm::vec3(10, 5, 10), "
                             "glm::vec3(0, 1, 0), glm::vec3(-10, -5, -10), "
                             "1.500000, 45.000000, 0.100000, 100.000000);\n");
}
#endif