Development on Emscripte
==========

Debug build
----------

Default is a `Release` build. Debug information such as stack traces cannot be displayed correctly.

Specify the `--debug` option to generate debug information.

```
dotnet run -- BuildEngine wasm32-emscripten --debug
```

Browser cache and force reload
----------

Files are cached by the browser and changes to local asset files may not be reflected.

If you are using Chrome, the following procedure is recommended for reloading.

1. Press and hold the reload button.
2. [Delete cache and Hard reload]


