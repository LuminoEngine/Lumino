#pragma once

namespace ln {

/** コンソール表示に関係する機能提供します。 */
class Console
{
public:
    /** アプリケーションにコンソールウィンドウを割り当てます。
     *
     * 主にデバッグ目的で使用します。
     * 既に割り当てられている場合は何もしません。
     */
    static void allocate();

private:
};

} // namespace ln

