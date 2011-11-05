﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;
using FDK;

namespace DTXMania
{
	internal class CConfigIni
	{
		// クラス

		public class CKeyAssign
		{
			public class CKeyAssignPad
			{
				public CConfigIni.CKeyAssign.STKEYASSIGN[] HH
				{
					get
					{
						return this.padHH_R;
					}
					set
					{
						this.padHH_R = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] R
				{
					get
					{
						return this.padHH_R;
					}
					set
					{
						this.padHH_R = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] SD
				{
					get
					{
						return this.padSD_G;
					}
					set
					{
						this.padSD_G = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] G
				{
					get
					{
						return this.padSD_G;
					}
					set
					{
						this.padSD_G = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] BD
				{
					get
					{
						return this.padBD_B;
					}
					set
					{
						this.padBD_B = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] B
				{
					get
					{
						return this.padBD_B;
					}
					set
					{
						this.padBD_B = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] HT
				{
					get
					{
						return this.padHT_Pick;
					}
					set
					{
						this.padHT_Pick = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] Pick
				{
					get
					{
						return this.padHT_Pick;
					}
					set
					{
						this.padHT_Pick = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] LT
				{
					get
					{
						return this.padLT_Wail;
					}
					set
					{
						this.padLT_Wail = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] Wail
				{
					get
					{
						return this.padLT_Wail;
					}
					set
					{
						this.padLT_Wail = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] FT
				{
					get
					{
						return this.padFT_Cancel;
					}
					set
					{
						this.padFT_Cancel = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] Cancel
				{
					get
					{
						return this.padFT_Cancel;
					}
					set
					{
						this.padFT_Cancel = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] CY
				{
					get
					{
						return this.padCY_Decide;
					}
					set
					{
						this.padCY_Decide = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] Decide
				{
					get
					{
						return this.padCY_Decide;
					}
					set
					{
						this.padCY_Decide = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] HHO
				{
					get
					{
						return this.padHHO;
					}
					set
					{
						this.padHHO = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] RD
				{
					get
					{
						return this.padRD;
					}
					set
					{
						this.padRD = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] LC
				{
					get
					{
						return this.padLC;
					}
					set
					{
						this.padLC = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] Capture
				{
					get
					{
						return this.padCapture;
					}
					set
					{
						this.padCapture = value;
					}
				}
				public CConfigIni.CKeyAssign.STKEYASSIGN[] this[ int index ]
				{
					get
					{
						switch ( index )
						{
							case (int) EKeyConfigPad.HH:
								return this.padHH_R;

							case (int) EKeyConfigPad.SD:
								return this.padSD_G;

							case (int) EKeyConfigPad.BD:
								return this.padBD_B;

							case (int) EKeyConfigPad.HT:
								return this.padHT_Pick;

							case (int) EKeyConfigPad.LT:
								return this.padLT_Wail;

							case (int) EKeyConfigPad.FT:
								return this.padFT_Cancel;

							case (int) EKeyConfigPad.CY:
								return this.padCY_Decide;

							case (int) EKeyConfigPad.HHO:
								return this.padHHO;

							case (int) EKeyConfigPad.RD:
								return this.padRD;

							case (int) EKeyConfigPad.LC:
								return this.padLC;

							case (int) EKeyConfigPad.Capture:
								return this.padCapture;
						}
						throw new IndexOutOfRangeException();
					}
					set
					{
						switch ( index )
						{
							case (int) EKeyConfigPad.HH:
								this.padHH_R = value;
								return;

							case (int) EKeyConfigPad.SD:
								this.padSD_G = value;
								return;

							case (int) EKeyConfigPad.BD:
								this.padBD_B = value;
								return;

							case (int) EKeyConfigPad.Pick:
								this.padHT_Pick = value;
								return;

							case (int) EKeyConfigPad.LT:
								this.padLT_Wail = value;
								return;

							case (int) EKeyConfigPad.FT:
								this.padFT_Cancel = value;
								return;

							case (int) EKeyConfigPad.CY:
								this.padCY_Decide = value;
								return;

							case (int) EKeyConfigPad.HHO:
								this.padHHO = value;
								return;

							case (int) EKeyConfigPad.RD:
								this.padRD = value;
								return;

							case (int) EKeyConfigPad.LC:
								this.padLC = value;
								return;

							case (int) EKeyConfigPad.Capture:
								this.padCapture = value;
								return;
						}
						throw new IndexOutOfRangeException();
					}
				}

				#region [ private ]
				//-----------------
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padBD_B;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padCY_Decide;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padFT_Cancel;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padHH_R;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padHHO;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padHT_Pick;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padLC;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padLT_Wail;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padRD;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padSD_G;
				private CConfigIni.CKeyAssign.STKEYASSIGN[] padCapture;
				//-----------------
				#endregion
			}

			[StructLayout( LayoutKind.Sequential )]
			public struct STKEYASSIGN
			{
				public E入力デバイス 入力デバイス;
				public int ID;
				public int コード;
				public STKEYASSIGN( E入力デバイス DeviceType, int nID, int nCode )
				{
					this.入力デバイス = DeviceType;
					this.ID = nID;
					this.コード = nCode;
				}
			}

			public CKeyAssignPad Bass = new CKeyAssignPad();
			public CKeyAssignPad Drums = new CKeyAssignPad();
			public CKeyAssignPad Guitar = new CKeyAssignPad();
			public CKeyAssignPad System = new CKeyAssignPad();
			public CKeyAssignPad this[ int index ]
			{
				get
				{
					switch( index )
					{
						case (int) EKeyConfigPart.DRUMS:
							return this.Drums;

						case (int) EKeyConfigPart.GUITAR:
							return this.Guitar;

						case (int) EKeyConfigPart.BASS:
							return this.Bass;

						case (int) EKeyConfigPart.SYSTEM:
							return this.System;
					}
					throw new IndexOutOfRangeException();
				}
				set
				{
					switch( index )
					{
						case (int) EKeyConfigPart.DRUMS:
							this.Drums = value;
							return;

						case (int) EKeyConfigPart.GUITAR:
							this.Guitar = value;
							return;

						case (int) EKeyConfigPart.BASS:
							this.Bass = value;
							return;

						case (int) EKeyConfigPart.SYSTEM:
							this.System = value;
							return;
					}
					throw new IndexOutOfRangeException();
				}
			}
		}


		// プロパティ

#if false		// #23625 2011.1.11 Config.iniからダメージ/回復値の定数変更を行う場合はここを有効にする 087リリースに合わせ機能無効化
		//----------------------------------------
		public float[,] fGaugeFactor = new float[5,2];
		public float[] fDamageLevelFactor = new float[3];
		//----------------------------------------
#endif
		public int nBGAlpha;
		public bool bAVI有効;
		public bool bBGA有効;
		public bool bBGM音を発声する;
		public STDGBVALUE<bool> bHidden;
		public STDGBVALUE<bool> bLeft;
		public STDGBVALUE<bool> bLight;
		public bool bLogDTX詳細ログ出力;
		public bool bLog曲検索ログ出力;
		public bool bLog作成解放ログ出力;
		public STDGBVALUE<bool> bReverse;
		public bool bScoreIniを出力する;
		public bool bSTAGEFAILED有効;
		public STDGBVALUE<bool> bSudden;
		public bool bTight;
		public STDGBVALUE<bool> bGraph;     // #24074 2011.01.23 add ikanick
		public bool bWave再生位置自動調整機能有効;
		public bool bシンバルフリー;
		public bool bストイックモード;
		public bool bドラム打音を発声する;
		public bool bフィルイン有効;
		public bool bランダムセレクトで子BOXを検索対象とする;
		public bool bログ出力;
		public STDGBVALUE<bool> b演奏音を強調する;
		public bool b演奏情報を表示する;
		public bool b歓声を発声する;
		public bool b垂直帰線待ちを行う;
		public bool b選曲リストフォントを斜体にする;
		public bool b選曲リストフォントを太字にする;
		public bool b全画面モード;
		public int nウインドウwidth;				// #23510 2010.10.31 yyagi add
		public int nウインドウheight;				// #23510 2010.10.31 yyagi add
		public Dictionary<int, string> dicJoystick;
		public ECYGroup eCYGroup;
		public Eダークモード eDark;
		public EFTGroup eFTGroup;
		public EHHGroup eHHGroup;
		public E打ち分け時の再生の優先順位 eHitSoundPriorityCY;
		public E打ち分け時の再生の優先順位 eHitSoundPriorityFT;
		public E打ち分け時の再生の優先順位 eHitSoundPriorityHH;
		public STDGBVALUE<Eランダムモード> eRandom;
		public Eダメージレベル eダメージレベル;
        public CKeyAssign KeyAssign;
        public int n非フォーカス時スリープms;       // #23568 2010.11.04 ikanick add
		public int n演奏速度;
		public int n曲が選択されてからプレビュー音が鳴るまでのウェイトms;
		public int n曲が選択されてからプレビュー画像が表示開始されるまでのウェイトms;
		public int n自動再生音量;
		public int n手動再生音量;
		public int n選曲リストフォントのサイズdot;
		public STDGBVALUE<int> n表示可能な最小コンボ数;
		public STDGBVALUE<int> n譜面スクロール速度;
		public string strDTXManiaのバージョン;
		public string str曲データ検索パス;
		public string str選曲リストフォント;
		public Eドラムコンボ文字の表示位置 ドラムコンボ文字の表示位置;
		public STDGBVALUE<E判定文字表示位置> 判定文字表示位置;
//		public int nハイハット切り捨て下限Velocity;
//		public int n切り捨て下限Velocity;			// #23857 2010.12.12 yyagi VelocityMin
		public STDGBVALUE<int> nInputAdjustTimeMs;	// #23580 2011.1.3 yyagi タイミングアジャスト機能
		public int	nShowLagType;					// #25370 2011.6.5 yyagi ズレ時間表示機能
		public bool bIsAutoResultCapture;			// #25399 2011.6.9 yyagi リザルト画像自動保存機能のON/OFF制御
		public bool bバッファ入力を行う;
		public bool bConfigIniがないかDTXManiaのバージョンが異なる
		{
			get
			{
				return ( !this.bConfigIniが存在している || !CDTXMania.VERSION.Equals( this.strDTXManiaのバージョン ) );
			}
		}
		public bool bDrums有効
		{
			get
			{
				return this._bDrums有効;
			}
			set
			{
				this._bDrums有効 = value;
				if( !this._bGuitar有効 && !this._bDrums有効 )
				{
					this._bGuitar有効 = true;
				}
			}
		}
		public bool bEnterがキー割り当てのどこにも使用されていない
		{
			get
			{
				for( int i = 0; i <= (int)EKeyConfigPart.SYSTEM; i++ )
				{
					for( int j = 0; j <= (int)EKeyConfigPad.Capture; j++ )
					{
						for( int k = 0; k < 0x10; k++ )
						{
							if( ( this.KeyAssign[ i ][ j ][ k ].入力デバイス == E入力デバイス.キーボード ) && ( this.KeyAssign[ i ][ j ][ k ].コード == (int) SlimDX.DirectInput.Key.Return ) )
							{
								return false;
							}
						}
					}
				}
				return true;
			}
		}
		public bool bGuitar有効
		{
			get
			{
				return this._bGuitar有効;
			}
			set
			{
				this._bGuitar有効 = value;
				if( !this._bGuitar有効 && !this._bDrums有効 )
				{
					this._bDrums有効 = true;
				}
			}
		}
		public bool bウィンドウモード
		{
			get
			{
				return !this.b全画面モード;
			}
			set
			{
				this.b全画面モード = !value;
			}
		}
		public bool bギタレボモード
		{
			get
			{
				return ( !this.bDrums有効 && this.bGuitar有効 );
			}
		}
		public bool bドラムが全部オートプレイである
		{
			get
			{
				for( int i = 0; i < 8; i++ )
				{
					if( !this.bAutoPlay[ i ] )
					{
						return false;
					}
				}
				return true;
			}
		}
		public bool b演奏情報を表示しない
		{
			get
			{
				return !this.b演奏情報を表示する;
			}
			set
			{
				this.b演奏情報を表示する = !value;
			}
		}
		public int n背景の透過度
		{
			get
			{
				return this.nBGAlpha;
			}
			set
			{
				if( value < 0 )
				{
					this.nBGAlpha = 0;
				}
				else if( value > 0xff )
				{
					this.nBGAlpha = 0xff;
				}
				else
				{
					this.nBGAlpha = value;
				}
			}
		}
		public int nRisky;						// #23559 2011.6.20 yyagi Riskyでの残ミス数。0で閉店
		public bool bIsSwappedGuitarBass			// #24063 2011.1.16 yyagi ギターとベースの切り替え中か否か
		{
			get;
			set;
		}
		public bool bIsSwappedGuitarBass_AutoFlagsAreSwapped	// #24415 2011.2.21 yyagi FLIP中にalt-f4終了で、AUTOフラグがswapした状態でconfig.iniが出力されてしまうことを避けるためのフラグ
		{
		    get;
		    set;
		}
		public STAUTOPLAY bAutoPlay;
		[StructLayout( LayoutKind.Sequential )]
		public struct STAUTOPLAY
		{
			public bool LC;
			public bool HH;
			public bool SD;
			public bool BD;
			public bool HT;
			public bool LT;
			public bool FT;
			public bool CY;
			public bool Guitar;
			public bool Bass;
			public bool this[ int index ]
			{
				get
				{
					switch( index )
					{
						case (int) Eドラムレーン.LC:
							return this.LC;

						case (int) Eドラムレーン.HH:
							return this.HH;

						case (int) Eドラムレーン.SD:
							return this.SD;

						case (int) Eドラムレーン.BD:
							return this.BD;

						case (int) Eドラムレーン.HT:
							return this.HT;

						case (int) Eドラムレーン.LT:
							return this.LT;

						case (int) Eドラムレーン.FT:
							return this.FT;

						case (int) Eドラムレーン.CY:
							return this.CY;

						case (int) Eドラムレーン.GT:
							return this.Guitar;

						case (int) Eドラムレーン.BS:
							return this.Bass;
					}
					throw new IndexOutOfRangeException();
				}
				set
				{
					switch( index )
					{
						case (int) Eドラムレーン.LC:
							this.LC = value;
							return;

						case (int) Eドラムレーン.HH:
							this.HH = value;
							return;

						case (int) Eドラムレーン.SD:
							this.SD = value;
							return;

						case (int) Eドラムレーン.BD:
							this.BD = value;
							return;

						case (int) Eドラムレーン.HT:
							this.HT = value;
							return;

						case (int) Eドラムレーン.LT:
							this.LT = value;
							return;

						case (int) Eドラムレーン.FT:
							this.FT = value;
							return;

						case (int) Eドラムレーン.CY:
							this.CY = value;
							return;

						case (int) Eドラムレーン.CY + 1:
							this.Guitar = value;
							return;

						case (int) Eドラムレーン.CY + 2:
							this.Bass = value;
							return;
					}
					throw new IndexOutOfRangeException();
				}
			}
		}

		public STRANGE nヒット範囲ms;
		[StructLayout( LayoutKind.Sequential )]
		public struct STRANGE
		{
			public int Perfect;
			public int Great;
			public int Good;
			public int Poor;
			public int this[ int index ]
			{
				get
				{
					switch( index )
					{
						case 0:
							return this.Perfect;

						case 1:
							return this.Great;

						case 2:
							return this.Good;

						case 3:
							return this.Poor;
					}
					throw new IndexOutOfRangeException();
				}
				set
				{
					switch( index )
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
					}
					throw new IndexOutOfRangeException();
				}
			}
		}

		public STLANEVALUE nVelocityMin;
		[StructLayout( LayoutKind.Sequential )]
		public struct STLANEVALUE
		{
			public int LC;
			public int HH;
			public int SD;
			public int BD;
			public int HT;
			public int LT;
			public int FT;
			public int CY;
			public int RD;
			public int Guitar;
			public int Bass;
			public int this[ int index ]
			{
				get
				{
					switch( index )
					{
						case 0:
							return this.LC;

						case 1:
							return this.HH;

						case 2:
							return this.SD;

						case 3:
							return this.BD;

						case 4:
							return this.HT;

						case 5:
							return this.LT;

						case 6:
							return this.FT;

						case 7:
							return this.CY;

						case 8:
							return this.RD;

						case 9:
							return this.Guitar;

						case 10:
							return this.Bass;
					}
					throw new IndexOutOfRangeException();
				}
				set
				{
					switch( index )
					{
						case 0:
							this.LC = value;
							return;

						case 1:
							this.HH = value;
							return;

						case 2:
							this.SD = value;
							return;

						case 3:
							this.BD = value;
							return;

						case 4:
							this.HT = value;
							return;

						case 5:
							this.LT = value;
							return;

						case 6:
							this.FT = value;
							return;

						case 7:
							this.CY = value;
							return;

						case 8:
							this.RD = value;
							return;

						case 9:
							this.Guitar = value;
							return;

						case 10:
							this.Bass = value;
							return;
					}
					throw new IndexOutOfRangeException();
				}
			}
		}


		// コンストラクタ

		public CConfigIni()
		{
#if false		// #23625 2011.1.11 Config.iniからダメージ/回復値の定数変更を行う場合はここを有効にする 087リリースに合わせ機能無効化
			//----------------------------------------
			this.fGaugeFactor[0,0] =  0.004f;
			this.fGaugeFactor[0,1] =  0.006f;
			this.fGaugeFactor[1,0] =  0.002f;
			this.fGaugeFactor[1,1] =  0.003f;
			this.fGaugeFactor[2,0] =  0.000f;
			this.fGaugeFactor[2,1] =  0.000f;
			this.fGaugeFactor[3,0] = -0.020f;
			this.fGaugeFactor[3,1] = -0.030f;
			this.fGaugeFactor[4,0] = -0.050f;
			this.fGaugeFactor[4,1] = -0.050f;

			this.fDamageLevelFactor[0] = 0.5f;
			this.fDamageLevelFactor[1] = 1.0f;
			this.fDamageLevelFactor[2] = 1.5f;
			//----------------------------------------
#endif
			this.strDTXManiaのバージョン = "Unknown";
			this.str曲データ検索パス = @".\";
			this.b全画面モード = false;
			this.b垂直帰線待ちを行う = true;
			this.nウインドウwidth = SampleFramework.GameWindowSize.Width;			// #23510 2010.10.31 yyagi add
			this.nウインドウheight = SampleFramework.GameWindowSize.Height;			// 
			this.n非フォーカス時スリープms = 1;			// #23568 2010.11.04 ikanick add
			this._bGuitar有効 = true;
			this._bDrums有効 = true;
			this.nBGAlpha = 100;
			this.eダメージレベル = Eダメージレベル.普通;
			this.bSTAGEFAILED有効 = true;
			this.bAVI有効 = true;
			this.bBGA有効 = true;
			this.bフィルイン有効 = true;
			this.n曲が選択されてからプレビュー音が鳴るまでのウェイトms = 1000;
			this.n曲が選択されてからプレビュー画像が表示開始されるまでのウェイトms = 100;
			this.bWave再生位置自動調整機能有効 = true;
			this.bBGM音を発声する = true;
			this.bドラム打音を発声する = true;
			this.b歓声を発声する = true;
			this.bScoreIniを出力する = true;
			this.bランダムセレクトで子BOXを検索対象とする = true;
			this.n表示可能な最小コンボ数 = new STDGBVALUE<int>();
			this.n表示可能な最小コンボ数.Drums = 11;
			this.n表示可能な最小コンボ数.Guitar = 2;
			this.n表示可能な最小コンボ数.Bass = 2;
			this.str選曲リストフォント = "MS PGothic";
			this.n選曲リストフォントのサイズdot = 20;
			this.b選曲リストフォントを太字にする = true;
			this.n自動再生音量 = 80;
			this.n手動再生音量 = 100;
			this.bログ出力 = true;
			this.b演奏音を強調する = new STDGBVALUE<bool>();
			this.bSudden = new STDGBVALUE<bool>();
			this.bHidden = new STDGBVALUE<bool>();
			this.bReverse = new STDGBVALUE<bool>();
			this.eRandom = new STDGBVALUE<Eランダムモード>();
			this.bLight = new STDGBVALUE<bool>();
			this.bLeft = new STDGBVALUE<bool>();
			this.判定文字表示位置 = new STDGBVALUE<E判定文字表示位置>();
			this.n譜面スクロール速度 = new STDGBVALUE<int>();
			this.nInputAdjustTimeMs = new STDGBVALUE<int>();	// #23580 2011.1.3 yyagi
			for ( int i = 0; i < 3; i++ )
			{
				this.b演奏音を強調する[ i ] = true;
				this.bSudden[ i ] = false;
				this.bHidden[ i ] = false;
				this.bReverse[ i ] = false;
				this.eRandom[ i ] = Eランダムモード.OFF;
				this.bLight[ i ] = false;
				this.bLeft[ i ] = false;
				this.判定文字表示位置[ i ] = E判定文字表示位置.レーン上;
				this.n譜面スクロール速度[ i ] = 1;
				this.nInputAdjustTimeMs[ i ] = 0;
			}
			this.n演奏速度 = 20;
			this.bAutoPlay = new STAUTOPLAY();
			this.bAutoPlay.HH = false;
			this.bAutoPlay.SD = false;
			this.bAutoPlay.BD = false;
			this.bAutoPlay.HT = false;
			this.bAutoPlay.LT = false;
			this.bAutoPlay.FT = false;
			this.bAutoPlay.CY = false;
			this.bAutoPlay.LC = false;
			this.bAutoPlay.Guitar = true;
			this.bAutoPlay.Bass = true;
			this.nヒット範囲ms = new STRANGE();
			this.nヒット範囲ms.Perfect = 34;
			this.nヒット範囲ms.Great = 67;
			this.nヒット範囲ms.Good = 84;
			this.nヒット範囲ms.Poor = 117;
			this.ConfigIniファイル名 = "";
			this.dicJoystick = new Dictionary<int, string>( 10 );
			this.tデフォルトのキーアサインに設定する();
			this.nVelocityMin.LC = 0;					// #23857 2011.1.31 yyagi VelocityMin
			this.nVelocityMin.HH = 20;
			this.nVelocityMin.SD = 0;
			this.nVelocityMin.BD = 0;
			this.nVelocityMin.HT = 0;
			this.nVelocityMin.LT = 0;
			this.nVelocityMin.FT = 0;
			this.nVelocityMin.CY = 0;
			this.nVelocityMin.RD = 0;
			this.nRisky = 0;							// #23539 2011.7.26 yyagi RISKYモード
			this.nShowLagType = (int) EShowLagType.OFF;	// #25370 2011.6.3 yyagi ズレ時間表示
			this.bIsAutoResultCapture = false;			// #25399 2011.6.9 yyagi リザルト画像自動保存機能ON/OFF

			this.bバッファ入力を行う = true;
			this.bIsSwappedGuitarBass = false;			// #24063 2011.1.16 yyagi ギターとベースの切り替え
		}
		public CConfigIni( string iniファイル名 )
			: this()
		{
			this.tファイルから読み込み( iniファイル名 );
		}


		// メソッド

		public void t指定した入力が既にアサイン済みである場合はそれを全削除する( E入力デバイス DeviceType, int nID, int nCode )
		{
			for( int i = 0; i <= (int)EKeyConfigPart.SYSTEM; i++ )
			{
				for( int j = 0; j <= (int)EKeyConfigPad.Capture; j++ )
				{
					for( int k = 0; k < 0x10; k++ )
					{
						if( ( ( this.KeyAssign[ i ][ j ][ k ].入力デバイス == DeviceType ) && ( this.KeyAssign[ i ][ j ][ k ].ID == nID ) ) && ( this.KeyAssign[ i ][ j ][ k ].コード == nCode ) )
						{
							for( int m = k; m < 15; m++ )
							{
								this.KeyAssign[ i ][ j ][ m ] = this.KeyAssign[ i ][ j ][ m + 1 ];
							}
							this.KeyAssign[ i ][ j ][ 15 ].入力デバイス = E入力デバイス.不明;
							this.KeyAssign[ i ][ j ][ 15 ].ID = 0;
							this.KeyAssign[ i ][ j ][ 15 ].コード = 0;
							k--;
						}
					}
				}
			}
		}
		public void t書き出し( string iniファイル名 )
		{
			StreamWriter sw = new StreamWriter( iniファイル名, false, Encoding.GetEncoding( "shift-jis" ) );
			sw.WriteLine( ";-------------------" );
			sw.WriteLine( "[System]" );
			sw.WriteLine();

#if false		// #23625 2011.1.11 Config.iniからダメージ/回復値の定数変更を行う場合はここを有効にする 087リリースに合わせ機能無効化
	//------------------------------
			sw.WriteLine("; ライフゲージのパラメータ調整(調整完了後削除予定)");
			sw.WriteLine("; GaugeFactorD: ドラムのPerfect, Great,... の回復量(ライフMAXを1.0としたときの値を指定)");
			sw.WriteLine("; GaugeFactorG:  Gt/BsのPerfect, Great,... の回復量(ライフMAXを1.0としたときの値を指定)");
			sw.WriteLine("; DamageFactorD: DamageLevelがSmall, Normal, Largeの時に対するダメージ係数");
			sw.WriteLine("GaugeFactorD={0}, {1}, {2}, {3}, {4}", this.fGaugeFactor[0, 0], this.fGaugeFactor[1, 0], this.fGaugeFactor[2, 0], this.fGaugeFactor[3, 0], this.fGaugeFactor[4, 0]);
			sw.WriteLine("GaugeFactorG={0}, {1}, {2}, {3}, {4}", this.fGaugeFactor[0, 1], this.fGaugeFactor[1, 1], this.fGaugeFactor[2, 1], this.fGaugeFactor[3, 1], this.fGaugeFactor[4, 1]);
			sw.WriteLine("DamageFactor={0}, {1}, {2}", this.fDamageLevelFactor[0], this.fDamageLevelFactor[1], fDamageLevelFactor[2]);
			sw.WriteLine();
	//------------------------------
#endif

			sw.WriteLine( "; リリースバージョン" );
			sw.WriteLine( "Version={0}", CDTXMania.VERSION );
			sw.WriteLine();
			sw.WriteLine( "; 演奏データの格納されているフォルダへのパス。" );
			sw.WriteLine( @"; セミコロン(;)で区切ることにより複数のパスを指定できます。（例: d:\DTXFiles1\;e:\DTXFiles2\）" );
			sw.WriteLine( "DTXPath={0}", this.str曲データ検索パス );
			sw.WriteLine();
			sw.WriteLine( "; 画面モード(0:ウィンドウ, 1:全画面)" );
			sw.WriteLine( "FullScreen={0}", this.b全画面モード ? 1 : 0 );
            sw.WriteLine();

			sw.WriteLine("; ウインドウモード時の画面幅");				// #23510 2010.10.31 yyagi add
			sw.WriteLine("; A width size in the window mode.");			//
			sw.WriteLine("WindowWidth={0}", this.nウインドウwidth);		//
			sw.WriteLine();												//
			sw.WriteLine("; ウインドウモード時の画面高さ");				//
			sw.WriteLine("; A height size in the window mode.");		//
			sw.WriteLine("WindowHeight={0}", this.nウインドウheight);	//
			sw.WriteLine();												//

			sw.WriteLine("; 垂直帰線同期(0:OFF,1:ON)");
			sw.WriteLine( "VSyncWait={0}", this.b垂直帰線待ちを行う ? 1 : 0 );
            sw.WriteLine();

            sw.WriteLine("; 非フォーカス時のsleep値[ms]");	    			    // #23568 2011.11.04 ikanick add
			sw.WriteLine("; A sleep time[ms] while the window is inactive.");	//
			sw.WriteLine("BackSleep={0}", this.n非フォーカス時スリープms);		// そのまま引用（苦笑）
            sw.WriteLine();											        	//

			sw.WriteLine( "; ギター/ベース有効(0:OFF,1:ON)" );
			sw.WriteLine( "Guitar={0}", this.bGuitar有効 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ドラム有効(0:OFF,1:ON)" );
			sw.WriteLine( "Drums={0}", this.bDrums有効 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 背景画像の半透明割合(0:透明～255:不透明)" );
			sw.WriteLine( "BGAlpha={0}", this.nBGAlpha );
			sw.WriteLine();
			sw.WriteLine( "; Missヒット時のゲージ減少割合(0:少, 1:普通, 2:大)" );
			sw.WriteLine( "DamageLevel={0}", (int) this.eダメージレベル );
			sw.WriteLine();
			sw.WriteLine( "; LC/HHC/HHO 打ち分けモード(0:LC|HHC|HHO, 1:LC&(HHC|HHO), 2:LC&HHC&HHO)" );
			sw.WriteLine( "HHGroup={0}", (int) this.eHHGroup );
			sw.WriteLine();
			sw.WriteLine( "; LT/FT 打ち分けモード(0:LT|FT, 1:LT&FT)" );
			sw.WriteLine( "FTGroup={0}", (int) this.eFTGroup );
			sw.WriteLine();
			sw.WriteLine( "; CY/RD 打ち分けモード(0:CY|RD, 1:CY&RD)" );
			sw.WriteLine( "CYGroup={0}", (int) this.eCYGroup );
			sw.WriteLine();
			sw.WriteLine( "; 打ち分け時の再生音の優先順位(HHGroup)(0:Chip>Pad, 1:Pad>Chip)" );
			sw.WriteLine( "HitSoundPriorityHH={0}", (int) this.eHitSoundPriorityHH );
			sw.WriteLine();
			sw.WriteLine( "; 打ち分け時の再生音の優先順位(FTGroup)(0:Chip>Pad, 1:Pad>Chip)" );
			sw.WriteLine( "HitSoundPriorityFT={0}", (int) this.eHitSoundPriorityFT );
			sw.WriteLine();
			sw.WriteLine( "; 打ち分け時の再生音の優先順位(CYGroup)(0:Chip>Pad, 1:Pad>Chip)" );
			sw.WriteLine( "HitSoundPriorityCY={0}", (int) this.eHitSoundPriorityCY );
			sw.WriteLine();
			sw.WriteLine( "; ゲージゼロでSTAGE FAILED (0:OFF, 1:ON)" );
			sw.WriteLine( "StageFailed={0}", this.bSTAGEFAILED有効 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; AVIの表示(0:OFF, 1:ON)" );
			sw.WriteLine( "AVI={0}", this.bAVI有効 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; BGAの表示(0:OFF, 1:ON)" );
			sw.WriteLine( "BGA={0}", this.bBGA有効 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; フィルイン効果(0:OFF, 1:ON)" );
			sw.WriteLine( "FillInEffect={0}", this.bフィルイン有効 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 曲選択からプレビュー音の再生までのウェイト[ms]" );
			sw.WriteLine( "PreviewSoundWait={0}", this.n曲が選択されてからプレビュー音が鳴るまでのウェイトms );
			sw.WriteLine();
			sw.WriteLine( "; 曲選択からプレビュー画像表示までのウェイト[ms]" );
			sw.WriteLine( "PreviewImageWait={0}", this.n曲が選択されてからプレビュー画像が表示開始されるまでのウェイトms );
			sw.WriteLine();
			sw.WriteLine( "; Waveの再生位置自動補正(0:OFF, 1:ON)" );
			sw.WriteLine( "AdjustWaves={0}", this.bWave再生位置自動調整機能有効 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; BGM の再生(0:OFF, 1:ON)" );
			sw.WriteLine( "BGMSound={0}", this.bBGM音を発声する ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ドラム打音の再生(0:OFF, 1:ON)" );
			sw.WriteLine( "HitSound={0}", this.bドラム打音を発声する ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; フィルイン達成時の歓声の再生(0:OFF, 1:ON)" );
			sw.WriteLine( "AudienceSound={0}", this.b歓声を発声する ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 演奏記録（～.score.ini）の出力 (0:OFF, 1:ON)" );
			sw.WriteLine( "SaveScoreIni={0}", this.bScoreIniを出力する ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; RANDOM SELECT で子BOXを検索対象に含める (0:OFF, 1:ON)" );
			sw.WriteLine( "RandomFromSubBox={0}", this.bランダムセレクトで子BOXを検索対象とする ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ドラム演奏時にドラム音を強調する (0:OFF, 1:ON)" );
			sw.WriteLine( "SoundMonitorDrums={0}", this.b演奏音を強調する.Drums ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ギター演奏時にギター音を強調する (0:OFF, 1:ON)" );
			sw.WriteLine( "SoundMonitorGuitar={0}", this.b演奏音を強調する.Guitar ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ベース演奏時にベース音を強調する (0:OFF, 1:ON)" );
			sw.WriteLine( "SoundMonitorBass={0}", this.b演奏音を強調する.Bass ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ドラムの表示可能な最小コンボ数(1～99999)" );
			sw.WriteLine( "MinComboDrums={0}", this.n表示可能な最小コンボ数.Drums );
			sw.WriteLine();
			sw.WriteLine( "; ギターの表示可能な最小コンボ数(1～99999)" );
			sw.WriteLine( "MinComboGuitar={0}", this.n表示可能な最小コンボ数.Guitar );
			sw.WriteLine();
			sw.WriteLine( "; ベースの表示可能な最小コンボ数(1～99999)" );
			sw.WriteLine( "MinComboBass={0}", this.n表示可能な最小コンボ数.Bass );
			sw.WriteLine();
			sw.WriteLine( "; 演奏情報を表示する (0:OFF, 1:ON)" );
			sw.WriteLine( "ShowDebugStatus={0}", this.b演奏情報を表示する ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 選曲リストのフォント名" );
			sw.WriteLine( "SelectListFontName={0}", this.str選曲リストフォント );
			sw.WriteLine();
			sw.WriteLine( "; 選曲リストのフォントのサイズ[dot]" );
			sw.WriteLine( "SelectListFontSize={0}", this.n選曲リストフォントのサイズdot );
			sw.WriteLine();
			sw.WriteLine( "; 選曲リストのフォントを斜体にする (0:OFF, 1:ON)" );
			sw.WriteLine( "SelectListFontItalic={0}", this.b選曲リストフォントを斜体にする ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 選曲リストのフォントを太字にする (0:OFF, 1:ON)" );
			sw.WriteLine( "SelectListFontBold={0}", this.b選曲リストフォントを太字にする ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 打音の音量(0～100%)" );
			sw.WriteLine( "ChipVolume={0}", this.n手動再生音量 );
			sw.WriteLine();
			sw.WriteLine( "; 自動再生音の音量(0～100%)" );
			sw.WriteLine( "AutoChipVolume={0}", this.n自動再生音量 );
			sw.WriteLine();
			sw.WriteLine( "; ストイックモード(0:OFF, 1:ON)" );
			sw.WriteLine( "StoicMode={0}", this.bストイックモード ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; シンバルフリーモード(0:OFF, 1:ON)" );
			sw.WriteLine( "CymbalFree={0}", this.bシンバルフリー ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; バッファ入力モード(0:OFF, 1:ON)" );
			sw.WriteLine( "BufferedInput={0}", this.bバッファ入力を行う ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 判定ズレ時間表示(0:OFF, 1:ON, 2=GREAT-POOR)" );				// #25370 2011.6.3 yyagi
			sw.WriteLine( "; Whether displaying the lag times from the just timing or not." );	//
			sw.WriteLine( "ShowLagTime={0}", this.nShowLagType );							//
			sw.WriteLine();
			sw.WriteLine( "; リザルト画像自動保存機能(0:OFF, 1:ON)" );						// #25399 2011.6.9 yyagi
			sw.WriteLine( "; Set ON if you'd like to save result screen image automatically");	//
			sw.WriteLine( "; when you get hiscore/hiskill.");								//
			sw.WriteLine( "AutoResultCapture={0}", this.bIsAutoResultCapture? 1 : 0 );		//
			sw.WriteLine();
			sw.WriteLine( "; 判定タイミング調整(ドラム, ギター, ベース)(-99～0)[ms]" );		// #23580 2011.1.3 yyagi
			sw.WriteLine("; Revision value to adjust judgement timing for the drums, guitar and bass.");	//
			sw.WriteLine("InputAdjustTimeDrums={0}", this.nInputAdjustTimeMs.Drums);		//
			sw.WriteLine("InputAdjustTimeGuitar={0}", this.nInputAdjustTimeMs.Guitar);		//
			sw.WriteLine("InputAdjustTimeBass={0}", this.nInputAdjustTimeMs.Bass);			//
			sw.WriteLine();
			sw.WriteLine( "; LC, HH, SD,...の入力切り捨て下限Velocity値(0～127)" );			// #23857 2011.1.31 yyagi
			sw.WriteLine( "; Minimum velocity value for LC, HH, SD, ... to accept." );		//
			sw.WriteLine( "LCVelocityMin={0}", this.nVelocityMin.LC );						//
			sw.WriteLine("HHVelocityMin={0}", this.nVelocityMin.HH );						//
//			sw.WriteLine("; ハイハット以外の入力切り捨て下限Velocity値(0～127)");			// #23857 2010.12.12 yyagi
//			sw.WriteLine("; Minimum velocity value to accept. (except HiHat)");				//
//			sw.WriteLine("VelocityMin={0}", this.n切り捨て下限Velocity);					//
//			sw.WriteLine();																	//
			sw.WriteLine( "SDVelocityMin={0}", this.nVelocityMin.SD );						//
			sw.WriteLine( "BDVelocityMin={0}", this.nVelocityMin.BD );						//
			sw.WriteLine( "HTVelocityMin={0}", this.nVelocityMin.HT );						//
			sw.WriteLine( "LTVelocityMin={0}", this.nVelocityMin.LT );						//
			sw.WriteLine( "FTVelocityMin={0}", this.nVelocityMin.FT );						//
			sw.WriteLine( "CYVelocityMin={0}", this.nVelocityMin.CY );						//
			sw.WriteLine( "RDVelocityMin={0}", this.nVelocityMin.RD );						//
			sw.WriteLine();																	//
			sw.WriteLine( ";-------------------" );
			sw.WriteLine( "[Log]" );
			sw.WriteLine();
			sw.WriteLine( "; Log出力(0:OFF, 1:ON)" );
			sw.WriteLine( "OutputLog={0}", this.bログ出力 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 曲データ検索に関するLog出力(0:OFF, 1:ON)" );
			sw.WriteLine( "TraceSongSearch={0}", this.bLog曲検索ログ出力 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; 画像やサウンドの作成・解放に関するLog出力(0:OFF, 1:ON)" );
			sw.WriteLine( "TraceCreatedDisposed={0}", this.bLog作成解放ログ出力 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; DTX読み込み詳細に関するLog出力(0:OFF, 1:ON)" );
			sw.WriteLine( "TraceDTXDetails={0}", this.bLogDTX詳細ログ出力 ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( ";-------------------" );
			sw.WriteLine( "[PlayOption]" );
			sw.WriteLine();
			sw.WriteLine( "; DARKモード(0:OFF, 1:HALF, 2:FULL)" );
			sw.WriteLine( "Dark={0}", (int) this.eDark );
			sw.WriteLine();
			sw.WriteLine( "; ドラムSUDDENモード(0:OFF, 1:ON)" );
			sw.WriteLine( "DrumsSudden={0}", this.bSudden.Drums ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ギターSUDDENモード(0:OFF, 1:ON)" );
			sw.WriteLine( "GuitarSudden={0}", this.bSudden.Guitar ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ベースSUDDENモード(0:OFF, 1:ON)" );
			sw.WriteLine( "BassSudden={0}", this.bSudden.Bass ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ドラムHIDDENモード(0:OFF, 1:ON)" );
			sw.WriteLine( "DrumsHidden={0}", this.bHidden.Drums ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ギターHIDDENモード(0:OFF, 1:ON)" );
			sw.WriteLine( "GuitarHidden={0}", this.bHidden.Guitar ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ベースHIDDENモード(0:OFF, 1:ON)" );
			sw.WriteLine( "BassHidden={0}", this.bHidden.Bass ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ドラムREVERSEモード(0:OFF, 1:ON)" );
			sw.WriteLine( "DrumsReverse={0}", this.bReverse.Drums ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ギターREVERSEモード(0:OFF, 1:ON)" );
			sw.WriteLine( "GuitarReverse={0}", this.bReverse.Guitar ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ベースREVERSEモード(0:OFF, 1:ON)" );
			sw.WriteLine( "BassReverse={0}", this.bReverse.Bass ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ギターRANDOMモード(0:OFF, 1:Random, 2:SuperRandom, 3:HyperRandom)" );
			sw.WriteLine( "GuitarRandom={0}", (int) this.eRandom.Guitar );
			sw.WriteLine();
			sw.WriteLine( "; ベースRANDOMモード(0:OFF, 1:Random, 2:SuperRandom, 3:HyperRandom)" );
			sw.WriteLine( "BassRandom={0}", (int) this.eRandom.Bass );
			sw.WriteLine();
			sw.WriteLine( "; ギターLIGHTモード(0:OFF, 1:ON)" );
			sw.WriteLine( "GuitarLight={0}", this.bLight.Guitar ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ベースLIGHTモード(0:OFF, 1:ON)" );
			sw.WriteLine( "BassLight={0}", this.bLight.Bass ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ギターLEFTモード(0:OFF, 1:ON)" );
			sw.WriteLine( "GuitarLeft={0}", this.bLeft.Guitar ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; ベースLEFTモード(0:OFF, 1:ON)" );
			sw.WriteLine( "BassLeft={0}", this.bLeft.Bass ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; RISKYモード(0:OFF, 1-10)" );									// #23559 2011.6.23 yyagi
			sw.WriteLine( "; RISKY mode. 0=OFF, 1-10 is the times of misses to be Failed." );	//
			sw.WriteLine( "Risky={0}", this.nRisky );			//
			sw.WriteLine();
			sw.WriteLine( "; ドラム判定文字表示位置(0:レーン上,1:判定ライン上,2:表示OFF)" );
			sw.WriteLine( "DrumsPosition={0}", (int) this.判定文字表示位置.Drums );
			sw.WriteLine();
			sw.WriteLine( "; ギター判定文字表示位置(0:レーン上,1:判定ライン横,2:表示OFF)" );
			sw.WriteLine( "GuitarPosition={0}", (int) this.判定文字表示位置.Guitar );
			sw.WriteLine();
			sw.WriteLine( "; ベース判定文字表示位置(0:レーン上,1:判定ライン横,2:表示OFF)" );
			sw.WriteLine( "BassPosition={0}", (int) this.判定文字表示位置.Bass );
			sw.WriteLine();
			sw.WriteLine( "; ドラム譜面スクロール速度(0:x0.5, 1:x1.0, 2:x1.5,…,1999:x1000.0)" );
			sw.WriteLine( "DrumsScrollSpeed={0}", this.n譜面スクロール速度.Drums );
			sw.WriteLine();
			sw.WriteLine( "; ギター譜面スクロール速度(0:x0.5, 1:x1.0, 2:x1.5,…,1999:x1000.0)" );
			sw.WriteLine( "GuitarScrollSpeed={0}", this.n譜面スクロール速度.Guitar );
			sw.WriteLine();
			sw.WriteLine( "; ベース譜面スクロール速度(0:x0.5, 1:x1.0, 2:x1.5,…,1999:x1000.0)" );
			sw.WriteLine( "BassScrollSpeed={0}", this.n譜面スクロール速度.Bass );
			sw.WriteLine();
			sw.WriteLine( "; 演奏速度(5～40)(→x5/20～x40/20)" );
			sw.WriteLine( "PlaySpeed={0}", this.n演奏速度 );
			sw.WriteLine();
			sw.WriteLine( "; ドラムCOMBO文字表示位置(0:左, 1:中, 2:右, 3:OFF)" );
			sw.WriteLine( "ComboPosition={0}", (int) this.ドラムコンボ文字の表示位置 );
			sw.WriteLine();

            // #24074 2011.01.23 add ikanick
			sw.WriteLine( "; ドラムグラフ表示(0:OFF, 1:ON)" );
			sw.WriteLine( "DrumsGraph={0}", this.bGraph.Drums ? 1 : 0 );
			sw.WriteLine();

			sw.WriteLine( ";-------------------" );
			sw.WriteLine( "[AutoPlay]" );
			sw.WriteLine();
			sw.WriteLine( "; 自動演奏(0:OFF, 1:ON)" );
			sw.WriteLine();
			sw.WriteLine( "; Drums" );
			sw.WriteLine( "LC={0}", this.bAutoPlay.LC ? 1 : 0 );
			sw.WriteLine( "HH={0}", this.bAutoPlay.HH ? 1 : 0 );
			sw.WriteLine( "SD={0}", this.bAutoPlay.SD ? 1 : 0 );
			sw.WriteLine( "BD={0}", this.bAutoPlay.BD ? 1 : 0 );
			sw.WriteLine( "HT={0}", this.bAutoPlay.HT ? 1 : 0 );
			sw.WriteLine( "LT={0}", this.bAutoPlay.LT ? 1 : 0 );
			sw.WriteLine( "FT={0}", this.bAutoPlay.FT ? 1 : 0 );
			sw.WriteLine( "CY={0}", this.bAutoPlay.CY ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; Guitar" );
			sw.WriteLine( "Guitar={0}", this.bAutoPlay.Guitar ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( "; Bass" );
			sw.WriteLine( "Bass={0}", this.bAutoPlay.Bass ? 1 : 0 );
			sw.WriteLine();
			sw.WriteLine( ";-------------------" );
			sw.WriteLine( "[HitRange]" );
			sw.WriteLine();
			sw.WriteLine( "; Perfect～Poor とみなされる範囲[ms]" );
			sw.WriteLine( "Perfect={0}", this.nヒット範囲ms.Perfect );
			sw.WriteLine( "Great={0}", this.nヒット範囲ms.Great );
			sw.WriteLine( "Good={0}", this.nヒット範囲ms.Good );
			sw.WriteLine( "Poor={0}", this.nヒット範囲ms.Poor );
			sw.WriteLine();
			sw.WriteLine( ";-------------------" );
			sw.WriteLine( "[GUID]" );
			sw.WriteLine();
			foreach( KeyValuePair<int, string> pair in this.dicJoystick )
			{
				sw.WriteLine( "JoystickID={0},{1}", pair.Key, pair.Value );
			}
			sw.WriteLine();
			sw.WriteLine( ";-------------------" );
			sw.WriteLine( "; キーアサイン" );
			sw.WriteLine( ";   項　目：Keyboard → 'K'＋'0'＋キーコード(10進数)" );
			sw.WriteLine( ";           Mouse    → 'N'＋'0'＋ボタン番号(0～7)" );
			sw.WriteLine( ";           MIDI In  → 'M'＋デバイス番号1桁(0～9,A～Z)＋ノート番号(10進数)" );
			sw.WriteLine( ";           Joystick → 'J'＋デバイス番号1桁(0～9,A～Z)＋ 0 ...... Ｘ減少(左)ボタン" );
			sw.WriteLine( ";                                                         1 ...... Ｘ増加(右)ボタン" );
			sw.WriteLine( ";                                                         2 ...... Ｙ減少(上)ボタン" );
			sw.WriteLine( ";                                                         3 ...... Ｙ増加(下)ボタン" );
			sw.WriteLine( ";                                                         4 ...... Ｚ減少(前)ボタン" );
			sw.WriteLine( ";                                                         5 ...... Ｚ増加(後)ボタン" );
			sw.WriteLine( ";                                                         6～133.. ボタン1～128" );
			sw.WriteLine( ";           これらの項目を 16 個まで指定可能(',' で区切って記述）。" );
			sw.WriteLine( ";" );
			sw.WriteLine( ";   表記例：HH=K044,M042,J16" );
			sw.WriteLine( ";           → HiHat を Keyboard の 44 ('Z'), MidiIn#0 の 42, JoyPad#1 の 6(ボタン1) に割当て" );
			sw.WriteLine( ";" );
			sw.WriteLine( ";   ※Joystick のデバイス番号とデバイスとの関係は [GUID] セクションに記してあるものが有効。" );
			sw.WriteLine( ";" );
			sw.WriteLine();
			sw.WriteLine( "[DrumsKeyAssign]" );
			sw.WriteLine();
			sw.Write( "HH=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.HH );
			sw.WriteLine();
			sw.Write( "SD=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.SD );
			sw.WriteLine();
			sw.Write( "BD=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.BD );
			sw.WriteLine();
			sw.Write( "HT=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.HT );
			sw.WriteLine();
			sw.Write( "LT=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.LT );
			sw.WriteLine();
			sw.Write( "FT=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.FT );
			sw.WriteLine();
			sw.Write( "CY=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.CY );
			sw.WriteLine();
			sw.Write( "HO=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.HHO );
			sw.WriteLine();
			sw.Write( "RD=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.RD );
			sw.WriteLine();
			sw.Write( "LC=" );
			this.tキーの書き出し( sw, this.KeyAssign.Drums.LC );
			sw.WriteLine();
			sw.WriteLine();
			sw.WriteLine( "[GuitarKeyAssign]" );
			sw.WriteLine();
			sw.Write( "R=" );
			this.tキーの書き出し( sw, this.KeyAssign.Guitar.R );
			sw.WriteLine();
			sw.Write( "G=" );
			this.tキーの書き出し( sw, this.KeyAssign.Guitar.G );
			sw.WriteLine();
			sw.Write( "B=" );
			this.tキーの書き出し( sw, this.KeyAssign.Guitar.B );
			sw.WriteLine();
			sw.Write( "Pick=" );
			this.tキーの書き出し( sw, this.KeyAssign.Guitar.Pick );
			sw.WriteLine();
			sw.Write( "Wail=" );
			this.tキーの書き出し( sw, this.KeyAssign.Guitar.Wail );
			sw.WriteLine();
			sw.Write( "Decide=" );
			this.tキーの書き出し( sw, this.KeyAssign.Guitar.Decide );
			sw.WriteLine();
			sw.Write( "Cancel=" );
			this.tキーの書き出し( sw, this.KeyAssign.Guitar.Cancel );
			sw.WriteLine();
			sw.WriteLine();
			sw.WriteLine( "[BassKeyAssign]" );
			sw.WriteLine();
			sw.Write( "R=" );
			this.tキーの書き出し( sw, this.KeyAssign.Bass.R );
			sw.WriteLine();
			sw.Write( "G=" );
			this.tキーの書き出し( sw, this.KeyAssign.Bass.G );
			sw.WriteLine();
			sw.Write( "B=" );
			this.tキーの書き出し( sw, this.KeyAssign.Bass.B );
			sw.WriteLine();
			sw.Write( "Pick=" );
			this.tキーの書き出し( sw, this.KeyAssign.Bass.Pick );
			sw.WriteLine();
			sw.Write( "Wail=" );
			this.tキーの書き出し( sw, this.KeyAssign.Bass.Wail );
			sw.WriteLine();
			sw.Write( "Decide=" );
			this.tキーの書き出し( sw, this.KeyAssign.Bass.Decide );
			sw.WriteLine();
			sw.Write( "Cancel=" );
			this.tキーの書き出し( sw, this.KeyAssign.Bass.Cancel );
			sw.WriteLine();
			sw.WriteLine();
			sw.WriteLine( "[SystemKeyAssign]" );
			sw.WriteLine();
			sw.Write( "Capture=" );
			this.tキーの書き出し( sw, this.KeyAssign.System.Capture );
			sw.WriteLine();
			sw.WriteLine();
			sw.Close();
		}
		public void tファイルから読み込み( string iniファイル名 )
		{
			this.ConfigIniファイル名 = iniファイル名;
			this.bConfigIniが存在している = File.Exists( this.ConfigIniファイル名 );
			if( this.bConfigIniが存在している )
			{
				string str;
				this.tキーアサインを全部クリアする();
				StreamReader reader = new StreamReader( this.ConfigIniファイル名, Encoding.GetEncoding( "shift-jis" ) );
				str = reader.ReadToEnd();
				t文字列から読み込み( str );
				CDTXVersion version = new CDTXVersion( this.strDTXManiaのバージョン );
				if( version.n整数部 <= 69 )
				{
					this.tデフォルトのキーアサインに設定する();
				}
			}
		}

		private void t文字列から読み込み( string strAllSettings )	// 2011.4.13 yyagi; refactored to make initial KeyConfig easier.
		{
			Eセクション種別 unknown = Eセクション種別.Unknown;
			string[] delimiter = { "\n" };
			string[] strSingleLine = strAllSettings.Split( delimiter, StringSplitOptions.RemoveEmptyEntries );
			foreach ( string s in strSingleLine )
			{
				string str = s.Replace( '\t', ' ' ).TrimStart( new char[] { '\t', ' ' } );
				if ( ( str.Length != 0 ) && ( str[ 0 ] != ';' ) )
				{
					try
					{
						string str3;
						string str4;
						if ( str[ 0 ] == '[' )
						{
							#region [ セクションの変更 ]
							//-----------------------------
							StringBuilder builder = new StringBuilder( 0x20 );
							int num = 1;
							while ( ( num < str.Length ) && ( str[ num ] != ']' ) )
							{
								builder.Append( str[ num++ ] );
							}
							string str2 = builder.ToString();
							if ( str2.Equals( "System" ) )
							{
								unknown = Eセクション種別.System;
							}
							else if ( str2.Equals( "Log" ) )
							{
								unknown = Eセクション種別.Log;
							}
							else if ( str2.Equals( "PlayOption" ) )
							{
								unknown = Eセクション種別.PlayOption;
							}
							else if ( str2.Equals( "AutoPlay" ) )
							{
								unknown = Eセクション種別.AutoPlay;
							}
							else if ( str2.Equals( "HitRange" ) )
							{
								unknown = Eセクション種別.HitRange;
							}
							else if ( str2.Equals( "GUID" ) )
							{
								unknown = Eセクション種別.GUID;
							}
							else if ( str2.Equals( "DrumsKeyAssign" ) )
							{
								unknown = Eセクション種別.DrumsKeyAssign;
							}
							else if ( str2.Equals( "GuitarKeyAssign" ) )
							{
								unknown = Eセクション種別.GuitarKeyAssign;
							}
							else if ( str2.Equals( "BassKeyAssign" ) )
							{
								unknown = Eセクション種別.BassKeyAssign;
							}
							else if ( str2.Equals( "SystemKeyAssign" ) )
							{
								unknown = Eセクション種別.SystemKeyAssign;
							}
							else
							{
								unknown = Eセクション種別.Unknown;
							}
							//-----------------------------
							#endregion
						}
						else
						{
							string[] strArray = str.Split( new char[] { '=' } );
							if ( strArray.Length == 2 )
							{
								str3 = strArray[ 0 ].Trim();
								str4 = strArray[ 1 ].Trim();
								switch ( unknown )
								{
									#region [ [System] ]
									//-----------------------------
									case Eセクション種別.System:
										{
#if false		// #23625 2011.1.11 Config.iniからダメージ/回復値の定数変更を行う場合はここを有効にする 087リリースに合わせ機能無効化
										//----------------------------------------
												if (str3.Equals("GaugeFactorD"))
												{
													int p = 0;
													string[] splittedFactor = str4.Split(',');
													foreach (string s in splittedFactor) {
														this.fGaugeFactor[p++, 0] = Convert.ToSingle(s);
													}
												} else
												if (str3.Equals("GaugeFactorG"))
												{
													int p = 0;
													string[] splittedFactor = str4.Split(',');
													foreach (string s in splittedFactor)
													{
														this.fGaugeFactor[p++, 1] = Convert.ToSingle(s);
													}
												}
												else
												if (str3.Equals("DamageFactor"))
												{
													int p = 0;
													string[] splittedFactor = str4.Split(',');
													foreach (string s in splittedFactor)
													{
														this.fDamageLevelFactor[p++] = Convert.ToSingle(s);
													}
												}
												else
										//----------------------------------------
#endif
											if ( str3.Equals( "Version" ) )
											{
												this.strDTXManiaのバージョン = str4;
											}
											else if ( str3.Equals( "DTXPath" ) )
											{
												this.str曲データ検索パス = str4;
											}
											else if ( str3.Equals( "FullScreen" ) )
											{
												this.b全画面モード = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "WindowWidth" ) )		// #23510 2010.10.31 yyagi add
											{
												this.nウインドウwidth = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 1, 65535, this.nウインドウwidth );
												if ( this.nウインドウwidth <= 0 )
												{
													this.nウインドウwidth = SampleFramework.GameWindowSize.Width;
												}
											}
											else if ( str3.Equals( "WindowHeight" ) )		// #23510 2010.10.31 yyagi add
											{
												this.nウインドウheight = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 1, 65535, this.nウインドウheight );
												if ( this.nウインドウheight <= 0 )
												{
													this.nウインドウheight = SampleFramework.GameWindowSize.Height;
												}
											}
											else if ( str3.Equals( "VSyncWait" ) )
											{
												this.b垂直帰線待ちを行う = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BackSleep" ) )		// #23568 2010.11.04 ikanick add
											{
												this.n非フォーカス時スリープms = C変換.n値を文字列から取得して範囲内にちゃんと丸めて返す( str4, 0, 50, this.n非フォーカス時スリープms );
											}
											else if ( str3.Equals( "Guitar" ) )
											{
												this.bGuitar有効 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "Drums" ) )
											{
												this.bDrums有効 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BGAlpha" ) )
											{
												this.n背景の透過度 = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0xff, this.n背景の透過度 );
											}
											else if ( str3.Equals( "DamageLevel" ) )
											{
												this.eダメージレベル = (Eダメージレベル) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 2, (int) this.eダメージレベル );
											}
											else if ( str3.Equals( "HHGroup" ) )
											{
												this.eHHGroup = (EHHGroup) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 3, (int) this.eHHGroup );
											}
											else if ( str3.Equals( "FTGroup" ) )
											{
												this.eFTGroup = (EFTGroup) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 2, (int) this.eFTGroup );
											}
											else if ( str3.Equals( "CYGroup" ) )
											{
												this.eCYGroup = (ECYGroup) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 2, (int) this.eCYGroup );
											}
											else if ( str3.Equals( "HitSoundPriorityHH" ) )
											{
												this.eHitSoundPriorityHH = (E打ち分け時の再生の優先順位) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 1, (int) this.eHitSoundPriorityHH );
											}
											else if ( str3.Equals( "HitSoundPriorityFT" ) )
											{
												this.eHitSoundPriorityFT = (E打ち分け時の再生の優先順位) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 1, (int) this.eHitSoundPriorityFT );
											}
											else if ( str3.Equals( "HitSoundPriorityCY" ) )
											{
												this.eHitSoundPriorityCY = (E打ち分け時の再生の優先順位) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 1, (int) this.eHitSoundPriorityCY );
											}
											else if ( str3.Equals( "StageFailed" ) )
											{
												this.bSTAGEFAILED有効 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "AVI" ) )
											{
												this.bAVI有効 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BGA" ) )
											{
												this.bBGA有効 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "FillInEffect" ) )
											{
												this.bフィルイン有効 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "PreviewSoundWait" ) )
											{
												this.n曲が選択されてからプレビュー音が鳴るまでのウェイトms = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x5f5e0ff, this.n曲が選択されてからプレビュー音が鳴るまでのウェイトms );
											}
											else if ( str3.Equals( "PreviewImageWait" ) )
											{
												this.n曲が選択されてからプレビュー画像が表示開始されるまでのウェイトms = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x5f5e0ff, this.n曲が選択されてからプレビュー画像が表示開始されるまでのウェイトms );
											}
											else if ( str3.Equals( "AdjustWaves" ) )
											{
												this.bWave再生位置自動調整機能有効 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BGMSound" ) )
											{
												this.bBGM音を発声する = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "HitSound" ) )
											{
												this.bドラム打音を発声する = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "AudienceSound" ) )
											{
												this.b歓声を発声する = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "SaveScoreIni" ) )
											{
												this.bScoreIniを出力する = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "RandomFromSubBox" ) )
											{
												this.bランダムセレクトで子BOXを検索対象とする = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "SoundMonitorDrums" ) )
											{
												this.b演奏音を強調する.Drums = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "SoundMonitorGuitar" ) )
											{
												this.b演奏音を強調する.Guitar = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "SoundMonitorBass" ) )
											{
												this.b演奏音を強調する.Bass = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "MinComboDrums" ) )
											{
												this.n表示可能な最小コンボ数.Drums = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 1, 0x1869f, this.n表示可能な最小コンボ数.Drums );
											}
											else if ( str3.Equals( "MinComboGuitar" ) )
											{
												this.n表示可能な最小コンボ数.Guitar = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 1, 0x1869f, this.n表示可能な最小コンボ数.Guitar );
											}
											else if ( str3.Equals( "MinComboBass" ) )
											{
												this.n表示可能な最小コンボ数.Bass = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 1, 0x1869f, this.n表示可能な最小コンボ数.Bass );
											}
											else if ( str3.Equals( "ShowDebugStatus" ) )
											{
												this.b演奏情報を表示する = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "SelectListFontName" ) )
											{
												this.str選曲リストフォント = str4;
											}
											else if ( str3.Equals( "SelectListFontSize" ) )
											{
												this.n選曲リストフォントのサイズdot = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 1, 0x3e7, this.n選曲リストフォントのサイズdot );
											}
											else if ( str3.Equals( "SelectListFontItalic" ) )
											{
												this.b選曲リストフォントを斜体にする = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "SelectListFontBold" ) )
											{
												this.b選曲リストフォントを太字にする = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "ChipVolume" ) )
											{
												this.n手動再生音量 = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 100, this.n手動再生音量 );
											}
											else if ( str3.Equals( "AutoChipVolume" ) )
											{
												this.n自動再生音量 = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 100, this.n自動再生音量 );
											}
											else if ( str3.Equals( "StoicMode" ) )
											{
												this.bストイックモード = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "CymbalFree" ) )
											{
												this.bシンバルフリー = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "ShowLagTime" ) )				// #25370 2011.6.3 yyagi
											{
												this.nShowLagType = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 2, this.nShowLagType );
											}
											else if ( str3.Equals( "AutoResultCapture" ) )			// #25399 2011.6.9 yyagi
											{
												this.bIsAutoResultCapture = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "InputAdjustTimeDrums" ) )		// #23580 2011.1.3 yyagi
											{
												this.nInputAdjustTimeMs.Drums = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, -99, 0, this.nInputAdjustTimeMs.Drums );
											}
											else if ( str3.Equals( "InputAdjustTimeGuitar" ) )	// #23580 2011.1.3 yyagi
											{
												this.nInputAdjustTimeMs.Guitar = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, -99, 0, this.nInputAdjustTimeMs.Guitar );
											}
											else if ( str3.Equals( "InputAdjustTimeBass" ) )		// #23580 2011.1.3 yyagi
											{
												this.nInputAdjustTimeMs.Bass = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, -99, 0, this.nInputAdjustTimeMs.Bass );
											}
											else if ( str3.Equals( "BufferedInput" ) )
											{
												this.bバッファ入力を行う = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "LCVelocityMin" ) )			// #23857 2010.12.12 yyagi
											{
												this.nVelocityMin.LC = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.LC );
											}
											else if ( str3.Equals( "HHVelocityMin" ) )
											{
												this.nVelocityMin.HH = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.HH );
											}
											else if ( str3.Equals( "SDVelocityMin" ) )			// #23857 2011.1.31 yyagi
											{
												this.nVelocityMin.SD = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.SD );
											}
											else if ( str3.Equals( "BDVelocityMin" ) )			// #23857 2011.1.31 yyagi
											{
												this.nVelocityMin.BD = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.BD );
											}
											else if ( str3.Equals( "HTVelocityMin" ) )			// #23857 2011.1.31 yyagi
											{
												this.nVelocityMin.HT = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.HT );
											}
											else if ( str3.Equals( "LTVelocityMin" ) )			// #23857 2011.1.31 yyagi
											{
												this.nVelocityMin.LT = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.LT );
											}
											else if ( str3.Equals( "FTVelocityMin" ) )			// #23857 2011.1.31 yyagi
											{
												this.nVelocityMin.FT = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.FT );
											}
											else if ( str3.Equals( "CYVelocityMin" ) )			// #23857 2011.1.31 yyagi
											{
												this.nVelocityMin.CY = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.CY );
											}
											else if ( str3.Equals( "RDVelocityMin" ) )			// #23857 2011.1.31 yyagi
											{
												this.nVelocityMin.RD = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 127, this.nVelocityMin.RD );
											}
											continue;
										}
									//-----------------------------
									#endregion

									#region [ [Log] ]
									//-----------------------------
									case Eセクション種別.Log:
										{
											if ( str3.Equals( "OutputLog" ) )
											{
												this.bログ出力 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "TraceCreatedDisposed" ) )
											{
												this.bLog作成解放ログ出力 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "TraceDTXDetails" ) )
											{
												this.bLogDTX詳細ログ出力 = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "TraceSongSearch" ) )
											{
												this.bLog曲検索ログ出力 = C変換.bONorOFF( str4[ 0 ] );
											}
											continue;
										}
									//-----------------------------
									#endregion

									#region [ [PlayOption] ]
									//-----------------------------
									case Eセクション種別.PlayOption:
										{
											if ( str3.Equals( "Dark" ) )
											{
												this.eDark = (Eダークモード) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 2, (int) this.eDark );
											}
											else if ( str3.Equals( "DrumsTight" ) )
											{
												this.bTight = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "DrumsGraph" ) )  // #24074 2011.01.23 addikanick
											{
												this.bGraph.Drums = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "DrumsSudden" ) )
											{
												this.bSudden.Drums = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "GuitarSudden" ) )
											{
												this.bSudden.Guitar = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BassSudden" ) )
											{
												this.bSudden.Bass = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "DrumsHidden" ) )
											{
												this.bHidden.Drums = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "GuitarHidden" ) )
											{
												this.bHidden.Guitar = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BassHidden" ) )
											{
												this.bHidden.Bass = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "DrumsReverse" ) )
											{
												this.bReverse.Drums = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "GuitarReverse" ) )
											{
												this.bReverse.Guitar = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BassReverse" ) )
											{
												this.bReverse.Bass = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "GuitarRandom" ) )
											{
												this.eRandom.Guitar = (Eランダムモード) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 3, (int) this.eRandom.Guitar );
											}
											else if ( str3.Equals( "BassRandom" ) )
											{
												this.eRandom.Bass = (Eランダムモード) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 3, (int) this.eRandom.Bass );
											}
											else if ( str3.Equals( "GuitarLight" ) )
											{
												this.bLight.Guitar = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BassLight" ) )
											{
												this.bLight.Bass = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "GuitarLeft" ) )
											{
												this.bLeft.Guitar = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BassLeft" ) )
											{
												this.bLeft.Bass = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "DrumsPosition" ) )
											{
												this.判定文字表示位置.Drums = (E判定文字表示位置) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 2, (int) this.判定文字表示位置.Drums );
											}
											else if ( str3.Equals( "GuitarPosition" ) )
											{
												this.判定文字表示位置.Guitar = (E判定文字表示位置) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 2, (int) this.判定文字表示位置.Guitar );
											}
											else if ( str3.Equals( "BassPosition" ) )
											{
												this.判定文字表示位置.Bass = (E判定文字表示位置) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 2, (int) this.判定文字表示位置.Bass );
											}
											else if ( str3.Equals( "DrumsScrollSpeed" ) )
											{
												this.n譜面スクロール速度.Drums = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x7cf, this.n譜面スクロール速度.Drums );
											}
											else if ( str3.Equals( "GuitarScrollSpeed" ) )
											{
												this.n譜面スクロール速度.Guitar = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x7cf, this.n譜面スクロール速度.Guitar );
											}
											else if ( str3.Equals( "BassScrollSpeed" ) )
											{
												this.n譜面スクロール速度.Bass = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x7cf, this.n譜面スクロール速度.Bass );
											}
											else if ( str3.Equals( "PlaySpeed" ) )
											{
												this.n演奏速度 = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 5, 40, this.n演奏速度 );
											}
											else if ( str3.Equals( "ComboPosition" ) )
											{
												this.ドラムコンボ文字の表示位置 = (Eドラムコンボ文字の表示位置) C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 3, (int) this.ドラムコンボ文字の表示位置 );
											}
											else if ( str3.Equals( "Risky" ) )					// #2359 2011.6.23  yyagi
											{
												this.nRisky = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 10, this.nRisky );
											}

											continue;
										}
									//-----------------------------
									#endregion

									#region [ [AutoPlay] ]
									//-----------------------------
									case Eセクション種別.AutoPlay:
										{
											if ( str3.Equals( "LC" ) )
											{
												this.bAutoPlay.LC = C変換.bONorOFF( str4[ 0 ] );
											}
											if ( str3.Equals( "HH" ) )
											{
												this.bAutoPlay.HH = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "SD" ) )
											{
												this.bAutoPlay.SD = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "BD" ) )
											{
												this.bAutoPlay.BD = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "HT" ) )
											{
												this.bAutoPlay.HT = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "LT" ) )
											{
												this.bAutoPlay.LT = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "FT" ) )
											{
												this.bAutoPlay.FT = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "CY" ) )
											{
												this.bAutoPlay.CY = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "Guitar" ) )
											{
												this.bAutoPlay.Guitar = C変換.bONorOFF( str4[ 0 ] );
											}
											else if ( str3.Equals( "Bass" ) )
											{
												this.bAutoPlay.Bass = C変換.bONorOFF( str4[ 0 ] );
											}
											continue;
										}
									//-----------------------------
									#endregion

									#region [ [HitRange] ]
									//-----------------------------
									case Eセクション種別.HitRange:
										{
											if ( str3.Equals( "Perfect" ) )
											{
												this.nヒット範囲ms.Perfect = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x3e7, this.nヒット範囲ms.Perfect );
											}
											else if ( str3.Equals( "Great" ) )
											{
												this.nヒット範囲ms.Great = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x3e7, this.nヒット範囲ms.Great );
											}
											else if ( str3.Equals( "Good" ) )
											{
												this.nヒット範囲ms.Good = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x3e7, this.nヒット範囲ms.Good );
											}
											else if ( str3.Equals( "Poor" ) )
											{
												this.nヒット範囲ms.Poor = C変換.n値を文字列から取得して範囲内に丸めて返す( str4, 0, 0x3e7, this.nヒット範囲ms.Poor );
											}
											continue;
										}
									//-----------------------------
									#endregion

									#region [ [GUID] ]
									//-----------------------------
									case Eセクション種別.GUID:
										{
											if ( str3.Equals( "JoystickID" ) )
											{
												this.tJoystickIDの取得( str4 );
											}
											continue;
										}
									//-----------------------------
									#endregion

									#region [ [DrumsKeyAssign] ]
									//-----------------------------
									case Eセクション種別.DrumsKeyAssign:
										{
											if ( str3.Equals( "HH" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.HH );
											}
											else if ( str3.Equals( "SD" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.SD );
											}
											else if ( str3.Equals( "BD" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.BD );
											}
											else if ( str3.Equals( "HT" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.HT );
											}
											else if ( str3.Equals( "LT" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.LT );
											}
											else if ( str3.Equals( "FT" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.FT );
											}
											else if ( str3.Equals( "CY" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.CY );
											}
											else if ( str3.Equals( "HO" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.HHO );
											}
											else if ( str3.Equals( "RD" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.RD );
											}
											else if ( str3.Equals( "LC" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Drums.LC );
											}
											continue;
										}
									//-----------------------------
									#endregion

									#region [ [GuitarKeyAssign] ]
									//-----------------------------
									case Eセクション種別.GuitarKeyAssign:
										{
											if ( str3.Equals( "R" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Guitar.R );
											}
											else if ( str3.Equals( "G" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Guitar.G );
											}
											else if ( str3.Equals( "B" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Guitar.B );
											}
											else if ( str3.Equals( "Pick" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Guitar.Pick );
											}
											else if ( str3.Equals( "Wail" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Guitar.Wail );
											}
											else if ( str3.Equals( "Decide" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Guitar.Decide );
											}
											else if ( str3.Equals( "Cancel" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Guitar.Cancel );
											}
											continue;
										}
									//-----------------------------
									#endregion

									#region [ [BassKeyAssign] ]
									//-----------------------------
									case Eセクション種別.BassKeyAssign:
										{
											if ( str3.Equals( "R" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Bass.R );
											}
											else if ( str3.Equals( "G" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Bass.G );
											}
											else if ( str3.Equals( "B" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Bass.B );
											}
											else if ( str3.Equals( "Pick" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Bass.Pick );
											}
											else if ( str3.Equals( "Wail" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Bass.Wail );
											}
											else if ( str3.Equals( "Decide" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Bass.Decide );
											}
											else if ( str3.Equals( "Cancel" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.Bass.Cancel );
											}
											continue;
										}
									//-----------------------------
									#endregion

									#region [ [SystemKeyAssign] ]
									//-----------------------------
									case Eセクション種別.SystemKeyAssign:
										{
											if ( str3.Equals( "Capture" ) )
											{
												this.tキーの読み出しと設定( str4, this.KeyAssign.System.Capture );
											}
											continue;
										}
									//-----------------------------
									#endregion
								}
							}
						}
						continue;
					}
					catch ( Exception exception )
					{
						Trace.TraceError( exception.Message );
						continue;
					}
				}
			}
		}

		/// <summary>
		/// ギターとベースのキーアサイン入れ替え
		/// </summary>
		public void SwapGuitarBassKeyAssign()		// #24063 2011.1.16 yyagi
		{
			for ( int j = 0; j <= (int)EKeyConfigPad.Capture; j++ )
			{
				CKeyAssign.STKEYASSIGN t; //= new CConfigIni.CKeyAssign.STKEYASSIGN();
				for ( int k = 0; k < 16; k++ )
				{
					t = this.KeyAssign[ (int)EKeyConfigPart.GUITAR ][ j ][ k ];
					this.KeyAssign[ (int)EKeyConfigPart.GUITAR ][ j ][ k ] = this.KeyAssign[ (int)EKeyConfigPart.BASS ][ j ][ k ];
					this.KeyAssign[ (int)EKeyConfigPart.BASS ][ j ][ k ] = t;
				}
			}
			this.bIsSwappedGuitarBass = !bIsSwappedGuitarBass;
		}


		// その他

		#region [ private ]
		//-----------------
		private enum Eセクション種別
		{
			Unknown,
			System,
			Log,
			PlayOption,
			AutoPlay,
			HitRange,
			GUID,
			DrumsKeyAssign,
			GuitarKeyAssign,
			BassKeyAssign,
			SystemKeyAssign
		}

		private bool _bDrums有効;
		private bool _bGuitar有効;
		private bool bConfigIniが存在している;
		private string ConfigIniファイル名;

		private void tJoystickIDの取得( string strキー記述 )
		{
			string[] strArray = strキー記述.Split( new char[] { ',' } );
			if( strArray.Length >= 2 )
			{
				int result = 0;
				if( ( int.TryParse( strArray[ 0 ], out result ) && ( result >= 0 ) ) && ( result <= 9 ) )
				{
					if( this.dicJoystick.ContainsKey( result ) )
					{
						this.dicJoystick.Remove( result );
					}
					this.dicJoystick.Add( result, strArray[ 1 ] );
				}
			}
		}
		private void tキーアサインを全部クリアする()
		{
			this.KeyAssign = new CKeyAssign();
			for( int i = 0; i <= (int)EKeyConfigPart.SYSTEM; i++ )
			{
				for( int j = 0; j <= (int)EKeyConfigPad.Capture; j++ )
				{
					this.KeyAssign[ i ][ j ] = new CKeyAssign.STKEYASSIGN[ 0x10 ];
					for( int k = 0; k < 0x10; k++ )
					{
						this.KeyAssign[ i ][ j ][ k ] = new CKeyAssign.STKEYASSIGN( E入力デバイス.不明, 0, 0 );
					}
				}
			}
		}
		private void tキーの書き出し( StreamWriter sw, CKeyAssign.STKEYASSIGN[] assign )
		{
			bool flag = true;
			for( int i = 0; i < 0x10; i++ )
			{
				if( assign[ i ].入力デバイス == E入力デバイス.不明 )
				{
					continue;
				}
				if( !flag )
				{
					sw.Write( ',' );
				}
				flag = false;
				switch( assign[ i ].入力デバイス )
				{
					case E入力デバイス.キーボード:
						sw.Write( 'K' );
						break;

					case E入力デバイス.MIDI入力:
						sw.Write( 'M' );
						break;

					case E入力デバイス.ジョイパッド:
						sw.Write( 'J' );
						break;

					case E入力デバイス.マウス:
						sw.Write( 'N' );
						break;
				}
				sw.Write( "{0}{1}", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ".Substring( assign[ i ].ID, 1 ), assign[ i ].コード );	// #24166 2011.1.15 yyagi: to support ID > 10, change 2nd character from Decimal to 36-numeral system. (e.g. J1023 -> JA23)
			}
		}
		private void tキーの読み出しと設定( string strキー記述, CKeyAssign.STKEYASSIGN[] assign )
		{
			string[] strArray = strキー記述.Split( new char[] { ',' } );
			for( int i = 0; ( i < strArray.Length ) && ( i < 0x10 ); i++ )
			{
				E入力デバイス e入力デバイス;
				int id;
				int code;
				string str = strArray[ i ].Trim().ToUpper();
				if ( str.Length >= 3 )
				{
					e入力デバイス = E入力デバイス.不明;
					switch ( str[ 0 ] )
					{
						case 'J':
							e入力デバイス = E入力デバイス.ジョイパッド;
							break;

						case 'K':
							e入力デバイス = E入力デバイス.キーボード;
							break;

						case 'L':
							continue;

						case 'M':
							e入力デバイス = E入力デバイス.MIDI入力;
							break;

						case 'N':
							e入力デバイス = E入力デバイス.マウス;
							break;
					}
				}
				else
				{
					continue;
				}
				id = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ".IndexOf( str[ 1 ] );	// #24166 2011.1.15 yyagi: to support ID > 10, change 2nd character from Decimal to 36-numeral system. (e.g. J1023 -> JA23)
				if( ( ( id >= 0 ) && int.TryParse( str.Substring( 2 ), out code ) ) && ( ( code >= 0 ) && ( code <= 0xff ) ) )
				{
					this.t指定した入力が既にアサイン済みである場合はそれを全削除する( e入力デバイス, id, code );
					assign[ i ].入力デバイス = e入力デバイス;
					assign[ i ].ID = id;
					assign[ i ].コード = code;
				}
			}
		}
		private void tデフォルトのキーアサインに設定する()
		{
			this.tキーアサインを全部クリアする();

			string strDefaultKeyAssign = @"
[DrumsKeyAssign]

HH=K035,M042,M093
SD=K033,M025,M026,M027,M028,M029,M031,M032,M034,M037,M038,M040,M0113
BD=K012,K0126,M033,M035,M036,M0112
HT=K031,M048,M050
LT=K011,M047
FT=K023,M041,M043,M045
CY=K022,M049,M052,M055,M057,M091
HO=K010,M046,M092
RD=K020,M051,M053,M059,M089
LC=K026

[GuitarKeyAssign]

R=K055
G=K056,J012
B=K057
Pick=K0115,K046,J06
Wail=K0116
Decide=K060
Cancel=K061

[BassKeyAssign]

R=K090
G=K091,J013
B=K092
Pick=K0103,K0100,J08
Wail=K089
Decide=K096
Cancel=K097

[SystemKeyAssign]
Capture=K065
";
			t文字列から読み込み( strDefaultKeyAssign );
		}
		//-----------------
		#endregion
	}
}
