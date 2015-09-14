using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class OutputBuffer
    {
        public const string NewLineCode = "\r\n";

        private StringBuilder _buffer = new StringBuilder();
        private int _indentLevel = 0;
        private string _indent;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="indentLevel"></param>
        public OutputBuffer()
        {
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="indentLevel"></param>
        public OutputBuffer(int indentLevel)
        {
            _indentLevel = indentLevel;
            for (int i = 0; i < _indentLevel; i++)
                _indent += "    ";
        }

        /// <summary>
        /// 空であるか
        /// </summary>
        public bool IsEmpty
        {
            get { return _buffer.Length == 0; }
        }

        /// <summary>
        /// インデントレベルをひとつ増やす
        /// </summary>
        public OutputBuffer IncreaseIndent()
        {
            _indentLevel++;
            _indent = "";
            for (int i = 0; i < _indentLevel; i++)
                _indent += "    ";
            return this;
        }

        /// <summary>
        /// インデントレベルをひとつ減らす
        /// </summary>
        public OutputBuffer DecreaseIndent()
        {
            _indentLevel--;
            _indent = "";
            for (int i = 0; i < _indentLevel; i++)
                _indent += "    ";
            return this;
        }

        /// <summary>
        /// 文字列を追加する
        /// </summary>
        public OutputBuffer Append(string str)
        {
            _buffer.Append(str);
            return this;
        }

        /// <summary>
        /// 文字列を追加する
        /// </summary>
        public OutputBuffer Append(string format, params string[] args)
        {
            _buffer.Append(string.Format(format, args));
            return this;
        }

        /// <summary>
        /// 文字列を追加する
        /// </summary>
        public OutputBuffer Append(OutputBuffer buffer)
        {
            return Append(buffer.ToString());
        }

        /// <summary>
        /// 文字列を追加する
        /// </summary>
        public OutputBuffer AppendLine(string str)
        {
            Indent();
            _buffer.Append(str);
            NewLine();
            return this;
        }

        /// <summary>
        /// 文字列を追加する
        /// </summary>
        public OutputBuffer AppendLine(string format, params string[] args)
        {
            Indent();
            _buffer.Append(string.Format(format, args));
            NewLine();
            return this;
        }

        /// <summary>
        /// 文字列を追加する
        /// </summary>
        public OutputBuffer AppendLine(OutputBuffer buffer)
        {
            Indent();
            Append(buffer.ToString());
            NewLine();
            return this;
        }





        /// <summary>
        /// 文字列を追加する (各行の先頭をインデント)
        /// 行が \n だけの場合はインデントしない
        /// </summary>
        public OutputBuffer AppendWithIndent(string str)
        {
            //str = str.TrimEnd('\n');    // 終端改行は取り除く
            //Regex.Replace(str, "^")
            //_buffer.Append(_indent + str.Replace("\n", "\n" + _indent));
            string[] lines = str.Split(new char[] { '\n' }, StringSplitOptions.None);
            for (int i = 0; i < lines.Count(); i++)
            {
                if (lines[i].Length > 0)
                    _buffer.Append(_indent);
                _buffer.Append(lines[i]);

                // 最後の一つは改行しない
                if (i != lines.Count() - 1)
                    _buffer.Append('\n');
            }
            return this;
        }

        /// <summary>
        /// 文字列を追加する (各行の先頭をインデント)
        /// 行が \n だけの場合はインデントしない
        /// </summary>
        public OutputBuffer AppendWithIndent(string format, params string[] args)
        {
            var str = string.Format(format, args).Replace("\r", "");

            //str = str.TrimEnd('\n');    // 終端改行は取り除く
            //Regex.Replace(str, "^")
            //_buffer.Append(_indent + str.Replace("\n", "\n" + _indent));
            string[] lines = str.Split(new char[] { '\n' }, StringSplitOptions.None);
            for (int i = 0; i < lines.Count(); i++)
            {
                if (lines[i].Length > 0)
                    _buffer.Append(_indent);
                _buffer.Append(lines[i]);

                // 最後の一つは改行しない
                if (i != lines.Count() - 1)
                    _buffer.Append(NewLineCode);
            }
            return this;
        }

        public OutputBuffer Indent()
        {
            //AppendWithIndent("");
            _buffer.Append(_indent);
            return this;
        }

        /// <summary>
        /// 改行する (インデントは考慮せず単に \n を追加)
        /// </summary>
        public OutputBuffer NewLine(int count = 1)
        {
            for (int i = 0; i < count; i++)
                _buffer.Append(NewLineCode);
            return this;
        }

        /// <summary>
        /// string 取得
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return _buffer.ToString();
        }

        /// <summary>
        /// 既に文字列が存在すれば , を挿入して文字列を追加する
        /// </summary>
        public OutputBuffer AppendCommad(string str, params string[] args)
        {
            if (_buffer.Length != 0)
                _buffer.Append(", ");
            _buffer.Append(string.Format(str, args));
            return this;
        }

        public void Clear()
        {
            _buffer.Clear();
        }
    }
}
