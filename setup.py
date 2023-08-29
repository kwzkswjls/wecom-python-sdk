import os
import sys
import subprocess
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, cmake_lists_dir="."):
        Extension.__init__(self, name, sources=[])
        self.cmake_lists_dir = os.path.abspath(cmake_lists_dir)


# noinspection PyPep8Naming
class cmake_build_ext(build_ext):
    def build_extensions(self) -> None:
        try:
            out = subprocess.check_output(["cmake", "--version"])
        except OSError:
            raise RuntimeError("Cannot find CMake executable")

        for ext in self.extensions:
            ext_dir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
            cfg = "Debug" if self.debug else "Release"

            cmake_args = [
                "-DCMAKE_BUILD_TYPE=" + cfg,
                "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + ext_dir,
                "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=" + self.build_temp,
                "-DPYTHON_EXECUTABLE=" + sys.executable,
            ]

            if not os.path.exists(self.build_temp):
                os.makedirs(self.build_temp)

            subprocess.check_call(["cmake", ext.cmake_lists_dir] + cmake_args, cwd=self.build_temp)
            subprocess.check_call(["cmake", "--build", ".", "--config", cfg], cwd=self.build_temp)


setup(
    name="wecom-sdk",
    version="1.0",
    description="Python wrap for wecom C-sdk",
    author="Fei Yu",
    packages=["wecom"],
    package_dir={"wecom": "./stub"},
    package_data={"wecom": ["__init__.pyi"]},
    ext_modules=[CMakeExtension("wecom")],
    cmdclass={"build_ext": cmake_build_ext}
)
