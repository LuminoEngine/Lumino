using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    // lumino-cli で作成したプロジェクト内で、Engine/Native にリポジトリを直接配置して Lumino を使うパターンで使用する。
    // リポジトリルートに lib フォルダを作り、通常のプロジェクトと同じパスで lib を参照できるようにする。
    class CopyEngineLibsToRepoRoot : BuildTask
    {
        public override string CommandName => "CopyEngineLibsToRepoRoot";

        public override string Description => "CopyEngineLibsToRepoRoot";

        public override void Build(Builder builder)
        {
            var tempInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir);
            var nativeEngineRoot = builder.LuminoRootDir;
            MakeReleasePackage.CopyEngineLibs(builder, tempInstallDir, nativeEngineRoot);
        }
    }
}
