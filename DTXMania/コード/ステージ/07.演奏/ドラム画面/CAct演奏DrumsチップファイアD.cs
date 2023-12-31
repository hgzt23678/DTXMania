﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using SharpDX;
using FDK;

namespace DTXMania
{
	internal class CAct演奏DrumsチップファイアD : CActivity
	{
		public CAct演奏DrumsチップファイアD()
		{
		}

		[StructLayout(LayoutKind.Sequential)]
		struct ST火花
		{
			public int nLane;
			public bool b使用中;
			public CCounter ct進行;
			public float f回転単位;
			public float f回転方向;
			public float fサイズ;
		}

		[StructLayout(LayoutKind.Sequential)]
		struct ST細波
		{
			public int nLane;
			public bool b使用中;
			public CCounter ct進行;
			public float f角度X;
			public float f角度Y;
			public float f半径;
			public int n進行速度ms;
		}

		[StructLayout(LayoutKind.Sequential)]
		struct ST青い星
		{
			public int nLane;
			public bool b使用中;
			public CCounter ct進行;
			public int n前回のValue;
			public float fX;
			public float fY;
			public float f加速度X;
			public float f加速度Y;
			public float f加速度の加速度X;
			public float f加速度の加速度Y;
			public float f重力加速度;
			public float f半径;
		}

		[StructLayout(LayoutKind.Sequential)]
		struct ST大波
		{
			public int nLane;
			public bool b使用中;
			public CCounter ct進行;
			public float f角度X;
			public float f角度Y;
			public float f半径;
			public int n進行速度ms;
			public float f回転単位;
			public float f回転方向;
		}

		const int BIGWAVE_MAX = 20;
		bool b細波Balance;
		bool b大波Balance;
		const int FIRE_MAX = 8 * 8;
		readonly float[] fY波の最小仰角 = new float[] { -130f, -126f, -120f, -118f, -110f, -108f, -103f, -97f };
		readonly float[] fY波の最大仰角 = new float[] { 70f, 72f, 77f, 84f, 89f, 91f, 99f, 107f };
		private int nレーンの中央X座標(int l)
		{
			return CDTXMania.Instance.ConfigIni.GetLaneX( (ELane)l ) + CDTXMania.Instance.ConfigIni.GetLaneW( (ELane)l ) / 2;
		}
		const int STAR_MAX = 0x100;
		ST火花[] st火花 = new ST火花[FIRE_MAX];
		ST大波[] st大波 = new ST大波[BIGWAVE_MAX];
		ST細波[] st細波 = new ST細波[BIGWAVE_MAX];
		ST青い星[] st青い星 = new ST青い星[STAR_MAX];
		CTexture tx火花;
		CTexture tx細波;
		CTexture tx青い星;
		CTexture tx大波;
		int posY;

		public void Start(ELane lane, bool bフィルイン = false, bool b大波 = false, bool b細波 = false)
		{
			if (CDTXMania.Instance.ConfigIni.bDrums有効 && CDTXMania.Instance.DTX.bチップがある.Drums)
			{
				if (this.tx火花 != null)
				{
					for (int j = 0; j < FIRE_MAX; j++)
					{
						// yyagi 負荷軽減のつもり・・・だが、あまり効果なさげ
						if (this.st火花[j].b使用中 && this.st火花[j].nLane == (int)lane)
						{
							this.st火花[j].ct進行.t停止();
							this.st火花[j].b使用中 = false;
						}
					}
					float n回転初期値 = CDTXMania.Instance.Random.Next(360);
					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < FIRE_MAX; j++)
						{
							if (!this.st火花[j].b使用中)
							{
								this.st火花[j].b使用中 = true;
								this.st火花[j].nLane = (int)lane;
								this.st火花[j].ct進行 = new CCounter(0, 70, 3, CDTXMania.Instance.Timer);
								this.st火花[j].f回転単位 = C変換.DegreeToRadian((float)(n回転初期値 + (i * 90f)));
								this.st火花[j].f回転方向 = (i < 4) ? 1f : -2f;
								this.st火花[j].fサイズ = (i < 4) ? 1f : 0.5f;
								break;
							}
						}
					}
				}
				if (bフィルイン && (this.tx青い星 != null))
				{
					for (int i = 0; i < 0x10; i++)
					{
						for (int j = 0; j < STAR_MAX; j++)
						{
							if (!this.st青い星[j].b使用中)
							{
								this.st青い星[j].b使用中 = true;
								int n回転初期値 = CDTXMania.Instance.Random.Next(360);
								double num7 = 0.9 + (((double)CDTXMania.Instance.Random.Next(40)) / 100.0);
								this.st青い星[j].nLane = (int)lane;
								this.st青い星[j].ct進行 = new CCounter(0, 100, 7, CDTXMania.Instance.Timer);
								this.st青い星[j].fX = nレーンの中央X座標( (int)lane );	// this.nレーンの中央X座標[ (int) lane ];
								this.st青い星[j].fY = posY;
								this.st青い星[j].f加速度X = (float)(num7 * Math.Cos((Math.PI * 2 * n回転初期値) / 360.0));
								this.st青い星[j].f加速度Y = (float)(num7 * (Math.Sin((Math.PI * 2 * n回転初期値) / 360.0) - 0.2));
								this.st青い星[j].f加速度の加速度X = 0.995f;
								this.st青い星[j].f加速度の加速度Y = 0.995f;
								this.st青い星[j].f重力加速度 = 0.00355f;
								this.st青い星[j].f半径 = (float)(0.5 + (((double)CDTXMania.Instance.Random.Next(30)) / 100.0));
								break;
							}
						}
					}
				}
				if (b大波 && (this.tx大波 != null))
				{
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < BIGWAVE_MAX; j++)
						{
							if (!this.st大波[j].b使用中)
							{
								this.st大波[j].b使用中 = true;
								this.st大波[j].nLane = (int)lane;
								this.st大波[j].f半径 = ((float)((20 - CDTXMania.Instance.Random.Next(40)) + 100)) / 100f;
								this.st大波[j].n進行速度ms = 10;
								this.st大波[j].ct進行 = new CCounter(0, 100, this.st大波[j].n進行速度ms, CDTXMania.Instance.Timer);
								this.st大波[j].ct進行.n現在の値 = i * 10;
								this.st大波[j].f角度X = C変換.DegreeToRadian((float)((((double)(CDTXMania.Instance.Random.Next(100) * 50)) / 100.0) + 30.0));
								this.st大波[j].f角度Y = C変換.DegreeToRadian(this.b大波Balance ? (this.fY波の最小仰角[(int)lane] + CDTXMania.Instance.Random.Next(30)) : (this.fY波の最大仰角[(int)lane] - CDTXMania.Instance.Random.Next(30)));
								this.st大波[j].f回転単位 = C変換.DegreeToRadian((float)0f);
								this.st大波[j].f回転方向 = 1f;
								this.b大波Balance = !this.b大波Balance;
								break;
							}
						}
					}
				}
				if (b細波 && (this.tx細波 != null))
				{
					for (int i = 0; i < 1; i++)
					{
						for (int j = 0; j < BIGWAVE_MAX; j++)
						{
							if (!this.st細波[j].b使用中)
							{
								this.st細波[j].b使用中 = true;
								this.st細波[j].nLane = (int)lane;
								this.st細波[j].f半径 = ((float)((20 - CDTXMania.Instance.Random.Next(40)) + 100)) / 100f;
								this.st細波[j].n進行速度ms = 8;
								this.st細波[j].ct進行 = new CCounter(0, 100, this.st細波[j].n進行速度ms, CDTXMania.Instance.Timer);
								this.st細波[j].ct進行.n現在の値 = 0;
								this.st細波[j].f角度X = C変換.DegreeToRadian((float)((((double)(CDTXMania.Instance.Random.Next(100) * 50)) / 100.0) + 30.0));
								this.st細波[j].f角度Y = C変換.DegreeToRadian(this.b細波Balance ? (this.fY波の最小仰角[(int)lane] + CDTXMania.Instance.Random.Next(30)) : (this.fY波の最大仰角[(int)lane] - CDTXMania.Instance.Random.Next(30)));
								this.b細波Balance = !this.b細波Balance;
								break;
							}
						}
					}
				}
			}
		}

		public override void On活性化()
		{
			if (b活性化してない)
			{
				for (int i = 0; i < FIRE_MAX; i++)
				{
					this.st火花[i] = new ST火花();
					this.st火花[i].b使用中 = false;
					this.st火花[i].ct進行 = new CCounter();
				}
				for (int i = 0; i < STAR_MAX; i++)
				{
					this.st青い星[i] = new ST青い星();
					this.st青い星[i].b使用中 = false;
					this.st青い星[i].ct進行 = new CCounter();
				}
				for (int i = 0; i < BIGWAVE_MAX; i++)
				{
					this.st大波[i] = new ST大波();
					this.st大波[i].b使用中 = false;
					this.st大波[i].ct進行 = new CCounter();
					this.st細波[i] = new ST細波();
					this.st細波[i].b使用中 = false;
					this.st細波[i].ct進行 = new CCounter();
				}

				posY = C演奏判定ライン座標共通.n判定ラインY座標(EPart.Drums, false, CDTXMania.Instance.ConfigIni.bReverse.Drums );
				int offsety = CDTXMania.Instance.Coordinates.ImgJudgeLine.H / 2;
				posY += offsety;

				base.On活性化();
			}
		}

		public override void On非活性化()
		{
			if (b活性化してる)
			{
				for (int i = 0; i < FIRE_MAX; i++)
				{
					this.st火花[i].ct進行 = null;
				}
				for (int i = 0; i < STAR_MAX; i++)
				{
					this.st青い星[i].ct進行 = null;
				}
				for (int i = 0; i < BIGWAVE_MAX; i++)
				{
					this.st大波[i].ct進行 = null;
					this.st細波[i].ct進行 = null;
				}
				base.On非活性化();
			}
		}

		public override void OnManagedリソースの作成()
		{
			if (b活性化してる)
			{
				this.tx火花 = TextureFactory.tテクスチャの生成(CSkin.Path(@"Graphics\ScreenPlayDrums chip fire.png"));
				if (this.tx火花 != null)
				{
					this.tx火花.b加算合成 = true;
				}
				this.tx青い星 = TextureFactory.tテクスチャの生成(CSkin.Path(@"Graphics\ScreenPlayDrums chip star.png"));
				if (this.tx青い星 != null)
				{
					this.tx青い星.b加算合成 = true;
				}
				this.tx大波 = TextureFactory.tテクスチャの生成(CSkin.Path(@"Graphics\ScreenPlayDrums chip wave.png"));
				if (this.tx大波 != null)
				{
					this.tx大波.b加算合成 = true;
				}
				this.tx細波 = TextureFactory.tテクスチャの生成(CSkin.Path(@"Graphics\ScreenPlayDrums chip wave2.png"));
				if (this.tx細波 != null)
				{
					this.tx細波.b加算合成 = true;
				}
				base.OnManagedリソースの作成();
			}
		}

		public override void OnManagedリソースの解放()
		{
			if (b活性化してる)
			{
				TextureFactory.tテクスチャの解放(ref this.tx火花);
				TextureFactory.tテクスチャの解放(ref this.tx青い星);
				TextureFactory.tテクスチャの解放(ref this.tx大波);
				TextureFactory.tテクスチャの解放(ref this.tx細波);
				base.OnManagedリソースの解放();
			}
		}

		public override int On進行描画()
		{
			if (b活性化してる && CDTXMania.Instance.ConfigIni.bDrums有効 && CDTXMania.Instance.DTX.bチップがある.Drums)
			{
				for (int i = 0; i < FIRE_MAX; i++)
				{
					if (this.st火花[i].b使用中)
					{
						this.st火花[i].ct進行.t進行();
						if (this.st火花[i].ct進行.b終了値に達した)
						{
							this.st火花[i].ct進行.t停止();
							this.st火花[i].b使用中 = false;
						}
						Matrix identity = Matrix.Identity;
						float num2 = ((float)this.st火花[i].ct進行.n現在の値) / 70f;
						float num3 = this.st火花[i].f回転単位 + (this.st火花[i].f回転方向 * C変換.DegreeToRadian((float)(60f * num2)));
						float num4 = ((float)(0.2 + (0.8 * Math.Cos((((double)this.st火花[i].ct進行.n現在の値) / 50.0) * Math.PI / 2)))) * this.st火花[i].fサイズ;
						identity *= Matrix.Scaling(0.2f + num4, 0.2f + this.st火花[i].fサイズ, 1f);
						identity *= Matrix.RotationZ(num3 + ((float)Math.PI / 2));
						float num5 = ((float)(0.8 * Math.Sin(num2 * Math.PI / 2))) * this.st火花[i].fサイズ;

						float dx = (nレーンの中央X座標(this.st火花[i].nLane) + (((float)Math.Cos((double)num3)) * num5) - SampleFramework.GameWindowSize.Width / 2);
						float dy = -(posY + (((float)Math.Sin((double)num3)) * num5) - SampleFramework.GameWindowSize.Height / 2f);
						identity *= Matrix.Translation(dx, dy, 0f);
						if (this.tx火花 != null)
						{
							this.tx火花.t3D描画(CDTXMania.Instance.Device, identity);
						}
					}
				}
				for (int i = 0; i < STAR_MAX; i++)
				{
					if (this.st青い星[i].b使用中)
					{
						this.st青い星[i].n前回のValue = this.st青い星[i].ct進行.n現在の値;
						this.st青い星[i].ct進行.t進行();
						if (this.st青い星[i].ct進行.b終了値に達した)
						{
							this.st青い星[i].ct進行.t停止();
							this.st青い星[i].b使用中 = false;
						}
						for (int n = this.st青い星[i].n前回のValue; n < this.st青い星[i].ct進行.n現在の値; n++)
						{
							this.st青い星[i].fX += this.st青い星[i].f加速度X;
							this.st青い星[i].fY -= this.st青い星[i].f加速度Y;
							this.st青い星[i].f加速度X *= this.st青い星[i].f加速度の加速度X;
							this.st青い星[i].f加速度Y *= this.st青い星[i].f加速度の加速度Y;
							this.st青い星[i].f加速度Y -= this.st青い星[i].f重力加速度;
						}
						Matrix mat = Matrix.Identity;
						float x = (float)(this.st青い星[i].f半径 * Math.Cos((Math.PI / 2 * this.st青い星[i].ct進行.n現在の値) / 100.0));
						mat *= Matrix.Scaling(x, x, 1f);
						mat *= Matrix.Translation(
							this.st青い星[i].fX - SampleFramework.GameWindowSize.Width / 2,
							-(this.st青い星[i].fY - SampleFramework.GameWindowSize.Height / 2),
							0f
						);
						if (this.tx青い星 != null)
						{
							this.tx青い星.t3D描画(CDTXMania.Instance.Device, mat);
						}
					}
				}
				for (int i = 0; i < BIGWAVE_MAX; i++)
				{
					if (this.st大波[i].b使用中)
					{
						this.st大波[i].ct進行.t進行();
						if (this.st大波[i].ct進行.b終了値に達した)
						{
							this.st大波[i].ct進行.t停止();
							this.st大波[i].b使用中 = false;
						}
						if (this.st大波[i].ct進行.n現在の値 >= 0)
						{
							Matrix matrix3 = Matrix.Identity;
							float num10 = ((float)this.st大波[i].ct進行.n現在の値) / 100f;
							float angle = this.st大波[i].f回転単位 + (this.st大波[i].f回転方向 * C変換.DegreeToRadian((float)(60f * num10)));
							float num12 = 1f;
							if (num10 < 0.4f)
							{
								num12 = 2.5f * num10;
							}
							else if (num10 < 0.8f)
							{
								num12 = (float)(1.0 + (10.1 * (1.0 - Math.Cos((Math.PI / 2 * (num10 - 0.4)) * 2.5))));
							}
							else
							{
								num12 = 11.1f + (12.5f * (num10 - 0.8f));
							}
							int num13 = 0xff;
							if (num10 < 0.75f)
							{
								num13 = 0x37;
							}
							else
							{
								num13 = (int)((55f * (1f - num10)) / 0.25f);
							}
							matrix3 *= Matrix.Scaling(num12 * this.st大波[i].f半径, num12 * this.st大波[i].f半径, 1f);
							matrix3 *= Matrix.RotationZ(angle);
							matrix3 *= Matrix.RotationX(this.st大波[i].f角度X);
							matrix3 *= Matrix.RotationY(this.st大波[i].f角度Y);

							float dx = nレーンの中央X座標(this.st大波[i].nLane) - SampleFramework.GameWindowSize.Width / 2;
							float dy = -(posY - SampleFramework.GameWindowSize.Height / 2);
							matrix3 *= Matrix.Translation(dx, dy, 0f);
							if (this.tx大波 != null)
							{
								this.tx大波.n透明度 = num13;
								this.tx大波.t3D描画(CDTXMania.Instance.Device, matrix3);
							}
						}
					}
				}
				for (int i = 0; i < BIGWAVE_MAX; i++)
				{
					if (this.st細波[i].b使用中)
					{
						this.st細波[i].ct進行.t進行();
						if (this.st細波[i].ct進行.b終了値に達した)
						{
							this.st細波[i].ct進行.t停止();
							this.st細波[i].b使用中 = false;
						}
						if (this.st細波[i].ct進行.n現在の値 >= 0)
						{
							Matrix matrix4 = Matrix.Identity;
							float num15 = ((float)this.st細波[i].ct進行.n現在の値) / 100f;
							float num16 = 14f * num15;
							int num17 = (num15 < 0.5f) ? 155 : ((int)((155f * (1f - num15)) / 1f));
							matrix4 *= Matrix.Scaling(
											num16 * this.st細波[i].f半径,
											num16 * this.st細波[i].f半径,
											1f
							);
							matrix4 *= Matrix.RotationX(this.st細波[i].f角度X);
							matrix4 *= Matrix.RotationY(this.st細波[i].f角度Y);

							float dx = nレーンの中央X座標(this.st細波[i].nLane) - SampleFramework.GameWindowSize.Width / 2;
							float dy = -(posY - SampleFramework.GameWindowSize.Height / 2);
							matrix4 *= Matrix.Translation(dx, dy, 0f);
							if (this.tx細波 != null)
							{
								this.tx細波.n透明度 = num17;
								this.tx細波.t3D描画(CDTXMania.Instance.Device, matrix4);
							}
						}
					}
				}

			}
			return 0;
		}
	}
}
