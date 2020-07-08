VFX
==========

```plantuml
frame "Models" {
    [ParticleModel]
    [ParticleEmitterModel]
}
frame "Instances" {
    [ParticleEmitterInstance]
    [ParticleInstance]
}
frame "Renderers" {
    [ParticleRenderer]
}

[ParticleModel] - [ParticleEmitterModel]
[ParticleEmitterModel] -- [ParticleEmitterInstance]
[ParticleModel] -- [ParticleInstance]
[ParticleInstance] - [ParticleEmitterInstance]
[ParticleEmitterInstance] -- [ParticleRenderer]
```

