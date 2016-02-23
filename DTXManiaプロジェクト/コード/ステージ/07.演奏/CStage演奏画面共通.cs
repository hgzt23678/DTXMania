using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Diagnostics;
using System.Threading;
using SlimDX;
using SlimDX.Direct3D9;
using FDK;

namespace DTXMania
{
	/// <summary>
	/// 演奏画面の共通クラス (ドラム演奏画面, ギター演奏画面の継承元)
	/// </summary>
	internal abstract class CStage演奏画面共通 : CStage
	{
		protected CStage演奏画面共通()
		{
		}
		// メソッド

		#region [ t演奏結果を格納する_ドラム() ]
		public void t演奏結果を格納する_ドラム(out CScoreIni.C演奏記録 Drums)
		{
			Drums = new CScoreIni.C演奏記録();

			if (CDTXMania.app.DTX.bチップがある.Drums && !CDTXMania.app.ConfigIni.bギタレボモード)
			{
				Drums.nスコア = (long)this.actScore.Get(E楽器パート.DRUMS);
				Drums.dbゲーム型スキル値 = CScoreIni.tゲーム型スキルを計算して返す(CDTXMania.app.DTX.LEVEL.Drums, CDTXMania.app.DTX.n可視チップ数.Drums, this.nヒット数_Auto含まない.Drums.Perfect, this.actCombo.n現在のコンボ数.Drums最高値, E楽器パート.DRUMS, bIsAutoPlay);
				Drums.db演奏型スキル値 = CScoreIni.t演奏型スキルを計算して返す(CDTXMania.app.DTX.n可視チップ数.Drums, this.nヒット数_Auto含まない.Drums.Perfect, this.nヒット数_Auto含まない.Drums.Great, this.nヒット数_Auto含まない.Drums.Good, this.nヒット数_Auto含まない.Drums.Poor, this.nヒット数_Auto含まない.Drums.Miss, E楽器パート.DRUMS, bIsAutoPlay);
				Drums.nPerfect数 = CDTXMania.app.ConfigIni.bドラムが全部オートプレイである ? this.nヒット数_Auto含む.Drums.Perfect : this.nヒット数_Auto含まない.Drums.Perfect;
				Drums.nGreat数 = CDTXMania.app.ConfigIni.bドラムが全部オートプレイである ? this.nヒット数_Auto含む.Drums.Great : this.nヒット数_Auto含まない.Drums.Great;
				Drums.nGood数 = CDTXMania.app.ConfigIni.bドラムが全部オートプレイである ? this.nヒット数_Auto含む.Drums.Good : this.nヒット数_Auto含まない.Drums.Good;
				Drums.nPoor数 = CDTXMania.app.ConfigIni.bドラムが全部オートプレイである ? this.nヒット数_Auto含む.Drums.Poor : this.nヒット数_Auto含まない.Drums.Poor;
				Drums.nMiss数 = CDTXMania.app.ConfigIni.bドラムが全部オートプレイである ? this.nヒット数_Auto含む.Drums.Miss : this.nヒット数_Auto含まない.Drums.Miss;
				Drums.nPerfect数_Auto含まない = this.nヒット数_Auto含まない.Drums.Perfect;
				Drums.nGreat数_Auto含まない = this.nヒット数_Auto含まない.Drums.Great;
				Drums.nGood数_Auto含まない = this.nヒット数_Auto含まない.Drums.Good;
				Drums.nPoor数_Auto含まない = this.nヒット数_Auto含まない.Drums.Poor;
				Drums.nMiss数_Auto含まない = this.nヒット数_Auto含まない.Drums.Miss;
				Drums.n最大コンボ数 = this.actCombo.n現在のコンボ数.Drums最高値;
				Drums.n全チップ数 = CDTXMania.app.DTX.n可視チップ数.Drums;
				for (int i = 0; i < (int)Eレーン.MAX; i++)
				{
					Drums.bAutoPlay[i] = bIsAutoPlay[i];
				}
				Drums.bTight = CDTXMania.app.ConfigIni.bTight;
				for (int i = 0; i < 3; i++)
				{
					Drums.bSudden[i] = CDTXMania.app.ConfigIni.bSudden[i];
					Drums.bHidden[i] = CDTXMania.app.ConfigIni.bHidden[i];
					Drums.eInvisible[i] = CDTXMania.app.ConfigIni.eInvisible[i];
					Drums.bReverse[i] = CDTXMania.app.ConfigIni.bReverse[i];
					Drums.eRandom[i] = CDTXMania.app.ConfigIni.eRandom[i];
					Drums.bLight[i] = CDTXMania.app.ConfigIni.bLight[i];
					Drums.bLeft[i] = CDTXMania.app.ConfigIni.bLeft[i];
					Drums.f譜面スクロール速度[i] = ((float)(CDTXMania.app.ConfigIni.n譜面スクロール速度[i] + 1)) * 0.5f;
				}
				Drums.eDark = CDTXMania.app.ConfigIni.eDark;
				Drums.n演奏速度分子 = CDTXMania.app.ConfigIni.n演奏速度;
				Drums.n演奏速度分母 = 20;
				Drums.eHHGroup = CDTXMania.app.ConfigIni.eHHGroup;
				Drums.eFTGroup = CDTXMania.app.ConfigIni.eFTGroup;
				Drums.eCYGroup = CDTXMania.app.ConfigIni.eCYGroup;
				Drums.eHitSoundPriorityHH = CDTXMania.app.ConfigIni.eHitSoundPriorityHH;
				Drums.eHitSoundPriorityFT = CDTXMania.app.ConfigIni.eHitSoundPriorityFT;
				Drums.eHitSoundPriorityCY = CDTXMania.app.ConfigIni.eHitSoundPriorityCY;
				Drums.bGuitar有効 = CDTXMania.app.ConfigIni.bGuitar有効;
				Drums.bDrums有効 = CDTXMania.app.ConfigIni.bDrums有効;
				Drums.bSTAGEFAILED有効 = CDTXMania.app.ConfigIni.bSTAGEFAILED有効;
				Drums.eダメージレベル = CDTXMania.app.ConfigIni.eダメージレベル;
				Drums.b演奏にキーボードを使用した = this.b演奏にキーボードを使った.Drums;
				Drums.b演奏にMIDI入力を使用した = this.b演奏にMIDI入力を使った.Drums;
				Drums.b演奏にジョイパッドを使用した = this.b演奏にジョイパッドを使った.Drums;
				Drums.b演奏にマウスを使用した = this.b演奏にマウスを使った.Drums;
				Drums.nPerfectになる範囲ms = CDTXMania.app.nPerfect範囲ms;
				Drums.nGreatになる範囲ms = CDTXMania.app.nGreat範囲ms;
				Drums.nGoodになる範囲ms = CDTXMania.app.nGood範囲ms;
				Drums.nPoorになる範囲ms = CDTXMania.app.nPoor範囲ms;
				Drums.strDTXManiaのバージョン = CDTXMania.VERSION;
				Drums.最終更新日時 = DateTime.Now.ToString();
				Drums.Hash = CScoreIni.t演奏セクションのMD5を求めて返す(Drums);
				Drums.nRisky = CDTXMania.app.ConfigIni.nRisky; // #35461 chnmr0 add
				Drums.bギターとベースを入れ替えた = CDTXMania.app.ConfigIni.bIsSwappedGuitarBass; // #35417 chnmr0 add
			}
		}
		#endregion
		#region [ t演奏結果を格納する_ギター() ]
		public void t演奏結果を格納する_ギター(out CScoreIni.C演奏記録 Guitar)
		{
			Guitar = new CScoreIni.C演奏記録();

			if (CDTXMania.app.DTX.bチップがある.Guitar)
			{
				Guitar.nスコア = (long)this.actScore.Get(E楽器パート.GUITAR);
				Guitar.dbゲーム型スキル値 = CScoreIni.tゲーム型スキルを計算して返す(CDTXMania.app.DTX.LEVEL.Guitar, CDTXMania.app.DTX.n可視チップ数.Guitar, this.nヒット数_Auto含まない.Guitar.Perfect, this.actCombo.n現在のコンボ数.Guitar最高値, E楽器パート.GUITAR, bIsAutoPlay);
				Guitar.db演奏型スキル値 = CScoreIni.t演奏型スキルを計算して返す(CDTXMania.app.DTX.n可視チップ数.Guitar, this.nヒット数_Auto含まない.Guitar.Perfect, this.nヒット数_Auto含まない.Guitar.Great, this.nヒット数_Auto含まない.Guitar.Good, this.nヒット数_Auto含まない.Guitar.Poor, this.nヒット数_Auto含まない.Guitar.Miss, E楽器パート.GUITAR, bIsAutoPlay);
				Guitar.nPerfect数 = CDTXMania.app.ConfigIni.bギターが全部オートプレイである ? this.nヒット数_Auto含む.Guitar.Perfect : this.nヒット数_Auto含まない.Guitar.Perfect;
				Guitar.nGreat数 = CDTXMania.app.ConfigIni.bギターが全部オートプレイである ? this.nヒット数_Auto含む.Guitar.Great : this.nヒット数_Auto含まない.Guitar.Great;
				Guitar.nGood数 = CDTXMania.app.ConfigIni.bギターが全部オートプレイである ? this.nヒット数_Auto含む.Guitar.Good : this.nヒット数_Auto含まない.Guitar.Good;
				Guitar.nPoor数 = CDTXMania.app.ConfigIni.bギターが全部オートプレイである ? this.nヒット数_Auto含む.Guitar.Poor : this.nヒット数_Auto含まない.Guitar.Poor;
				Guitar.nMiss数 = CDTXMania.app.ConfigIni.bギターが全部オートプレイである ? this.nヒット数_Auto含む.Guitar.Miss : this.nヒット数_Auto含まない.Guitar.Miss;
				Guitar.nPerfect数_Auto含まない = this.nヒット数_Auto含まない.Guitar.Perfect;
				Guitar.nGreat数_Auto含まない = this.nヒット数_Auto含まない.Guitar.Great;
				Guitar.nGood数_Auto含まない = this.nヒット数_Auto含まない.Guitar.Good;
				Guitar.nPoor数_Auto含まない = this.nヒット数_Auto含まない.Guitar.Poor;
				Guitar.nMiss数_Auto含まない = this.nヒット数_Auto含まない.Guitar.Miss;
				Guitar.n最大コンボ数 = this.actCombo.n現在のコンボ数.Guitar最高値;
				Guitar.n全チップ数 = CDTXMania.app.DTX.n可視チップ数.Guitar;
				for (int i = 0; i < (int)Eレーン.MAX; i++)
				{
					Guitar.bAutoPlay[i] = bIsAutoPlay[i];
				}
				Guitar.bTight = CDTXMania.app.ConfigIni.bTight;
				for (int i = 0; i < 3; i++)
				{
					Guitar.bSudden[i] = CDTXMania.app.ConfigIni.bSudden[i];
					Guitar.bHidden[i] = CDTXMania.app.ConfigIni.bHidden[i];
					Guitar.eInvisible[i] = CDTXMania.app.ConfigIni.eInvisible[i];
					Guitar.bReverse[i] = CDTXMania.app.ConfigIni.bReverse[i];
					Guitar.eRandom[i] = CDTXMania.app.ConfigIni.eRandom[i];
					Guitar.bLight[i] = CDTXMania.app.ConfigIni.bLight[i];
					Guitar.bLeft[i] = CDTXMania.app.ConfigIni.bLeft[i];
					Guitar.f譜面スクロール速度[i] = ((float)(CDTXMania.app.ConfigIni.n譜面スクロール速度[i] + 1)) * 0.5f;
				}
				Guitar.eDark = CDTXMania.app.ConfigIni.eDark;
				Guitar.n演奏速度分子 = CDTXMania.app.ConfigIni.n演奏速度;
				Guitar.n演奏速度分母 = 20;
				Guitar.eHHGroup = CDTXMania.app.ConfigIni.eHHGroup;
				Guitar.eFTGroup = CDTXMania.app.ConfigIni.eFTGroup;
				Guitar.eCYGroup = CDTXMania.app.ConfigIni.eCYGroup;
				Guitar.eHitSoundPriorityHH = CDTXMania.app.ConfigIni.eHitSoundPriorityHH;
				Guitar.eHitSoundPriorityFT = CDTXMania.app.ConfigIni.eHitSoundPriorityFT;
				Guitar.eHitSoundPriorityCY = CDTXMania.app.ConfigIni.eHitSoundPriorityCY;
				Guitar.bGuitar有効 = CDTXMania.app.ConfigIni.bGuitar有効;
				Guitar.bDrums有効 = CDTXMania.app.ConfigIni.bDrums有効;
				Guitar.bSTAGEFAILED有効 = CDTXMania.app.ConfigIni.bSTAGEFAILED有効;
				Guitar.eダメージレベル = CDTXMania.app.ConfigIni.eダメージレベル;
				Guitar.b演奏にキーボードを使用した = this.b演奏にキーボードを使った.Guitar;
				Guitar.b演奏にMIDI入力を使用した = this.b演奏にMIDI入力を使った.Guitar;
				Guitar.b演奏にジョイパッドを使用した = this.b演奏にジョイパッドを使った.Guitar;
				Guitar.b演奏にマウスを使用した = this.b演奏にマウスを使った.Guitar;
				Guitar.nPerfectになる範囲ms = CDTXMania.app.nPerfect範囲ms;
				Guitar.nGreatになる範囲ms = CDTXMania.app.nGreat範囲ms;
				Guitar.nGoodになる範囲ms = CDTXMania.app.nGood範囲ms;
				Guitar.nPoorになる範囲ms = CDTXMania.app.nPoor範囲ms;
				Guitar.strDTXManiaのバージョン = CDTXMania.VERSION;
				Guitar.最終更新日時 = DateTime.Now.ToString();
				Guitar.Hash = CScoreIni.t演奏セクションのMD5を求めて返す(Guitar);
				Guitar.bギターとベースを入れ替えた = CDTXMania.app.ConfigIni.bIsSwappedGuitarBass; // #35417 chnmr0 add
			}
		}
		#endregion
		#region [ t演奏結果を格納する_ベース() ]
		public void t演奏結果を格納する_ベース(out CScoreIni.C演奏記録 Bass)
		{
			Bass = new CScoreIni.C演奏記録();

			if (CDTXMania.app.DTX.bチップがある.Bass)
			{
				Bass.nスコア = (long)this.actScore.Get(E楽器パート.BASS);
				Bass.dbゲーム型スキル値 = CScoreIni.tゲーム型スキルを計算して返す(CDTXMania.app.DTX.LEVEL.Bass, CDTXMania.app.DTX.n可視チップ数.Bass, this.nヒット数_Auto含まない.Bass.Perfect, this.actCombo.n現在のコンボ数.Bass最高値, E楽器パート.BASS, bIsAutoPlay);
				Bass.db演奏型スキル値 = CScoreIni.t演奏型スキルを計算して返す(CDTXMania.app.DTX.n可視チップ数.Bass, this.nヒット数_Auto含まない.Bass.Perfect, this.nヒット数_Auto含まない.Bass.Great, this.nヒット数_Auto含まない.Bass.Good, this.nヒット数_Auto含まない.Bass.Poor, this.nヒット数_Auto含まない.Bass.Miss, E楽器パート.BASS, bIsAutoPlay);
				Bass.nPerfect数 = CDTXMania.app.ConfigIni.bベースが全部オートプレイである ? this.nヒット数_Auto含む.Bass.Perfect : this.nヒット数_Auto含まない.Bass.Perfect;
				Bass.nGreat数 = CDTXMania.app.ConfigIni.bベースが全部オートプレイである ? this.nヒット数_Auto含む.Bass.Great : this.nヒット数_Auto含まない.Bass.Great;
				Bass.nGood数 = CDTXMania.app.ConfigIni.bベースが全部オートプレイである ? this.nヒット数_Auto含む.Bass.Good : this.nヒット数_Auto含まない.Bass.Good;
				Bass.nPoor数 = CDTXMania.app.ConfigIni.bベースが全部オートプレイである ? this.nヒット数_Auto含む.Bass.Poor : this.nヒット数_Auto含まない.Bass.Poor;
				Bass.nMiss数 = CDTXMania.app.ConfigIni.bベースが全部オートプレイである ? this.nヒット数_Auto含む.Bass.Miss : this.nヒット数_Auto含まない.Bass.Miss;
				Bass.nPerfect数_Auto含まない = this.nヒット数_Auto含まない.Bass.Perfect;
				Bass.nGreat数_Auto含まない = this.nヒット数_Auto含まない.Bass.Great;
				Bass.nGood数_Auto含まない = this.nヒット数_Auto含まない.Bass.Good;
				Bass.nPoor数_Auto含まない = this.nヒット数_Auto含まない.Bass.Poor;
				Bass.nMiss数_Auto含まない = this.nヒット数_Auto含まない.Bass.Miss;
				Bass.n最大コンボ数 = this.actCombo.n現在のコンボ数.Bass最高値;
				Bass.n全チップ数 = CDTXMania.app.DTX.n可視チップ数.Bass;
				for (int i = 0; i < (int)Eレーン.MAX; i++)
				{
					Bass.bAutoPlay[i] = bIsAutoPlay[i];
				}
				Bass.bTight = CDTXMania.app.ConfigIni.bTight;
				for (int i = 0; i < 3; i++)
				{
					Bass.bSudden[i] = CDTXMania.app.ConfigIni.bSudden[i];
					Bass.bHidden[i] = CDTXMania.app.ConfigIni.bHidden[i];
					Bass.eInvisible[i] = CDTXMania.app.ConfigIni.eInvisible[i];
					Bass.bReverse[i] = CDTXMania.app.ConfigIni.bReverse[i];
					Bass.eRandom[i] = CDTXMania.app.ConfigIni.eRandom[i];
					Bass.bLight[i] = CDTXMania.app.ConfigIni.bLight[i];
					Bass.bLeft[i] = CDTXMania.app.ConfigIni.bLeft[i];
					Bass.f譜面スクロール速度[i] = ((float)(CDTXMania.app.ConfigIni.n譜面スクロール速度[i] + 1)) * 0.5f;
				}
				Bass.eDark = CDTXMania.app.ConfigIni.eDark;
				Bass.n演奏速度分子 = CDTXMania.app.ConfigIni.n演奏速度;
				Bass.n演奏速度分母 = 20;
				Bass.eHHGroup = CDTXMania.app.ConfigIni.eHHGroup;
				Bass.eFTGroup = CDTXMania.app.ConfigIni.eFTGroup;
				Bass.eCYGroup = CDTXMania.app.ConfigIni.eCYGroup;
				Bass.eHitSoundPriorityHH = CDTXMania.app.ConfigIni.eHitSoundPriorityHH;
				Bass.eHitSoundPriorityFT = CDTXMania.app.ConfigIni.eHitSoundPriorityFT;
				Bass.eHitSoundPriorityCY = CDTXMania.app.ConfigIni.eHitSoundPriorityCY;
				Bass.bGuitar有効 = CDTXMania.app.ConfigIni.bGuitar有効;
				Bass.bDrums有効 = CDTXMania.app.ConfigIni.bDrums有効;
				Bass.bSTAGEFAILED有効 = CDTXMania.app.ConfigIni.bSTAGEFAILED有効;
				Bass.eダメージレベル = CDTXMania.app.ConfigIni.eダメージレベル;
				Bass.b演奏にキーボードを使用した = this.b演奏にキーボードを使った.Bass;			// #24280 2011.1.29 yyagi
				Bass.b演奏にMIDI入力を使用した = this.b演奏にMIDI入力を使った.Bass;				//
				Bass.b演奏にジョイパッドを使用した = this.b演奏にジョイパッドを使った.Bass;		//
				Bass.b演奏にマウスを使用した = this.b演奏にマウスを使った.Bass;					//
				Bass.nPerfectになる範囲ms = CDTXMania.app.nPerfect範囲ms;
				Bass.nGreatになる範囲ms = CDTXMania.app.nGreat範囲ms;
				Bass.nGoodになる範囲ms = CDTXMania.app.nGood範囲ms;
				Bass.nPoorになる範囲ms = CDTXMania.app.nPoor範囲ms;
				Bass.strDTXManiaのバージョン = CDTXMania.VERSION;
				Bass.最終更新日時 = DateTime.Now.ToString();
				Bass.Hash = CScoreIni.t演奏セクションのMD5を求めて返す(Bass);
				Bass.bギターとベースを入れ替えた = CDTXMania.app.ConfigIni.bIsSwappedGuitarBass; // #35417 chnmr0 add
			}
		}
		#endregion

		// CStage 実装

		public override void On活性化()
		{
			listChip = CDTXMania.app.DTX.listChip;
			listWAV = CDTXMania.app.DTX.listWAV;

			this.eフェードアウト完了時の戻り値 = E演奏画面の戻り値.継続;
			this.n現在のトップChip = (listChip.Count > 0) ? 0 : -1;
			this.L最後に再生したHHの実WAV番号 = new List<int>(16);
			this.e最後に再生したHHのチャンネル番号 = 0;
			this.n最後に再生した実WAV番号.Guitar = -1;
			this.n最後に再生した実WAV番号.Bass = -1;
			for (int i = 0; i < 50; i++)
			{
				this.n最後に再生したBGMの実WAV番号[i] = -1;
			}
			this.r次にくるギターChip = null;
			this.r次にくるベースChip = null;
			for (int j = 0; j < 10; j++)
			{
				this.r現在の空うちドラムChip[j] = null;
			}
			this.r現在の空うちギターChip = null;
			this.r現在の空うちベースChip = null;
			cInvisibleChip = new CInvisibleChip(CDTXMania.app.ConfigIni.nChipDisplayTimeMs, CDTXMania.app.ConfigIni.nChipFadeoutTimeMs);
			this.演奏判定ライン座標 = new C演奏判定ライン座標共通();
			this.n最大コンボ数_TargetGhost = new STDGBVALUE<int>(); // #35411 2015.08.21 chnmr0 add
			for (int k = 0; k < 3; k++)
			{
				//for ( int n = 0; n < 5; n++ )
				//{
				this.nヒット数_Auto含まない[k] = new CHITCOUNTOFRANK();
				this.nヒット数_Auto含む[k] = new CHITCOUNTOFRANK();
				this.nヒット数_TargetGhost[k] = new CHITCOUNTOFRANK(); // #35411 2015.08.21 chnmr0 add
				//}
				this.queWailing[k] = new Queue<CChip>();
				this.r現在の歓声Chip[k] = null;
				cInvisibleChip.eInvisibleMode[k] = CDTXMania.app.ConfigIni.eInvisible[k];
				if (CDTXMania.app.DTXVmode.Enabled)
				{
					CDTXMania.app.ConfigIni.n譜面スクロール速度[k] = CDTXMania.app.ConfigIni.nViewerScrollSpeed[k];
				}

				this.nInputAdjustTimeMs[k] = CDTXMania.app.ConfigIni.nInputAdjustTimeMs[k];			// #23580 2011.1.3 yyagi
				//        2011.1.7 ikanick 修正
				//this.nJudgeLinePosY_delta[ k ] = CDTXMania.app.ConfigIni.nJudgeLinePosOffset[ k ];		// #31602 2013.6.23 yyagi

				this.演奏判定ライン座標.n判定位置[k] = CDTXMania.app.ConfigIni.e判定位置[k];
				this.演奏判定ライン座標.nJudgeLinePosY_delta[k] = CDTXMania.app.ConfigIni.nJudgeLinePosOffset[k];
				this.bReverse[k] = CDTXMania.app.ConfigIni.bReverse[k];					//

			}
			actCombo.演奏判定ライン座標 = 演奏判定ライン座標;
			for (int i = 0; i < 3; i++)
			{
				this.b演奏にキーボードを使った[i] = false;
				this.b演奏にジョイパッドを使った[i] = false;
				this.b演奏にMIDI入力を使った[i] = false;
				this.b演奏にマウスを使った[i] = false;
			}
			cInvisibleChip.Reset();
			base.On活性化();
			this.tステータスパネルの選択();
			this.tパネル文字列の設定();
			//this.演奏判定ライン座標();

			this.bIsAutoPlay = CDTXMania.app.ConfigIni.bAutoPlay;									// #24239 2011.1.23 yyagi


			//this.bIsAutoPlay.Guitar = CDTXMania.app.ConfigIni.bギターが全部オートプレイである;
			//this.bIsAutoPlay.Bass = CDTXMania.app.ConfigIni.bベースが全部オートプレイである;
			//			this.nRisky = CDTXMania.app.ConfigIni.nRisky;											// #23559 2011.7.28 yyagi
			actGauge.Init(CDTXMania.app.ConfigIni.nRisky);									// #23559 2011.7.28 yyagi
			this.nPolyphonicSounds = CDTXMania.app.ConfigIni.nPoliphonicSounds;
			e判定表示優先度 = CDTXMania.app.ConfigIni.e判定表示優先度;

			CDTXMania.app.Skin.tRemoveMixerAll();	// 効果音のストリームをミキサーから解除しておく

			queueMixerSound = new Queue<stmixer>(64);
			bIsDirectSound = (CDTXMania.app.Sound管理.GetCurrentSoundDeviceType() == "DirectSound");
			bUseOSTimer = CDTXMania.app.ConfigIni.bUseOSTimer;
			this.bPAUSE = false;
			if (CDTXMania.app.DTXVmode.Enabled)
			{
				db再生速度 = CDTXMania.app.DTX.dbDTXVPlaySpeed;
				CDTXMania.app.ConfigIni.n演奏速度 = (int)(CDTXMania.app.DTX.dbDTXVPlaySpeed * 20 + 0.5);
			}
			else
			{
				db再生速度 = ((double)CDTXMania.app.ConfigIni.n演奏速度) / 20.0;
			}
			bValidScore = (CDTXMania.app.DTXVmode.Enabled) ? false : true;

			cWailingChip = new CWailingChip共通[3];	// 0:未使用, 1:Gutiar, 2:Bass
			if (CDTXMania.app.ConfigIni.bDrums有効)
			{
				cWailingChip[1] = new CWailngChip_Guitar_Drum画面(ref 演奏判定ライン座標);
				cWailingChip[2] = new CWailngChip_Bass_Drum画面(ref 演奏判定ライン座標);
			}
			else
			{
				cWailingChip[1] = new CWailngChip_Guitar_GR画面(ref 演奏判定ライン座標);
				cWailingChip[2] = new CWailngChip_Bass_GR画面(ref 演奏判定ライン座標);
			}

			#region [ 演奏開始前にmixer登録しておくべきサウンド(開幕してすぐに鳴らすことになるチップ音)を登録しておく ]
			foreach (CChip pChip in listChip)
			{
				//				Debug.WriteLine( "CH=" + pChip.nチャンネル番号.ToString( "x2" ) + ", 整数値=" + pChip.n整数値 +  ", time=" + pChip.n発声時刻ms );
				if (pChip.n発声時刻ms <= 0)
				{
					if (pChip.eチャンネル番号 == Ech定義.MixerAdd)
					{
						pChip.bHit = true;
						//						Trace.TraceInformation( "first [DA] BAR=" + pChip.n発声位置 / 384 + " ch=" + pChip.nチャンネル番号.ToString( "x2" ) + ", wav=" + pChip.n整数値 + ", time=" + pChip.n発声時刻ms );
						if (listWAV.ContainsKey(pChip.n整数値_内部番号))
						{
							CDTX.CWAV wc = listWAV[pChip.n整数値_内部番号];
							for (int i = 0; i < nPolyphonicSounds; i++)
							{
								if (wc.rSound[i] != null)
								{
									CDTXMania.app.Sound管理.AddMixer(wc.rSound[i], db再生速度, pChip.b演奏終了後も再生が続くチップである);
									//AddMixer( wc.rSound[ i ] );		// 最初はqueueを介さず直接ミキサー登録する
								}
							}
						}
					}
				}
				else
				{
					break;
				}
			}
			#endregion

			if (CDTXMania.app.ConfigIni.bIsSwappedGuitarBass)	// #24063 2011.1.24 yyagi Gt/Bsの譜面情報入れ替え
			{
				CDTXMania.app.DTX.SwapGuitarBassInfos();
			}
			this.sw = new Stopwatch();
			this.sw2 = new Stopwatch();
			//			this.gclatencymode = GCSettings.LatencyMode;
			//			GCSettings.LatencyMode = GCLatencyMode.Batch;	// 演奏画面中はGCを抑止する
		}
		public override void On非活性化()
		{
			this.L最後に再生したHHの実WAV番号.Clear();	// #23921 2011.1.4 yyagi
			this.L最後に再生したHHの実WAV番号 = null;	//
			for (int i = 0; i < 3; i++)
			{
				this.queWailing[i].Clear();
				this.queWailing[i] = null;
			}
			this.ctWailingチップ模様アニメ = null;
			this.ctチップ模様アニメ.Drums = null;
			this.ctチップ模様アニメ.Guitar = null;
			this.ctチップ模様アニメ.Bass = null;
			//listWAV.Clear();
			listWAV = null;
			listChip = null;
			queueMixerSound.Clear();
			queueMixerSound = null;
			cInvisibleChip.Dispose();
			cInvisibleChip = null;
			//			GCSettings.LatencyMode = this.gclatencymode;
			base.On非活性化();
		}
		public override void OnManagedリソースの作成()
		{
			if (!base.b活性化してない)
			{
				this.t背景テクスチャの生成();

				this.txWailing枠 = TextureFactory.tテクスチャの生成(CSkin.Path(@"Graphics\ScreenPlay wailing cursor.png"));

				base.OnManagedリソースの作成();
			}
		}
		public override void OnManagedリソースの解放()
		{
			if (!base.b活性化してない)
			{
				TextureFactory.tテクスチャの解放(ref this.tx背景);

				TextureFactory.tテクスチャの解放(ref this.txWailing枠);
				base.OnManagedリソースの解放();
			}
		}

		// その他

		#region [ protected ]
		//-----------------
		protected class CHITCOUNTOFRANK
		{
			// Fields
			public int Good;
			public int Great;
			public int Miss;
			public int Perfect;
			public int Poor;

			// Properties
			public int this[int index]
			{
				get
				{
					switch (index)
					{
						case 0:
							return this.Perfect;

						case 1:
							return this.Great;

						case 2:
							return this.Good;

						case 3:
							return this.Poor;

						case 4:
							return this.Miss;
					}
					throw new IndexOutOfRangeException();
				}
				set
				{
					switch (index)
					{
						case 0:
							this.Perfect = value;
							return;

						case 1:
							this.Great = value;
							return;

						case 2:
							this.Good = value;
							return;

						case 3:
							this.Poor = value;
							return;

						case 4:
							this.Miss = value;
							return;
					}
					throw new IndexOutOfRangeException();
				}
			}
		}

		[StructLayout(LayoutKind.Sequential)]
		protected struct STKARAUCHI
		{
			public CChip HH;
			public CChip SD;
			public CChip BD;
			public CChip HT;
			public CChip LT;
			public CChip FT;
			public CChip CY;
			public CChip HHO;
			public CChip RD;
			public CChip LC;
			public CChip this[int index]
			{
				get
				{
					switch (index)
					{
						case 0:
							return this.HH;

						case 1:
							return this.SD;

						case 2:
							return this.BD;

						case 3:
							return this.HT;

						case 4:
							return this.LT;

						case 5:
							return this.FT;

						case 6:
							return this.CY;

						case 7:
							return this.HHO;

						case 8:
							return this.RD;

						case 9:
							return this.LC;
					}
					throw new IndexOutOfRangeException();
				}
				set
				{
					switch (index)
					{
						case 0:
							this.HH = value;
							return;

						case 1:
							this.SD = value;
							return;

						case 2:
							this.BD = value;
							return;

						case 3:
							this.HT = value;
							return;

						case 4:
							this.LT = value;
							return;

						case 5:
							this.FT = value;
							return;

						case 6:
							this.CY = value;
							return;

						case 7:
							this.HHO = value;
							return;

						case 8:
							this.RD = value;
							return;

						case 9:
							this.LC = value;
							return;
					}
					throw new IndexOutOfRangeException();
				}
			}
		}

		protected struct stmixer
		{
			internal bool bIsAdd;
			internal CSound csound;
			internal bool b演奏終了後も再生が続くチップである;
		};

		protected CAct演奏AVI actAVI;
		protected CAct演奏BGA actBGA;

		protected CAct演奏チップファイアGB actChipFireGB;
		protected CAct演奏Combo共通 actCombo;
		protected CAct演奏Danger共通 actDANGER;
		protected CActFIFOBlack actFI;
		protected CActFIFOBlack actFO;
		protected CActFIFOWhite actFOClear;
		protected CAct演奏ゲージ共通 actGauge;

		protected CAct演奏判定文字列共通 actJudgeString;
		protected CAct演奏DrumsレーンフラッシュD actLaneFlushD;
		protected CAct演奏レーンフラッシュGB共通 actLaneFlushGB;
		protected CAct演奏パネル文字列 actPanel;
		protected CAct演奏演奏情報 actPlayInfo;
		protected CAct演奏RGB共通 actRGB;
		protected CAct演奏スコア共通 actScore;
		protected CAct演奏ステージ失敗 actStageFailed;
		protected CAct演奏ステータスパネル共通 actStatusPanels;
		protected CAct演奏WailingBonus共通 actWailingBonus;
		protected CAct演奏スクロール速度 act譜面スクロール速度;
		public C演奏判定ライン座標共通 演奏判定ライン座標;
		protected bool bPAUSE;
		protected STDGBVALUE<bool> b演奏にMIDI入力を使った;
		protected STDGBVALUE<bool> b演奏にキーボードを使った;
		protected STDGBVALUE<bool> b演奏にジョイパッドを使った;
		protected STDGBVALUE<bool> b演奏にマウスを使った;
		protected CCounter ctWailingチップ模様アニメ;
		protected STDGBVALUE<CCounter> ctチップ模様アニメ;

		protected E演奏画面の戻り値 eフェードアウト完了時の戻り値;
		protected readonly Ech定義[,] nBGAスコープチャンネルマップ = new Ech定義[2, 8]
		{
			{
				Ech定義.BGALayer1_Swap,
				Ech定義.BGALayer2_Swap,
				Ech定義.BGALayer3_Swap,
				Ech定義.BGALayer4_Swap,
				Ech定義.BGALayer5_Swap,
				Ech定義.BGALayer6_Swap,
				Ech定義.BGALayer7_Swap,
				Ech定義.BGALayer8_Swap
			},
			{
				Ech定義.BGALayer1,
				Ech定義.BGALayer2,
				Ech定義.BGALayer3,
				Ech定義.BGALayer4,
				Ech定義.BGALayer5,
				Ech定義.BGALayer6,
				Ech定義.BGALayer7,
				Ech定義.BGALayer8
			}
		};
		public static readonly int[] nチャンネル0Atoレーン07 = new int[] { 1, 2, 3, 4, 5, 7, 6, 1, 7, 0 };
		protected readonly int[] nチャンネル0Atoパッド08 = new int[] { 1, 2, 3, 4, 5, 7, 6, 1, 8, 0 };
		protected readonly Ech定義[] nパッド0Atoチャンネル0A = new Ech定義[]
		{
			Ech定義.HiHatClose,
			Ech定義.Snare,
			Ech定義.BassDrum,
			Ech定義.HighTom,
			Ech定義.LowTom,
			Ech定義.FloorTom,
			Ech定義.Cymbal,
			Ech定義.HiHatOpen,
			Ech定義.RideCymbal,
			Ech定義.LeftCymbal
		};
		protected readonly int[] nパッド0Atoパッド08 = new int[] { 1, 2, 3, 4, 5, 6, 7, 1, 8, 0 };	// パッド画像のヒット処理用
		protected readonly int[] nパッド0Atoレーン07 = new int[] { 1, 2, 3, 4, 5, 6, 7, 1, 7, 0 };
		protected STDGBVALUE<CHITCOUNTOFRANK> nヒット数_Auto含まない;
		protected STDGBVALUE<CHITCOUNTOFRANK> nヒット数_Auto含む;
		protected STDGBVALUE<CHITCOUNTOFRANK> nヒット数_TargetGhost; // #35411 2015.08.21 chnmr0 add
		protected STDGBVALUE<int> nコンボ数_TargetGhost;
		protected STDGBVALUE<int> n最大コンボ数_TargetGhost;
		protected int n現在のトップChip = -1;
		protected int[] n最後に再生したBGMの実WAV番号 = new int[50];
		protected Ech定義 e最後に再生したHHのチャンネル番号;
		protected List<int> L最後に再生したHHの実WAV番号;		// #23921 2011.1.4 yyagi: change "int" to "List<int>", for recording multiple wav No.
		protected STLANEVALUE<int> n最後に再生した実WAV番号;	// #26388 2011.11.8 yyagi: change "n最後に再生した実WAV番号.GUITAR" and "n最後に再生した実WAV番号.BASS"
		//							into "n最後に再生した実WAV番号";
		//		protected int n最後に再生した実WAV番号.GUITAR;
		//		protected int n最後に再生した実WAV番号.BASS;

		protected volatile Queue<stmixer> queueMixerSound;		// #24820 2013.1.21 yyagi まずは単純にAdd/Removeを1個のキューでまとめて管理するやり方で設計する
		protected DateTime dtLastQueueOperation;				//
		protected bool bIsDirectSound;							//
		protected double db再生速度;
		protected bool bValidScore;
		//		protected bool bDTXVmode;
		//		protected STDGBVALUE<int> nJudgeLinePosY_delta;			// #31602 2013.6.23 yyagi 表示遅延対策として、判定ラインの表示位置をずらす機能を追加する
		protected STDGBVALUE<bool> bReverse;

		protected STDGBVALUE<Queue<CChip>> queWailing;
		protected STDGBVALUE<CChip> r現在の歓声Chip;
		protected CChip r現在の空うちギターChip;
		protected STKARAUCHI r現在の空うちドラムChip;
		protected CChip r現在の空うちベースChip;
		protected CChip r次にくるギターChip;
		protected CChip r次にくるベースChip;
		protected CTexture txWailing枠;
		protected CTexture txチップ;
		protected CTexture txヒットバー;

		protected CTexture tx背景;

		protected STDGBVALUE<int> nInputAdjustTimeMs;		// #23580 2011.1.3 yyagi
		protected STAUTOPLAY bIsAutoPlay;		// #24239 2011.1.23 yyagi
		//		protected int nRisky_InitialVar, nRiskyTime;		// #23559 2011.7.28 yyagi → CAct演奏ゲージ共通クラスに隠蔽
		protected int nPolyphonicSounds;
		protected List<CChip> listChip;
		protected Dictionary<int, CDTX.CWAV> listWAV;
		protected CInvisibleChip cInvisibleChip;
		protected bool bUseOSTimer;
		protected E判定表示優先度 e判定表示優先度;
		protected CWailingChip共通[] cWailingChip;

		protected Stopwatch sw;		// 2011.6.13 最適化検討用のストップウォッチ
		protected Stopwatch sw2;
		//		protected GCLatencyMode gclatencymode;

		public void AddMixer(CSound cs, bool _b演奏終了後も再生が続くチップである)
		{
			stmixer stm = new stmixer()
			{
				bIsAdd = true,
				csound = cs,
				b演奏終了後も再生が続くチップである = _b演奏終了後も再生が続くチップである
			};
			queueMixerSound.Enqueue(stm);
			//		Debug.WriteLine( "★Queue: add " + Path.GetFileName( stm.csound.strファイル名 ));
		}
		public void RemoveMixer(CSound cs)
		{
			stmixer stm = new stmixer()
			{
				bIsAdd = false,
				csound = cs,
				b演奏終了後も再生が続くチップである = false
			};
			queueMixerSound.Enqueue(stm);
			//		Debug.WriteLine( "★Queue: remove " + Path.GetFileName( stm.csound.strファイル名 ));
		}
		public void ManageMixerQueue()
		{
			// もしサウンドの登録/削除が必要なら、実行する
			if (queueMixerSound.Count > 0)
			{
				//Debug.WriteLine( "☆queueLength=" + queueMixerSound.Count );
				DateTime dtnow = DateTime.Now;
				TimeSpan ts = dtnow - dtLastQueueOperation;
				if (ts.Milliseconds > 7)
				{
					for (int i = 0; i < 2 && queueMixerSound.Count > 0; i++)
					{
						dtLastQueueOperation = dtnow;
						stmixer stm = queueMixerSound.Dequeue();
						if (stm.bIsAdd)
						{
							CDTXMania.app.Sound管理.AddMixer(stm.csound, db再生速度, stm.b演奏終了後も再生が続くチップである);
						}
						else
						{
							CDTXMania.app.Sound管理.RemoveMixer(stm.csound);
						}
					}
				}
			}
		}

		/// <summary>
		/// 演奏開始前に適切なサイズのAVIテクスチャを作成しておくことで、AVI再生開始時のもたつきをなくす
		/// </summary>
		protected void PrepareAVITexture()
		{
			if (CDTXMania.app.ConfigIni.bAVI有効)
			{
				foreach (CChip pChip in listChip)
				{
					if (pChip.eチャンネル番号 == Ech定義.Movie || pChip.eチャンネル番号 == Ech定義.MovieFull)
					{
						// 最初に再生するAVIチップに合わせて、テクスチャを準備しておく
						if (pChip.rAVI != null)
						{
							this.actAVI.PrepareProperSizeTexture((int)pChip.rAVI.avi.nフレーム幅, (int)pChip.rAVI.avi.nフレーム高さ);
						}
						break;
					}
				}
			}
		}

		protected E判定 e指定時刻からChipのJUDGEを返す(long nTime, CChip pChip, int nInputAdjustTime, bool saveLag = true)
		{
			if (pChip != null)
			{
				// #35411 2015.08.22 chnmr0 modified add check save lag flag for ghost
				int lag = (int)(nTime + nInputAdjustTime - pChip.n発声時刻ms);
				if (saveLag)
				{
					pChip.nLag = lag;       // #23580 2011.1.3 yyagi: add "nInputAdjustTime" to add input timing adjust feature
					if (pChip.e楽器パート != E楽器パート.UNKNOWN)
					{
						pChip.extendInfoForGhost = this.actCombo.n現在のコンボ数[(int)pChip.e楽器パート] > 0 ? true : false;
					}
				}
				// #35411 modify end

				int nDeltaTime = Math.Abs(lag);
				//Debug.WriteLine("nAbsTime=" + (nTime - pChip.n発声時刻ms) + ", nDeltaTime=" + (nTime + nInputAdjustTime - pChip.n発声時刻ms));
				if (nDeltaTime <= CDTXMania.app.nPerfect範囲ms)
				{
					return E判定.Perfect;
				}
				if (nDeltaTime <= CDTXMania.app.nGreat範囲ms)
				{
					return E判定.Great;
				}
				if (nDeltaTime <= CDTXMania.app.nGood範囲ms)
				{
					return E判定.Good;
				}
				if (nDeltaTime <= CDTXMania.app.nPoor範囲ms)
				{
					return E判定.Poor;
				}
			}
			return E判定.Miss;
		}
		protected CChip r空うちChip(E楽器パート part, Eパッド pad)
		{
			switch (part)
			{
				case E楽器パート.DRUMS:
					switch (pad)
					{
						case Eパッド.HH:
							if (this.r現在の空うちドラムChip.HH != null)
							{
								return this.r現在の空うちドラムChip.HH;
							}
							if (CDTXMania.app.ConfigIni.eHHGroup != EHHGroup.ハイハットのみ打ち分ける)
							{
								if (CDTXMania.app.ConfigIni.eHHGroup == EHHGroup.左シンバルのみ打ち分ける)
								{
									return this.r現在の空うちドラムChip.HHO;
								}
								if (this.r現在の空うちドラムChip.HHO != null)
								{
									return this.r現在の空うちドラムChip.HHO;
								}
							}
							return this.r現在の空うちドラムChip.LC;

						case Eパッド.SD:
							return this.r現在の空うちドラムChip.SD;

						case Eパッド.BD:
							return this.r現在の空うちドラムChip.BD;

						case Eパッド.HT:
							return this.r現在の空うちドラムChip.HT;

						case Eパッド.LT:
							if (this.r現在の空うちドラムChip.LT != null)
							{
								return this.r現在の空うちドラムChip.LT;
							}
							if (CDTXMania.app.ConfigIni.eFTGroup == EFTGroup.共通)
							{
								return this.r現在の空うちドラムChip.FT;
							}
							return null;

						case Eパッド.FT:
							if (this.r現在の空うちドラムChip.FT != null)
							{
								return this.r現在の空うちドラムChip.FT;
							}
							if (CDTXMania.app.ConfigIni.eFTGroup == EFTGroup.共通)
							{
								return this.r現在の空うちドラムChip.LT;
							}
							return null;

						case Eパッド.CY:
							if (this.r現在の空うちドラムChip.CY != null)
							{
								return this.r現在の空うちドラムChip.CY;
							}
							if (CDTXMania.app.ConfigIni.eCYGroup == ECYGroup.共通)
							{
								return this.r現在の空うちドラムChip.RD;
							}
							return null;

						case Eパッド.HHO:
							if (this.r現在の空うちドラムChip.HHO != null)
							{
								return this.r現在の空うちドラムChip.HHO;
							}
							if (CDTXMania.app.ConfigIni.eHHGroup != EHHGroup.ハイハットのみ打ち分ける)
							{
								if (CDTXMania.app.ConfigIni.eHHGroup == EHHGroup.左シンバルのみ打ち分ける)
								{
									return this.r現在の空うちドラムChip.HH;
								}
								if (this.r現在の空うちドラムChip.HH != null)
								{
									return this.r現在の空うちドラムChip.HH;
								}
							}
							return this.r現在の空うちドラムChip.LC;

						case Eパッド.RD:
							if (this.r現在の空うちドラムChip.RD != null)
							{
								return this.r現在の空うちドラムChip.RD;
							}
							if (CDTXMania.app.ConfigIni.eCYGroup == ECYGroup.共通)
							{
								return this.r現在の空うちドラムChip.CY;
							}
							return null;

						case Eパッド.LC:
							if (this.r現在の空うちドラムChip.LC != null)
							{
								return this.r現在の空うちドラムChip.LC;
							}
							if ((CDTXMania.app.ConfigIni.eHHGroup != EHHGroup.ハイハットのみ打ち分ける) && (CDTXMania.app.ConfigIni.eHHGroup != EHHGroup.全部共通))
							{
								return null;
							}
							if (this.r現在の空うちドラムChip.HH != null)
							{
								return this.r現在の空うちドラムChip.HH;
							}
							return this.r現在の空うちドラムChip.HHO;
					}
					break;

				case E楽器パート.GUITAR:
					return this.r現在の空うちギターChip;

				case E楽器パート.BASS:
					return this.r現在の空うちベースChip;
			}
			return null;
		}
		protected CChip r指定時刻に一番近いChip_ヒット未済問わず不可視考慮(long nTime, Ech定義 eChannel, int nInputAdjustTime)
		{
			sw2.Start();
			//Trace.TraceInformation( "NTime={0}, nChannel={1:x2}", nTime, nChannel );
			nTime += nInputAdjustTime;						// #24239 2011.1.23 yyagi InputAdjust

			int nIndex_InitialPositionSearchingToPast;
			if (this.n現在のトップChip == -1)				// 演奏データとして1個もチップがない場合は
			{
				sw2.Stop();
				return null;
			}
			int count = listChip.Count;
			int nIndex_NearestChip_Future = nIndex_InitialPositionSearchingToPast = this.n現在のトップChip;
			if (this.n現在のトップChip >= count)			// その時点で演奏すべきチップが既に全部無くなっていたら
			{
				nIndex_NearestChip_Future = nIndex_InitialPositionSearchingToPast = count - 1;
			}
			//int nIndex_NearestChip_Future;	// = nIndex_InitialPositionSearchingToFuture;
			//while ( nIndex_NearestChip_Future < count )		// 未来方向への検索
			for (; nIndex_NearestChip_Future < count; nIndex_NearestChip_Future++)
			{
				CChip chip = listChip[nIndex_NearestChip_Future];
				if (chip.b空打ちチップである)
				{
					continue;
				}
				if (((Ech定義.HiHatClose <= eChannel) && (eChannel <= Ech定義.LeftCymbal)))
				{
					if ((chip.eチャンネル番号 == eChannel) || (chip.eチャンネル番号 == (eChannel + 0x20)))
					{
						if (chip.n発声時刻ms > nTime)
						{
							break;
						}
						nIndex_InitialPositionSearchingToPast = nIndex_NearestChip_Future;
					}
					continue;	// ほんの僅かながら高速化
				}
				else if (((eChannel == Ech定義.Guitar_WailingSound) && (chip.e楽器パート == E楽器パート.GUITAR)) ||
					(((Ech定義.Guitar_Open <= eChannel) && (eChannel <= Ech定義.Guitar_Wailing)) && (chip.eチャンネル番号 == eChannel)))
				{
					if (chip.n発声時刻ms > nTime)
					{
						break;
					}
					nIndex_InitialPositionSearchingToPast = nIndex_NearestChip_Future;
				}
				else if (((eChannel == Ech定義.Bass_WailingSound) && (chip.e楽器パート == E楽器パート.BASS)) ||
					(((Ech定義.Bass_Open <= eChannel) && (eChannel <= Ech定義.Bass_Wailing)) && (chip.eチャンネル番号 == eChannel)))
				{
					if (chip.n発声時刻ms > nTime)
					{
						break;
					}
					nIndex_InitialPositionSearchingToPast = nIndex_NearestChip_Future;
				}
				// nIndex_NearestChip_Future++;
			}
			int nIndex_NearestChip_Past = nIndex_InitialPositionSearchingToPast;
			//while ( nIndex_NearestChip_Past >= 0 )			// 過去方向への検索
			for (; nIndex_NearestChip_Past >= 0; nIndex_NearestChip_Past--)
			{
				CChip chip = listChip[nIndex_NearestChip_Past];
				if (chip.b空打ちチップである)
				{
					continue;
				}
				if ((Ech定義.HiHatClose <= eChannel) && (eChannel <= Ech定義.LeftCymbal))
				{
					if ((chip.eチャンネル番号 == eChannel) || (chip.eチャンネル番号 == (eChannel + 0x20)))
					{
						break;
					}
				}
				else if (((eChannel == Ech定義.Guitar_WailingSound) && (chip.e楽器パート == E楽器パート.GUITAR)) ||
					(((Ech定義.Guitar_Open <= eChannel) && (eChannel <= Ech定義.Guitar_Wailing)) && (chip.eチャンネル番号 == eChannel)))
				{
					if ((Ech定義.Guitar_Open <= chip.eチャンネル番号) && (chip.eチャンネル番号 <= Ech定義.Guitar_Wailing))
					{
						break;
					}
				}
				else if (((eChannel == Ech定義.Bass_WailingSound) && (chip.e楽器パート == E楽器パート.BASS)) ||
					(((Ech定義.Bass_Open <= eChannel) && (eChannel <= Ech定義.Bass_Wailing)) && (chip.eチャンネル番号 == eChannel)))
				{
					if ((Ech定義.Bass_Open <= chip.eチャンネル番号) && (chip.eチャンネル番号 <= Ech定義.Bass_Wailing))
					{
						break;
					}
				}
				// nIndex_NearestChip_Past--;
			}

			if (nIndex_NearestChip_Future >= count)
			{
				if (nIndex_NearestChip_Past < 0)	// 検索対象が過去未来どちらにも見つからなかった場合
				{
					return null;
				}
				else 								// 検索対象が未来方向には見つからなかった(しかし過去方向には見つかった)場合
				{
					sw2.Stop();
					return listChip[nIndex_NearestChip_Past];
				}
			}
			else if (nIndex_NearestChip_Past < 0)	// 検索対象が過去方向には見つからなかった(しかし未来方向には見つかった)場合
			{
				sw2.Stop();
				return listChip[nIndex_NearestChip_Future];
			}
			// 検索対象が過去未来の双方に見つかったなら、より近い方を採用する
			CChip nearestChip_Future = listChip[nIndex_NearestChip_Future];
			CChip nearestChip_Past = listChip[nIndex_NearestChip_Past];
			int nDiffTime_Future = Math.Abs((int)(nTime - nearestChip_Future.n発声時刻ms));
			int nDiffTime_Past = Math.Abs((int)(nTime - nearestChip_Past.n発声時刻ms));
			if (nDiffTime_Future >= nDiffTime_Past)
			{
				sw2.Stop();
				return nearestChip_Past;
			}
			sw2.Stop();
			return nearestChip_Future;
		}
		protected void tサウンド再生(CChip rChip, long n再生開始システム時刻ms, E楽器パート part)
		{
			this.tサウンド再生(rChip, n再生開始システム時刻ms, part, CDTXMania.app.ConfigIni.n手動再生音量, false, false);
		}
		protected void tサウンド再生(CChip rChip, long n再生開始システム時刻ms, E楽器パート part, int n音量)
		{
			this.tサウンド再生(rChip, n再生開始システム時刻ms, part, n音量, false, false);
		}
		protected void tサウンド再生(CChip rChip, long n再生開始システム時刻ms, E楽器パート part, int n音量, bool bモニタ)
		{
			this.tサウンド再生(rChip, n再生開始システム時刻ms, part, n音量, bモニタ, false);
		}
		protected void tサウンド再生(CChip pChip, long n再生開始システム時刻ms, E楽器パート part, int n音量, bool bモニタ, bool b音程をずらして再生)
		{
			// mute sound (auto)
			// 4A: HH
			// 4B: CY
			// 4C: RD
			// 4D: LC
			// 2A: Gt
			// AA: Bs
			//

			if (pChip != null)
			{
				bool overwrite = false;
				switch (part)
				{
					case E楽器パート.DRUMS:
						#region [ DRUMS ]
						{
							Ech定義 channel = pChip.eチャンネル番号;
							int index = -1;
							if ((Ech定義.HiHatClose <= channel) && (channel <= Ech定義.LeftCymbal))
							{
								index = channel - Ech定義.HiHatClose;
							}
							else if ((Ech定義.HiHatClose_Hidden <= channel) && (channel <= Ech定義.LeftCymbal_Hidden))
							{
								index = channel - Ech定義.HiHatClose_Hidden;
							}
							// mute sound (auto)
							// 4A: 84: HH (HO/HC)
							// 4B: 85: CY
							// 4C: 86: RD
							// 4D: 87: LC
							// 2A: 88: Gt
							// AA: 89: Bs
							else if (Ech定義.SE24 == channel)	// 仮に今だけ追加 HHは消音処理があるので overwriteフラグ系の処理は改めて不要
							{
								index = 0;
							}
							else if ((Ech定義.SE25 <= channel) && (channel <= Ech定義.SE27))	// 仮に今だけ追加
							{
								pChip.ConvertSE25_26_27ToCY_RCY_LCY();
								index = pChip.eチャンネル番号 - Ech定義.HiHatClose;
								overwrite = true;
							}
							else
							{
								return;
							}
							int nLane = CStage演奏画面共通.nチャンネル0Atoレーン07[index];
							if ((nLane == 1) &&	// 今回演奏するのがHC or HO
								(index == 0 || (
								index == 7 &&
								this.e最後に再生したHHのチャンネル番号 != Ech定義.HiHatOpen &&
								this.e最後に再生したHHのチャンネル番号 != Ech定義.HiHatOpen_Hidden))
								// HCを演奏するか、またはHO演奏＆以前HO演奏でない＆以前不可視HO演奏でない
							)
							// #24772 2011.4.4 yyagi
							// == HH mute condition == 
							//			current HH		So, the mute logics are:
							//				HC	HO		1) All played HC/HOs should be queueing
							// last HH	HC  Yes	Yes		2) If you aren't in "both current/last HH are HO", queued HH should be muted.
							//			HO	Yes	No
							{
								// #23921 2011.1.4 yyagi: 2種類以上のオープンハイハットが発音済みだと、最後のHHOしか消せない問題に対応。
#if TEST_NOTEOFFMODE	// 2011.1.1 yyagi test
								if (CDTXMania.app.DTX.b演奏で直前の音を消音する.HH)
								{
#endif
								for (int i = 0; i < this.L最後に再生したHHの実WAV番号.Count; i++)		// #23921 2011.1.4 yyagi
								{
									// CDTXMania.app.DTX.tWavの再生停止(this.L最後に再生したHHの実WAV番号);
									CDTXMania.app.DTX.tWavの再生停止(this.L最後に再生したHHの実WAV番号[i]);	// #23921 yyagi ストック分全て消音する
								}
								this.L最後に再生したHHの実WAV番号.Clear();
#if TEST_NOTEOFFMODE	// 2011.1.1 yyagi test
								}
#endif
								//this.n最後に再生したHHの実WAV番号 = pChip.n整数値_内部番号;
								this.e最後に再生したHHのチャンネル番号 = pChip.eチャンネル番号;
							}
#if TEST_NOTEOFFMODE	// 2011.1.4 yyagi test
							if (CDTXMania.app.DTX.b演奏で直前の音を消音する.HH)
							{
#endif
							if (index == 0 || index == 7 || index == 0x20 || index == 0x27)			// #23921 HOまたは不可視HO演奏時はそのチップ番号をストックしておく
							{																			// #24772 HC, 不可視HCも消音キューに追加
								if (this.L最後に再生したHHの実WAV番号.Count >= 16)	// #23921 ただしストック数が16以上になるようなら、頭の1個を削って常に16未満に抑える
								{													// (ストックが増えてList<>のrealloc()が発生するのを予防する)
									this.L最後に再生したHHの実WAV番号.RemoveAt(0);
								}
								if (!this.L最後に再生したHHの実WAV番号.Contains(pChip.n整数値_内部番号))	// チップ音がまだストックされてなければ
								{
									this.L最後に再生したHHの実WAV番号.Add(pChip.n整数値_内部番号);			// ストックする
								}
							}
#if TEST_NOTEOFFMODE	// 2011.1.4 yyagi test
							}
#endif
							if (overwrite)
							{
								CDTXMania.app.DTX.tWavの再生停止(this.n最後に再生した実WAV番号[index]);
							}
							CDTXMania.app.DTX.tチップの再生(pChip, n再生開始システム時刻ms, nLane, n音量, bモニタ);
							this.n最後に再生した実WAV番号[nLane] = pChip.n整数値_内部番号;		// nLaneでなくindexにすると、LC(1A-11=09)とギター(enumで09)がかぶってLC音が消されるので注意
							return;
						}
						#endregion
					case E楽器パート.GUITAR:
						#region [ GUITAR ]
#if TEST_NOTEOFFMODE	// 2011.1.1 yyagi test
						if (CDTXMania.app.DTX.b演奏で直前の音を消音する.Guitar) {
#endif
						CDTXMania.app.DTX.tWavの再生停止(this.n最後に再生した実WAV番号.Guitar);
#if TEST_NOTEOFFMODE
						}
#endif
						CDTXMania.app.DTX.tチップの再生(pChip, n再生開始システム時刻ms, (int)Eレーン.Guitar, n音量, bモニタ, b音程をずらして再生);
						this.n最後に再生した実WAV番号.Guitar = pChip.n整数値_内部番号;
						return;
						#endregion
					case E楽器パート.BASS:
						#region [ BASS ]
#if TEST_NOTEOFFMODE
						if (CDTXMania.app.DTX.b演奏で直前の音を消音する.Bass) {
#endif
						CDTXMania.app.DTX.tWavの再生停止(this.n最後に再生した実WAV番号.Bass);
#if TEST_NOTEOFFMODE
						}
#endif
						CDTXMania.app.DTX.tチップの再生(pChip, n再生開始システム時刻ms, (int)Eレーン.Bass, n音量, bモニタ, b音程をずらして再生);
						this.n最後に再生した実WAV番号.Bass = pChip.n整数値_内部番号;
						return;
						#endregion

					default:
						break;
				}
			}
		}
		protected void tステータスパネルの選択()
		{
			if (CDTXMania.app.bコンパクトモード)
			{
				this.actStatusPanels.tラベル名からステータスパネルを決定する(null);
			}
			else if (CDTXMania.app.stage選曲.r確定された曲 != null)
			{
				this.actStatusPanels.tラベル名からステータスパネルを決定する(CDTXMania.app.stage選曲.r確定された曲.ar難易度ラベル[CDTXMania.app.stage選曲.n確定された曲の難易度]);
			}
		}
		protected E判定 tチップのヒット処理(long nHitTime, CChip pChip)
		{
			return tチップのヒット処理(nHitTime, pChip, true);
		}
		protected abstract E判定 tチップのヒット処理(long nHitTime, CChip pChip, bool bCorrectLane);
		protected E判定 tチップのヒット処理(long nHitTime, CChip pChip, E楽器パート screenmode)		// E楽器パート screenmode
		{
			return tチップのヒット処理(nHitTime, pChip, screenmode, true);
		}
		protected E判定 tチップのヒット処理(long nHitTime, CChip pChip, E楽器パート screenmode, bool bCorrectLane)
		{
			pChip.bHit = true;
			#region [メソッド化する前の記述(注釈化)]
			//            bool bPChipIsAutoPlay = false;
			//            bool bGtBsR = ( ( pChip.nチャンネル番号 & 4 ) > 0 );
			//            bool bGtBsG = ( ( pChip.nチャンネル番号 & 2 ) > 0 );
			//            bool bGtBsB = ( ( pChip.nチャンネル番号 & 1 ) > 0 );
			//            bool bGtBsW = ( ( pChip.nチャンネル番号 & 0x0F ) == 0x08 );
			//            bool bGtBsO = ( ( pChip.nチャンネル番号 & 0x0F ) == 0x00 );
			//            if ( pChip.e楽器パート == E楽器パート.DRUMS )
			//            {
			//                if ( bIsAutoPlay[ this.nチャンネル0Atoレーン07[ pChip.nチャンネル番号 - 0x11 ] ] )
			//                {
			//                    bPChipIsAutoPlay = true;
			//                }
			//            }
			//            else if ( pChip.e楽器パート == E楽器パート.GUITAR )
			//            {
			////Trace.TraceInformation( "chip:{0}{1}{2} ", bGtBsR, bGtBsG, bGtBsB );
			////Trace.TraceInformation( "auto:{0}{1}{2} ", bIsAutoPlay[ (int) Eレーン.GtR ], bIsAutoPlay[ (int) Eレーン.GtG ], bIsAutoPlay[ (int) Eレーン.GtB ]);
			//                bPChipIsAutoPlay = true;
			//                if ( !bIsAutoPlay[ (int) Eレーン.GtPick ] ) bPChipIsAutoPlay = false;
			//                else
			//                {
			//                    if ( bGtBsR  && !bIsAutoPlay[ (int) Eレーン.GtR ] ) bPChipIsAutoPlay = false;
			//                    else if ( bGtBsG && !bIsAutoPlay[ (int) Eレーン.GtG ] ) bPChipIsAutoPlay = false;
			//                    else if ( bGtBsB && !bIsAutoPlay[ (int) Eレーン.GtB ] ) bPChipIsAutoPlay = false;
			//                    else if ( bGtBsW && !bIsAutoPlay[ (int) Eレーン.GtW ] ) bPChipIsAutoPlay = false;
			//                    else if ( bGtBsO &&
			//                        ( !bIsAutoPlay[ (int) Eレーン.GtR] || !bIsAutoPlay[ (int) Eレーン.GtG] || !bIsAutoPlay[ (int) Eレーン.GtB] ) )
			//                        bPChipIsAutoPlay = false;
			//                }
			//            }
			//            else if ( pChip.e楽器パート == E楽器パート.BASS )
			//            {
			//                bPChipIsAutoPlay = true;
			//                if ( !bIsAutoPlay[ (int) Eレーン.BsPick ] ) bPChipIsAutoPlay = false;
			//                else
			//                {
			//                    if ( bGtBsR && !bIsAutoPlay[ (int) Eレーン.BsR ] ) bPChipIsAutoPlay = false;
			//                    else if ( bGtBsG && bIsAutoPlay[ (int) Eレーン.BsG ] ) bPChipIsAutoPlay = false;
			//                    else if ( bGtBsB && bIsAutoPlay[ (int) Eレーン.BsB ] ) bPChipIsAutoPlay = false;
			//                    else if ( bGtBsW && bIsAutoPlay[ (int) Eレーン.BsW ] ) bPChipIsAutoPlay = false;
			//                    else if ( bGtBsO &&
			//                        ( !bIsAutoPlay[ (int) Eレーン.BsR ] || !bIsAutoPlay[ (int) Eレーン.BsG ] || !bIsAutoPlay[ (int) Eレーン.BsB ] ) )
			//                        bPChipIsAutoPlay = false;
			//                }
			//            }
			//            else
			//            {
			//                this.bAUTOでないチップが１つでもバーを通過した = true;
			//            }
			////Trace.TraceInformation( "ch={0:x2}, flag={1}",  pChip.nチャンネル番号, bPChipIsAutoPlay.ToString() );
			#endregion
			if (pChip.e楽器パート != E楽器パート.UNKNOWN)
			{
				cInvisibleChip.StartSemiInvisible(pChip.e楽器パート);
			}
			bool bPChipIsAutoPlay = pChip.bAssignAutoPlayState(bIsAutoPlay); // 2011.6.10 yyagi
			E判定 eJudgeResult = E判定.Auto;

			// #35411 2015.08.20 chnmr0 modified (begin)
			bool bIsPerfectGhost = CDTXMania.app.ConfigIni.eAutoGhost[(int)pChip.e楽器パート] == EAutoGhostData.PERFECT ||
					CDTXMania.app.DTX.listAutoGhostLag[(int)pChip.e楽器パート] == null;
			int nInputAdjustTime = bPChipIsAutoPlay && bIsPerfectGhost ? 0 : this.nInputAdjustTimeMs[(int)pChip.e楽器パート];
			eJudgeResult = (bCorrectLane) ? this.e指定時刻からChipのJUDGEを返す(nHitTime, pChip, nInputAdjustTime) : E判定.Miss;

			if (pChip.e楽器パート != E楽器パート.UNKNOWN)
			{
				int nLane = -1;
				switch (pChip.e楽器パート)
				{
					case E楽器パート.DRUMS:
						nLane = CStage演奏画面共通.nチャンネル0Atoレーン07[pChip.eチャンネル番号 - Ech定義.HiHatClose];
						break;
					case E楽器パート.GUITAR:
						nLane = 10;
						break;
					case E楽器パート.BASS:
						nLane = 11;
						break;
				}
				this.actJudgeString.Start(nLane, bPChipIsAutoPlay && bIsPerfectGhost ? E判定.Auto : eJudgeResult, pChip.nLag);
			}
			// #35411 end

			if (!bPChipIsAutoPlay && (pChip.e楽器パート != E楽器パート.UNKNOWN))
			{
				// this.t判定にあわせてゲージを増減する( screenmode, pChip.e楽器パート, eJudgeResult );
				actGauge.Damage(screenmode, pChip.e楽器パート, eJudgeResult);
			}
			if (eJudgeResult == E判定.Poor || eJudgeResult == E判定.Miss || eJudgeResult == E判定.Bad)
			{
				cInvisibleChip.ShowChipTemporally(pChip.e楽器パート);
			}
			switch (pChip.e楽器パート)
			{
				case E楽器パート.DRUMS:
					switch (eJudgeResult)
					{
						case E判定.Miss:
						case E判定.Bad:
							this.nヒット数_Auto含む.Drums.Miss++;
							if (!bPChipIsAutoPlay)
							{
								this.nヒット数_Auto含まない.Drums.Miss++;
							}
							break;
						default:
							this.nヒット数_Auto含む.Drums[(int)eJudgeResult]++;
							if (!bPChipIsAutoPlay)
							{
								this.nヒット数_Auto含まない.Drums[(int)eJudgeResult]++;
							}
							break;
					}

					if (CDTXMania.app.ConfigIni.bドラムが全部オートプレイである || !bPChipIsAutoPlay)
					{
						switch (eJudgeResult)
						{
							case E判定.Perfect:
							case E判定.Great:
							case E判定.Good:
								this.actCombo.n現在のコンボ数.Drums++;
								break;

							default:
								this.actCombo.n現在のコンボ数.Drums = 0;
								break;
						}
					}
					break;

				case E楽器パート.GUITAR:
				case E楽器パート.BASS:
					int indexInst = (int)pChip.e楽器パート;
					switch (eJudgeResult)
					{
						case E判定.Miss:
						case E判定.Bad:
							this.nヒット数_Auto含む[indexInst].Miss++;
							if (!bPChipIsAutoPlay)
							{
								this.nヒット数_Auto含まない[indexInst].Miss++;
							}
							break;
						default:	// #24068 2011.1.10 ikanick changed
							// #24167 2011.1.16 yyagi changed
							this.nヒット数_Auto含む[indexInst][(int)eJudgeResult]++;
							if (!bPChipIsAutoPlay)
							{
								this.nヒット数_Auto含まない[indexInst][(int)eJudgeResult]++;
							}
							break;
					}
					switch (eJudgeResult)
					{
						case E判定.Perfect:
						case E判定.Great:
						case E判定.Good:
							this.actCombo.n現在のコンボ数[indexInst]++;
							break;

						default:
							this.actCombo.n現在のコンボ数[indexInst] = 0;
							break;
					}
					break;

				default:
					break;
			}
			if ((!bPChipIsAutoPlay && (pChip.e楽器パート != E楽器パート.UNKNOWN)) && (eJudgeResult != E判定.Miss) && (eJudgeResult != E判定.Bad))
			{
				int nCombos = this.actCombo.n現在のコンボ数[(int)pChip.e楽器パート];
				long nScoreDelta = 0;
				long[] nComboScoreDelta = new long[] { 350L, 200L, 50L, 0L };
				if ((nCombos <= 500) || (eJudgeResult == E判定.Good))
				{
					nScoreDelta = nComboScoreDelta[(int)eJudgeResult] * nCombos;
				}
				else if ((eJudgeResult == E判定.Perfect) || (eJudgeResult == E判定.Great))
				{
					nScoreDelta = nComboScoreDelta[(int)eJudgeResult] * 500L;
				}
				this.actScore.Add(pChip.e楽器パート, bIsAutoPlay, nScoreDelta);
			}
			return eJudgeResult;
		}
		protected abstract void tチップのヒット処理_BadならびにTight時のMiss(E楽器パート part);
		protected abstract void tチップのヒット処理_BadならびにTight時のMiss(E楽器パート part, int nLane);
		protected void tチップのヒット処理_BadならびにTight時のMiss(E楽器パート part, E楽器パート screenmode)
		{
			this.tチップのヒット処理_BadならびにTight時のMiss(part, 0, screenmode);
		}
		protected void tチップのヒット処理_BadならびにTight時のMiss(E楽器パート part, int nLane, E楽器パート screenmode)
		{
			cInvisibleChip.StartSemiInvisible(part);
			cInvisibleChip.ShowChipTemporally(part);
			//this.t判定にあわせてゲージを増減する( screenmode, part, E判定.Miss );
			actGauge.Damage(screenmode, part, E判定.Miss);
			switch (part)
			{
				case E楽器パート.DRUMS:
					if ((nLane >= 0) && (nLane <= 7))
					{
						this.actJudgeString.Start(nLane, bIsAutoPlay[nLane] ? E判定.Auto : E判定.Miss, 999);
					}
					this.actCombo.n現在のコンボ数.Drums = 0;
					return;

				case E楽器パート.GUITAR:
					this.actJudgeString.Start(10, E判定.Bad, 999);
					this.actCombo.n現在のコンボ数.Guitar = 0;
					return;

				case E楽器パート.BASS:
					this.actJudgeString.Start(11, E判定.Bad, 999);
					this.actCombo.n現在のコンボ数.Bass = 0;
					break;

				default:
					return;
			}
		}

		protected CChip r指定時刻に一番近い未ヒットChip(long nTime, Ech定義 eChannelFlag, int nInputAdjustTime)
		{
			return this.r指定時刻に一番近い未ヒットChip(nTime, eChannelFlag, nInputAdjustTime, 0);
		}
		protected CChip r指定時刻に一番近い未ヒットChip(long nTime, Ech定義 eChannel, int nInputAdjustTime, int n検索範囲時間ms)
		{
			sw2.Start();
			//Trace.TraceInformation( "nTime={0}, nChannel={1:x2}, 現在のTop={2}", nTime, nChannel,CDTXMania.app.DTX.listChip[ this.n現在のトップChip ].n発声時刻ms );
			nTime += nInputAdjustTime;

			int nIndex_InitialPositionSearchingToPast;
			int nTimeDiff;
			if (this.n現在のトップChip == -1)			// 演奏データとして1個もチップがない場合は
			{
				sw2.Stop();
				return null;
			}
			int count = listChip.Count;
			int nIndex_NearestChip_Future = nIndex_InitialPositionSearchingToPast = this.n現在のトップChip;
			if (this.n現在のトップChip >= count)		// その時点で演奏すべきチップが既に全部無くなっていたら
			{
				nIndex_NearestChip_Future = nIndex_InitialPositionSearchingToPast = count - 1;
			}
			// int nIndex_NearestChip_Future = nIndex_InitialPositionSearchingToFuture;
			//			while ( nIndex_NearestChip_Future < count )	// 未来方向への検索
			for (; nIndex_NearestChip_Future < count; nIndex_NearestChip_Future++)
			{
				CChip chip = listChip[nIndex_NearestChip_Future];
				if (!chip.bHit)
				{
					if (chip.b空打ちチップである)
					{
						continue;
					}
					if ((Ech定義.HiHatClose <= eChannel) && (eChannel <= Ech定義.LeftCymbal))
					{
						if ((chip.eチャンネル番号 == eChannel) || (chip.eチャンネル番号 == (eChannel + 0x20)))
						{
							if (chip.n発声時刻ms > nTime)
							{
								break;
							}
							nIndex_InitialPositionSearchingToPast = nIndex_NearestChip_Future;
						}
						continue;
					}
					else if ((((eChannel == Ech定義.Guitar_WailingSound) && (chip.e楽器パート == E楽器パート.GUITAR)) ||
						(((Ech定義.Guitar_Open <= eChannel) && (eChannel <= Ech定義.Guitar_Wailing)) && (chip.eチャンネル番号 == eChannel))))
					{
						if (chip.n発声時刻ms > nTime)
						{
							break;
						}
						nIndex_InitialPositionSearchingToPast = nIndex_NearestChip_Future;
					}
					else if ((((eChannel == Ech定義.Bass_WailingSound) && (chip.e楽器パート == E楽器パート.BASS)) ||
						(((Ech定義.Bass_Open <= eChannel) && (eChannel <= Ech定義.Bass_Wailing)) && (chip.eチャンネル番号 == eChannel))))
					{
						if (chip.n発声時刻ms > nTime)
						{
							break;
						}
						nIndex_InitialPositionSearchingToPast = nIndex_NearestChip_Future;
					}
				}
				//				nIndex_NearestChip_Future++;
			}
			int nIndex_NearestChip_Past = nIndex_InitialPositionSearchingToPast;
			//			while ( nIndex_NearestChip_Past >= 0 )		// 過去方向への検索
			for (; nIndex_NearestChip_Past >= 0; nIndex_NearestChip_Past--)
			{
				CChip chip = listChip[nIndex_NearestChip_Past];
				if (chip.b空打ちチップである)
				{
					continue;
				}
				if ((!chip.bHit) &&
						(
							((eChannel >= Ech定義.HiHatClose) && (eChannel <= Ech定義.LeftCymbal) &&
								((chip.eチャンネル番号 == eChannel) || (chip.eチャンネル番号 == (eChannel + 0x20)))
							)
							||
							(
								((eChannel == Ech定義.Guitar_WailingSound) && (chip.e楽器パート == E楽器パート.GUITAR)) ||
								(((eChannel >= Ech定義.Guitar_Open) && (eChannel <= Ech定義.Guitar_Wailing)) && (chip.eチャンネル番号 == eChannel))
							)
							||
							(
								((eChannel == Ech定義.Bass_WailingSound) && (chip.e楽器パート == E楽器パート.BASS)) ||
								(((eChannel >= Ech定義.Bass_Open) && (eChannel <= Ech定義.Bass_Wailing)) && (chip.eチャンネル番号 == eChannel))
							)
						)
					)
				{
					break;
				}
				//				nIndex_NearestChip_Past--;
			}
			if ((nIndex_NearestChip_Future >= count) && (nIndex_NearestChip_Past < 0))	// 検索対象が過去未来どちらにも見つからなかった場合
			{
				sw2.Stop();
				return null;
			}
			CChip nearestChip;	// = null;	// 以下のifブロックのいずれかで必ずnearestChipには非nullが代入されるので、null初期化を削除
			if (nIndex_NearestChip_Future >= count)											// 検索対象が未来方向には見つからなかった(しかし過去方向には見つかった)場合
			{
				nearestChip = listChip[nIndex_NearestChip_Past];
				//				nTimeDiff = Math.Abs( (int) ( nTime - nearestChip.n発声時刻ms ) );
			}
			else if (nIndex_NearestChip_Past < 0)												// 検索対象が過去方向には見つからなかった(しかし未来方向には見つかった)場合
			{
				nearestChip = listChip[nIndex_NearestChip_Future];
				//				nTimeDiff = Math.Abs( (int) ( nTime - nearestChip.n発声時刻ms ) );
			}
			else
			{
				int nTimeDiff_Future = Math.Abs((int)(nTime - listChip[nIndex_NearestChip_Future].n発声時刻ms));
				int nTimeDiff_Past = Math.Abs((int)(nTime - listChip[nIndex_NearestChip_Past].n発声時刻ms));
				if (nTimeDiff_Future < nTimeDiff_Past)
				{
					nearestChip = listChip[nIndex_NearestChip_Future];
					//					nTimeDiff = Math.Abs( (int) ( nTime - nearestChip.n発声時刻ms ) );
				}
				else
				{
					nearestChip = listChip[nIndex_NearestChip_Past];
					//					nTimeDiff = Math.Abs( (int) ( nTime - nearestChip.n発声時刻ms ) );
				}
			}
			nTimeDiff = Math.Abs((int)(nTime - nearestChip.n発声時刻ms));
			if ((n検索範囲時間ms > 0) && (nTimeDiff > n検索範囲時間ms))					// チップは見つかったが、検索範囲時間外だった場合
			{
				sw2.Stop();
				return null;
			}
			sw2.Stop();
			return nearestChip;
		}

		protected CChip r次に来る指定楽器Chipを更新して返す(E楽器パート inst)
		{
			switch ((int)inst)
			{
				case (int)E楽器パート.GUITAR:
					return r次にくるギターChipを更新して返す();
				case (int)E楽器パート.BASS:
					return r次にくるベースChipを更新して返す();
				default:
					return null;
			}
		}
		protected CChip r次にくるギターChipを更新して返す()
		{
			int nInputAdjustTime = this.bIsAutoPlay.GtPick ? 0 : this.nInputAdjustTimeMs.Guitar;
			this.r次にくるギターChip = this.r指定時刻に一番近い未ヒットChip(CSound管理.rc演奏用タイマ.n現在時刻, Ech定義.Guitar_WailingSound, nInputAdjustTime, 500);
			return this.r次にくるギターChip;
		}
		protected CChip r次にくるベースChipを更新して返す()
		{
			int nInputAdjustTime = this.bIsAutoPlay.BsPick ? 0 : this.nInputAdjustTimeMs.Bass;
			this.r次にくるベースChip = this.r指定時刻に一番近い未ヒットChip(CSound管理.rc演奏用タイマ.n現在時刻, Ech定義.Bass_WailingSound, nInputAdjustTime, 500);
			return this.r次にくるベースChip;
		}

		protected void ChangeInputAdjustTimeInPlaying(IInputDevice keyboard, int plusminus)		// #23580 2011.1.16 yyagi UI for InputAdjustTime in playing screen.
		{
			int part, offset = plusminus;
			if (keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.LeftShift) || keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.RightShift))	// Guitar InputAdjustTime
			{
				part = (int)E楽器パート.GUITAR;
			}
			else if (keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.LeftAlt) || keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.RightAlt))	// Bass InputAdjustTime
			{
				part = (int)E楽器パート.BASS;
			}
			else	// Drums InputAdjustTime
			{
				part = (int)E楽器パート.DRUMS;
			}
			if (!keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.LeftControl) && !keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.RightControl))
			{
				offset *= 10;
			}

			this.nInputAdjustTimeMs[part] += offset;
			if (this.nInputAdjustTimeMs[part] > 99)
			{
				this.nInputAdjustTimeMs[part] = 99;
			}
			else if (this.nInputAdjustTimeMs[part] < -99)
			{
				this.nInputAdjustTimeMs[part] = -99;
			}
			CDTXMania.app.ConfigIni.nInputAdjustTimeMs[part] = this.nInputAdjustTimeMs[part];
		}

		protected abstract void t入力処理_ドラム();
		protected abstract void ドラムスクロール速度アップ();
		protected abstract void ドラムスクロール速度ダウン();
		private int nStartTime_ = 0;
		protected void tキー入力()
		{
			IInputDevice keyboard = CDTXMania.app.Input管理.Keyboard;
			if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.F1) &&
				(keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.RightShift) || keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.LeftShift)))
			{	// shift+f1 (pause)
				this.bPAUSE = !this.bPAUSE;
				if (this.bPAUSE)
				{
					nStartTime_ = (int)CSound管理.rc演奏用タイマ.n現在時刻;
					CSound管理.rc演奏用タイマ.t一時停止();
					CDTXMania.app.Timer.t一時停止();
					CDTXMania.app.DTX.t全チップの再生一時停止();
					CDTXMania.app.DTX.t全AVIの一時停止();
				}
				else
				{
					CDTXMania.app.DTX.t全AVIの再生再開();
					//CDTXMania.app.DTX.t全チップの再生再開();
					#region [ PAUSE連打でのBGMずれ対策 (AVIはずれたままになるが無視・・・) ]
					List<CSound> pausedCSound = new List<CSound>();
					for (int i = this.n現在のトップChip; i >= 0; i--)
					{
						CChip pChip = CDTXMania.app.DTX.listChip[i];
						int nDuration = pChip.GetDuration();

						if ((pChip.n発声時刻ms + nDuration > 0) && (pChip.n発声時刻ms <= nStartTime_) && (nStartTime_ <= pChip.n発声時刻ms + nDuration))
						{
							if (pChip.bWAVを使うチャンネルである && !pChip.b空打ちチップである)	// wav系チャンネル、且つ、空打ちチップではない
							{
								CDTX.CWAV wc;
								bool b = CDTXMania.app.DTX.listWAV.TryGetValue(pChip.n整数値_内部番号, out wc);
								if (!b) continue;

								if ((wc.bIsBGMSound && CDTXMania.app.ConfigIni.bBGM音を発声する) || (!wc.bIsBGMSound))
								{
									CDTXMania.app.DTX.tチップの再生(pChip, CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, (int)Eレーン.BGM, CDTXMania.app.DTX.nモニタを考慮した音量(E楽器パート.UNKNOWN));
									#region [ PAUSEする ]
									int j = wc.n現在再生中のサウンド番号;
									if (wc.rSound[j] != null)
									{
										wc.rSound[j].t再生を一時停止する();
										wc.rSound[j].t再生位置を変更する(nStartTime_ - pChip.n発声時刻ms);
										pausedCSound.Add(wc.rSound[j]);
									}
									#endregion
								}
							}
						}
					}
					foreach (CSound cs in pausedCSound)
					{
						cs.tサウンドを再生する();
					}
					#endregion
					CDTXMania.app.Timer.t再開();
					CSound管理.rc演奏用タイマ.t再開();

				}
			}
			if ((!this.bPAUSE && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED)) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト))
			{
				this.t入力処理_ドラム();
				this.t入力処理_ギターベース(E楽器パート.GUITAR);
				this.t入力処理_ギターベース(E楽器パート.BASS);
				if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.UpArrow) && (keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.RightShift) || keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.LeftShift)))
				{	// shift (+ctrl) + UpArrow (BGMAdjust)
					CDTXMania.app.DTX.t各自動再生音チップの再生時刻を変更する((keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.LeftControl) || keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.RightControl)) ? 1 : 10);
					CDTXMania.app.DTX.tWave再生位置自動補正();
				}
				else if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.DownArrow) && (keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.RightShift) || keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.LeftShift)))
				{	// shift + DownArrow (BGMAdjust)
					CDTXMania.app.DTX.t各自動再生音チップの再生時刻を変更する((keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.LeftControl) || keyboard.bキーが押されている((int)SlimDX.DirectInput.Key.RightControl)) ? -1 : -10);
					CDTXMania.app.DTX.tWave再生位置自動補正();
				}
				else if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.UpArrow))
				{	// UpArrow(scrollspeed up)
					ドラムスクロール速度アップ();
				}
				else if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.DownArrow))
				{	// DownArrow (scrollspeed down)
					ドラムスクロール速度ダウン();
				}
				else if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.Delete))
				{	// del (debug info)
					CDTXMania.app.ConfigIni.b演奏情報を表示する = !CDTXMania.app.ConfigIni.b演奏情報を表示する;
				}
				else if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.LeftArrow))		// #24243 2011.1.16 yyagi UI for InputAdjustTime in playing screen.
				{
					ChangeInputAdjustTimeInPlaying(keyboard, -1);
				}
				else if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.RightArrow))		// #24243 2011.1.16 yyagi UI for InputAdjustTime in playing screen.
				{
					ChangeInputAdjustTimeInPlaying(keyboard, +1);
				}
				else if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.F5))
				{
					int n描画遅延ms = CDTXMania.app.ConfigIni.nJudgeLinePosOffset.Drums;
					n描画遅延ms = Math.Max(n描画遅延ms - 1, -99);
					CDTXMania.app.ConfigIni.nJudgeLinePosOffset.Drums =
					CDTXMania.app.ConfigIni.nJudgeLinePosOffset.Guitar =
					CDTXMania.app.ConfigIni.nJudgeLinePosOffset.Bass = n描画遅延ms;
					this.演奏判定ライン座標.nJudgeLinePosY_delta.Drums =
					this.演奏判定ライン座標.nJudgeLinePosY_delta.Guitar =
					this.演奏判定ライン座標.nJudgeLinePosY_delta.Bass = n描画遅延ms;
				}
				else if (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.F6))
				{
					int n描画遅延ms = CDTXMania.app.ConfigIni.nJudgeLinePosOffset.Drums;
					n描画遅延ms = Math.Min(n描画遅延ms + 1, 99);
					CDTXMania.app.ConfigIni.nJudgeLinePosOffset.Drums =
					CDTXMania.app.ConfigIni.nJudgeLinePosOffset.Guitar =
					CDTXMania.app.ConfigIni.nJudgeLinePosOffset.Bass = n描画遅延ms;
					this.演奏判定ライン座標.nJudgeLinePosY_delta.Drums =
					this.演奏判定ライン座標.nJudgeLinePosY_delta.Guitar =
					this.演奏判定ライン座標.nJudgeLinePosY_delta.Bass = n描画遅延ms;
				}
				else if ((base.eフェーズID == CStage.Eフェーズ.共通_通常状態) && (keyboard.bキーが押された((int)SlimDX.DirectInput.Key.Escape) || CDTXMania.app.Pad.b押されたGB(Eパッド.FT)))
				{	// escape (exit)
					this.actFO.tフェードアウト開始();
					base.eフェーズID = CStage.Eフェーズ.共通_フェードアウト;
					this.eフェードアウト完了時の戻り値 = E演奏画面の戻り値.演奏中断;
				}
			}
		}

		protected void t入力メソッド記憶(E楽器パート part)
		{
			if (CDTXMania.app.Pad.st検知したデバイス.Keyboard)
			{
				this.b演奏にキーボードを使った[(int)part] = true;
			}
			if (CDTXMania.app.Pad.st検知したデバイス.Joypad)
			{
				this.b演奏にジョイパッドを使った[(int)part] = true;
			}
			if (CDTXMania.app.Pad.st検知したデバイス.MIDIIN)
			{
				this.b演奏にMIDI入力を使った[(int)part] = true;
			}
			if (CDTXMania.app.Pad.st検知したデバイス.Mouse)
			{
				this.b演奏にマウスを使った[(int)part] = true;
			}
		}
		protected abstract void t進行描画_AVI();
		protected void t進行描画_AVI(int x, int y)
		{
			if (((base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト)) && (!CDTXMania.app.ConfigIni.bストイックモード && CDTXMania.app.ConfigIni.bAVI有効))
			{
				this.actAVI.t進行描画(x, y, 556, 710);
			}
		}
		protected abstract void t進行描画_BGA();
		protected void t進行描画_BGA(int x, int y)
		{
			if (((base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト)) && (!CDTXMania.app.ConfigIni.bストイックモード && CDTXMania.app.ConfigIni.bBGA有効))
			{
				this.actBGA.t進行描画(x, y);
			}
		}
		protected abstract void t進行描画_DANGER();
		protected void t進行描画_MIDIBGM()
		{
			if (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED)
			{
				CStage.Eフェーズ eフェーズid1 = base.eフェーズID;
			}
		}
		protected void t進行描画_RGBボタン()
		{
			if (CDTXMania.app.ConfigIni.eDark != Eダークモード.FULL)
			{
				this.actRGB.t進行描画(演奏判定ライン座標);
			}
		}
		protected void t進行描画_STAGEFAILED()
		{
			if (((base.eフェーズID == CStage.Eフェーズ.演奏_STAGE_FAILED) || (base.eフェーズID == CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト)) && ((this.actStageFailed.On進行描画() != 0) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト)))
			{
				this.eフェードアウト完了時の戻り値 = E演奏画面の戻り値.ステージ失敗;
				base.eフェーズID = CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト;
				this.actFO.tフェードアウト開始();
			}
		}
		protected void t進行描画_WailingBonus()
		{
			if ((base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト))
			{
				this.actWailingBonus.On進行描画();
			}
		}
		protected abstract void t進行描画_Wailing枠();
		protected void t進行描画_Wailing枠(int GtWailingFrameX, int BsWailingFrameX, int GtWailingFrameY, int BsWailingFrameY)
		{
			if ((CDTXMania.app.ConfigIni.eDark != Eダークモード.FULL) && CDTXMania.app.ConfigIni.bGuitar有効)
			{
				if (this.txWailing枠 != null)
				{
					if (CDTXMania.app.DTX.bチップがある.Guitar)
					{
						this.txWailing枠.t2D描画(CDTXMania.app.Device, GtWailingFrameX, GtWailingFrameY);
					}
					if (CDTXMania.app.DTX.bチップがある.Bass)
					{
						this.txWailing枠.t2D描画(CDTXMania.app.Device, BsWailingFrameX, BsWailingFrameY);
					}
				}
			}
		}


		protected void t進行描画_チップファイアGB()
		{
			this.actChipFireGB.On進行描画();
		}
		protected abstract void t進行描画_パネル文字列();
		protected void t進行描画_パネル文字列(int x, int y)
		{
			if ((base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト))
			{
				this.actPanel.t進行描画(x, y);
			}
		}
		protected void tパネル文字列の設定()
		{
			this.actPanel.SetPanelString(string.IsNullOrEmpty(CDTXMania.app.DTX.PANEL) ? CDTXMania.app.DTX.TITLE : CDTXMania.app.DTX.PANEL);
		}


		protected void t進行描画_ゲージ()
		{
			if (((CDTXMania.app.ConfigIni.eDark != Eダークモード.HALF) && (CDTXMania.app.ConfigIni.eDark != Eダークモード.FULL)) && ((base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト)))
			{
				this.actGauge.On進行描画();
			}
		}
		protected void t進行描画_コンボ()
		{
			this.actCombo.On進行描画();
		}
		protected void t進行描画_スコア()
		{
			this.actScore.On進行描画();
		}
		protected void t進行描画_ステータスパネル()
		{
			this.actStatusPanels.On進行描画();
		}
		protected bool t進行描画_チップ(E楽器パート ePlayMode)
		{
			if ((base.eフェーズID == CStage.Eフェーズ.演奏_STAGE_FAILED) || (base.eフェーズID == CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト))
			{
				return true;
			}
			if ((this.n現在のトップChip == -1) || (this.n現在のトップChip >= listChip.Count))
			{
				return true;
			}
			if (this.n現在のトップChip == -1)
			{
				return true;
			}

			CDTX dTX = CDTXMania.app.DTX;
			CConfigIni configIni = CDTXMania.app.ConfigIni;
			for (int nCurrentTopChip = this.n現在のトップChip; nCurrentTopChip < dTX.listChip.Count; nCurrentTopChip++)
			{
				CChip pChip = dTX.listChip[nCurrentTopChip];
				//Debug.WriteLine( "nCurrentTopChip=" + nCurrentTopChip + ", ch=" + pChip.nチャンネル番号.ToString("x2") + ", 発音位置=" + pChip.n発声位置 + ", 発声時刻ms=" + pChip.n発声時刻ms );
				pChip.CalcDistanceFromBar(CSound管理.rc演奏用タイマ.n現在時刻, this.act譜面スクロール速度.db現在の譜面スクロール速度);
				if (Math.Min(Math.Min(pChip.nバーからの距離dot.Drums, pChip.nバーからの距離dot.Guitar), pChip.nバーからの距離dot.Bass) > 450 * Scale.Y)
				{
					break;
				}
				//				if ( ( ( nCurrentTopChip == this.n現在のトップChip ) && ( pChip.nバーからの距離dot.Drums < -65 ) ) && pChip.bHit )
				// #28026 2012.4.5 yyagi; 信心ワールドエンドの曲終了後リザルトになかなか行かない問題の修正

				if ((dTX.listChip[this.n現在のトップChip].nバーからの距離dot.Drums < -65 * Scale.Y) &&	// 小節線の消失処理などに影響するため、
					(dTX.listChip[this.n現在のトップChip].nバーからの距離dot.Guitar < -65 * Scale.Y) &&	// Drumsのスクロールスピードだけには依存させない。
					(dTX.listChip[this.n現在のトップChip].nバーからの距離dot.Bass < -65 * Scale.Y) &&
					dTX.listChip[this.n現在のトップChip].bHit)
				{
					//					nCurrentTopChip = ++this.n現在のトップChip;
					++this.n現在のトップChip;
					continue;
				}


				bool bPChipIsAutoPlay = pChip.bAssignAutoPlayState(bIsAutoPlay);

				int nInputAdjustTime = (bPChipIsAutoPlay || (pChip.e楽器パート == E楽器パート.UNKNOWN)) ? 0 : this.nInputAdjustTimeMs[(int)pChip.e楽器パート];

				int instIndex = (int)pChip.e楽器パート;
				if (((pChip.e楽器パート != E楽器パート.UNKNOWN) && !pChip.bHit) &&
						((pChip.nバーからの距離dot[instIndex] < -40 * Scale.Y) &&
					(this.e指定時刻からChipのJUDGEを返す(CSound管理.rc演奏用タイマ.n現在時刻, pChip, nInputAdjustTime) == E判定.Miss)))
				{
					this.tチップのヒット処理(CSound管理.rc演奏用タイマ.n現在時刻, pChip);	//チップ消失(Hitせずスルーした場合)
				}
				if (((pChip.e楽器パート != E楽器パート.UNKNOWN) && !pChip.bHit) &&
					((pChip.nバーからの距離dot[instIndex] + this.演奏判定ライン座標.nJudgeLinePosY_delta[instIndex] < 0)))
				{
					//Debug.WriteLine( "透明度＝" + pChip.n透明度 );
					pChip.n透明度 -= 12;		// チップが判定バーを越えたら、徐々に透明にする。VSyncWaitの有無で加減が変わるが・・
					if (pChip.n透明度 < 0)
					{
						pChip.n透明度 = 0;
					}
				}

				// #35411 chnmr0 add (ターゲットゴースト)
				if (CDTXMania.app.ConfigIni.eTargetGhost[instIndex] != ETargetGhostData.NONE &&
						 CDTXMania.app.DTX.listTargetGhsotLag[instIndex] != null &&
						 pChip.e楽器パート != E楽器パート.UNKNOWN &&
						 pChip.nバーからの距離dot[instIndex] < 0)
				{
					if (!pChip.bTargetGhost判定済み)
					{
						pChip.bTargetGhost判定済み = true;

						int ghostLag = 128;
						if (0 <= pChip.n楽器パートでの出現順 && pChip.n楽器パートでの出現順 < CDTXMania.app.DTX.listTargetGhsotLag[instIndex].Count)
						{
							ghostLag = CDTXMania.app.DTX.listTargetGhsotLag[instIndex][pChip.n楽器パートでの出現順];
							// 上位８ビットが１ならコンボが途切れている（ギターBAD空打ちでコンボ数を再現するための措置）
							if (ghostLag > 255)
							{
								this.nコンボ数_TargetGhost[instIndex] = 0;
							}
							ghostLag = (ghostLag & 255) - 128;
						}
						else if (CDTXMania.app.ConfigIni.eTargetGhost[instIndex] == ETargetGhostData.PERFECT)
						{
							ghostLag = 0;
						}

						if (ghostLag <= 127)
						{
							E判定 eJudge = this.e指定時刻からChipのJUDGEを返す(pChip.n発声時刻ms + ghostLag, pChip, 0, false);
							this.nヒット数_TargetGhost[instIndex][(int)eJudge]++;
							if (eJudge == E判定.Miss || eJudge == E判定.Poor)
							{
								this.n最大コンボ数_TargetGhost[instIndex] = Math.Max(this.n最大コンボ数_TargetGhost[instIndex], this.nコンボ数_TargetGhost[instIndex]);
								this.nコンボ数_TargetGhost[instIndex] = 0;
							}
							else
							{
								this.nコンボ数_TargetGhost[instIndex]++;
							}
						}
					}
				}



				switch (pChip.eチャンネル番号)
				{
					#region [ 01: BGM ]
					case Ech定義.BGM:	// BGM
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							if (configIni.bBGM音を発声する)
							{
								//long t = CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms;
								//Trace.TraceInformation( "BGM再生開始: 演奏タイマのn前回リセットしたときのシステム時刻=" + CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + ", pChip.n発生時刻ms=" + pChip.n発声時刻ms + ", 合計=" + t );
								dTX.tチップの再生(pChip, CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, (int)Eレーン.BGM, dTX.nモニタを考慮した音量(E楽器パート.UNKNOWN));
							}
						}
						break;
					#endregion
					#region [ 03: BPM変更 ]
					case Ech定義.BPM:	// BPM変更
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							this.actPlayInfo.dbBPM = (pChip.n整数値 * (((double)configIni.n演奏速度) / 20.0)) + dTX.BASEBPM;
						}
						break;
					#endregion
					#region [ 04, 07, 55, 56,57, 58, 59, 60:レイヤーBGA ]
					case Ech定義.BGALayer1:	// レイヤーBGA
					case Ech定義.BGALayer2:
					case Ech定義.BGALayer3:
					case Ech定義.BGALayer4:
					case Ech定義.BGALayer5:
					case Ech定義.BGALayer6:
					case Ech定義.BGALayer7:
					case Ech定義.BGALayer8:
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							if (configIni.bBGA有効)
							{
								switch (pChip.eBGA種別)
								{
									case EBGA種別.BMPTEX:
										if (pChip.rBMPTEX != null)
										{
											this.actBGA.Start(pChip.eチャンネル番号, null, pChip.rBMPTEX, pChip.rBMPTEX.tx画像.sz画像サイズ.Width, pChip.rBMPTEX.tx画像.sz画像サイズ.Height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
										}
										break;

									case EBGA種別.BGA:
										if ((pChip.rBGA != null) && ((pChip.rBMP != null) || (pChip.rBMPTEX != null)))
										{
											this.actBGA.Start(pChip.eチャンネル番号, pChip.rBMP, pChip.rBMPTEX, pChip.rBGA.pt画像側右下座標.X - pChip.rBGA.pt画像側左上座標.X, pChip.rBGA.pt画像側右下座標.Y - pChip.rBGA.pt画像側左上座標.Y, 0, 0, pChip.rBGA.pt画像側左上座標.X, pChip.rBGA.pt画像側左上座標.Y, 0, 0, pChip.rBGA.pt表示座標.X, pChip.rBGA.pt表示座標.Y, 0, 0, 0);
										}
										break;

									case EBGA種別.BGAPAN:
										if ((pChip.rBGAPan != null) && ((pChip.rBMP != null) || (pChip.rBMPTEX != null)))
										{
											this.actBGA.Start(pChip.eチャンネル番号, pChip.rBMP, pChip.rBMPTEX, pChip.rBGAPan.sz開始サイズ.Width, pChip.rBGAPan.sz開始サイズ.Height, pChip.rBGAPan.sz終了サイズ.Width, pChip.rBGAPan.sz終了サイズ.Height, pChip.rBGAPan.pt画像側開始位置.X, pChip.rBGAPan.pt画像側開始位置.Y, pChip.rBGAPan.pt画像側終了位置.X, pChip.rBGAPan.pt画像側終了位置.Y, pChip.rBGAPan.pt表示側開始位置.X, pChip.rBGAPan.pt表示側開始位置.Y, pChip.rBGAPan.pt表示側終了位置.X, pChip.rBGAPan.pt表示側終了位置.Y, pChip.n総移動時間);
										}
										break;

									default:
										if (pChip.rBMP != null)
										{
											this.actBGA.Start(pChip.eチャンネル番号, pChip.rBMP, null, pChip.rBMP.n幅, pChip.rBMP.n高さ, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
										}
										break;
								}
							}
						}
						break;
					#endregion
					#region [ 08: BPM変更(拡張) ]
					case Ech定義.BPMEx:	// BPM変更(拡張)
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							if (dTX.listBPM.ContainsKey(pChip.n整数値_内部番号))
							{
								this.actPlayInfo.dbBPM = (dTX.listBPM[pChip.n整数値_内部番号].dbBPM値 * (((double)configIni.n演奏速度) / 20.0)) + dTX.BASEBPM;
							}
						}
						break;
					#endregion
					#region [ 11-1a: ドラム演奏 ]
					case Ech定義.HiHatClose:	// ドラム演奏
					case Ech定義.Snare:
					case Ech定義.BassDrum:
					case Ech定義.HighTom:
					case Ech定義.LowTom:
					case Ech定義.Cymbal:
					case Ech定義.FloorTom:
					case Ech定義.HiHatOpen:
					case Ech定義.RideCymbal:
					case Ech定義.LeftCymbal:
						if (pChip.b空打ちチップである)
						{
							this.t進行描画_チップ_空打ち音設定_ドラム(configIni, ref dTX, ref pChip);
						}
						else
						{
							this.t進行描画_チップ_ドラムス(configIni, ref dTX, ref pChip);
						}
						break;
					#endregion
					#region [ 1f: フィルインサウンド(ドラム) ]
					case Ech定義.DrumsFillin:	// フィルインサウンド(ドラム)
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							this.r現在の歓声Chip.Drums = pChip;
						}
						break;
					#endregion
					#region [ 20-27: ギター演奏 ]
					case Ech定義.Guitar_Open:	// ギター演奏
					case Ech定義.Guitar_xxB:
					case Ech定義.Guitar_xGx:
					case Ech定義.Guitar_xGB:
					case Ech定義.Guitar_Rxx:
					case Ech定義.Guitar_RxB:
					case Ech定義.Guitar_RGx:
					case Ech定義.Guitar_RGB:
						this.t進行描画_チップ_ギターベース(configIni, ref dTX, ref pChip, E楽器パート.GUITAR);
						break;
					#endregion
					#region [ 28: ウェイリング(ギター) ]
					case Ech定義.Guitar_Wailing:	// ウェイリング(ギター)
						this.t進行描画_チップ_ギター_ウェイリング(configIni, ref dTX, ref pChip, !CDTXMania.app.ConfigIni.bDrums有効);
						break;
					#endregion
					#region [ 2f: ウェイリングサウンド(ギター) ]
					case Ech定義.Guitar_WailingSound:	// ウェイリングサウンド(ギター)
						if (!pChip.bHit && (pChip.nバーからの距離dot.Guitar < 0))
						{
							pChip.bHit = true;
							this.r現在の歓声Chip.Guitar = pChip;
						}
						break;
					#endregion
					#region [ 31-3a: 不可視チップ配置(ドラム) ]
					case Ech定義.HiHatClose_Hidden:	// 不可視チップ配置(ドラム)
					case Ech定義.Snare_Hidden:
					case Ech定義.BassDrum_Hidden:
					case Ech定義.HighTom_Hidden:
					case Ech定義.LowTom_Hidden:
					case Ech定義.Cymbal_Hidden:
					case Ech定義.FloorTom_Hidden:
					case Ech定義.HiHatOpen_Hidden:
					case Ech定義.RideCymbal_Hidden:
					case Ech定義.LeftCymbal_Hidden:
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
						}
						break;
					#endregion
					#region [ 50: 小節線 ]
					case Ech定義.BarLine:	// 小節線
						{
							this.t進行描画_チップ_小節線(configIni, ref dTX, ref pChip);
							break;
						}
					#endregion
					#region [ 51: 拍線 ]
					case Ech定義.BeatLine:	// 拍線
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
						}
						if ((ePlayMode == E楽器パート.DRUMS) && (configIni.eDark != Eダークモード.FULL) && pChip.b可視 && (this.txチップ != null))
						{
							this.txチップ.t2D描画(CDTXMania.app.Device,
								configIni.eドラムレーン表示位置 == Eドラムレーン表示位置.Left ?
									105 : 619,
								configIni.bReverse.Drums ?
								//(int) ( ( ( 0x38 + pChip.nバーからの距離dot.Drums ) - 1 ) * Scale.Y ) :
								//(int) ( ( ( 0x1a6 - pChip.nバーからの距離dot.Drums ) - 1 ) * Scale.Y ),
									124 + pChip.nバーからの距離dot.Drums : 947 - pChip.nバーからの距離dot.Drums,
								new Rectangle(
									0,
									1006,
									(configIni.eドラムレーン表示位置 == Eドラムレーン表示位置.Left) ? 888 : 682,
									2
								)
							);
						}
						break;
					#endregion
					#region [ 52: MIDIコーラス ]
					case Ech定義.MIDIChorus:	// MIDIコーラス
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
						}
						break;
					#endregion
					#region [ 53: フィルイン ]
					case Ech定義.FillIn:	// フィルイン
						this.t進行描画_チップ_フィルイン(configIni, ref dTX, ref pChip);
						break;
					#endregion
					#region [ 54: 動画再生(BGA領域), 5A: 動画再生(全画面) ]
					case Ech定義.Movie:		// 動画再生 (BGA領域)
					case Ech定義.MovieFull:	// 動画再生 (全画面)
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							if (configIni.bAVI有効)
							{
								if (CDTXMania.app.DTX.bチップがある.BGA)
								{
									this.actAVI.bHasBGA = true;
								}
								if (pChip.eチャンネル番号 == Ech定義.MovieFull || CDTXMania.app.ConfigIni.bForceAVIFullscreen)
								{
									this.actAVI.bFullScreenMovie = true;
								}
								switch (pChip.eAVI種別)
								{
									case EAVI種別.AVI:
										//if ( pChip.rAVI != null )
										{
											//int startWidth = ( CDTXMania.app.DTX.bチップがある.BGA ) ? 278 : SampleFramework.GameWindowSize.Width;
											//int startHeight = ( CDTXMania.app.DTX.bチップがある.BGA ) ? 355 : SampleFramework.GameWindowSize.Height;
											int startWidth = !this.actAVI.bFullScreenMovie ? 278 : SampleFramework.GameWindowSize.Width;
											int startHeight = !this.actAVI.bFullScreenMovie ? 355 : SampleFramework.GameWindowSize.Height;
											this.actAVI.Start(pChip.eチャンネル番号, pChip.rAVI, startWidth, startHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, pChip.n発声時刻ms);
										}
										break;

									case EAVI種別.AVIPAN:
										if (pChip.rAVIPan != null)
										{
											this.actAVI.Start(pChip.eチャンネル番号, pChip.rAVI, pChip.rAVIPan.sz開始サイズ.Width, pChip.rAVIPan.sz開始サイズ.Height, pChip.rAVIPan.sz終了サイズ.Width, pChip.rAVIPan.sz終了サイズ.Height, pChip.rAVIPan.pt動画側開始位置.X, pChip.rAVIPan.pt動画側開始位置.Y, pChip.rAVIPan.pt動画側終了位置.X, pChip.rAVIPan.pt動画側終了位置.Y, pChip.rAVIPan.pt表示側開始位置.X, pChip.rAVIPan.pt表示側開始位置.Y, pChip.rAVIPan.pt表示側終了位置.X, pChip.rAVIPan.pt表示側終了位置.Y, pChip.n総移動時間, pChip.n発声時刻ms);
										}
										break;
								}
							}
						}
						break;
					#endregion
					#region [ 61-92: 自動再生(BGM, SE) ]
					case Ech定義.SE01:
					case Ech定義.SE02:
					case Ech定義.SE03:
					case Ech定義.SE04:	// 自動再生(BGM, SE)
					case Ech定義.SE05:
					case Ech定義.SE06:
					case Ech定義.SE07:
					case Ech定義.SE08:
					case Ech定義.SE09:
					case Ech定義.SE10:
					case Ech定義.SE11:
					case Ech定義.SE12:
					case Ech定義.SE13:
					case Ech定義.SE14:
					case Ech定義.SE15:
					case Ech定義.SE16:
					case Ech定義.SE17:
					case Ech定義.SE18:
					case Ech定義.SE19:
					case Ech定義.SE20:
					case Ech定義.SE21:
					case Ech定義.SE22:
					case Ech定義.SE23:
					case Ech定義.SE30:
					case Ech定義.SE31:
					case Ech定義.SE32:
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							if (configIni.bBGM音を発声する)
							{
								dTX.tWavの再生停止(this.n最後に再生したBGMの実WAV番号[pChip.eチャンネル番号 - Ech定義.SE01]);
								dTX.tチップの再生(pChip, CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, (int)Eレーン.BGM, dTX.nモニタを考慮した音量(E楽器パート.UNKNOWN));
								this.n最後に再生したBGMの実WAV番号[pChip.eチャンネル番号 - Ech定義.SE01] = pChip.n整数値_内部番号;
							}
						}
						break;
					#endregion


					#region [ 84-89: 仮: override sound ]	// #26338 2011.11.8 yyagi
					case Ech定義.SE24:	// HH (HO/HC)
					case Ech定義.SE25:	// CY
					case Ech定義.SE26:	// RD
					case Ech定義.SE27:	// LC
					case Ech定義.SE28:	// Guitar
					case Ech定義.SE29:	// Bass
						// mute sound (auto)
						// 4A: 84: HH (HO/HC)
						// 4B: 85: CY
						// 4C: 86: RD
						// 4D: 87: LC
						// 2A: 88: Gt
						// AA: 89: Bs

						//	CDTXMania.app.DTX.tWavの再生停止( this.n最後に再生した実WAV番号.Guitar );
						//	CDTXMania.app.DTX.tチップの再生( pChip, n再生開始システム時刻ms, 8, n音量, bモニタ, b音程をずらして再生 );
						//	this.n最後に再生した実WAV番号.Guitar = pChip.n整数値_内部番号;

						//	protected void tサウンド再生( CDTX.CChip pChip, long n再生開始システム時刻ms, E楽器パート part, int n音量, bool bモニタ, bool b音程をずらして再生 )
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							E楽器パート[] p = { E楽器パート.DRUMS, E楽器パート.DRUMS, E楽器パート.DRUMS, E楽器パート.DRUMS, E楽器パート.GUITAR, E楽器パート.BASS };

							E楽器パート pp = p[pChip.eチャンネル番号 - Ech定義.SE24];

							//							if ( pp == E楽器パート.DRUMS ) {			// pChip.nチャンネル番号= ..... HHとか、ドラムの場合は変える。
							//								//            HC    CY    RD    LC
							//								int[] ch = { 0x11, 0x16, 0x19, 0x1A };
							//								pChip.nチャンネル番号 = ch[ pChip.nチャンネル番号 - 0x84 ]; 
							//							}
							this.tサウンド再生(pChip, CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, pp, dTX.nモニタを考慮した音量(pp));
						}
						break;
					#endregion

					#region [ a0-a7: ベース演奏 ]
					case Ech定義.Bass_Open:	// ベース演奏
					case Ech定義.Bass_xxB:
					case Ech定義.Bass_xGx:
					case Ech定義.Bass_xGB:
					case Ech定義.Bass_Rxx:
					case Ech定義.Bass_RxB:
					case Ech定義.Bass_RGx:
					case Ech定義.Bass_RGB:
						this.t進行描画_チップ_ギターベース(configIni, ref dTX, ref pChip, E楽器パート.BASS);
						break;
					#endregion
					#region [ a8: ウェイリング(ベース) ]
					case Ech定義.Bass_Wailing:	// ウェイリング(ベース)
						this.t進行描画_チップ_ベース_ウェイリング(configIni, ref dTX, ref pChip, !CDTXMania.app.ConfigIni.bDrums有効);
						break;
					#endregion
					#region [ af: ウェイリングサウンド(ベース) ]
					case Ech定義.Bass_WailingSound:	// ウェイリングサウンド(ベース)
						if (!pChip.bHit && (pChip.nバーからの距離dot.Bass < 0))
						{
							pChip.bHit = true;
							this.r現在の歓声Chip.Bass = pChip;
						}
						break;
					#endregion
					#region [ b1-b9, bc: 空打ち音設定(ドラム) ]
					case Ech定義.HiHatClose_NoChip:	// 空打ち音設定(ドラム)
					case Ech定義.Snare_NoChip:
					case Ech定義.BassDrum_NoChip:
					case Ech定義.HighTom_NoChip:
					case Ech定義.LowTom_NoChip:
					case Ech定義.Cymbal_NoChip:
					case Ech定義.FloorTom_NoChip:
					case Ech定義.HiHatOpen_NoChip:
					case Ech定義.RideCymbal_NoChip:
					case Ech定義.LeftCymbal_NoChip:
						// ここには来なくなったはずだが、一応残しておく
						this.t進行描画_チップ_空打ち音設定_ドラム(configIni, ref dTX, ref pChip);
						break;
					#endregion
					#region [ ba: 空打ち音設定(ギター) ]
					case Ech定義.Guitar_NoChip:	// 空打ち音設定(ギター)
						if (!pChip.bHit && (pChip.nバーからの距離dot.Guitar < 0))
						{
							pChip.bHit = true;
							this.r現在の空うちギターChip = pChip;
							pChip.SetNoChipGuitarToOpen();
						}
						break;
					#endregion
					#region [ bb: 空打ち音設定(ベース) ]
					case Ech定義.Bass_NoChip:	// 空打ち音設定(ベース)
						if (!pChip.bHit && (pChip.nバーからの距離dot.Bass < 0))
						{
							pChip.bHit = true;
							this.r現在の空うちベースChip = pChip;
							pChip.SetNoChipBassToOpen();
						}
						break;
					#endregion
					#region [ c4, c7, d5-d9, e0: BGA画像入れ替え ]
					case Ech定義.BGALayer1_Swap:
					case Ech定義.BGALayer2_Swap:
					case Ech定義.BGALayer3_Swap:
					case Ech定義.BGALayer4_Swap:	// BGA画像入れ替え
					case Ech定義.BGALayer5_Swap:
					case Ech定義.BGALayer6_Swap:
					case Ech定義.BGALayer7_Swap:
					case Ech定義.BGALayer8_Swap:
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
							if ((configIni.bBGA有効 && (pChip.eBGA種別 == EBGA種別.BMP)) || (pChip.eBGA種別 == EBGA種別.BMPTEX))
							{
								for (int i = 0; i < 8; i++)
								{
									if (this.nBGAスコープチャンネルマップ[0, i] == pChip.eチャンネル番号)
									{
										this.actBGA.ChangeScope(this.nBGAスコープチャンネルマップ[1, i], pChip.rBMP, pChip.rBMPTEX);
									}
								}
							}
						}
						break;
					#endregion
					#region [ da: ミキサーへチップ音追加 ]
					case Ech定義.MixerAdd:
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							//Debug.WriteLine( "[DA(AddMixer)] BAR=" + pChip.n発声位置 / 384 + " ch=" + pChip.nチャンネル番号.ToString( "x2" ) + ", wav=" + pChip.n整数値.ToString( "x2" ) + ", time=" + pChip.n発声時刻ms );
							pChip.bHit = true;
							if (listWAV.ContainsKey(pChip.n整数値_内部番号))	// 参照が遠いので後日最適化する
							{
								CDTX.CWAV wc = listWAV[pChip.n整数値_内部番号];
								//Debug.Write( "[AddMixer] BAR=" + pChip.n発声位置 / 384 + ", wav=" + Path.GetFileName( wc.strファイル名 ) + ", time=" + pChip.n発声時刻ms );

								for (int i = 0; i < nPolyphonicSounds; i++)
								{
									if (wc.rSound[i] != null)
									{
										//CDTXMania.app.Sound管理.AddMixer( wc.rSound[ i ] );
										AddMixer(wc.rSound[i], pChip.b演奏終了後も再生が続くチップである);
									}
									//else
									//{
									//    Debug.WriteLine( ", nPoly=" + i + ", Mix=" + CDTXMania.app.Sound管理.GetMixingStreams() );
									//    break;
									//}
									//if ( i == nPolyphonicSounds - 1 )
									//{
									//    Debug.WriteLine( ", nPoly=" + nPolyphonicSounds + ", Mix=" + CDTXMania.app.Sound管理.GetMixingStreams() );
									//}
								}
							}
						}
						break;
					#endregion
					#region [ db: ミキサーからチップ音削除 ]
					case Ech定義.MixerRemove:
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							//Debug.WriteLine( "[DB(RemoveMixer)] BAR=" + pChip.n発声位置 / 384 + " ch=" + pChip.nチャンネル番号.ToString( "x2" ) + ", wav=" + pChip.n整数値.ToString( "x2" ) + ", time=" + pChip.n発声時刻ms );
							pChip.bHit = true;
							if (listWAV.ContainsKey(pChip.n整数値_内部番号))	// 参照が遠いので後日最適化する
							{
								CDTX.CWAV wc = listWAV[pChip.n整数値_内部番号];
								//Debug.Write( "[DelMixer] BAR=" + pChip.n発声位置 / 384 +  ", wav=" + Path.GetFileName( wc.strファイル名 ) + ", time=" + pChip.n発声時刻ms );
								for (int i = 0; i < nPolyphonicSounds; i++)
								{
									if (wc.rSound[i] != null)
									{
										//CDTXMania.app.Sound管理.RemoveMixer( wc.rSound[ i ] );
										if (!wc.rSound[i].b演奏終了後も再生が続くチップである)	// #32248 2013.10.16 yyagi
										{															// DTX終了後も再生が続くチップの0xDB登録をなくすことはできず。
											RemoveMixer(wc.rSound[i]);							// (ミキサー解除のタイミングが遅延する場合の対応が面倒なので。)
										}															// そこで、代わりにフラグをチェックしてミキサー削除ロジックへの遷移をカットする。
									}
									//else
									//{
									//    Debug.WriteLine( ", nPoly=" + i + ", Mix=" + CDTXMania.app.Sound管理.GetMixingStreams() );
									//    break;
									//}
									//if ( i == nPolyphonicSounds - 1 )
									//{
									//    Debug.WriteLine( ", nPoly=" + nPolyphonicSounds + ", Mix=" + CDTXMania.app.Sound管理.GetMixingStreams() );
									//}
								}
							}
						}
						break;
					#endregion
					#region [ その他(未定義) ]
					default:
						if (!pChip.bHit && (pChip.nバーからの距離dot.Drums < 0))
						{
							pChip.bHit = true;
						}
						break;
					#endregion
				}
			}
			return false;
		}

		public void t再読込()
		{
			CDTXMania.app.DTX.t全チップの再生停止とミキサーからの削除();
			this.eフェードアウト完了時の戻り値 = E演奏画面の戻り値.再読込_再演奏;
			base.eフェーズID = CStage.Eフェーズ.演奏_再読込;
			this.bPAUSE = false;

			// #34048 2014.7.16 yyagi
			#region [ 読み込み画面に遷移する前に、設定変更した可能性があるパラメータをConfigIniクラスに書き戻す ]
			for (int i = 0; i < 3; i++)
			{
				CDTXMania.app.ConfigIni.nViewerScrollSpeed[i] = CDTXMania.app.ConfigIni.n譜面スクロール速度[i];
			}
			CDTXMania.app.ConfigIni.b演奏情報を表示する = CDTXMania.app.ConfigIni.bViewerShowDebugStatus;
			#endregion
		}

		public void t停止()
		{
			CDTXMania.app.DTX.t全チップの再生停止とミキサーからの削除();
			this.actAVI.Stop();
			this.actBGA.Stop();
			this.actPanel.Stop();				// PANEL表示停止
			CDTXMania.app.Timer.t一時停止();		// 再生時刻カウンタ停止

			this.n現在のトップChip = CDTXMania.app.DTX.listChip.Count - 1;	// 終端にシーク

			// 自分自身のOn活性化()相当の処理もすべき。
		}

		/// <summary>
		/// 演奏位置を変更する。
		/// </summary>
		/// <param name="nStartBar">演奏開始小節番号</param>
		/// <param name="bResetHitStatus">演奏済み情報(bHit)をクリアするかどうか</param>
		public void t演奏位置の変更(int nStartBar)
		{
			// まず全サウンドオフにする
			CDTXMania.app.DTX.t全チップの再生停止();
			this.actAVI.Stop();
			this.actBGA.Stop();

			#region [ 再生開始小節の変更 ]
			nStartBar++;									// +1が必要

			#region [ 演奏済みフラグのついたChipをリセットする ]
			for (int i = 0; i < CDTXMania.app.DTX.listChip.Count; i++)
			{
				CChip pChip = CDTXMania.app.DTX.listChip[i];
				if (pChip.bHit)
				{
					CChip p = (CChip)pChip.Clone();
					p.bHit = false;
					CDTXMania.app.DTX.listChip[i] = p;
				}
			}
			#endregion

			#region [ 処理を開始するチップの特定 ]
			//for ( int i = this.n現在のトップChip; i < CDTXMania.app.DTX.listChip.Count; i++ )
			bool bSuccessSeek = false;
			for (int i = 0; i < CDTXMania.app.DTX.listChip.Count; i++)
			{
				CChip pChip = CDTXMania.app.DTX.listChip[i];
				if (pChip.n発声位置 < 384 * nStartBar)
				{
					continue;
				}
				else
				{
					bSuccessSeek = true;
					this.n現在のトップChip = i;
					break;
				}
			}
			if (!bSuccessSeek)
			{
				// this.n現在のトップChip = CDTXMania.app.DTX.listChip.Count - 1;
				this.n現在のトップChip = 0;		// 対象小節が存在しないなら、最初から再生
			}
			#endregion

			#region [ 演奏開始の発声時刻msを取得し、タイマに設定 ]
			int nStartTime = CDTXMania.app.DTX.listChip[this.n現在のトップChip].n発声時刻ms;

			CSound管理.rc演奏用タイマ.tリセット();	// これでPAUSE解除されるので、次のPAUSEチェックは不要
			//if ( !this.bPAUSE )
			//{
			CSound管理.rc演奏用タイマ.t一時停止();
			//}
			CSound管理.rc演奏用タイマ.n現在時刻 = nStartTime;
			#endregion

			List<CSound> pausedCSound = new List<CSound>();

			#region [ BGMやギターなど、演奏開始のタイミングで再生がかかっているサウンドのの途中再生開始 ] // (CDTXのt入力・行解析・チップ配置()で小節番号が+1されているのを削っておくこと)
			for (int i = this.n現在のトップChip; i >= 0; i--)
			{
				CChip pChip = CDTXMania.app.DTX.listChip[i];
				int nDuration = pChip.GetDuration();

				if ((pChip.n発声時刻ms + nDuration > 0) && (pChip.n発声時刻ms <= nStartTime) && (nStartTime <= pChip.n発声時刻ms + nDuration))
				{
					if (pChip.bWAVを使うチャンネルである && !pChip.b空打ちチップである)	// wav系チャンネル、且つ、空打ちチップではない
					{
						CDTX.CWAV wc;
						bool b = CDTXMania.app.DTX.listWAV.TryGetValue(pChip.n整数値_内部番号, out wc);
						if (!b) continue;

						if ((wc.bIsBGMSound && CDTXMania.app.ConfigIni.bBGM音を発声する) || (!wc.bIsBGMSound))
						{
							CDTXMania.app.DTX.tチップの再生(pChip, CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, (int)Eレーン.BGM, CDTXMania.app.DTX.nモニタを考慮した音量(E楽器パート.UNKNOWN));
							#region [ PAUSEする ]
							int j = wc.n現在再生中のサウンド番号;
							if (wc.rSound[j] != null)
							{
								wc.rSound[j].t再生を一時停止する();
								wc.rSound[j].t再生位置を変更する(nStartTime - pChip.n発声時刻ms);
								pausedCSound.Add(wc.rSound[j]);
							}
							#endregion
						}
					}
				}
			}
			#endregion
			#region [ 演奏開始時点で既に表示されているBGAとAVIの、シークと再生 ]
			this.actBGA.SkipStart(nStartTime);
			this.actAVI.SkipStart(nStartTime);
			#endregion
			#region [ PAUSEしていたサウンドを一斉に再生再開する(ただしタイマを止めているので、ここではまだ再生開始しない) ]
			foreach (CSound cs in pausedCSound)
			{
				cs.tサウンドを再生する();
			}
			pausedCSound.Clear();
			pausedCSound = null;
			#endregion
			#region [ タイマを再開して、PAUSEから復帰する ]
			CSound管理.rc演奏用タイマ.n現在時刻 = nStartTime;
			CDTXMania.app.Timer.tリセット();						// これでPAUSE解除されるので、3行先の再開()は不要
			CDTXMania.app.Timer.n現在時刻 = nStartTime;				// Debug表示のTime: 表記を正しくするために必要
			CSound管理.rc演奏用タイマ.t再開();
			//CDTXMania.app.Timer.t再開();
			this.bPAUSE = false;								// システムがPAUSE状態だったら、強制解除
			this.actPanel.Start();
			#endregion
			#endregion
		}


		/// <summary>
		/// DTXV用の設定をする。(全AUTOなど)
		/// 元の設定のバックアップなどはしないので、あとでConfig.iniを上書き保存しないこと。
		/// </summary>
		protected void tDTXV用の設定()
		{
			CDTXMania.app.ConfigIni.bAutoPlay.HH = true;
			CDTXMania.app.ConfigIni.bAutoPlay.SD = true;
			CDTXMania.app.ConfigIni.bAutoPlay.BD = true;
			CDTXMania.app.ConfigIni.bAutoPlay.HT = true;
			CDTXMania.app.ConfigIni.bAutoPlay.LT = true;
			CDTXMania.app.ConfigIni.bAutoPlay.CY = true;
			CDTXMania.app.ConfigIni.bAutoPlay.FT = true;
			CDTXMania.app.ConfigIni.bAutoPlay.RD = true;
			CDTXMania.app.ConfigIni.bAutoPlay.LC = true;
			CDTXMania.app.ConfigIni.bAutoPlay.GtR = true;
			CDTXMania.app.ConfigIni.bAutoPlay.GtG = true;
			CDTXMania.app.ConfigIni.bAutoPlay.GtB = true;
			CDTXMania.app.ConfigIni.bAutoPlay.GtPick = true;
			CDTXMania.app.ConfigIni.bAutoPlay.GtW = true;
			CDTXMania.app.ConfigIni.bAutoPlay.BsR = true;
			CDTXMania.app.ConfigIni.bAutoPlay.BsG = true;
			CDTXMania.app.ConfigIni.bAutoPlay.BsB = true;
			CDTXMania.app.ConfigIni.bAutoPlay.BsPick = true;
			CDTXMania.app.ConfigIni.bAutoPlay.BsW = true;

			this.bIsAutoPlay = CDTXMania.app.ConfigIni.bAutoPlay;

			CDTXMania.app.ConfigIni.bAVI有効 = true;
			CDTXMania.app.ConfigIni.bBGA有効 = true;
			for (int i = 0; i < 3; i++)
			{
				CDTXMania.app.ConfigIni.bGraph[i] = false;
				CDTXMania.app.ConfigIni.bHidden[i] = false;
				CDTXMania.app.ConfigIni.bLeft[i] = false;
				CDTXMania.app.ConfigIni.bLight[i] = false;
				CDTXMania.app.ConfigIni.bReverse[i] = false;
				CDTXMania.app.ConfigIni.bSudden[i] = false;
				CDTXMania.app.ConfigIni.eInvisible[i] = EInvisible.OFF;
				CDTXMania.app.ConfigIni.eRandom[i] = Eランダムモード.OFF;
				CDTXMania.app.ConfigIni.n表示可能な最小コンボ数[i] = 65535;
				CDTXMania.app.ConfigIni.判定文字表示位置[i] = E判定文字表示位置.表示OFF;
				// CDTXMania.app.ConfigIni.n譜面スクロール速度[ i ] = CDTXMania.app.ConfigIni.nViewerScrollSpeed[ i ];	// これだけはOn活性化()で行うこと。
				// そうしないと、演奏開始直後にスクロール速度が変化して見苦しい。
			}

			CDTXMania.app.ConfigIni.eDark = Eダークモード.OFF;

			CDTXMania.app.ConfigIni.b演奏情報を表示する = CDTXMania.app.ConfigIni.bViewerShowDebugStatus;
			CDTXMania.app.ConfigIni.bフィルイン有効 = true;
			CDTXMania.app.ConfigIni.bScoreIniを出力する = false;
			CDTXMania.app.ConfigIni.bSTAGEFAILED有効 = false;
			CDTXMania.app.ConfigIni.bTight = false;
			CDTXMania.app.ConfigIni.bストイックモード = false;
			CDTXMania.app.ConfigIni.bドラム打音を発声する = true;
			CDTXMania.app.ConfigIni.bBGM音を発声する = true;

			CDTXMania.app.ConfigIni.nRisky = 0;
			CDTXMania.app.ConfigIni.nShowLagType = 0;
			CDTXMania.app.ConfigIni.ドラムコンボ文字の表示位置 = Eドラムコンボ文字の表示位置.OFF;
		}



		protected abstract void t進行描画_チップ_ドラムス(CConfigIni configIni, ref CDTX dTX, ref CChip pChip);
		//protected abstract void t進行描画_チップ_ギター( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip );
		protected abstract void t進行描画_チップ_ギターベース(CConfigIni configIni, ref CDTX dTX, ref CChip pChip, E楽器パート inst);
		/// <summary>
		/// ギター・ベースのチップ表示
		/// </summary>
		/// <param name="configIni"></param>
		/// <param name="dTX"></param>
		/// <param name="pChip">描画するチップ</param>
		/// <param name="inst">楽器種別</param>
		/// <param name="barYNormal">Normal時判定ライン表示Y座標</param>
		/// <param name="barYReverse">Reverse時判定ライン表示Y座標</param>
		/// <param name="showRangeY0">チップ表示Y座標範囲(最小値)</param>
		/// <param name="showRangeY1">チップ表示Y座標範囲(最大値)</param>
		/// <param name="openXg">オープンチップの表示X座標(ギター用)</param>
		/// <param name="openXb">オープンチップの表示X座標(ベース用)</param>
		/// <param name="rectOpenOffsetX">テクスチャ内のオープンチップregionのx座標</param>
		/// <param name="rectOpenOffsetY">テクスチャ内のオープンチップregionのy座標</param>
		/// <param name="openChipWidth">テクスチャ内のオープンチップregionのwidth</param>
		/// <param name="chipHeight">テクスチャ内のチップのheight</param>
		/// <param name="chipWidth">テクスチャ内のチップのwidth</param>
		/// <param name="guitarNormalX">ギターチップ描画のx座標(Normal)</param>
		/// <param name="guitarLeftyX">ギターチップ描画のx座標(Lefty)</param>
		/// <param name="bassNormalX">ベースチップ描画のx座標(Normal)</param>
		/// <param name="bassLeftyX">ベースチップ描画のx座標(Lefty)</param>
		/// <param name="drawDeltaX">描画のX座標間隔(R,G,B...)</param>
		/// <param name="chipTexDeltaX">テクスチャののX座標間隔(R,G,B...)</param>
		protected void t進行描画_チップ_ギターベース(CConfigIni configIni, ref CDTX dTX, ref CChip pChip, E楽器パート inst,
			int barYNormal, int barYReverse,
			int showRangeY0, int showRangeY1, int openXg, int openXb,
			int rectOpenOffsetX, int rectOpenOffsetY, int openChipWidth, int chipHeight, int chipWidth,
			int guitarNormalX, int guitarLeftyX, int bassNormalX, int bassLeftyX, int drawDeltaX, int chipTexDeltaX)
		{
			int instIndex = (int)inst;
			if (configIni.bGuitar有効)
			{
				#region [ Invisible処理 ]
				if (configIni.eInvisible[instIndex] != EInvisible.OFF)
				{
					cInvisibleChip.SetInvisibleStatus(ref pChip);
				}
				#endregion
				else
				{
					#region [ Hidden/Sudden処理 ]
					if (configIni.bSudden[instIndex])
					{
						pChip.b可視 = (pChip.nバーからの距離dot[instIndex] < 200 * Scale.Y);
					}
					if (configIni.bHidden[instIndex] && (pChip.nバーからの距離dot[instIndex] < 100 * Scale.Y))
					{
						pChip.b可視 = false;
					}
					#endregion
				}

				bool bChipHasR = pChip.bGuitarBass_R;
				bool bChipHasG = pChip.bGuitarBass_G;
				bool bChipHasB = pChip.bGuitarBass_B;
				bool bChipHasW = pChip.bGuitarBass_Wailing;
				bool bChipIsO = pChip.bGuitarBass_Open;

				#region [ chip描画 ]
				Ech定義 OPEN = (inst == E楽器パート.GUITAR) ? Ech定義.Guitar_Open : Ech定義.Bass_Open;
				if (!pChip.bHit && pChip.b可視)
				{
					if (this.txチップ != null)
					{
						this.txチップ.n透明度 = pChip.n透明度;
					}
					int y = configIni.bReverse[instIndex] ?
						(int)(barYReverse - pChip.nバーからの距離dot[instIndex]) :
						(int)(barYNormal + pChip.nバーからの距離dot[instIndex]);
					int n小節線消失距離dot = configIni.bReverse[instIndex] ?
						(int)(-100 * Scale.Y) :
						(configIni.e判定位置[instIndex] == E判定位置.標準) ? (int)(-36 * Scale.Y) : (int)(-25 * Scale.Y);
					if (configIni.bReverse[instIndex])
					{
						//showRangeY1 = barYReverse - n小節線消失距離dot;
					}
					else
					{
						showRangeY0 = barYNormal + n小節線消失距離dot;
					}
					if ((showRangeY0 < y) && (y < showRangeY1))
					{
						if (this.txチップ != null)
						{
							int nアニメカウンタ現在の値 = this.ctチップ模様アニメ[instIndex].n現在の値;
							#region [ OPENチップの描画 ]
							if (pChip.eチャンネル番号 == OPEN)
							{
								int xo = (inst == E楽器パート.GUITAR) ? openXg : openXb;
								this.txチップ.t2D描画(CDTXMania.app.Device,
									xo * Scale.X,
									y - (2 * Scale.Y),
									new Rectangle(
										(int)(rectOpenOffsetX * Scale.X),
										(int)(rectOpenOffsetY * Scale.Y) + (int)(((nアニメカウンタ現在の値 % 5) * chipHeight * Scale.Y)),
										(int)(openChipWidth * Scale.X),
										(int)(chipHeight * Scale.Y)
									)
								);
							}
							#endregion
							Rectangle rc = new Rectangle(
								(int)(rectOpenOffsetX * Scale.X),
								(int)(nアニメカウンタ現在の値 * chipHeight * Scale.Y),
								(int)(chipWidth * Scale.X),
								(int)(chipHeight * Scale.Y)
							);
							#region [ RGBチップのX座標初期化 ]
							int x;
							if (inst == E楽器パート.GUITAR)
							{
								x = (configIni.bLeft.Guitar) ? guitarLeftyX : guitarNormalX;
							}
							else
							{
								x = (configIni.bLeft.Bass) ? bassLeftyX : bassNormalX;
							}
							int deltaX = (configIni.bLeft[instIndex]) ? -drawDeltaX : +drawDeltaX;
							#endregion
							//Trace.TraceInformation( "chip={0:x2}, E楽器パート={1}, x={2}", pChip.nチャンネル番号, inst, x );
							#region [ Rチップ描画 ]
							if (bChipHasR)
							{
								this.txチップ.t2D描画(CDTXMania.app.Device,
									x * Scale.X,
									y - (chipHeight / 2) * Scale.Y,
									rc
								);
							}
							#endregion
							#region [ Gチップ描画 ]
							rc.X += (int)(chipTexDeltaX * Scale.X);
							x += deltaX;
							if (bChipHasG)
							{
								this.txチップ.t2D描画(CDTXMania.app.Device,
									x * Scale.X,
									y - (chipHeight / 2) * Scale.Y,
									rc
								);
							}
							#endregion
							#region [ Bチップ描画 ]
							rc.X += (int)(chipTexDeltaX * Scale.X);
							x += deltaX;
							if (bChipHasB)
							{
								this.txチップ.t2D描画(CDTXMania.app.Device,
									x * Scale.X,
									y - (chipHeight / 2) * Scale.Y,
									rc
								);
							}
							#endregion
						}
					}
				}
				#endregion
				//if ( ( configIni.bAutoPlay.Guitar && !pChip.bHit ) && ( pChip.nバーからの距離dot.Guitar < 0 ) )


				// #35411 2015.08.20 chnmr0 modified
				// 従来のAUTO処理に加えてプレーヤーゴーストの再生機能を追加
				bool autoPlayCondition = (!pChip.bHit) && (pChip.nバーからの距離dot[instIndex] < 0);
				if (autoPlayCondition)
				{
					cInvisibleChip.StartSemiInvisible(inst);
				}

				bool autoPick = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtPick : bIsAutoPlay.BsPick;
				autoPlayCondition = !pChip.bHit && autoPick;
				long ghostLag = 0;
				bool bUsePerfectGhost = true;

				if ((pChip.e楽器パート == E楽器パート.GUITAR || pChip.e楽器パート == E楽器パート.BASS) &&
						CDTXMania.app.ConfigIni.eAutoGhost[(int)(pChip.e楽器パート)] != EAutoGhostData.PERFECT &&
						CDTXMania.app.DTX.listAutoGhostLag[(int)pChip.e楽器パート] != null &&
						0 <= pChip.n楽器パートでの出現順 &&
						pChip.n楽器パートでの出現順 < CDTXMania.app.DTX.listAutoGhostLag[(int)pChip.e楽器パート].Count)
				{
					// #35411 (mod) Ghost data が有効なので 従来のAUTOではなくゴーストのラグを利用
					// 発生時刻と現在時刻からこのタイミングで演奏するかどうかを決定
					ghostLag = CDTXMania.app.DTX.listAutoGhostLag[(int)pChip.e楽器パート][pChip.n楽器パートでの出現順];
					bool resetCombo = ghostLag > 255;
					ghostLag = (ghostLag & 255) - 128;
					ghostLag -= (pChip.e楽器パート == E楽器パート.GUITAR ? nInputAdjustTimeMs.Guitar : nInputAdjustTimeMs.Bass);
					autoPlayCondition &= (pChip.n発声時刻ms + ghostLag <= CSound管理.rc演奏用タイマ.n現在時刻ms);
					if (resetCombo && autoPlayCondition)
					{
						this.actCombo.n現在のコンボ数[(int)pChip.e楽器パート] = 0;
					}
					bUsePerfectGhost = false;
				}

				if (bUsePerfectGhost)
				{
					// 従来のAUTOを使用する場合
					autoPlayCondition &= (pChip.nバーからの距離dot[instIndex] < 0);
				}

				if (autoPlayCondition)
				{
					int lo = (inst == E楽器パート.GUITAR) ? 0 : 3;	// lane offset
					bool autoR = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtR : bIsAutoPlay.BsR;
					bool autoG = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtG : bIsAutoPlay.BsG;
					bool autoB = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtB : bIsAutoPlay.BsB;
					bool pushingR = CDTXMania.app.Pad.b押されている(inst, Eパッド.R);
					bool pushingG = CDTXMania.app.Pad.b押されている(inst, Eパッド.G);
					bool pushingB = CDTXMania.app.Pad.b押されている(inst, Eパッド.B);

					#region [ Chip Fire effects (auto時用) ]
					// autoPickでない時の処理は、 t入力処理・ギターベース(E楽器パート) で行う
					bool bSuccessOPEN = bChipIsO && (autoR || !pushingR) && (autoG || !pushingG) && (autoB || !pushingB);
					{
						if ((bChipHasR && (autoR || pushingR)) || bSuccessOPEN)
						{
							this.actChipFireGB.Start(0 + lo, 演奏判定ライン座標);
						}
						if ((bChipHasG && (autoG || pushingG)) || bSuccessOPEN)
						{
							this.actChipFireGB.Start(1 + lo, 演奏判定ライン座標);
						}
						if ((bChipHasB && (autoB || pushingB)) || bSuccessOPEN)
						{
							this.actChipFireGB.Start(2 + lo, 演奏判定ライン座標);
						}
					}
					#endregion
					#region [ autopick ]
					{
						bool bMiss = true;
						if (bChipHasR == autoR && bChipHasG == autoG && bChipHasB == autoB)		// autoレーンとチップレーン一致時はOK
						{																			// この条件を加えないと、同時に非autoレーンを押下している時にNGとなってしまう。
							bMiss = false;
						}
						else if ((autoR || (bChipHasR == pushingR)) && (autoG || (bChipHasG == pushingG)) && (autoB || (bChipHasB == pushingB)))
						// ( bChipHasR == ( pushingR | autoR ) ) && ( bChipHasG == ( pushingG | autoG ) ) && ( bChipHasB == ( pushingB | autoB ) ) )
						{
							bMiss = false;
						}
						else if (((bChipIsO == true) && (!pushingR | autoR) && (!pushingG | autoG) && (!pushingB | autoB)))	// OPEN時
						{
							bMiss = false;
						}
						pChip.bHit = true;
						this.tサウンド再生(pChip, CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms + ghostLag, inst, dTX.nモニタを考慮した音量(inst), false, bMiss);
						this.r次にくるギターChip = null;
						if (!bMiss)
						{
							this.tチップのヒット処理(pChip.n発声時刻ms + ghostLag, pChip);
						}
						else
						{
							pChip.nLag = 0;		// tチップのヒット処理()の引数最後がfalseの時はpChip.nLagを計算しないため、ここでAutoPickかつMissのLag=0を代入
							this.tチップのヒット処理(pChip.n発声時刻ms + ghostLag, pChip, false);
						}
						Ech定義 chWailingChip = (inst == E楽器パート.GUITAR) ? Ech定義.Guitar_Wailing : Ech定義.Bass_Wailing;
						CChip item = this.r指定時刻に一番近い未ヒットChip(pChip.n発声時刻ms + ghostLag, chWailingChip, this.nInputAdjustTimeMs[instIndex], 140);
						if (item != null && !bMiss)
						{
							this.queWailing[instIndex].Enqueue(item);
						}
					}
					#endregion
					// #35411 modify end
				}
				return;
			}	// end of "if configIni.bGuitar有効"
			if (!pChip.bHit && (pChip.nバーからの距離dot[instIndex] < 0))	// Guitar/Bass無効の場合は、自動演奏する
			{
				pChip.bHit = true;
				this.tサウンド再生(pChip, CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, inst, dTX.nモニタを考慮した音量(inst));
			}
		}


		protected virtual void t進行描画_チップ_ギターベース_ウェイリング(
			CConfigIni configIni, ref CDTX dTX, ref CChip pChip, E楽器パート inst, bool bGRmode)
		{
			int indexInst = (int)inst;
			if (configIni.bGuitar有効)
			{
				#region [ Invisible処理 ]
				if (configIni.eInvisible[indexInst] != EInvisible.OFF)
				{
					cInvisibleChip.SetInvisibleStatus(ref pChip);
				}
				#endregion
				#region [ Sudden/Hidden処理 ]
				if (configIni.bSudden[indexInst])
				{
					pChip.b可視 = (pChip.nバーからの距離dot[indexInst] < 200 * Scale.Y);
				}
				if (configIni.bHidden[indexInst] && (pChip.nバーからの距離dot[indexInst] < 100 * Scale.Y))
				{
					pChip.b可視 = false;
				}
				#endregion

				cWailingChip[indexInst].t進行描画_チップ_ギターベース_ウェイリング(
					configIni, ref dTX, ref pChip,
					ref txチップ, ref 演奏判定ライン座標, ref ctWailingチップ模様アニメ
				);

				if (!pChip.bHit && (pChip.nバーからの距離dot[indexInst] < 0))
				{
					if (pChip.nバーからの距離dot[indexInst] < -234 * Scale.Y)	// #25253 2011.5.29 yyagi: Don't set pChip.bHit=true for wailing at once. It need to 1sec-delay (234pix per 1sec). 
					{
						pChip.bHit = true;
					}
					bool autoW = (inst == E楽器パート.GUITAR) ? configIni.bAutoPlay.GtW : configIni.bAutoPlay.BsW;
					//if ( configIni.bAutoPlay[ ((int) Eレーン.Guitar - 1) + indexInst ] )	// このような、バグの入りやすい書き方(GT/BSのindex値が他と異なる)はいずれ見直したい
					if (autoW)
					{
						//    pChip.bHit = true;								// #25253 2011.5.29 yyagi: Set pChip.bHit=true if autoplay.
						//    this.actWailingBonus.Start( inst, this.r現在の歓声Chip[indexInst] );
						// #23886 2012.5.22 yyagi; To support auto Wailing; Don't do wailing for ALL wailing chips. Do wailing for queued wailing chip.
						// wailing chips are queued when 1) manually wailing and not missed at that time 2) AutoWailing=ON and not missed at that time
						long nTimeStamp_Wailed = pChip.n発声時刻ms + CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻;
						DoWailingFromQueue(inst, nTimeStamp_Wailed, autoW);
					}
					cInvisibleChip.StartSemiInvisible(inst);
				}
				return;
			}
			pChip.bHit = true;
		}
		protected virtual void t進行描画_チップ_ギター_ウェイリング(CConfigIni configIni, ref CDTX dTX, ref CChip pChip, bool bGRmode)
		{
			t進行描画_チップ_ギターベース_ウェイリング(configIni, ref dTX, ref pChip, E楽器パート.GUITAR, bGRmode);
		}
		protected abstract void t進行描画_チップ_フィルイン(CConfigIni configIni, ref CDTX dTX, ref CChip pChip);
		protected abstract void t進行描画_チップ_小節線(CConfigIni configIni, ref CDTX dTX, ref CChip pChip);
		//protected abstract void t進行描画_チップ_ベース( CConfigIni configIni, ref CDTX dTX, ref CChip pChip );
		protected virtual void t進行描画_チップ_ベース_ウェイリング(CConfigIni configIni, ref CDTX dTX, ref CChip pChip, bool bGRmode)
		{
			t進行描画_チップ_ギターベース_ウェイリング(configIni, ref dTX, ref pChip, E楽器パート.BASS, bGRmode);
		}



		protected abstract void t進行描画_チップ_空打ち音設定_ドラム(CConfigIni configIni, ref CDTX dTX, ref CChip pChip);
		protected void t進行描画_チップアニメ()
		{
			for (int i = 0; i < 3; i++)			// 0=drums, 1=guitar, 2=bass
			{
				if (this.ctチップ模様アニメ[i] != null)
				{
					this.ctチップ模様アニメ[i].t進行Loop();
				}
			}
			if (this.ctWailingチップ模様アニメ != null)
			{
				this.ctWailingチップ模様アニメ.t進行Loop();
			}
		}

		protected bool t進行描画_フェードイン_アウト()
		{
			switch (base.eフェーズID)
			{
				case CStage.Eフェーズ.共通_フェードイン:
					if (this.actFI.On進行描画() != 0)
					{
						base.eフェーズID = CStage.Eフェーズ.共通_通常状態;
					}
					break;

				case CStage.Eフェーズ.共通_フェードアウト:
				case CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト:
					if (this.actFO.On進行描画() != 0)
					{
						return true;
					}
					break;

				case CStage.Eフェーズ.演奏_STAGE_CLEAR_フェードアウト:
					if (this.actFOClear.On進行描画() == 0)
					{
						break;
					}
					return true;

			}
			return false;
		}
		protected void t進行描画_レーンフラッシュD()
		{
			if ((CDTXMania.app.ConfigIni.eDark == Eダークモード.OFF) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED) && (base.eフェーズID != CStage.Eフェーズ.演奏_STAGE_FAILED_フェードアウト))
			{
				this.actLaneFlushD.On進行描画();
			}
		}
		protected void t進行描画_レーンフラッシュGB()
		{
			if ((CDTXMania.app.ConfigIni.eDark == Eダークモード.OFF) && CDTXMania.app.ConfigIni.bGuitar有効)
			{
				this.actLaneFlushGB.On進行描画();
			}
		}
		protected abstract void t進行描画_演奏情報();
		protected void t進行描画_演奏情報(int x, int y)
		{
			if (!CDTXMania.app.ConfigIni.b演奏情報を表示しない)
			{
				this.actPlayInfo.t進行描画(x, y);
			}
		}
		protected void t進行描画_背景()
		{
			if (CDTXMania.app.ConfigIni.eDark == Eダークモード.OFF)
			{
				if (this.tx背景 != null)
				{
					this.tx背景.t2D描画(CDTXMania.app.Device, 0, 0);
				}
			}
			else
			{
				// FullHD版では、背景描画のさらに奥でAVI描画をするため、
				// Dark!=OFF時下記の画面クリアをすると、AVI描画がクリアされてしまう
				// CDTXMania.app.Device.Clear( ClearFlags.ZBuffer | ClearFlags.Target, Color.Black, 0f, 0 );
			}
		}

		protected void t進行描画_判定ライン()
		{
			if (CDTXMania.app.ConfigIni.eDark != Eダークモード.FULL)
			{
				int y = this.演奏判定ライン座標.n判定ラインY座標(E楽器パート.DRUMS, false, bReverse[(int)E楽器パート.DRUMS], false, true);	// -(int) ( 3 * Scale.Y );
				// #31602 2016.2.11 yyagi 描画遅延対策として、判定ラインの表示位置をオフセット調整できるようにする
				// #31602 2013.6.23 yyagi 描画遅延対策として、判定ラインの表示位置をオフセット調整できるようにする
				if (this.txヒットバー != null)
				{
					int xStart = (CDTXMania.app.ConfigIni.eドラムレーン表示位置 == Eドラムレーン表示位置.Left) ? 32 * 3 : 619;
					int xEnd = (CDTXMania.app.ConfigIni.eドラムレーン表示位置 == Eドラムレーン表示位置.Left) ? 335 * 3 : 619 + 682;
					for (int x = xStart; x < xEnd; x += 24)
					{
						this.txヒットバー.t2D描画(CDTXMania.app.Device,
							x,
							y,
							new Rectangle(
								0,
								0,
								((x + 24) >= xEnd) ? (int)((23 - ((x + 24) - xEnd))) : 24,
								18
							)
						);
					}
				}
			}
		}
		protected void t進行描画_判定文字列()
		{
			this.actJudgeString.t進行描画(演奏判定ライン座標);
		}
		protected void t進行描画_判定文字列1_通常位置指定の場合()
		{
			if (((E判定文字表示位置)CDTXMania.app.ConfigIni.判定文字表示位置.Drums) != E判定文字表示位置.コンボ下)	// 判定ライン上または横
			{
				this.actJudgeString.t進行描画(演奏判定ライン座標);
			}
		}
		protected void t進行描画_判定文字列2_判定ライン上指定の場合()
		{
			if (((E判定文字表示位置)CDTXMania.app.ConfigIni.判定文字表示位置.Drums) == E判定文字表示位置.コンボ下)	// 判定ライン上または横
			{
				this.actJudgeString.t進行描画(演奏判定ライン座標);
			}
		}

		protected void t進行描画_譜面スクロール速度()
		{
			this.act譜面スクロール速度.On進行描画();
		}

		protected abstract void t背景テクスチャの生成();
		protected void t背景テクスチャの生成(string DefaultBgFilename, string DefaultLaneFilename, Rectangle bgrect, string bgfilename)
		{									// Default...: レーン等があるレイヤー		bgfilename: DTXファイルで指定する背景
			Bitmap image = null;
			bool bSuccessLoadDTXbgfile = false;

			int[] offsetX = new int[2] { 96, 506 };
			int nLanePosition = (int)CDTXMania.app.ConfigIni.eドラムレーン表示位置;
			//int nLanePosition = (int) Eドラムレーン表示位置.Left;

			if (bgfilename != null && File.Exists(bgfilename) && !CDTXMania.app.DTX.bチップがある.Movie)
			{
				try
				{
					#region [ DTXデータで指定されている背景画像を読み込む ]
					Bitmap bitmap1 = null;
					bitmap1 = new Bitmap(bgfilename);
					if ((bitmap1.Size.Width == 0) && (bitmap1.Size.Height == 0))
					{
						this.tx背景 = null;
						return;
					}
					#endregion

					int newWidth = (int)(bitmap1.Width * Scale.X);
					int newHeight = (int)(bitmap1.Height * Scale.Y);
					Bitmap bitmap2;

					#region [ 背景画像がVGAサイズ以下なら、FullHDサイズに拡大する ]
					if (bitmap1.Width <= 640 && bitmap1.Height <= 480)
					{
						bitmap2 = new Bitmap(newWidth, newHeight);
						Graphics graphic2 = Graphics.FromImage(bitmap2);
						graphic2.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
						graphic2.DrawImage(bitmap1, 0, 0, newWidth, newHeight);
						graphic2.Dispose();
					}
					else
					{
						bitmap2 = (Bitmap)bitmap1.Clone();
					}
					bitmap1.Dispose();
					#endregion

					#region [ 実背景に格子状に配置するよう、コピーしていく ]
					Bitmap bitmap3 = new Bitmap(SampleFramework.GameWindowSize.Width, SampleFramework.GameWindowSize.Height);
					Graphics graphics3 = Graphics.FromImage(bitmap3);
					for (int i = 0; i < SampleFramework.GameWindowSize.Height; i += bitmap2.Size.Height)
					{
						for (int j = 0; j < SampleFramework.GameWindowSize.Width; j += bitmap2.Size.Width)
						{
							graphics3.DrawImage(bitmap2, j, i, bitmap2.Width, bitmap2.Height);
						}
					}
					graphics3.Dispose();
					bitmap2.Dispose();
					#endregion

					#region [ レーン外・レーンそのもののフレームを合成 ]
					image = new Bitmap(CSkin.Path(DefaultBgFilename));	// レーン外のフレーム
					graphics3 = Graphics.FromImage(image);

					//#region [ レーンのフレームがあれば、それを合成 ]
					//if ( DefaultLaneFilename != "" )
					//{
					//	Bitmap bmLane = new Bitmap( CSkin.Path( DefaultLaneFilename ) );
					//	graphics3.DrawImage( bmLane, offsetX[ nLanePosition ], 0 );
					//	bmLane.Dispose();
					//}
					//#endregion

					ColorMatrix matrix2 = new ColorMatrix();
					matrix2.Matrix00 = 1f;
					matrix2.Matrix11 = 1f;
					matrix2.Matrix22 = 1f;
					matrix2.Matrix33 = ((float)CDTXMania.app.ConfigIni.n背景の透過度) / 255f;
					matrix2.Matrix44 = 1f;
					ColorMatrix newColorMatrix = matrix2;
					ImageAttributes imageAttr = new ImageAttributes();
					imageAttr.SetColorMatrix(newColorMatrix);
					graphics3.DrawImage(bitmap3, new Rectangle(0, 0, SampleFramework.GameWindowSize.Width, SampleFramework.GameWindowSize.Height), 0, 0, SampleFramework.GameWindowSize.Width, SampleFramework.GameWindowSize.Height, GraphicsUnit.Pixel, imageAttr);
					//	graphics3.DrawImage( bitmap3, bgrect, bgrect.X, bgrect.Y, bgrect.Width, bgrect.Height, GraphicsUnit.Pixel );
					bitmap3.Dispose();
					#endregion

					imageAttr.Dispose();
					graphics3.Dispose();
					bSuccessLoadDTXbgfile = true;
				}
				catch
				{
					Trace.TraceError("背景画像とレーン画像の合成に失敗しました。({0})", bgfilename);
				}
			}
			#region [ DTXデータで指定する背景画像を合成しない場合は、レーン画像単体を背景画像とする ]
			if (!bSuccessLoadDTXbgfile)
			{
				bgfilename = CSkin.Path(DefaultBgFilename);
				try
				{
					image = new Bitmap(bgfilename);

					if (DefaultLaneFilename != "")
					{
						Bitmap bmLane = new Bitmap(CSkin.Path(DefaultLaneFilename));
						Graphics g = Graphics.FromImage(image);
						g.DrawImage(bmLane, offsetX[nLanePosition], 0);
						g.Dispose();
						bmLane.Dispose();
					}
				}
				catch
				{
					Trace.TraceError("レーン画像の読み込みに失敗しました。({0})", bgfilename);
					this.tx背景 = null;
					return;
				}
			}
			#endregion
			#region [ BGA画像を表示する予定がある場合は、背景画像からあらかじめその領域を黒抜きにしておく ]
			if ((CDTXMania.app.DTX.listBMP.Count > 0) || (CDTXMania.app.DTX.listBMPTEX.Count > 0) || CDTXMania.app.DTX.listAVI.Count > 0)
			{
				Graphics graphics2 = Graphics.FromImage(image);
				graphics2.FillRectangle(Brushes.Black, bgrect.X, bgrect.Y, bgrect.Width, bgrect.Height);
				graphics2.Dispose();
			}
			#endregion
			#region [ 背景画像をテクスチャにする。背景動画の表示予定がある場合は、更に透明度を付与する。 ]
			try
			{
				this.tx背景 = new CTexture(CDTXMania.app.Device, image, CDTXMania.app.TextureFormat);
				if (CDTXMania.app.DTX.bMovieをFullscreen再生する)						// Fullscreen動画再生が発生する場合は、動画レイヤーに対してレーン＋背景レイヤーに透明度を設定する
				{
					this.tx背景.n透明度 = 255 - CDTXMania.app.ConfigIni.n背景の透過度;	// 背景動画用
				}
			}
			catch (CTextureCreateFailedException)
			{
				Trace.TraceError("背景テクスチャの生成に失敗しました。");
				this.tx背景 = null;
			}
			#endregion
			image.Dispose();
		}

		protected virtual void t入力処理_ギター()
		{
			t入力処理_ギターベース(E楽器パート.GUITAR);
		}
		protected virtual void t入力処理_ベース()
		{
			t入力処理_ギターベース(E楽器パート.BASS);
		}


		protected virtual void t入力処理_ギターベース(E楽器パート inst)
		{
			int indexInst = (int)inst;
			#region [ スクロール速度変更 ]
			if (CDTXMania.app.Pad.b押されている(inst, Eパッド.Decide) && CDTXMania.app.Pad.b押された(inst, Eパッド.B))
			{
				float f = (float)this.演奏判定ライン座標.nJudgeLinePosY_delta[indexInst] / (CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst] + 1);
				CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst] = Math.Min(CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst] + 1, 1999);
				f *= CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst];
				this.演奏判定ライン座標.nJudgeLinePosY_delta[indexInst] = (int)(f + 0.5);
				CDTXMania.app.ConfigIni.nJudgeLinePosOffset[indexInst] = (int)(f + 0.5);
			}
			if (CDTXMania.app.Pad.b押されている(inst, Eパッド.Decide) && CDTXMania.app.Pad.b押された(inst, Eパッド.R))
			{
				CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst] = Math.Max(CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst] - 1, 0);

				float f = (float)this.演奏判定ライン座標.nJudgeLinePosY_delta[indexInst] / (CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst] + 1);
				CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst] = Math.Max(CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst] - 1, 0);
				f *= CDTXMania.app.ConfigIni.n譜面スクロール速度[indexInst];
				this.演奏判定ライン座標.nJudgeLinePosY_delta[indexInst] = (int)(f + 0.5);
				CDTXMania.app.ConfigIni.nJudgeLinePosOffset[indexInst] = (int)(f + 0.5);
			}
			#endregion

			if (!CDTXMania.app.ConfigIni.bGuitar有効 || !CDTXMania.app.DTX.bチップがある[indexInst])
			{
				return;
			}

			int R = (inst == E楽器パート.GUITAR) ? 0 : 3;
			int G = R + 1;
			int B = R + 2;
			bool autoW = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtW : bIsAutoPlay.BsW;
			bool autoR = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtR : bIsAutoPlay.BsR;
			bool autoG = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtG : bIsAutoPlay.BsG;
			bool autoB = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtB : bIsAutoPlay.BsB;
			bool autoPick = (inst == E楽器パート.GUITAR) ? bIsAutoPlay.GtPick : bIsAutoPlay.BsPick;
			int nAutoW = (autoW) ? 8 : 0;
			int nAutoR = (autoR) ? 4 : 0;
			int nAutoG = (autoG) ? 2 : 0;
			int nAutoB = (autoB) ? 1 : 0;
			int nAutoMask = nAutoW | nAutoR | nAutoG | nAutoB;

			//			if ( bIsAutoPlay[ (int) Eレーン.Guitar - 1 + indexInst ] )	// このような、バグの入りやすい書き方(GT/BSのindex値が他と異なる)はいずれ見直したい
			//			{
			CChip chip = this.r次に来る指定楽器Chipを更新して返す(inst);
			if (chip != null)
			{
				if ((chip.bGuitarBass_R) && autoR)
				{
					this.actLaneFlushGB.Start(R);
					this.actRGB.Push(R);
				}
				if ((chip.bGuitarBass_G) && autoG)
				{
					this.actLaneFlushGB.Start(G);
					this.actRGB.Push(G);
				}
				if ((chip.bGuitarBass_B) && autoB)
				{
					this.actLaneFlushGB.Start(B);
					this.actRGB.Push(B);
				}
				//			}

			}
			//			else
			{
				int pressingR = CDTXMania.app.Pad.b押されている(inst, Eパッド.R) ? 4 : 0;
				this.t入力メソッド記憶(inst);
				int pressingG = CDTXMania.app.Pad.b押されている(inst, Eパッド.G) ? 2 : 0;
				this.t入力メソッド記憶(inst);
				int pressingB = CDTXMania.app.Pad.b押されている(inst, Eパッド.B) ? 1 : 0;
				this.t入力メソッド記憶(inst);
				int pressingRGB = pressingR | pressingG | pressingB;
				if (pressingR != 0)
				{
					this.actLaneFlushGB.Start(R);
					this.actRGB.Push(R);
				}
				if (pressingG != 0)
				{
					this.actLaneFlushGB.Start(G);
					this.actRGB.Push(G);
				}
				if (pressingB != 0)
				{
					this.actLaneFlushGB.Start(B);
					this.actRGB.Push(B);
				}
				// auto pickだとここから先に行かないので注意
				List<STInputEvent> events = CDTXMania.app.Pad.GetEvents(inst, Eパッド.Pick);
				if ((events != null) && (events.Count > 0))
				{
					foreach (STInputEvent eventPick in events)
					{
						if (!eventPick.b押された)
						{
							continue;
						}
						this.t入力メソッド記憶(inst);
						long nTime = eventPick.nTimeStamp - CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻;
						Ech定義 chWailingSound = (inst == E楽器パート.GUITAR) ? Ech定義.Guitar_WailingSound : Ech定義.Bass_WailingSound;
						CChip pChip = this.r指定時刻に一番近い未ヒットChip(nTime, chWailingSound, this.nInputAdjustTimeMs[indexInst]);	// E楽器パート.GUITARなチップ全てにヒットする
						E判定 e判定 = this.e指定時刻からChipのJUDGEを返す(nTime, pChip, this.nInputAdjustTimeMs[indexInst]);
						//Trace.TraceInformation("ch={0:x2}, mask1={1:x1}, mask2={2:x2}", pChip.nチャンネル番号,  ( pChip.nチャンネル番号 & ~nAutoMask ) & 0x0F, ( flagRGB & ~nAutoMask) & 0x0F );
						if (
							(pChip != null) &&
							((((int)pChip.eチャンネル番号 & ~nAutoMask) & 0x0F) == ((pressingRGB & ~nAutoMask) & 0x0F)) &&
							(e判定 != E判定.Miss))
						{
							bool bChipHasR = pChip.bGuitarBass_R;
							bool bChipHasG = pChip.bGuitarBass_G;
							bool bChipHasB = pChip.bGuitarBass_B;
							bool bChipHasW = pChip.bGuitarBass_Wailing;
							bool bChipIsO = pChip.bGuitarBass_Open;
							bool bSuccessOPEN = bChipIsO && (autoR || pressingR == 0) && (autoG || pressingG == 0) && (autoB || pressingB == 0);
							if ((bChipHasR && (autoR || pressingR != 0)) || bSuccessOPEN)
							//if ( ( pushingR != 0 ) || autoR || ( flagRGB == 0 ) )
							{
								this.actChipFireGB.Start(R, 演奏判定ライン座標);
							}
							if ((bChipHasG && (autoG || pressingG != 0)) || bSuccessOPEN)
							//if ( ( pushingG != 0 ) || autoG || ( flagRGB == 0 ) )
							{
								this.actChipFireGB.Start(G, 演奏判定ライン座標);
							}
							if ((bChipHasB && (autoB || pressingB != 0)) || bSuccessOPEN)
							//if ( ( pushingB != 0 ) || autoB || ( flagRGB == 0 ) )
							{
								this.actChipFireGB.Start(B, 演奏判定ライン座標);
							}
							this.tチップのヒット処理(nTime, pChip);
							this.tサウンド再生(pChip, CSound管理.rc演奏用タイマ.nシステム時刻, inst, CDTXMania.app.ConfigIni.n手動再生音量, CDTXMania.app.ConfigIni.b演奏音を強調する[indexInst], e判定 == E判定.Poor);
							Ech定義 chWailingChip = (inst == E楽器パート.GUITAR) ? Ech定義.Guitar_Wailing : Ech定義.Bass_Wailing;
							CChip item = this.r指定時刻に一番近い未ヒットChip(nTime, chWailingChip, this.nInputAdjustTimeMs[indexInst], 140);
							if (item != null)
							{
								this.queWailing[indexInst].Enqueue(item);
							}
							continue;
						}

						// 以下、間違いレーンでのピック時
						CChip NoChipPicked = (inst == E楽器パート.GUITAR) ? this.r現在の空うちギターChip : this.r現在の空うちベースChip;
						if ((NoChipPicked != null) || ((NoChipPicked = this.r指定時刻に一番近いChip_ヒット未済問わず不可視考慮(nTime, chWailingSound, this.nInputAdjustTimeMs[indexInst])) != null))
						{
							this.tサウンド再生(NoChipPicked, CSound管理.rc演奏用タイマ.nシステム時刻, inst, CDTXMania.app.ConfigIni.n手動再生音量, CDTXMania.app.ConfigIni.b演奏音を強調する[indexInst], true);
						}
						if (!CDTXMania.app.ConfigIni.bLight[indexInst])
						{
							this.tチップのヒット処理_BadならびにTight時のMiss(inst);
						}
					}
				}
				List<STInputEvent> list = CDTXMania.app.Pad.GetEvents(inst, Eパッド.Wail);
				if ((list != null) && (list.Count > 0))
				{
					foreach (STInputEvent eventWailed in list)
					{
						if (!eventWailed.b押された)
						{
							continue;
						}
						DoWailingFromQueue(inst, eventWailed.nTimeStamp, autoW);
					}
				}
			}
		}

		private void DoWailingFromQueue(E楽器パート inst, long nTimeStamp_Wailed, bool autoW)
		{
			int indexInst = (int)inst;
			long nTimeWailed = nTimeStamp_Wailed - CSound管理.rc演奏用タイマ.n前回リセットした時のシステム時刻;
			CChip chipWailing;
			while ((this.queWailing[indexInst].Count > 0) && ((chipWailing = this.queWailing[indexInst].Dequeue()) != null))
			{
				if ((nTimeWailed - chipWailing.n発声時刻ms) <= 1000)		// #24245 2011.1.26 yyagi: 800 -> 1000
				{
					chipWailing.bHit = true;
					this.actWailingBonus.Start(inst, this.r現在の歓声Chip[indexInst]);
					//if ( !bIsAutoPlay[indexInst] )
					if (!autoW)
					{
						int nCombo = (this.actCombo.n現在のコンボ数[indexInst] < 500) ? this.actCombo.n現在のコンボ数[indexInst] : 500;
						this.actScore.Add(inst, bIsAutoPlay, nCombo * 3000L);		// #24245 2011.1.26 yyagi changed DRUMS->BASS, add nCombo conditions
					}
				}
			}
		}
		#endregion
	}
}
