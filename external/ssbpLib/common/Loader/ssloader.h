/*! \mainpage 
 *
 * \section 初めに
 *
 * このドキュメントはOPTPiX SpriteStudioのデータを取り扱うためのC++ライブラリのドキュメントです。<br>
 * また、SpriteStudioから排出されるXMLデータの構造を記載するドキュメントとなります。
 *
 * \section 本SDKの構成について
 * 本SDKは、下記のライブラリが含まれております。<br>
 * ・OPTPiX SpriteStudioのネイティブのファイルフォーマットである、sspj、ssae、ssceの読み込み用ライブラリ<br>
 * ・読み込んだデータの描画再生サンプル(簡易ビューア―)<br>
 *
 * \section SpriteStudioのXMLデータ構造について
 *
 * 基本的にsssdkのクラス名、メンバー名はXML内ドキュメントのタグと一致しております。\n
 * そのためXML内のタグの意味を知りたい場合、このドキュメントのクラスのメンバー、又は列挙型のドキュメントを参照下さい。\n
 * \n
 * OPTPiX SpriteStuidoで作成されるファイル形式はsspj、ssae、ssceの三種類となります。
 * 旧形式のssaxにつきましては、別途あるssaxのドキュメントを参照下さい。
 * （SpriteStudioのユーザーサポートページにございます。）
 * 
 * \section sspjファイルについて
 * \n
 * sspjは、SpriteStudioのプロジェクトファイルです。
 * このファイルには、プロジェクトに格納されているアニメーション、テクスチャ、セルマップの参照先の記載、
 * 参照ファイル名の格納が行われています。
 * \n
 * \section ssaeファイルについて
 * ssaeファイルは、アニメーションデータのパーツの内容とその組み合わせを記述した「Model」部と「Model」を使用してキーフレームアニメーションを記載したAnimation部で構成されています。\n
 * Model部はXMLドキュメント中の<Model>で定義され、Animation部はXMLドキュメント中の<animeList>以下<anime>で定義されます。\n
 * <animeList>のブロックには、<anime>が複数格納される場合があります。（同一のModelでアニメーションを作成している場合）\n
 * \n
 * <pre>
 *	<SpriteStudioAnimePack>
 *		<settings>
 *			アニメーションの再生設定
 *		</settings>
 *		<name>アニメーション名称</name>
 *		<Model>
 *			<partList>
 *			アニメーションを構成するパーツの種別、組み合わせデータの記述
 *				:
 *				:
 *			</partList>
 *		</Model>
 *		<cellmapNames>
 *			アニメーションで使用しているセルマップのファイル名リスト
 *				:
 *				:
 *		</cellmapNames>
 *		<animeList>
 *			<anime>
 *				アニメーションデータ
 *					:
 *					:
 *			</anime>
 *		</animeList>
 *	</SpriteStudioAnimePack>
 * </pre>
 *\n
 *
 * \subsection  ssaeファイル　partListブロックの解説
 * \n
 * partListブロックは、１つのパーツを<value>と定義しパーツの配列として機能しています。\n
 * valueの中のタグについては、SsPartのクラス説明ページを参照下さい。\n
 * \n
 *
 * \subsection  ssaeファイル　animeブロックの解説
 * 
 * <anime>ブロックは、アニメーションの再生FPSや名称などの設定値とそれぞれのキーフレームでパーツに適用するアトリビュート値が格納されています。\n
 * キーフレームのアトリビュート値は<partAnimes>ブロックに記載されています。\n
 * <partAnimes>は<partAnime>のコンテナとなっており、<partAnime>はモデルで定義された各パーツに対応したアトリビュート値がキーフレーム値と一緒に記載されています。\n
 * \n
 * (例）\n
 *<pre>
 * 	<attribute tag="POSX">
 *		<key time="0" ipType="linear">
 *			\<value\>0\</value\>
 *		</key>
 *	</attribute>
 * </pre>
 * \n
 * ここで、アトリビュートはPOSX=パーツの座標Xを示しており、そのキーフレームはtime=0の場所、補完方法はliner=線形補間となっており、\n
 * その値はvalue=0となっています。このvalue値がパーツの座標Xに適用されます。\n
 * このブロックはキーフレームがエディタ上で作成されている場合、作成されただけここに記載が行われます。\n
 * アトリビュートの種類については、SsAttributeKind で検索、補完方法については、SsInterpolationTypeを参照下さい。\n
 * \n
 * \section ssceファイルについて
 * ssceファイルは、OPTPiX SpriteStudioで作成されたセルマップの情報が記述されているファイルです。\n
 * 各セルは、<cells>のブロック内で一つづつ<cell>要素で記述が行われています。\n
 * 
 * 例）
 * <pre>
* 	<cells>
*		<cell>
*			<name>arm_l</name>
*			<pos>128 0</pos>
*			<size>128 192</size>
*			<pivot>0.4296875 -0.354166656732559</pivot>
*			<rotated>0</rotated>
*			<orgImageName></orgImageName>
*			<posStable>0</posStable>
*		</cell>
*		:
*		:
* </pre>
*\n
* 基本的にssceは、複雑なフォーマットにはなっておらず、cellの配列として記述が行われておりますので、\n
* 各タグを本ドキュメントで検索頂けるとご使用いただけます。\n
* \n
*/
 
#ifndef __SSLIB__
#define __SSLIB__

#include "sstypes.h"
#include "ssloader_ssee.h"





#endif