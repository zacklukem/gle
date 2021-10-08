# GLE

GL Engine (GLE) is meant to be a lightweight high level 3d graphics library for
realtime 3d rendering in modern C++.

GLE is a header-only C++ library that provides higher level abstractions for
3d rendering using modern C++ memory management features such as smart pointers.

I created GLE partly as a learning experience, but also because I wanted a
simple, modern 3d rendering library that allowed me to play just experiment.
As such, GLE is not designed for large serious production environments, but
you are free to use it as you wish.

Right now GLE is pretty sparse in terms of features, as the project just
started. In addition, since it hasn't used it for any major projects yet the
overall architecture may change drastically at any moment once I start using it
more.

The project is barely in infant stage right now, and I am not a seasoned GL
developer, so suggestions and contributions are welcome!

## Usage

GLE is a header only library, so to use it simply include it in your project
along with its dependencies.

GLE requires [GLM](https://github.com/g-truc/glm), [GLFW](https://www.glfw.org),
and [GLAD](https://glad.dav1d.de) to use in your project.

## Examples

```cpp
// Setup our scene
auto scene = std::make_shared<gle::Scene>();

// Use a basic solid color shader
auto solid_shader =
    std::make_shared<gle::SolidColorShader>();

auto red_material =
    std::make_shared<gle::SolidColorMaterial>(glm::vec3(1, 0, 0));

// Create an ico-sphere mesh with 3 subdivisions
auto sphere_mesh = gle::make_ico_sphere_mesh(3);

// Create an object with the green shader and the sphere mesh
auto sphere_object = std::make_shared<gle::Object>(
  /* shader */   solid_shader,
  /* material */ red_material,
  /* mesh */     sphere_mesh,
  /* position */ glm::vec3(3, 1, 2),
  /* rotation */ glm::vec3(0),
  /* scale */    glm::vec3(0.7));

// Create a directional light for our scene
auto light = std::make_shared<gle::Light>(
  /* type */      gle::DIRECTIONAL_LIGHT,
  /* position */  glm::vec3(0),
  /* direction */ glm::vec3(-1, -1, -5),
  /* color */     glm::vec3(1),
  /* intensity */ 0.7);

// Create a camera
auto camera = std::make_shared<gle::Camera>(
    /* position */ glm::vec3(5, 5, 5),
    /* up */       glm::vec3(0, 1, 0),
    /* look at */  glm::vec3(0, 0, 0),
    /* aspect */   720.0f / 480.0f,
    /* FOV */      glm::radians(45.0f),
    /* z_near */   0.1f,
    /* z_far */    100.0f);

scene->add_object(sphere_object);
scene->add_light(light);
scene->camera(camera);

// Create a 720 by 480px window
auto window = gle::Window("Basic Example", 720, 480);

// Create a new render pass using the object render pass to render a list of
// objects
auto render_pass = std::make_shared<gle::ObjectRenderPass>();
// Add our render pass to the window
window.add_render_pass(render_pass);

// Initialize opengl and the glfw window
window.init(scene);
// Start the rendering loop
window.start(scene);

// Everything is either a smart pointer or a regular object on the stack, so the
// destructors are called here and all the OpenGL stuff is cleaned up
// automagicly
```
