Rendering
==========

改善案
----------
- [ ] RenderingContext を、ある程度ステートマシンやめる。
	特に、Framebuffer 関係と Material,RenderState 関係は分離したい。
	各種 draw は必ず material を指定し、setBlendState や setMaterial は廃止したい。
	・・・ただ、簡単さがなくなるのでそこはちょっと考える。
	ゲーム記述用のものなのか、内部用途がメインなのかでわかれそう。
- [ ] AbstractMaterial 廃止

