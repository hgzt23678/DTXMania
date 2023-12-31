﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using System.IO;
using SharpDX;
using FDK;

using Rectangle = System.Drawing.Rectangle;

namespace DTXMania
{
	internal class CAct演奏グラフ : CActivity
	{
		// #24074 2011.01.23 ikanick グラフの描画
		// 実装内容
		// ・左を現在、右を目標
		// ・基準線(60,70,80,90,100%)を超えると線が黄色くなる（元は白）
		// ・目標を超えると現在が光る
		// ・オート時には描画しない
		// 要望・実装予定
		// ・グラフを波打たせるなどの視覚の向上→実装済
		// 修正等
		// ・画像がないと落ちる→修正済

		public STDGBSValue<double> dbTarget;
		private STDGBSValue<double> dbTargetDisp;
		public STDGBSValue<double> dbCurrent;
		private STDGBSValue<double> dbCurrentDisp;

		private CTexture txグラフ;

		private STDGBSValue<CCounter> counterYposInImg;
		private readonly int slices = 10;

		public CAct演奏グラフ()
		{
			base.b活性化してない = true;
		}

		public override void On活性化()
		{
			if (base.b活性化してない)
			{
				this.dbTarget = new STDGBSValue<double>();
				dbTarget.Drums = dbTarget.Guitar = dbTarget.Bass = 80.0;
				this.dbCurrent = new STDGBSValue<double>();
				dbCurrent.Drums = dbCurrent.Guitar = dbCurrent.Bass = 0.0;
				counterYposInImg = new STDGBSValue<CCounter>();
				base.On活性化();
			}
		}

		public override void OnManagedリソースの作成()
		{
			if (base.b活性化してる)
			{
				string pathグラフ = CSkin.Path(@"Graphics\ScreenPlay graph.png");
				if (File.Exists(pathグラフ))
				{
					this.txグラフ = TextureFactory.tテクスチャの生成(pathグラフ);
				}
				base.OnManagedリソースの作成();
			}
		}

		public override void OnManagedリソースの解放()
		{
			if (base.b活性化してる)
			{
				TextureFactory.tテクスチャの解放(ref this.txグラフ);
				base.OnManagedリソースの解放();
			}
		}

		public override int On進行描画()
		{
			if (b活性化してる && !CDTXMania.Instance.ConfigIni.bStoicMode)
			{

				for (EPart inst = EPart.Drums; inst <= EPart.Bass; ++inst)
				{
					if (CDTXMania.Instance.ConfigIni.b楽器有効(inst) &&
						CDTXMania.Instance.ConfigIni.bGraph[inst] &&
						CDTXMania.Instance.DTX.bチップがある[inst])
					{

						if (base.b初めての進行描画)
						{
							base.b初めての進行描画 = false;
							counterYposInImg.Drums = new CCounter(0, 35, 16, CDTXMania.Instance.Timer);
							counterYposInImg.Guitar = new CCounter(0, 35, 16, CDTXMania.Instance.Timer);
							counterYposInImg.Bass = new CCounter(0, 35, 16, CDTXMania.Instance.Timer);
						}

						counterYposInImg[inst].t進行Loop();
						int stYposInImg = counterYposInImg[inst].n現在の値;

						int gx = CDTXMania.Instance.ConfigIni.cdInstX[inst][CDTXMania.Instance.ConfigIni.eActiveInst].Value
							+ CDTXMania.Instance.Coordinates.Instrument[inst].W;
						int gy = 0;

						if (this.txグラフ != null)
						{
							// 背景暗幕
							this.txグラフ.n透明度 = 128;
							this.txグラフ.vc拡大縮小倍率.X = CDTXMania.Instance.Coordinates.Graph[inst].W;
							this.txグラフ.vc拡大縮小倍率.Y = CDTXMania.Instance.Coordinates.Graph[inst].H;
							this.txグラフ.t2D描画(CDTXMania.Instance.Device, gx, gy, new Rectangle(62, 0, 1, 1));

							// 基準線
							this.txグラフ.vc拡大縮小倍率.X = CDTXMania.Instance.Coordinates.Graph[inst].W;
							this.txグラフ.vc拡大縮小倍率.Y = 1f;

							for (int i = 0; i < slices; i++)
							{
								if (this.dbCurrent[inst] < (100 - i * slices))
								{
									// 通常の基準線(白)
									this.txグラフ.n透明度 = 128;
									this.txグラフ.t2D描画(CDTXMania.Instance.Device,
										gx, gy + CDTXMania.Instance.Coordinates.Graph[inst].H * i / slices, new Rectangle(60, 0, 1, 1));
								}
								else
								{
									// 基準線を越えたら線が黄色くなる
									this.txグラフ.n透明度 = 224;
									this.txグラフ.t2D描画(CDTXMania.Instance.Device,
										gx, gy + CDTXMania.Instance.Coordinates.Graph[inst].H * i / slices, new Rectangle(61, 0, 1, 1));
								}
							}

							// グラフのゲージ部分

							// --現在値
							this.txグラフ.vc拡大縮小倍率 = new Vector3(1f, 1f, 1f);

							if (this.dbCurrentDisp[inst] < this.dbCurrent[inst])
							{
								this.dbCurrentDisp[inst] += (this.dbCurrent[inst] - this.dbCurrentDisp[inst]) / 5 + 0.01;
							}
							else
							{
								this.dbCurrentDisp[inst] = this.dbCurrent[inst];
							}
							int ar = (int)(CDTXMania.Instance.Coordinates.Graph[inst].H * this.dbCurrentDisp[inst] / 100.0);

							this.txグラフ.n透明度 = 224;
							this.txグラフ.t2D描画(
								CDTXMania.Instance.Device,
								gx, gy + CDTXMania.Instance.Coordinates.Graph[inst].H - ar,
								new Rectangle(0, 5 + stYposInImg, 30, ar)
							);
							this.txグラフ.t2D描画( // 上部白いバー
								CDTXMania.Instance.Device,
								gx, gy + CDTXMania.Instance.Coordinates.Graph[inst].H - ar,
								new Rectangle(63 + 0, 0, 30, 5)
							);

							// 目標を超えた時に発光？させる
							if ((dbCurrent[inst] >= dbTarget[inst]))
							{
								this.txグラフ.n透明度 = 128;
								this.txグラフ.b加算合成 = true;
								this.txグラフ.t2D描画(
									CDTXMania.Instance.Device,
									gx, gy + CDTXMania.Instance.Coordinates.Graph[inst].H - ar,
									new Rectangle(0, 5 + stYposInImg, 30, ar)
								);
								this.txグラフ.b加算合成 = false;
							}

							// --目標値
							if (this.dbTargetDisp[inst] < this.dbTarget[inst])
							{
								this.dbTargetDisp[inst] += (this.dbTarget[inst] - this.dbTargetDisp[inst]) / 5 + 0.01;
							}
							else
							{
								this.dbTargetDisp[inst] = this.dbTarget[inst];
							}
							ar = (int)(CDTXMania.Instance.Coordinates.Graph[inst].H * this.dbTargetDisp[inst] / 100.0);
							
							this.txグラフ.n透明度 = 224;
							this.txグラフ.t2D描画(
								CDTXMania.Instance.Device,
								gx + 30,
								gy + CDTXMania.Instance.Coordinates.Graph[inst].H - ar,
								new Rectangle(30, 5 + stYposInImg, 30, ar)
							);
							this.txグラフ.t2D描画( // 上部白いバー
								CDTXMania.Instance.Device, gx + 30, gy + CDTXMania.Instance.Coordinates.Graph[inst].H - ar,
								new Rectangle(63 + 30, 0, 30, 5)
							);
						}
					}
				}
			}
			return 0;
		}
	}
}
