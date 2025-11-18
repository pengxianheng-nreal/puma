from conan import ConanFile

class Project(ConanFile):

    # all project are the same:
    python_requires = "project_base/1.0"
    python_requires_extend = "project_base.ProjectBase"

    def init(self):
        base = self.python_requires["project_base"].module.ProjectBase
        self.settings = base.settings
        self.options.update(base.options, base.default_options)
        self.revision_mode = base.revision_mode

    # difference between project:
    def requirements(self):
        self.requires(super().override_require("framework/develop"), run=True)
        self.requires("doctest/2.4.11")
        # self.requires("eigen/3.3.7")


    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "puma")
        self.cpp_info.set_property("cmake_target_name", "puma::puma")
        self.cpp_info.libs = ["puma_static"]
