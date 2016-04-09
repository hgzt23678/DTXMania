﻿using System;
using System.Collections.Generic;
using System.Text;
using FDK;

namespace DTXMania
{
	internal class CAct演奏スクロール速度 : CActivity
	{
		public STDGBSValue<double> db現在の譜面スクロール速度;

		public CAct演奏スクロール速度()
		{
			base.b活性化してない = true;
		}

		public override void On活性化()
		{
			for (EPart i = EPart.Drums; i <= EPart.Bass; i++)
			{
				this.db現在の譜面スクロール速度[i] = (double)CDTXMania.Instance.ConfigIni.nScrollSpeed[i];
				this.n速度変更制御タイマ[i] = -1;
			}
			base.On活性化();
		}

		public override unsafe int On進行描画()
		{
			if (!base.b活性化してない)
			{
				if (base.b初めての進行描画)
				{
					this.n速度変更制御タイマ.Drums = this.n速度変更制御タイマ.Guitar = this.n速度変更制御タイマ.Bass = CSound管理.rc演奏用タイマ.n現在時刻;
					base.b初めての進行描画 = false;
				}
				long n現在時刻 = CSound管理.rc演奏用タイマ.n現在時刻;
				for (EPart i = EPart.Drums; i <= EPart.Bass; i++)
				{
					double db譜面スクロールスピード = (double)CDTXMania.Instance.ConfigIni.nScrollSpeed[i];
					if (n現在時刻 < this.n速度変更制御タイマ[i])
					{
						this.n速度変更制御タイマ[i] = n現在時刻;
					}
					while ((n現在時刻 - this.n速度変更制御タイマ[i]) >= 2)               // 2msに1回ループ
					{
						if (this.db現在の譜面スクロール速度[i] < db譜面スクロールスピード)       // Config.iniのスクロール速度を変えると、それに追いつくように実画面のスクロール速度を変える
						{
							this.db現在の譜面スクロール速度[i] += 0.012;

							if (this.db現在の譜面スクロール速度[i] > db譜面スクロールスピード)
							{
								this.db現在の譜面スクロール速度[i] = db譜面スクロールスピード;
							}
						}
						else if (this.db現在の譜面スクロール速度[i] > db譜面スクロールスピード)
						{
							this.db現在の譜面スクロール速度[i] -= 0.012;

							if (this.db現在の譜面スクロール速度[i] < db譜面スクロールスピード)
							{
								this.db現在の譜面スクロール速度[i] = db譜面スクロールスピード;
							}
						}
						this.n速度変更制御タイマ[i] += 2;
					}
				}
			}
			return 0;
		}


		// その他
		private STDGBSValue<long> n速度変更制御タイマ;
	}
}
