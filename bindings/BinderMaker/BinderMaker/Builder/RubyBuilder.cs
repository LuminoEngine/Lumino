using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * Ruby のクラスは以下の要素でワンセット
 * ・フィールドを保持する struct (wrapStruct)
 * ・rb_define_class_under() で登録したクラスを保持するグローバル変数
 * ・メモリ確保関数 (alloc)
 *      alloc → rubyオブジェクトにラップ(mark、delete 登録) → 0クリア の定型文
 * ・メモリ解放・デストラクト関数 (delete)
 *      RefObj なら Release → free
 * ・GCマーク関数 (mark)
 *      wrapStruct の VALUE 型を mark
 * ×初期化関数 (initialize)
 *      コンストラクタ扱い。必須ではない
 *      
 * ■制限事項
 *      LSprite_Create() 等、コンストラクタの引数でプロパティに格納するべき値を受け取っても格納しない。
 *      そのままだと LSprite_GetTexture() で取得するタイミングで新しい Texture クラスが作られることになる。
 *      とりあえず、
 *      ・get を使う機会自体ほとんど無い
 *      ・インスタンス生成による new は初回の GetTexuter() 呼び出し時のみ
 *      ということで、現状維持とする。
 *      ただし、== と eql? はオーバーライドしておいて、Handle が一致すれば同一オブジェクトとする。
 *      → というか、他の言語も含めて全体的にこんな感じの仕様でいい気がする。
 *          C言語側内部で Handle が変わった時でも、常に最新にできる。
 */

namespace BinderMaker.Builder
{
    //class RubyBuilder : Builder
    //{

    //}
}
