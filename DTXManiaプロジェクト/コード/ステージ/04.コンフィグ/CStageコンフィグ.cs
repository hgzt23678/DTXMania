﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Diagnostics;
using FDK;

namespace DTXMania
{
	internal class CStageコンフィグ : CStage
	{
		// プロパティ

		public CActDFPFont actFont { get; private set; }


		// コンストラクタ

		public CStageコンフィグ()
		{
			CActDFPFont font;
			base.eステージID = CStage.Eステージ.コンフィグ;
			base.eフェーズID = CStage.Eフェーズ.共通_通常状態;
			this.actFont = font = new CActDFPFont();
			base.list子Activities.Add( font );
			base.list子Activities.Add( this.actFIFO = new CActFIFOWhite() );
			base.list子Activities.Add( this.actList = new CActConfigList() );
			base.list子Activities.Add( this.actKeyAssign = new CActConfigKeyAssign() );
			base.list子Activities.Add( this.actオプションパネル = new CActオプションパネル() );
			base.b活性化してない = true;
		}
		
		
		// メソッド

		public void tアサイン完了通知()
		{
			this.eItemPanelモード = EItemPanelモード.パッド一覧;
		}
		public void tパッド選択通知( E楽器パート part, Eパッド pad )
		{
			this.actKeyAssign.t開始( part, pad, this.actList.ib現在の選択項目.str項目名 );
			this.eItemPanelモード = EItemPanelモード.キーコード一覧;
		}
		public void t項目変更通知()
		{
			this.t説明文パネルに現在選択されている項目の説明を描画する();
		}

		
		// CStage 実装

		public override void On活性化()
		{
			Trace.TraceInformation( "コンフィグステージを活性化します。" );
			Trace.Indent();
			try
			{
				this.n現在のメニュー番号 = 0;
				this.ftフォント = new Font( "MS PGothic", 26f, GraphicsUnit.Pixel );
				for( int i = 0; i < 4; i++ )
				{
					this.ctキー反復用[ i ] = new CCounter( 0, 0, 0, CDTXMania.Timer );
				}
				this.bメニューにフォーカス中 = true;
				this.eItemPanelモード = EItemPanelモード.パッド一覧;
				base.On活性化();
			}
			finally
			{
				Trace.TraceInformation( "コンフィグステージの活性化を完了しました。" );
				Trace.Unindent();
			}
		}
		public override void On非活性化()
		{
			Trace.TraceInformation( "コンフィグステージを非活性化します。" );
			Trace.Indent();
			try
			{
				CDTXMania.ConfigIni.t書き出し( CDTXMania.strEXEのあるフォルダ + "Config.ini" );
				if( this.ftフォント != null )
				{
					this.ftフォント.Dispose();
					this.ftフォント = null;
				}
				for( int i = 0; i < 4; i++ )
				{
					this.ctキー反復用[ i ] = null;
				}
				base.On非活性化();
			}
			finally
			{
				Trace.TraceInformation( "コンフィグステージの非活性化を完了しました。" );
				Trace.Unindent();
			}
		}
		public override void OnManagedリソースの作成()
		{
			if( !base.b活性化してない )
			{
				this.tx背景 = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenConfig background.jpg" ), false );
				this.tx上部パネル = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenConfig header panel.png" ), true );
				this.tx下部パネル = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenConfig footer panel.png" ), true );
				this.txMenuカーソル = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenConfig menu cursor.png" ), false );
				if( this.bメニューにフォーカス中 )
				{
					this.t説明文パネルに現在選択されているメニューの説明を描画する();
				}
				else
				{
					this.t説明文パネルに現在選択されている項目の説明を描画する();
				}
				base.OnManagedリソースの作成();
			}
		}
		public override void OnManagedリソースの解放()
		{
			if( !base.b活性化してない )
			{
				CDTXMania.tテクスチャの解放( ref this.tx背景 );
				CDTXMania.tテクスチャの解放( ref this.tx上部パネル );
				CDTXMania.tテクスチャの解放( ref this.tx下部パネル );
				CDTXMania.tテクスチャの解放( ref this.txMenuカーソル );
				CDTXMania.tテクスチャの解放( ref this.tx説明文パネル );
				base.OnManagedリソースの解放();
			}
		}
		public override int On進行描画()
		{
			if( base.b活性化してない )
				return 0;

			if( base.b初めての進行描画 )
			{
				base.eフェーズID = CStage.Eフェーズ.共通_フェードイン;
				this.actFIFO.tフェードイン開始();
				base.b初めての進行描画 = false;
			}

			// 描画

			#region [ 背景 ]
			//---------------------
			if( this.tx背景 != null )
				this.tx背景.t2D描画( CDTXMania.app.Device, 0, 0 );
			//---------------------
			#endregion
			#region [ メニューカーソル ]
			//---------------------
			if( this.txMenuカーソル != null )
			{
				Rectangle rectangle;
				this.txMenuカーソル.n透明度 = this.bメニューにフォーカス中 ? 0xff : 0x80;
				int x = 0x37;
				int y = 0x61 + ( this.n現在のメニュー番号 * 0x19 );
				int num3 = 170;
				this.txMenuカーソル.t2D描画( CDTXMania.app.Device, x, y, new Rectangle( 0, 0, 0x10, 0x20 ) );
				this.txMenuカーソル.t2D描画( CDTXMania.app.Device, ( x + num3 ) - 0x10, y, new Rectangle( 10, 0, 0x10, 0x20 ) );
				x += 0x10;
				for( num3 -= 0x20; num3 > 0; num3 -= rectangle.Width )
				{
					rectangle = new Rectangle( 8, 0, 0x10, 0x20 );
					if( num3 < 0x10 )
					{
						rectangle.Width -= 0x10 - num3;
					}
					this.txMenuカーソル.t2D描画( CDTXMania.app.Device, x, y, rectangle );
					x += rectangle.Width;
				}
			}
			//---------------------
			#endregion
			#region [ メニュー ]
			//---------------------
			string str = "System";
			int num4 = this.actFont.n文字列長dot( str );
			bool flag = this.n現在のメニュー番号 == 0;
			this.actFont.t文字列描画( 0x8a - ( num4 / 2 ), 100, str, flag );
			str = "Drums Keys";
			num4 = this.actFont.n文字列長dot( str );
			flag = this.n現在のメニュー番号 == 1;
			this.actFont.t文字列描画( 0x8a - ( num4 / 2 ), 0x7d, str, flag );
			str = "Guitar Keys";
			num4 = this.actFont.n文字列長dot( str );
			flag = this.n現在のメニュー番号 == 2;
			this.actFont.t文字列描画( 0x8a - ( num4 / 2 ), 150, str, flag );
			str = "Bass Keys";
			num4 = this.actFont.n文字列長dot( str );
			flag = this.n現在のメニュー番号 == 3;
			this.actFont.t文字列描画( 0x8a - ( num4 / 2 ), 0xaf, str, flag );
			str = "Exit";
			num4 = this.actFont.n文字列長dot( str );
			flag = this.n現在のメニュー番号 == 4;
			this.actFont.t文字列描画( 0x8a - ( num4 / 2 ), 200, str, flag );
			//---------------------
			#endregion
			#region [ 説明文パネル ]
			//---------------------
			if( this.tx説明文パネル != null )
				this.tx説明文パネル.t2D描画( CDTXMania.app.Device, 0x1d, 0xf8 );
			//---------------------
			#endregion
			#region [ アイテム ]
			//---------------------
			switch( this.eItemPanelモード )
			{
				case EItemPanelモード.パッド一覧:
					this.actList.t進行描画( !this.bメニューにフォーカス中 );
					break;

				case EItemPanelモード.キーコード一覧:
					this.actKeyAssign.On進行描画();
					break;
			}
			//---------------------
			#endregion
			#region [ 上部パネル ]
			//---------------------
			if( this.tx上部パネル != null )
				this.tx上部パネル.t2D描画( CDTXMania.app.Device, 0, 0 );
			//---------------------
			#endregion
			#region [ 下部パネル ]
			//---------------------
			if( this.tx下部パネル != null )
				this.tx下部パネル.t2D描画( CDTXMania.app.Device, 0, 480 - this.tx下部パネル.szテクスチャサイズ.Height );
			//---------------------
			#endregion
			#region [ オプションパネル ]
			//---------------------
			this.actオプションパネル.On進行描画();
			//---------------------
			#endregion
			#region [ フェードイン・アウト ]
			//---------------------
			switch( base.eフェーズID )
			{
				case CStage.Eフェーズ.共通_フェードイン:
					if( this.actFIFO.On進行描画() != 0 )
					{
						CDTXMania.Skin.bgmコンフィグ画面.t再生する();
						base.eフェーズID = CStage.Eフェーズ.共通_通常状態;
					}
					break;

				case CStage.Eフェーズ.共通_フェードアウト:
					if( this.actFIFO.On進行描画() == 0 )
					{
						break;
					}
					return 1;
			}
			//---------------------
			#endregion

			// キー入力

			if( ( base.eフェーズID != CStage.Eフェーズ.共通_通常状態 )
				|| this.actKeyAssign.bキー入力待ちの最中である
				|| CDTXMania.act現在入力を占有中のプラグイン != null )
				return 0;

			if( ( CDTXMania.Input管理.Keyboard.bキーが押された( 0x35 ) || CDTXMania.Pad.b押された( E楽器パート.DRUMS, Eパッド.FT ) ) || CDTXMania.Pad.b押されたGB( Eパッド.FT ) )
			{
				CDTXMania.Skin.sound取消音.t再生する();
				if( !this.bメニューにフォーカス中 )
				{
					if( this.eItemPanelモード == EItemPanelモード.キーコード一覧 )
					{
						CDTXMania.stageコンフィグ.tアサイン完了通知();
						return 0;
					}
					this.bメニューにフォーカス中 = true;
					this.t説明文パネルに現在選択されているメニューの説明を描画する();
				}
				else
				{
					this.actFIFO.tフェードアウト開始();
					base.eフェーズID = CStage.Eフェーズ.共通_フェードアウト;
				}
			}
			else if( ( CDTXMania.Pad.b押されたDGB( Eパッド.CY ) || CDTXMania.Pad.b押された( E楽器パート.DRUMS, Eパッド.RD ) ) || ( CDTXMania.Pad.b押された( E楽器パート.DRUMS, Eパッド.LC ) || ( CDTXMania.ConfigIni.bEnterがキー割り当てのどこにも使用されていない && CDTXMania.Input管理.Keyboard.bキーが押された( (int)SlimDX.DirectInput.Key.Return) ) ) )
			{
				if( this.n現在のメニュー番号 == 4 )
				{
					CDTXMania.Skin.sound決定音.t再生する();
					this.actFIFO.tフェードアウト開始();
					base.eフェーズID = CStage.Eフェーズ.共通_フェードアウト;
				}
				else if( this.bメニューにフォーカス中 )
				{
					CDTXMania.Skin.sound決定音.t再生する();
					this.bメニューにフォーカス中 = false;
					this.t説明文パネルに現在選択されている項目の説明を描画する();
				}
				else
				{
					switch( this.eItemPanelモード )
					{
						case EItemPanelモード.パッド一覧:
							this.actList.tEnter押下();
							if( this.actList.b現在選択されている項目はReturnToMenuである )
							{
								this.bメニューにフォーカス中 = true;
								this.t説明文パネルに現在選択されているメニューの説明を描画する();
							}
							break;	// goto Label_0505;

						case EItemPanelモード.キーコード一覧:
							this.actKeyAssign.tEnter押下();
							break;	// goto Label_0505;
					}
				}
			}
//		Label_0505:
			this.ctキー反復用.Up.tキー反復( CDTXMania.Input管理.Keyboard.bキーが押されている( (int) SlimDX.DirectInput.Key.UpArrow ), new CCounter.DGキー処理( this.tカーソルを上へ移動する ) );
			this.ctキー反復用.R.tキー反復( CDTXMania.Pad.b押されているGB( Eパッド.HH ), new CCounter.DGキー処理( this.tカーソルを上へ移動する ) );
			if( CDTXMania.Pad.b押された( E楽器パート.DRUMS, Eパッド.SD ) )
			{
				this.tカーソルを上へ移動する();
			}
			this.ctキー反復用.Down.tキー反復( CDTXMania.Input管理.Keyboard.bキーが押されている( (int) SlimDX.DirectInput.Key.DownArrow ), new CCounter.DGキー処理( this.tカーソルを下へ移動する ) );
			this.ctキー反復用.B.tキー反復( CDTXMania.Pad.b押されているGB( Eパッド.BD ), new CCounter.DGキー処理( this.tカーソルを下へ移動する ) );
			if( CDTXMania.Pad.b押された( E楽器パート.DRUMS, Eパッド.LT ) )
			{
				this.tカーソルを下へ移動する();
			}
			return 0;
		}


		// その他

		#region [ private ]
		//-----------------
		private enum EItemPanelモード
		{
			パッド一覧,
			キーコード一覧
		}

		[StructLayout( LayoutKind.Sequential )]
		private struct STキー反復用カウンタ
		{
			public CCounter Up;
			public CCounter Down;
			public CCounter R;
			public CCounter B;
			public CCounter this[ int index ]
			{
				get
				{
					switch( index )
					{
						case 0:
							return this.Up;

						case 1:
							return this.Down;

						case 2:
							return this.R;

						case 3:
							return this.B;
					}
					throw new IndexOutOfRangeException();
				}
				set
				{
					switch( index )
					{
						case 0:
							this.Up = value;
							return;

						case 1:
							this.Down = value;
							return;

						case 2:
							this.R = value;
							return;

						case 3:
							this.B = value;
							return;
					}
					throw new IndexOutOfRangeException();
				}
			}
		}

		private CActFIFOWhite actFIFO;
		private CActConfigKeyAssign actKeyAssign;
		private CActConfigList actList;
		private CActオプションパネル actオプションパネル;
		private bool bメニューにフォーカス中;
		private STキー反復用カウンタ ctキー反復用;
		private const int DESC_H = 0x80;
		private const int DESC_W = 220;
		private EItemPanelモード eItemPanelモード;
		private Font ftフォント;
		private int n現在のメニュー番号;
		private CTexture txMenuカーソル;
		private CTexture tx下部パネル;
		private CTexture tx上部パネル;
		private CTexture tx説明文パネル;
		private CTexture tx背景;

		private void tカーソルを下へ移動する()
		{
			if( !this.bメニューにフォーカス中 )
			{
				switch( this.eItemPanelモード )
				{
					case EItemPanelモード.パッド一覧:
						this.actList.t次に移動();
						return;

					case EItemPanelモード.キーコード一覧:
						this.actKeyAssign.t次に移動();
						return;
				}
			}
			else
			{
				CDTXMania.Skin.soundカーソル移動音.t再生する();
				this.n現在のメニュー番号 = ( this.n現在のメニュー番号 + 1 ) % 5;
				switch( this.n現在のメニュー番号 )
				{
					case 0:
						this.actList.t項目リストの設定・System();
						break;

					case 1:
						this.actList.t項目リストの設定・KeyAssignDrums();
						break;

					case 2:
						this.actList.t項目リストの設定・KeyAssignGuitar();
						break;

					case 3:
						this.actList.t項目リストの設定・KeyAssignBass();
						break;

					case 4:
						this.actList.t項目リストの設定・Exit();
						break;
				}
				this.t説明文パネルに現在選択されているメニューの説明を描画する();
			}
		}
		private void tカーソルを上へ移動する()
		{
			if( !this.bメニューにフォーカス中 )
			{
				switch( this.eItemPanelモード )
				{
					case EItemPanelモード.パッド一覧:
						this.actList.t前に移動();
						return;

					case EItemPanelモード.キーコード一覧:
						this.actKeyAssign.t前に移動();
						return;
				}
			}
			else
			{
				CDTXMania.Skin.soundカーソル移動音.t再生する();
				this.n現在のメニュー番号 = ( ( this.n現在のメニュー番号 - 1 ) + 5 ) % 5;
				switch( this.n現在のメニュー番号 )
				{
					case 0:
						this.actList.t項目リストの設定・System();
						break;

					case 1:
						this.actList.t項目リストの設定・KeyAssignDrums();
						break;

					case 2:
						this.actList.t項目リストの設定・KeyAssignGuitar();
						break;

					case 3:
						this.actList.t項目リストの設定・KeyAssignBass();
						break;

					case 4:
						this.actList.t項目リストの設定・Exit();
						break;
				}
				this.t説明文パネルに現在選択されているメニューの説明を描画する();
			}
		}
		private void t説明文パネルに現在選択されているメニューの説明を描画する()
		{
			try
			{
				Bitmap image = new Bitmap( 440, 0x100 );
				Graphics graphics = Graphics.FromImage( image );
				string[,] str = new string[2, 2];
				switch( this.n現在のメニュー番号 )
				{
					case 0:
						str[0, 0] = "システムに関係する項目を設定しま";
						str[0, 1] = "す。";
						str[1, 0] = "Settings for an overall systems.";
						break;

					case 1:
						str[0, 0] = "ドラムのキー入力に関する項目を設";
						str[0, 1] = "定します。";
						str[1, 0] = "Settings for the drums key/pad inputs.";
						str[1, 1] = "";
						break;

					case 2:
						str[0, 0] = "ギターのキー入力に関する項目を設";
						str[0, 1] = "定します。";
						str[1, 0] = "Settings for the guitar key/pad inputs.";
						str[1, 1] = "";
						break;

					case 3:
						str[0, 0] = "ベースのキー入力に関する項目を設";
						str[0, 1] = "定します。";
						str[1, 0] = "Settings for the bass key/pad inputs.";
						str[1, 1] = "";
						break;

					case 4:
						str[0, 0] = "設定を保存し、コンフィグ画面を終了";
						str[0, 1] = "します。";
						str[1, 0] = "Save the settings and exit from";
						str[1, 1] = "CONFIGURATION menu.";
						break;
				}
				int c = (CultureInfo.CurrentCulture.TwoLetterISOLanguageName == "ja") ? 0 : 1;
				for (int i = 0; i < 2; i++)
				{
					graphics.DrawString(str[c, i], this.ftフォント, Brushes.White, new PointF(4f, (i * 30)));
				}
				graphics.Dispose();
				if( this.tx説明文パネル != null )
				{
					this.tx説明文パネル.Dispose();
				}
				this.tx説明文パネル = new CTexture( CDTXMania.app.Device, image, CDTXMania.TextureFormat );
				this.tx説明文パネル.vc拡大縮小倍率.X = 0.5f;
				this.tx説明文パネル.vc拡大縮小倍率.Y = 0.5f;
				image.Dispose();
			}
			catch( CTextureCreateFailedException )
			{
				Trace.TraceError( "説明文テクスチャの作成に失敗しました。" );
				this.tx説明文パネル = null;
			}
		}
		private void t説明文パネルに現在選択されている項目の説明を描画する()
		{
			try
			{
				Bitmap image = new Bitmap( 440, 0x100 );
				Graphics graphics = Graphics.FromImage( image );
				CItemBase base2 = this.actList.ib現在の選択項目;
				if( ( base2.str説明文 != null ) && ( base2.str説明文.Length > 0 ) )
				{
					int num = 0;
					foreach( string str in base2.str説明文.Split( new char[] { '\n' } ) )
					{
						graphics.DrawString( str, this.ftフォント, Brushes.White, new PointF( 4f, (float) num ) );
						num += 30;
					}
				}
				graphics.Dispose();
				if( this.tx説明文パネル != null )
				{
					this.tx説明文パネル.Dispose();
				}
				this.tx説明文パネル = new CTexture( CDTXMania.app.Device, image, CDTXMania.TextureFormat );
				this.tx説明文パネル.vc拡大縮小倍率.X = 0.5f;
				this.tx説明文パネル.vc拡大縮小倍率.Y = 0.5f;
				image.Dispose();
			}
			catch( CTextureCreateFailedException )
			{
				Trace.TraceError( "説明文パネルテクスチャの作成に失敗しました。" );
				this.tx説明文パネル = null;
			}
		}
		//-----------------
		#endregion
	}
}
