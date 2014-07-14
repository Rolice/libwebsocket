libwebsocket
============

Experimental C++ WebSocket Server Library


The library is being developed and tested on *Raspberry Pi*. It is experimental and in process of implementation against [**RFC 6455**](http://tools.ietf.org/html/rfc6455).

Simple Roadmap
--------------

The current status of the project is that the library is making successful Handshake with clients (browsers). Now the WebSocket frames are being implemented, but they are lacking valuable documentation, so I refer to outer repos.

The library is being developed for another executable, again, for *Raspberry Pi*, where the threading is handled, but it might worth to pass it along with polling mechanism here in this project, in order to encapsulate the whole protocol and process of WebSocket communication.

The library is currently under development for one platform, but it might be easily ported in future. There might be a Windows implementation as well.

Inspiration and More Information + Code
---------------------------------------

Here are some repositories I have inspected and borrowed some ideas, code or flow from:

* https://github.com/katzarsky/WebSocket
* https://github.com/WebKit/webkit


Actually the project is three days old, and idea for separate library came today when part of its code was put here. Sadly, most of the time was spent in understanding of the RFC implementaion of the WebSockets Protocol.

You may find the code looking ugly or incomplete, possibly true. Also you might be wondering why library lacks implementation for C linkage? These improvements and additions will probably appear in time with the advance of the project.

Any advices and suggestions are welcome!
Also you can feel free to join the project!
