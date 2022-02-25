using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Text;
using System.Linq;
using System.ComponentModel;
using System.IO.Compression;
using System.Runtime.InteropServices;
using System.Reflection;

namespace LuminoBuild
{
    /// <summary>
    /// ビルドタスク
    /// </summary>
    /// <remarks>
    /// タスク自体は依存関係を定義しない。一般的なビルドツールとはやや異なるが。
    /// これは、特定のタスクだけを繰り返し実行できるようにするため。例えば Ruby バインダのデバッグ中、これだけビルドしなおすなど。
    /// </remarks>
    abstract class BuildTask
    {
        internal int DependLevel = 0;

        /// <summary>
        /// ルールを実行するためのコマンド名
        /// </summary>
        public abstract string CommandName { get; }

        /// <summary>
        /// 依存 Task
        /// </summary>
        [Obsolete]
        public virtual List<string> Dependencies { get { return null; } }

        /// <summary>
        /// 依存タスク。このタスクの前に実行したいタスクがあれば、名前を列挙する。
        /// </summary>
        public virtual string[] Depends { get => new string[] { }; }

        /// <summary>
        /// このルールをビルドする
        /// </summary>
        /// <param name="builder">Builder.</param>
		public abstract void Build(Build b);
	}
}
