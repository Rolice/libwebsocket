libwebsocket
============

Experimental C++ WebSocket Server Library


The library is being developed and tested on Raspberry Pi. It is experimental and in process of implementation against [**RFC 6455**](http://tools.ietf.org/html/rfc6455). Propositions and advices are welcome.

Current status is that the library is making successful Handshake with clients (browsers). Now the WebSocket frames are being implemented, but they are lacking rich documentation, so I refer to outer repos.

The library is being developed for another app, again, for Raspberry Pi, where the threading is handled, but it might worth to pass the threading and polling here and to encapsulate the process of WebSocket communication.

Here are some repositories I have observed and borrowed some code from:

* https://github.com/katzarsky/WebSocket
* https://github.com/WebKit/webkit


The project is three days old, and idea for separate library came today.
The code might look ugly and the library lacks implementation for C linkage, but it will be improved (in time) and such C linkage implementation probably may appear with the advance of the project.

You are welcome, if you would like to join in the project.
