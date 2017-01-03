[![Build Status](https://travis-ci.org/mgerhardy/simpleai.svg?branch=master)](https://travis-ci.org/mgerhardy/simpleai)
[![Build status](https://ci.appveyor.com/api/projects/status/lo1co9g9xfxft4ui?svg=true)](https://ci.appveyor.com/project/mgerhardy/simpleai)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/d549361de7964845984a6e505379ba33)](https://www.codacy.com/app/martin-gerhardy/simpleai?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=mgerhardy/simpleai&amp;utm_campaign=Badge_Grade)

See the [doxygen documentation](http://mgerhardy.github.io/simpleai/) for a detailed description.

"External" dependencies are lua and tinyxml2 (both are included and can be disabled).

There is a small example application that can be used for testing and running behaviour trees.
It's called simpleiai-run.

The remote debugger network protocol can be inspected in wireshark with the included dissector
lua script.

The project is known to compile with:
* clang 4.x
* gcc 6.x
* msvc 2015
