﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Diagnostics;
using SlimDX.Direct3D9;
using FDK;

namespace DTXMania
{
	internal class CStage演奏ギター画面 : CStage演奏画面共通
	{
		// コンストラクタ

		public CStage演奏ギター画面()
		{
			base.eステージID = CStage.Eステージ.演奏;
			base.eフェーズID = CStage.Eフェーズ.共通_通常状態;
			base.b活性化してない = true;
			base.list子Activities.Add( this.actStageFailed = new CAct演奏ステージ失敗() );
			base.list子Activities.Add( this.actDANGER = new CAct演奏GuitarDanger() );
			base.list子Activities.Add( this.actAVI = new CAct演奏AVI() );
			base.list子Activities.Add( this.actBGA = new CAct演奏BGA() );
			base.list子Activities.Add( this.actPanel = new CAct演奏パネル文字列() );
			base.list子Activities.Add( this.act譜面スクロール速度 = new CAct演奏スクロール速度() );
			base.list子Activities.Add( this.actStatusPanels = new CAct演奏Guitarステータスパネル() );
			base.list子Activities.Add( this.actWailingBonus = new CAct演奏GuitarWailingBonus() );
			base.list子Activities.Add( this.actScore = new CAct演奏Guitarスコア() );
			base.list子Activities.Add( this.actRGB = new CAct演奏GuitarRGB() );
			base.list子Activities.Add( this.actLaneFlushGB = new CAct演奏GuitarレーンフラッシュGB() );
			base.list子Activities.Add( this.actJudgeString = new CAct演奏Guitar判定文字列() );
			base.list子Activities.Add( this.actGauge = new CAct演奏Guitarゲージ() );
			base.list子Activities.Add( this.actCombo = new CAct演奏Guitarコンボ() );
			base.list子Activities.Add( this.actChipFireGB = new CAct演奏Guitarチップファイア() );
			base.list子Activities.Add( this.actPlayInfo = new CAct演奏演奏情報() );
			base.list子Activities.Add( this.actFI = new CActFIFOBlack() );
			base.list子Activities.Add( this.actFO = new CActFIFOBlack() );
			base.list子Activities.Add( this.actFOClear = new CActFIFOWhite() );
		}


		// メソッド

		public void t演奏結果を格納する( out CScoreIni.C演奏記録 Drums, out CScoreIni.C演奏記録 Guitar, out CScoreIni.C演奏記録 Bass )
		{
			Drums = new CScoreIni.C演奏記録();

			base.t演奏結果を格納する・ギター( out Guitar );
			base.t演奏結果を格納する・ベース( out Bass );

//			if ( CDTXMania.ConfigIni.bIsSwappedGuitarBass )		// #24063 2011.1.24 yyagi Gt/Bsを入れ替えていたなら、演奏結果も入れ替える
//			{
//				CScoreIni.C演奏記録 t;
//				t = Guitar;
//				Guitar = Bass;
//				Bass = t;
//			
//				CDTXMania.DTX.SwapGuitarBassInfos();			// 譜面情報も元に戻す
//			}
		}
		

		// CStage 実装

		public override void On活性化()
		{
			base.On活性化();
		}
		public override void OnManagedリソースの作成()
		{
			if( !base.b活性化してない )
			{
				//this.t背景テクスチャの生成();
				this.txチップ = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenPlayGuitar chips.png" ) );
				this.txヒットバー = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenPlayGuitar hit-bar.png" ) );
				//this.txWailing枠 = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenPlay wailing cursor.png" ) );
				base.OnManagedリソースの作成();
			}
		}
		public override void OnManagedリソースの解放()
		{
			if( !base.b活性化してない )
			{
				//CDTXMania.tテクスチャの解放( ref this.tx背景 );
				CDTXMania.tテクスチャの解放( ref this.txチップ );
				CDTXMania.tテクスチャの解放( ref this.txヒットバー );
				//CDTXMania.tテクスチャの解放( ref this.txWailing枠 );
				base.OnManagedリソースの解放();
			}
		}
		public override int On進行描画()
		{
			if( !base.b活性化してない )
			{
				bool flag = false;
				bool flag2 = false;

				if( base.b初めての進行描画 )
				{
					CDTXMania.Timer.tリセット();
					this.ctチップ模様アニメ.Guitar = new CCounter( 0, 0x17, 20, CDTXMania.Timer );
					this.ctチップ模様アニメ.Bass = new CCounter( 0, 0x17, 20, CDTXMania.Timer );
					this.ctチップ模様アニメ[ 0 ] = null;
					this.ctWailingチップ模様アニメ = new CCounter( 0, 4, 50, CDTXMania.Timer );
					base.eフェーズID = CStage.Eフェーズ.共通_フェードイン;
					this.actFI.tフェードイン開始();
					base.b初めての進行描画 = false;
				}
				if( CDTXMania.ConfigIni.bSTAGEFAILED有効 && ( base.eフェーズID == CStage.Eフェーズ.共通_通常状態 ) )
				{
//					bool flag3 = ( CDTXMania.ConfigIni.bAutoPlay.Guitar || !CDTXMania.DTX.bチップがある.Guitar ) || ( this.actGauge.db現在のゲージ値.Guitar <= -0.1 );				// #23630
//					bool flag4 = ( CDTXMania.ConfigIni.bAutoPlay.Bass || !CDTXMania.DTX.bチップがある.Bass ) || ( this.actGauge.db現在のゲージ値.Bass <= -0.1 );					// #23630
					bool bFailedGuitar = ( this.actGauge.db現在のゲージ値.Guitar <= -0.1 );		// #23630 2011.11.12 yyagi: deleted AutoPlay condition: not to be failed at once
					bool bFailedBass   = (this.actGauge.db現在のゲージ値.Bass   <= -0.1);		// #23630
					bool bFailedNoChips = (!CDTXMania.DTX.bチップがある.Guitar && !CDTXMania.DTX.bチップがある.Bass);	// #25216 2011.5.21 yyagi add condition
					if ( bFailedGuitar || bFailedBass || bFailedNoChips )						// #25216 2011.5.21 yyagi: changed codition: && -> ||
					{
						this.actStageFailed.Start();
						CDTXMania.DTX.t全チップの再生停止();
						base.eフェーズID = CStage.Eフェーズ.演奏_STAGE_FAILED;
					}
				}
				this.t進行描画・背景();
				this.t進行描画・MIDIBGM();
				this.t進行描画・パネル文字列();
				this.t進行描画・スコア();
				this.t進行描画・AVI();
				this.t進行描画・BGA();
				this.t進行描画・ステータスパネル();
				this.t進行描画・レーンフラッシュGB();
				this.t進行描画・ギターベース判定ライン();
				this.t進行描画・ゲージ();
				this.t進行描画・DANGER();
				this.t進行描画・RGBボタン();
				this.t進行描画・判定文字列();
				this.t進行描画・コンボ();
				this.t進行描画・WailingBonus();
				this.t進行描画・譜面スクロール速度();
				this.t進行描画・チップアニメ();
				flag = this.t進行描画・チップ(E楽器パート.GUITAR);
				this.t進行描画・演奏情報();
				this.t進行描画・Wailing枠();
				this.t進行描画・チップファイアGB();
				this.t進行描画・STAGEFAILED();
				flag2 = this.t進行描画・フェードイン・アウト();
				if( flag && ( base.eフェーズID == CStage.Eフェーズ.共通_通常状態 ) )
				{
					this.eフェードアウト完了時の戻り値 = E演奏画面の戻り値.ステージクリア;
					base.eフェーズID = CStage.Eフェーズ.演奏_STAGE_CLEAR_フェードアウト;
					this.actFOClear.tフェードアウト開始();
				}
				if( flag2 )
				{
					return (int) this.eフェードアウト完了時の戻り値;
				}

				// キー入力

				if( CDTXMania.act現在入力を占有中のプラグイン == null )
				{
					this.tキー入力();
				}
			}
			return 0;
		}


		// その他

		#region [ private ]
		//-----------------

		protected override E判定 tチップのヒット処理( long nHitTime, CDTX.CChip pChip )
		{
			E判定 eJudgeResult = tチップのヒット処理( nHitTime, pChip, E楽器パート.GUITAR );
			return eJudgeResult;
		}
		protected override void tチップのヒット処理・BadならびにTight時のMiss( E楽器パート part )
		{
			this.tチップのヒット処理・BadならびにTight時のMiss( part, 0, E楽器パート.GUITAR );
		}
		protected override void tチップのヒット処理・BadならびにTight時のMiss( E楽器パート part, int nLane )
		{
			this.tチップのヒット処理・BadならびにTight時のMiss( part, nLane, E楽器パート.GUITAR );
		}

		protected override void t進行描画・AVI()
		{
		    base.t進行描画・AVI( 0xb5, 50 );
		}
		protected override void t進行描画・BGA()
		{
		    base.t進行描画・BGA( 0xb5, 50 );
		}
		protected override void t進行描画・DANGER()			// #23631 2011.4.19 yyagi
		{
			this.actDANGER.t進行描画( false, this.actGauge.db現在のゲージ値.Guitar < 0.3, this.actGauge.db現在のゲージ値.Bass < 0.3 );
		}

		protected override void t進行描画・Wailing枠()
		{
			base.t進行描画・Wailing枠( 0x8b, 0x251,
				CDTXMania.ConfigIni.bReverse.Guitar ? 340 : 11,
				CDTXMania.ConfigIni.bReverse.Bass ?   340 : 11
			);
		}
		private void t進行描画・ギターベース判定ライン()	// yyagi: ドラム画面とは座標が違うだけですが、まとめづらかったのでそのまま放置してます。
		{
			if ( ( CDTXMania.ConfigIni.eDark != Eダークモード.FULL ) && CDTXMania.ConfigIni.bGuitar有効 )
			{
				if ( CDTXMania.DTX.bチップがある.Guitar )
				{
					int y = ( CDTXMania.ConfigIni.bReverse.Guitar ? 0x171 : 40 ) - 3;
					for ( int i = 0; i < 4; i++ )
					{
						if ( this.txヒットバー != null )
						{
							this.txヒットバー.t2D描画( CDTXMania.app.Device, 0x17 + ( 0x1c * i ), y, new Rectangle( 0, i * 8, 0x1c, 8 ) );
						}
					}
				}
				if ( CDTXMania.DTX.bチップがある.Bass )
				{
					int y = ( CDTXMania.ConfigIni.bReverse.Bass ? 0x171 : 40 ) - 3;
					for ( int j = 0; j < 4; j++ )
					{
						if ( this.txヒットバー != null )
						{
							this.txヒットバー.t2D描画( CDTXMania.app.Device, 0x1dd + ( 0x1c * j ), y, new Rectangle( 0, j * 8, 0x1c, 8 ) );
						}
					}
				}
			}
		}

		protected override void t進行描画・パネル文字列()
		{
			base.t進行描画・パネル文字列( 0xb5, 430 );
		}

		protected override void t進行描画・演奏情報()
		{
			base.t進行描画・演奏情報( 0xb5, 50 );
		}

		protected override void ドラムスクロール速度アップ()
		{
			// ギタレボモードでは何もしない
		}
		protected override void ドラムスクロール速度ダウン()
		{
			// ギタレボモードでは何もしない
		}

		protected override void t入力処理・ドラム()
		{
			// ギタレボモードでは何もしない
		}

		protected override void t背景テクスチャの生成()
		{
			Rectangle bgrect = new Rectangle( 181, 50, 278, 355 );
			string DefaultBgFilename = @"Graphics\ScreenPlayGuitar background.jpg";
			string BgFilename = "";
			string BACKGROUND = null;
			if ( ( CDTXMania.DTX.BACKGROUND_GR != null ) && ( CDTXMania.DTX.BACKGROUND_GR.Length > 0 ) )
			{
				BACKGROUND = CDTXMania.DTX.BACKGROUND_GR;
			}
			else if ( ( CDTXMania.DTX.BACKGROUND != null ) && ( CDTXMania.DTX.BACKGROUND.Length > 0 ) )
			{
				BACKGROUND = CDTXMania.DTX.BACKGROUND;
			}
			if ( ( BACKGROUND != null ) && ( BACKGROUND.Length > 0 ) )
			{
				BgFilename = CDTXMania.DTX.strフォルダ名 + BACKGROUND;
			}
			base.t背景テクスチャの生成( DefaultBgFilename, bgrect, BgFilename );
		}

		protected override void t進行描画・チップ・ドラムス( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip )
		{
			// int indexSevenLanes = this.nチャンネル0Atoレーン07[ pChip.nチャンネル番号 - 0x11 ];
			if ( !pChip.bHit && ( pChip.nバーからの距離dot.Drums < 0 ) )
			{
				pChip.bHit = true;
				this.tサウンド再生( pChip, CDTXMania.Timer.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, E楽器パート.DRUMS, dTX.nモニタを考慮した音量( E楽器パート.DRUMS ) );
			}
		}
		protected override void t進行描画・チップ・ギター( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip )
		{
			if ( configIni.bGuitar有効 )
			{
				if ( configIni.bSudden.Guitar )
				{
					pChip.b可視 = pChip.nバーからの距離dot.Guitar < 200;
				}
				if ( configIni.bHidden.Guitar && ( pChip.nバーからの距離dot.Guitar < 100 ) )
				{
					pChip.b可視 = false;
				}
				if ( !pChip.bHit && pChip.b可視 )
				{
					int nバーからの距離 = configIni.bReverse.Guitar ? ( 0x171 - pChip.nバーからの距離dot.Guitar ) : ( 40 + pChip.nバーからの距離dot.Guitar );
					if ( ( nバーからの距離 > 0 ) && ( nバーからの距離 < 0x199 ) )
					{
						int nアニメカウンタ現在の値 = this.ctチップ模様アニメ.Guitar.n現在の値;
						if ( pChip.nチャンネル番号 == 0x20 )
						{
							if ( this.txチップ != null )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x1a, nバーからの距離 - 4, new Rectangle( 0, 0xc0 + ( ( nアニメカウンタ現在の値 % 5 ) * 8 ), 0x67, 8 ) );
							}
						}
						else if ( !configIni.bLeft.Guitar )
						{
							Rectangle rectangle = new Rectangle( 0, nアニメカウンタ現在の値 * 8, 0x20, 8 );
							if ( ( ( pChip.nチャンネル番号 & 4 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x1a, nバーからの距離 - 4, rectangle );
							}
							rectangle.X += 0x20;
							if ( ( ( pChip.nチャンネル番号 & 2 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x3e, nバーからの距離 - 4, rectangle );
							}
							rectangle.X += 0x20;
							if ( ( ( pChip.nチャンネル番号 & 1 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x62, nバーからの距離 - 4, rectangle );
							}
						}
						else
						{
							Rectangle rectangle2 = new Rectangle( 0, nアニメカウンタ現在の値 * 8, 0x20, 8 );
							if ( ( ( pChip.nチャンネル番号 & 4 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x62, nバーからの距離 - 4, rectangle2 );
							}
							rectangle2.X += 0x20;
							if ( ( ( pChip.nチャンネル番号 & 2 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x3e, nバーからの距離 - 4, rectangle2 );
							}
							rectangle2.X += 0x20;
							if ( ( ( pChip.nチャンネル番号 & 1 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x1a, nバーからの距離 - 4, rectangle2 );
							}
						}
					}
				}
				if ( ( configIni.bAutoPlay.Guitar && !pChip.bHit ) && ( pChip.nバーからの距離dot.Guitar < 0 ) )
				{
					pChip.bHit = true;
					if ( ( ( pChip.nチャンネル番号 & 4 ) != 0 ) || ( pChip.nチャンネル番号 == 0x20 ) )
					{
						this.actChipFireGB.Start( 0 );
					}
					if ( ( ( pChip.nチャンネル番号 & 2 ) != 0 ) || ( pChip.nチャンネル番号 == 0x20 ) )
					{
						this.actChipFireGB.Start( 1 );
					}
					if ( ( ( pChip.nチャンネル番号 & 1 ) != 0 ) || ( pChip.nチャンネル番号 == 0x20 ) )
					{
						this.actChipFireGB.Start( 2 );
					}
					this.tサウンド再生( pChip, CDTXMania.Timer.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, E楽器パート.GUITAR, dTX.nモニタを考慮した音量( E楽器パート.GUITAR ) );
					this.r次にくるギターChip = null;
					this.tチップのヒット処理( pChip.n発声時刻ms, pChip );
				}
				// break;
				return;
			}
			if ( !pChip.bHit && ( pChip.nバーからの距離dot.Guitar < 0 ) )
			{
				pChip.bHit = true;
				this.tサウンド再生( pChip, CDTXMania.Timer.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, E楽器パート.GUITAR, dTX.nモニタを考慮した音量( E楽器パート.GUITAR ) );
			}

		}
		protected override void t進行描画・チップ・ギター・ウェイリング( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip )
		{
			if ( configIni.bGuitar有効 )
			{
				//if ( configIni.bSudden.Guitar )
				//{
				//    pChip.b可視 = pChip.nバーからの距離dot.Guitar < 200;
				//}
				//if ( configIni.bHidden.Guitar && ( pChip.nバーからの距離dot.Guitar < 100 ) )
				//{
				//    pChip.b可視 = false;
				//}

				//
				// 後日、以下の部分を何とかCStage演奏画面共通.csに移したい。
				//
				if ( !pChip.bHit && pChip.b可視 )
				{
					int num10 = 0x19;
					int num11 = configIni.bReverse.Guitar ? ( 0x171 - pChip.nバーからの距離dot.Guitar ) : ( 40 + pChip.nバーからの距離dot.Guitar );
					int num12 = num11;
					int num13 = 0;
					if ( ( num12 < ( 0x199 + num10 ) ) && ( num12 > -num10 ) )
					{
						int num14 = this.ctWailingチップ模様アニメ.n現在の値;
						Rectangle rectangle5 = new Rectangle( 0x60 + ( num14 * 20 ), 0, 20, 50 );
						if ( num12 < num10 )
						{
							rectangle5.Y += num10 - num12;
							rectangle5.Height -= num10 - num12;
							num13 = num10 - num12;
						}
						if ( num12 > ( 0x199 - num10 ) )
						{
							rectangle5.Height -= num12 - ( 0x199 - num10 );
						}
						if ( ( rectangle5.Bottom > rectangle5.Top ) && ( this.txチップ != null ) )
						{
							this.txチップ.t2D描画( CDTXMania.app.Device, 0x8b, ( num11 - num10 ) + num13, rectangle5 );
						}
					}
				}
				//    if( !pChip.bHit && ( pChip.nバーからの距離dot.Guitar < 0 ) )
				//    {
				//        pChip.bHit = true;
				//        if( configIni.bAutoPlay.Guitar )
				//        {
				//            this.actWailingBonus.Start( E楽器パート.GUITAR, this.r現在の歓声Chip.Guitar );
				//        }
				//    }
				//    return;
				//}
				//pChip.bHit = true;
			}
			base.t進行描画・チップ・ギター・ウェイリング( configIni, ref dTX, ref pChip );
		}
		protected override void t進行描画・チップ・フィルイン( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip )
		{
			if ( !pChip.bHit && ( pChip.nバーからの距離dot.Drums < 0 ) )
			{
				pChip.bHit = true;
			}
#if TEST_NOTEOFFMODE	// 2011.1.1 yyagi TEST
			switch ( pChip.n整数値 )
			{
				case 0x04:	// HH消音あり(従来同等)
					CDTXMania.DTX.bHH演奏で直前のHHを消音する = true;
					break;
				case 0x05:	// HH消音無し
					CDTXMania.DTX.bHH演奏で直前のHHを消音する = false;
					break;
				case 0x06:	// ギター消音あり(従来同等)
					CDTXMania.DTX.bGUITAR演奏で直前のGUITARを消音する = true;
					break;
				case 0x07:	// ギター消音無し
					CDTXMania.DTX.bGUITAR演奏で直前のGUITARを消音する = false;
					break;
				case 0x08:	// ベース消音あり(従来同等)
					CDTXMania.DTX.bBASS演奏で直前のBASSを消音する = true;
					break;
				case 0x09:	// ベース消音無し
					CDTXMania.DTX.bBASS演奏で直前のBASSを消音する = false;
					break;
			}
#endif

		}
		protected override void t進行描画・チップ・ベース( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip )
		{
			if ( configIni.bGuitar有効 )
			{
				if ( configIni.bSudden.Bass )
				{
					pChip.b可視 = pChip.nバーからの距離dot.Bass < 200;
				}
				if ( configIni.bHidden.Bass && ( pChip.nバーからの距離dot.Bass < 100 ) )
				{
					pChip.b可視 = false;
				}
				if ( !pChip.bHit && pChip.b可視 )
				{
					int num8 = configIni.bReverse.Bass ? ( 0x171 - pChip.nバーからの距離dot.Bass ) : ( 40 + pChip.nバーからの距離dot.Bass );
					if ( ( num8 > 0 ) && ( num8 < 0x199 ) )
					{
						int num9 = this.ctチップ模様アニメ.Bass.n現在の値;
						if ( pChip.nチャンネル番号 == 160 )
						{
							if ( this.txチップ != null )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 480, num8 - 4, new Rectangle( 0, 0xc0 + ( ( num9 % 5 ) * 8 ), 0x67, 8 ) );
							}
						}
						else if ( !configIni.bLeft.Bass )
						{
							Rectangle rectangle3 = new Rectangle( 0, num9 * 8, 0x20, 8 );
							if ( ( ( pChip.nチャンネル番号 & 4 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 480, num8 - 4, rectangle3 );
							}
							rectangle3.X += 0x20;
							if ( ( ( pChip.nチャンネル番号 & 2 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x204, num8 - 4, rectangle3 );
							}
							rectangle3.X += 0x20;
							if ( ( ( pChip.nチャンネル番号 & 1 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x228, num8 - 4, rectangle3 );
							}
						}
						else
						{
							Rectangle rectangle4 = new Rectangle( 0, num9 * 8, 0x20, 8 );
							if ( ( ( pChip.nチャンネル番号 & 4 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x228, num8 - 4, rectangle4 );
							}
							rectangle4.X += 0x20;
							if ( ( ( pChip.nチャンネル番号 & 2 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 0x204, num8 - 4, rectangle4 );
							}
							rectangle4.X += 0x20;
							if ( ( ( pChip.nチャンネル番号 & 1 ) != 0 ) && ( this.txチップ != null ) )
							{
								this.txチップ.t2D描画( CDTXMania.app.Device, 480, num8 - 4, rectangle4 );
							}
						}
					}
				}
				if ( ( configIni.bAutoPlay.Bass && !pChip.bHit ) && ( pChip.nバーからの距離dot.Bass < 0 ) )
				{
					pChip.bHit = true;
					if ( ( ( pChip.nチャンネル番号 & 4 ) != 0 ) || ( pChip.nチャンネル番号 == 160 ) )
					{
						this.actChipFireGB.Start( 3 );
					}
					if ( ( ( pChip.nチャンネル番号 & 2 ) != 0 ) || ( pChip.nチャンネル番号 == 160 ) )
					{
						this.actChipFireGB.Start( 4 );
					}
					if ( ( ( pChip.nチャンネル番号 & 1 ) != 0 ) || ( pChip.nチャンネル番号 == 160 ) )
					{
						this.actChipFireGB.Start( 5 );
					}
					this.tサウンド再生( pChip, CDTXMania.Timer.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, E楽器パート.BASS, dTX.nモニタを考慮した音量( E楽器パート.BASS ) );
					this.r次にくるベースChip = null;
					this.tチップのヒット処理( pChip.n発声時刻ms, pChip );
				}
				return;
			}
			if ( !pChip.bHit && ( pChip.nバーからの距離dot.Bass < 0 ) )
			{
				pChip.bHit = true;
				this.tサウンド再生( pChip, CDTXMania.Timer.n前回リセットした時のシステム時刻 + pChip.n発声時刻ms, E楽器パート.BASS, dTX.nモニタを考慮した音量( E楽器パート.BASS ) );
			}
		}
		protected override void t進行描画・チップ・ベース・ウェイリング( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip )
		{
			if ( configIni.bGuitar有効 )
			{
				//if ( configIni.bSudden.Bass )
				//{
				//    pChip.b可視 = pChip.nバーからの距離dot.Bass < 200;
				//}
				//if ( configIni.bHidden.Bass && ( pChip.nバーからの距離dot.Bass < 100 ) )
				//{
				//    pChip.b可視 = false;
				//}

				//
				// 後日、以下の部分を何とかCStage演奏画面共通.csに移したい。
				//
				if ( !pChip.bHit && pChip.b可視 )
				{
					int[] y_base = { 40, 0x171 };

					const int num15 = 0x19;
					int y = configIni.bReverse.Bass ? ( y_base[1] - pChip.nバーからの距離dot.Bass ) : ( y_base[0] + pChip.nバーからの距離dot.Bass );
					int num17 = y;
					int num18 = 0;
					if ( ( num17 < ( 0x199 + num15 ) ) && ( num17 > -num15 ) )
					{
						int c = this.ctWailingチップ模様アニメ.n現在の値;
						Rectangle rectangle6 = new Rectangle( 0x60 + ( c * 20 ), 0, 20, 50 );
						if ( num17 < num15 )
						{
							rectangle6.Y += num15 - num17;
							rectangle6.Height -= num15 - num17;
							num18 = num15 - num17;
						}
						if ( num17 > ( 0x199 - num15 ) )
						{
							rectangle6.Height -= num17 - ( 0x199 - num15 );
						}
						if ( ( rectangle6.Bottom > rectangle6.Top ) && ( this.txチップ != null ) )
						{
							this.txチップ.t2D描画( CDTXMania.app.Device, 0x251, ( y - num15 ) + num18, rectangle6 );
						}
					}
				}
				//    if ( !pChip.bHit && ( pChip.nバーからの距離dot.Bass < 0 ) )
				//    {
				//        pChip.bHit = true;
				//        if ( configIni.bAutoPlay.Bass )
				//        {
				//            this.actWailingBonus.Start( E楽器パート.BASS, this.r現在の歓声Chip.Bass );
				//        }
				//    }
				//    return;
				//}
				//pChip.bHit = true;
				base.t進行描画・チップ・ベース・ウェイリング( configIni, ref dTX, ref pChip );
			}
		}
		protected override void t進行描画・チップ・空打ち音設定・ドラム( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip )
		{
			if ( !pChip.bHit && ( pChip.nバーからの距離dot.Drums < 0 ) )
			{
				pChip.bHit = true;
			}
		}
		protected override void t進行描画・チップ・小節線( CConfigIni configIni, ref CDTX dTX, ref CDTX.CChip pChip )
		{
			int n小節番号plus1 = pChip.n発声位置 / 0x180;
			if ( !pChip.bHit && ( pChip.nバーからの距離dot.Drums < 0 ) )
			{
				pChip.bHit = true;
				this.actPlayInfo.n小節番号 = n小節番号plus1 - 1;
				if ( configIni.bWave再生位置自動調整機能有効 )
				{
					dTX.tWave再生位置自動補正();
				}
			}
			if ( ( pChip.b可視 && configIni.bGuitar有効 ) && ( configIni.eDark != Eダークモード.FULL ) )
			{
				int y = configIni.bReverse.Guitar ? ( ( 0x171 - pChip.nバーからの距離dot.Guitar ) - 1 ) : ( ( 40 + pChip.nバーからの距離dot.Guitar ) - 1 );
				if ( ( dTX.bチップがある.Guitar && ( y > 0 ) ) && ( ( y < 0x199 ) && ( this.txチップ != null ) ) )
				{
					this.txチップ.t2D描画( CDTXMania.app.Device, 0x1a, y, new Rectangle( 0, 0xeb, 0x68, 1 ) );
				}
				y = configIni.bReverse.Bass ? ( ( 0x171 - pChip.nバーからの距離dot.Bass ) - 1 ) : ( ( 40 + pChip.nバーからの距離dot.Bass ) - 1 );
				if ( ( dTX.bチップがある.Bass && ( y > 0 ) ) && ( ( y < 0x199 ) && ( this.txチップ != null ) ) )
				{
					this.txチップ.t2D描画( CDTXMania.app.Device, 480, y, new Rectangle( 0, 0xeb, 0x68, 1 ) );
				}
			}

		}

		#endregion
	}
}
