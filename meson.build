project('press', 'cpp',
    version : '0.1',
    default_options : ['warning_level=3', 'cpp_std=c++20']
)

global_args = ['-Werror=return-type']
lib_args = [global_args, '-DBUILDING_PRESS']

asio = dependency('asio')

press = library('press', 
    'src/Server.cxx',
    'src/Connection.cxx',
    'src/Endpoint.cxx',
    'src/Route.cxx',
    'src/http/http.cxx',
    'src/http/Request.cxx',
    'src/http/Response.cxx',
    install: true,
    cpp_args: lib_args,
    dependencies: [
        asio
    ]
)

# Make this library usable as a Meson subproject.
press_dep = declare_dependency(
    include_directories: include_directories('./include'),
    link_with: press
)

param = executable(
    'param', 
    'examples/param.cxx',
    cpp_args: global_args,
    dependencies: [
        asio, press_dep
    ]
)
# Make this library usable from the system's
# package manager.
install_headers('include/press.hxx', subdir: 'press')
