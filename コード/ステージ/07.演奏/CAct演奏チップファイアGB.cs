﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using SlimDX;
using FDK;

namespace DTXMania
{
	internal class CAct演奏チップファイアGB : CActivity
	{
		// コンストラクタ

		public CAct演奏チップファイアGB()
		{
			base.b活性化してない = true;
		}


		// メソッド

		public virtual void Start( int nLane, int n中央X, int n中央Y )
		{
			if( ( nLane >= 0 ) || ( nLane <= 5 ) )
			{
				this.pt中央位置[ nLane ].X = n中央X;
				this.pt中央位置[ nLane ].Y = n中央Y;
				this.ct進行[ nLane ].t開始( 0, 0x38, 4, CDTXMania.Timer );
			}
		}


		// CActivity 実装

		public override void On活性化()
		{
			for( int i = 0; i < 6; i++ )
			{
				this.pt中央位置[ i ] = new Point( 0, 0 );
				this.ct進行[ i ] = new CCounter();
			}
			base.On活性化();
		}
		public override void On非活性化()
		{
			for( int i = 0; i < 6; i++ )
			{
				this.ct進行[ i ] = null;
			}
			base.On非活性化();
		}
		public override void OnManagedリソースの作成()
		{
			if( !base.b活性化してない )
			{
				this.tx火花[ 0 ] = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenPlay chip fire red.png" ) );
				if( this.tx火花[ 0 ] != null )
				{
					this.tx火花[ 0 ].b加算合成 = true;
				}
				this.tx火花[ 1 ] = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenPlay chip fire green.png" ) );
				if( this.tx火花[ 1 ] != null )
				{
					this.tx火花[ 1 ].b加算合成 = true;
				}
				this.tx火花[ 2 ] = CDTXMania.tテクスチャの生成( CSkin.Path( @"Graphics\ScreenPlay chip fire blue.png" ) );
				if( this.tx火花[ 2 ] != null )
				{
					this.tx火花[ 2 ].b加算合成 = true;
				}
				base.OnManagedリソースの作成();
			}
		}
		public override void OnManagedリソースの解放()
		{
			if( !base.b活性化してない )
			{
				CDTXMania.tテクスチャの解放( ref this.tx火花[ 0 ] );
				CDTXMania.tテクスチャの解放( ref this.tx火花[ 1 ] );
				CDTXMania.tテクスチャの解放( ref this.tx火花[ 2 ] );
				base.OnManagedリソースの解放();
			}
		}
		public override int On進行描画()
		{
			if( !base.b活性化してない )
			{
				for( int i = 0; i < 6; i++ )
				{
					this.ct進行[ i ].t進行();
					if( this.ct進行[ i ].b終了値に達した )
					{
						this.ct進行[ i ].t停止();
					}
				}
				for( int j = 0; j < 6; j++ )
				{
					if( ( this.ct進行[ j ].n現在の経過時間ms != -1 ) && ( this.tx火花[ j % 3 ] != null ) )
					{
						float x = (float) ( 3.0 * Math.Cos( ( 3.1415926535897931 * ( 90.0 - ( 90.0 * ( ( (double) this.ct進行[ j ].n現在の値 ) / 56.0 ) ) ) ) / 180.0 ) );
						int num4 = this.pt中央位置[ j ].X - ( (int) ( ( this.tx火花[ j % 3 ].sz画像サイズ.Width * x ) / 2f ) );
						int y = this.pt中央位置[ j ].Y - ( (int) ( ( this.tx火花[ j % 3 ].sz画像サイズ.Height * x ) / 2f ) );
						this.tx火花[ j % 3 ].n透明度 = ( this.ct進行[ j ].n現在の値 < 0x1c ) ? 0xff : ( 0xff - ( (int) ( 255.0 * Math.Cos( ( 3.1415926535897931 * ( 90.0 - ( 90.0 * ( ( (double) ( this.ct進行[ j ].n現在の値 - 0x1c ) ) / 28.0 ) ) ) ) / 180.0 ) ) ) );
						this.tx火花[ j % 3 ].vc拡大縮小倍率 = new Vector3( x, x, 1f );
						this.tx火花[ j % 3 ].t2D描画( CDTXMania.app.Device, num4, y );
					}
				}
			}
			return 0;
		}


		// その他

		#region [ private ]
		//-----------------
		private CCounter[] ct進行 = new CCounter[ 6 ];
		private Point[] pt中央位置 = new Point[ 6 ];
		private CTexture[] tx火花 = new CTexture[ 3 ];
		//-----------------
		#endregion
	}
}
