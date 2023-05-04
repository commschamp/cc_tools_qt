# Overview
This project also contains a set of tool applications , which can be used to 
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

- **cc_dump** is a command line utility, that recognises all the received
custom binary protocol messages and dumps them all in CSV format to standard output.
It can also record the incoming message into the file, which can be opened
later for visual analysis using **cc_view** GUI application. 
The tool has an ability to receive a file with definition of outgoing messages, 
created using **cc_view** GUI application, and send them one by one 
in parallel to dumping/recording the incoming messages.

The **CommsChampion Tools** contain the following
plugins that can be used with any application:

- **null_socket** - NULL socket, that doesn't produce any incoming data and
discards any outgoing data.
- **echo_socket** - Echo socket, all the data being sent is immediately reported
as an incoming data.
- **serial_socket** - Low level socket that sends and receives data over serial
(RS-232) I/O link.
- **tcp_client_socket** - Client TCP/IP socket, that connects to remote 
server, sends and receives data over TCP/IP network link.
- **tcp_server_socket** - Server TCP/IP socket, waits for and accepts all
connections from TCP/IP clients, sends and receives data to/from them.
- **tcp_proxy_socket** - Proxy server TCP/IP socket, combines Server and Client
side of TCP/IP connection, can be used to monitor traffic of the messages between
remote a client and a server.
- **udp_socket** - Generic (client/server) UDP/IP socket.
- **raw_data_protocol** - Protocol definition that defines only a single message
type with one field of unlimited length data. It can be used to review the
raw data being received from I/O socket.

### Developing Custom Socket/Filter/Protocol Plugin
The full tutorial as well as API documentation can be downloaded as
**doc_cc_tools_qt_vX.zip** archive from
from [release artefacts](https://github.com/commschamp/cc_tools_qt/releases).

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
