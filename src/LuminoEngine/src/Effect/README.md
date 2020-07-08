VFX
==========

```plantuml
left to right direction

frame "Models" {
    [ParticleModel]
    [ParticleEmitterModel]
    [SpriteParticleGeometry]
    [MeshParticleGeometry(予定)]
}
frame "Instances" {
    [ParticleEmitterInstance]
    [ParticleInstance]
    [TrailParticleModuleInstance]
}
frame "Renderers" {
    [SpriteParticleRenderer]
    [RibbonRenderer]
}

[ParticleModel] - [ParticleEmitterModel]
[ParticleEmitterModel] -- [ParticleEmitterInstance]
[ParticleModel] -- [ParticleInstance]
[ParticleInstance] - [ParticleEmitterInstance]
[ParticleEmitterInstance] -- [SpriteParticleRenderer]
[TrailParticleModuleInstance] -- [RibbonRenderer]
```

