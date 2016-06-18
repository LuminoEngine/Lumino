using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class RubyContext : LangContext
    {
        public RubyContext()
            : base(LangFlags.Ruby)
        {
        }

        public override bool CheckEnabled(CLMethod method)
        {
            var ignore = new string[]
            {
                "LNGameAudio_PlayBGMMem",
                "LNGameAudio_PlayBGSMem",
                "LNGameAudio_PlayMEMem",
                "LNGameAudio_PlaySEMem",
                "LNGameAudio_PlaySE3DMem",
                "LNGameAudio_PlaySE3DMemXYZ",
                "LNSound_CreateMem",
            };
            return !ignore.Contains(method.FuncDecl.OriginalFullName);
        }
    }
}
