How to build (Ruby package)
========

Install required packages
--------

### Windows

#### Chocolatey

```
choco install -y ruby
choco install -y msys2
refreshenv
ridk install 2 3
```

> msys2 is required for non-interactive execution of the risk command.
> https://community.chocolatey.org/packages/ruby


### macOS

`TBD`

### Linux

`TBD`

Build
--------

```
dotnet run -- MakePackage_Ruby
```
