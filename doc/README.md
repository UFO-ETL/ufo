UFO Core
=============

Setup
---------------------
UFO Core is the original UFO client and it builds the backbone of the network. It downloads and, by default, stores the entire history of UFO transactions, which requires a few hundred gigabytes of disk space. Depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

To download UFO Core, visit [UFOcore.org](https://UFOcore.org/en/download/).

Running
---------------------
The following are some helpful notes on how to run UFO Core on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/UFO-qt` (GUI) or
- `bin/UFOd` (headless)

### Windows

Unpack the files into a directory, and then run UFO-qt.exe.

### macOS

Drag UFO Core to your applications folder, and then run UFO Core.

### Need Help?

* See the documentation at the [UFO Wiki](https://en.UFO.it/wiki/Main_Page)
for help and more information.
* Ask for help on [#UFO](https://webchat.freenode.net/#UFO) on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#UFO).
* Ask for help on the [UFOTalk](https://UFOtalk.org/) forums, in the [Technical Support board](https://UFOtalk.org/index.php?board=4.0).

Building
---------------------
The following are developer notes on how to build UFO Core on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [FreeBSD Build Notes](build-freebsd.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [NetBSD Build Notes](build-netbsd.md)
- [Gitian Building Guide (External Link)](https://github.com/UFO-core/docs/blob/master/gitian-building.md)

Development
---------------------
The UFO repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Productivity Notes](productivity.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://doxygen.UFOcore.org/)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [JSON-RPC Interface](JSON-RPC-interface.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Resources
* Discuss on the [UFOTalk](https://UFOtalk.org/) forums, in the [Development & Technical Discussion board](https://UFOtalk.org/index.php?board=6.0).
* Discuss project-specific development on #UFO-core-dev on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#UFO-core-dev).
* Discuss general UFO development on #UFO-dev on Freenode. If you don't have an IRC client, use [webchat here](https://webchat.freenode.net/#UFO-dev).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [UFO.conf Configuration File](UFO-conf.md)
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [Reduce Memory](reduce-memory.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [ZMQ](zmq.md)
- [PSBT support](psbt.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
