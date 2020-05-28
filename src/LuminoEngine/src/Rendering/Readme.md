Rendering
==========


```plantuml
frame Data {
    [RenderElement]
    [RenderStage]
    [RenderElementList]
}

frame DataBulder {
    [RenderingContext]
    [DrawElementListBuilder]
}

frame Renderer {
    [SceneRenderer]
    [Renderfeature]
}
```



改善案
----------
- [ ] RenderingContext を、ある程度ステートマシンやめる。
	特に、Framebuffer 関係と Material,RenderState 関係は分離したい。
	各種 draw は必ず material を指定し、setBlendState や setMaterial は廃止したい。
	・・・ただ、簡単さがなくなるのでそこはちょっと考える。
	ゲーム記述用のものなのか、内部用途がメインなのかでわかれそう。
	というより
	・間違いを防ぎ正しく書くためのAPI (Vulkan とか)
	・早く実装するためのAPI (Cinder とか)
	は分けたほうがいいと思う。
	どっちつかずになると、またふらふら仕様変えることになりそう。


