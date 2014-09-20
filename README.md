libwebsocket
============

Experimental C++ WebSocket Server Library


The library was originally developed and tested on *Raspberry Pi*. Now it is developed on desktop machine and it is back-ported periodically. So far it works nicely under **Raspberry Pi** with about **0% CPU load** and **0.3% memory consumption** with the testing server application [libwebsocket-server](https://github.com/Rolice/libwebsocket-server). These stats are relative, but they clearly represent the low resource usage.

**Note**: These stats may vary on other hardware; if the system has additional, third-party software installed or the library is used in other, more complex server application.

It is still experimental and in process of implementation against [**RFC 6455**](http://tools.ietf.org/html/rfc6455).



Current Status
--------------

The library now can make successful handshake with the clients and it can respond to messages. As it is still in experimental phase it is not directly usable. See the road map below.

Example Code
------------

The experimental server is now also added here in github, but in additional repo: [libwebsocket-server](https://github.com/Rolice/libwebsocket-server).

Simple Roadmap
--------------

The current road map is divided below in few points:

* Callback Mechanism
* Threading and Polling
* SSL Support
* Compression Support
* C Compatible Linkage

The library is currently under development for linux platforms. There might be a Windows implementation as well with the advance and completion of the current road map points above.

Reference
---------------------------------------

Here are some repositories I have inspected and borrowed some ideas, code or pure logical flow from:

* https://github.com/katzarsky/WebSocket
* https://github.com/WebKit/webkit
