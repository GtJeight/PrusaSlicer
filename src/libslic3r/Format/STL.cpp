#include "../libslic3r.h"
#include "../Model.hpp"
#include "../TriangleMesh.hpp"

#include "STL.hpp"

#include <string>

#ifdef _WIN32
#define DIR_SEPARATOR '\\'
#else
#define DIR_SEPARATOR '/'
#endif

namespace Slic3r {

bool load_stl(const char *path, Model *model, const char *object_name_in)
{
    const char *path1 = "D:\\source\\PrusaSlicer\\build\\src\\Release\\resources\\shapes\\cylinder.stl";
    TriangleMesh cymesh;
    if (!cymesh.ReadSTLFile(path1)) {
        std::cout << "cymesh load failed!" << std::endl << std::endl;
    }
    if (cymesh.empty()) {
        std::cout << "cymesh empty!" << std::endl << std::endl;
    }
    std::string cyname = "cylin1";
    model->add_object(cyname.c_str(), path1, std::move(cymesh));

    std::cout << "STL.cpp: load_stl: " << std::endl
              << path << std::endl
              << ((object_name_in == nullptr) ? "null" : (object_name_in))
              << std::endl
              << std::endl;
    TriangleMesh mesh;
    if (! mesh.ReadSTLFile(path)) {
//    die "Failed to open $file\n" if !-e $path;
        return false;
    }
    if (mesh.empty()) {
        // die "This STL file couldn't be read because it's empty.\n"
        return false;
    }

    std::string object_name;
    if (object_name_in == nullptr) {
        const char *last_slash = strrchr(path, DIR_SEPARATOR);
        object_name.assign((last_slash == nullptr) ? path : last_slash + 1);
    } else
       object_name.assign(object_name_in);

    model->add_object(object_name.c_str(), path, std::move(mesh));
    std::cout << "STL.cpp: objects count: " << model->objects.size()
              << std::endl
              << std::endl;
    return true;
}

bool store_stl(const char *path, TriangleMesh *mesh, bool binary)
{
    if (binary)
        mesh->write_binary(path);
    else
        mesh->write_ascii(path);
    //FIXME returning false even if write failed.
    return true;
}

bool store_stl(const char *path, ModelObject *model_object, bool binary)
{
    TriangleMesh mesh = model_object->mesh();
    return store_stl(path, &mesh, binary);
}

bool store_stl(const char *path, Model *model, bool binary)
{
    TriangleMesh mesh = model->mesh();
    return store_stl(path, &mesh, binary);
}

}; // namespace Slic3r
