
```
npm start
```



NOTE
----------

```
<script type="text/javascript" src="dist/NWJSOverlap.js"></script>
lib = NWJSOverlap();
```

これだと __dirname が見つからない、というエラーになる。

nw.js 上では、ENVIRONMENT_IS_WEB と ENVIRONMENT_IS_NODE が同時に true になるため、node として処理されたことが原因のようだ。

`-s ENVIRONMENT=web,worker` を使うと、 if (ENVIRONMENT_IS_NODE) のような処理がすべて削除される。




### ccall と cwrap の違い

```js
// ccall
const result = Module.ccall('Add', 'number', ['number', 'number'], [1, 2]);

// cwrap
const add = Module.cwrap('Add', 'number', ['number', 'number']);
const result = add(4, 5);
```
