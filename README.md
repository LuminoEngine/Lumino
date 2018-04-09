Lumino
====================

Development after 0.5.0 will continue on the `dev` branch for a while.

--------------------

Lumino is an open source framework for real-time graphics applications.
Currently, it is developing as a game engine.

[Website](https://lriki.github.io/lumino/)

[Developmental status](https://lriki.github.io/lumino/articles/downloads/DevVer.html)

[Contributing](.github/CONTRIBUTING.md)

Supported platforms
--------------------
- [x] Windows Desktop
- [ ] Mac OS X

How to build
--------------------

### Winodws
Create a development project.

```
PS> ./build.ps1 -Target MakeProjects
```
Note: It is necessary to change ExecutionPolicy. `PS> Set-ExecutionPolicy RemoteSigned`

After this, please check in the "build/..." folder.



### Mac OS X
Create a development project.

```
$ chmod +x ./build.sh
$ ./build.sh -tMakeProjects
```

After this, please check in the "build/..." folder.


