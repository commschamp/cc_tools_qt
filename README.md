# CommsChampion Tools
This project contains tool application(s), which can be used to
develop, monitor and debug custom binary communication protocols, that were
developed using the [COMMS Library](https://github.com/commschamp/comms). 
All the applications are plug-in based, i.e. plug-ins are used to define 
I/O socket, data filters, and the custom protocol itself. The tools
use [Qt](http://www.qt.io/) framework for GUI interfaces as well as loading
and managing plug-ins.

The current list of available applications is below. Please refer to the
[wiki page](https://github.com/commschamp/cc_tools_qt/wiki/How-to-Use-CommsChampion-Tools)
for tutorial on how to use them.

- **cc_view** is the main generic GUI application for visualisation and analysis of the
communication protocols.  

The **CommsChampion Tools** contain the following
plugins that can be used with any application:

- **CC Echo Socket** - Echo socket, all the data being sent is immediately reported
  as an incoming data.
- **CC NULL Socket** - NULL socket, that doesn't produce any incoming data and
  discards any outgoing data.
- **CC Serial Socket** - Low level socket that sends and receives data over serial
  (RS-232) I/O link.
- **CC SSL Client Socket** - Client secure (SSL/TLS) connection socket.
- **CC TCP/IP Client Socket** - Client TCP/IP socket, that connects to remote 
  server, sends and receives data over TCP/IP network link.
- **CC TCP/IP Proxy Socket** - Proxy server TCP/IP socket, combines Server and Client
  side of TCP/IP connection, can be used to monitor traffic of the messages between
  remote a client and a server.
- **CC TCP/IP Server Socket** - Server TCP/IP socket, waits for and accepts all
  connections from TCP/IP clients, sends and receives data to/from them.
- **CC UDP/IP Socket** - Generic (client/server) UDP/IP socket.
- **CC UDP/IP Proxy Socket** - Proxy server UDP/IP socket, combines Server and Client
  side of UDP/IP connection, can be used to monitor traffic of the messages between
  remote a client and a server.
- **CC Raw Data Protocol** - Protocol definition that defines only a single message
  type with one field of unlimited length data. It can be used to review the
  raw data being received from I/O socket.

# How to Build
In addition to the [Qt](http://www.qt.io/) framework, this project depends on the 
[COMMS Library](https://github.com/commschamp/comms) as well and tries 
to find it through invocation of the `find_package()` cmake function. There is a need to 
provide its location using **CMAKE_PREFIX_PATH** variable.
```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install/dir -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/comms/install
cmake --build . --target install
```

For Windows platfrom there may be a need to also add a Qt installation path to the **CMAKE_PREFIX_PATH** as 
well as build extra **deploy_qt** target to deploy the Qt libraries into the installation directory.
```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install/dir -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_PREFIX_PATH=/path/to/comms/install;/path/to/qt/install
cmake --build . --target install
cmake --build . --target deploy_qt    
```

The build should support both Qt5 (default) and Qt6. It is possible to force usage of 
specific Qt major version by using **CC_TOOLS_QT_MAJOR_QT_VERSION** cmake variable.
```
cmake -DCC_TOOLS_QT_MAJOR_QT_VERSION=6 ...
```

It is highly recommended to open main [CMakeLists.txt](CMakeLists.txt) file and review the available
configuration options and variables.

Since **v5.0** this project uses and requires minimum **C++17** support from the compiler.

# Developing Custom Socket/Filter/Protocol Plugin
The full tutorial as well as API documentation can be downloaded as
**doc_cc_tools_qt_vX.zip** archive from
from [release artefacts](https://github.com/commschamp/cc_tools_qt/releases) or
the latest version can be browsed [online](https://commschamp.github.io/cc_tools_qt_doc).

# Branching Model
This repository will follow the 
[Successful Git Branching Model](http://nvie.com/posts/a-successful-git-branching-model/).

The **master** branch will always point to the latest release, the
development is performed on **develop** branch. As the result it is safe
to just clone the sources of this repository and use it without
any extra manipulations of looking for the latest stable version among the tags and
checking it out.

# Contact Information
For bug reports, feature requests, or any other question you may open an issue
here in **github** or e-mail me directly to: **arobenko@gmail.com**. I usually
respond within 24 hours.
