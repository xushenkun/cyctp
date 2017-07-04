try:
    from setuptools import setup
    from setuptools import Extension
except ImportError:
    from distutils.core import setup
    from distutils.extension import Extension
from setuptools.dist import Distribution
from setuptools import find_packages 
from Cython.Build import cythonize
from Cython.Distutils import build_ext
from distutils.sysconfig import customize_compiler
from glob import glob
import os

class BinaryDistribution(Distribution):
    def has_ext_modules(foo):
        return True

class my_build_ext(build_ext):
    def build_extensions(self):
        customize_compiler(self.compiler)
        try:
            self.compiler.compiler_so.remove("-Wstrict-prototypes")
        except (AttributeError, ValueError):
            pass
        build_ext.build_extensions(self)

ext_modules=[
    Extension("cyctp.ctp_market",
        sources=["src"+os.path.sep+"ctp_market.pyx", "src"+os.path.sep+"Util.cpp", "src"+os.path.sep+"MarketCollector.cpp"],
        include_dirs=["include"],
        library_dirs=["cyctp"+os.path.sep+"lib"],
        libraries=["thostmduserapi"],
        language="c++",
        #extra_compile_args=['-Wstrict-prototypes0'],#['-I.\include'],
        #extra_link_args=['/LIBPATH:.\lib']
    ),
    Extension("cyctp.ctp_trader",
        sources=["src"+os.path.sep+"ctp_trader.pyx", "src"+os.path.sep+"Util.cpp", "src"+os.path.sep+"TraderCollector.cpp"],
        include_dirs=["include"],
        library_dirs=["cyctp"+os.path.sep+"lib"],
        libraries=["thosttraderapi"],
        language="c++",
        #extra_compile_args=['-Wstrict-prototypes0'],#['-I.\include'],
        #extra_link_args=['/LIBPATH:.\lib']
    ),
]

setup(
    name = 'cyctp',
    version = '1.0.2',
    description = 'Cython wrapper for CTP',
    long_description = 'Cython wrapper for Comprehensive Transaction Platform',
    author = 'Xu Shenkun',
    author_email = 'xsk1983@126.com',
    license = 'MIT',
    keywords = 'cython ctp trade market stock',
    url = 'https://github.com/xushenkun/cyctp',
    download_url = 'https://github.com/xushenkun/cyctp/releases',
    packages = ["cyctp"],
    #packages = find_packages(),
    package_data={
        'cyctp': glob("lib/*") + glob("*.txt"),
    },
    include_package_data = True, 
    #data_files = {
    #    ('broker', glob("broker"+os.path.sep+'*.xml')),
    #    ('broker', glob("broker"+os.path.sep+"*"+os.path.sep+'*.xml')),
    #},
    cmdclass = {'build_ext': my_build_ext},
    ext_modules = cythonize(ext_modules),#, gdb_debug=True),       
    install_requires = ['cython'],
    distclass = BinaryDistribution,
)


#cython --gdb ctp.pyx --cplus

#python setup.py build_ext -i -f -R.\lib
