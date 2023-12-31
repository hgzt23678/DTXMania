﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using FDK;

namespace DTXMania
{
	internal class CStage終了 : CStage
	{
		private CCounter ct時間稼ぎ;
		private CTexture tx背景;

		public CStage終了()
		{
			base.eステージID = CStage.Eステージ.終了;
			base.eフェーズID = CStage.Eフェーズ.共通_通常状態;
			base.b活性化してない = true;
		}

		public override void On活性化()
		{
			if (b活性化してない)
			{
				Trace.TraceInformation("終了ステージを活性化します。");
				Trace.Indent();
				try
				{
					this.ct時間稼ぎ = new CCounter();
					base.On活性化();
				}
				finally
				{
					Trace.TraceInformation("終了ステージの活性化を完了しました。");
					Trace.Unindent();
				}
			}
		}
		public override void On非活性化()
		{
			if (b活性化してる)
			{
				Trace.TraceInformation("終了ステージを非活性化します。");
				Trace.Indent();
				try
				{
					base.On非活性化();
				}
				finally
				{
					Trace.TraceInformation("終了ステージの非活性化を完了しました。");
					Trace.Unindent();
				}
			}
		}
		public override void OnManagedリソースの作成()
		{
			if (base.b活性化してる)
			{
				this.tx背景 = TextureFactory.tテクスチャの生成(CSkin.Path(@"Graphics\ScreenExit background.jpg"), false);
				base.OnManagedリソースの作成();
			}
		}
		public override void OnManagedリソースの解放()
		{
			if (base.b活性化してる)
			{
				TextureFactory.tテクスチャの解放(ref this.tx背景);
				base.OnManagedリソースの解放();
			}
		}
		public override int On進行描画()
		{
			if (base.b活性化してる)
			{
				if (base.b初めての進行描画)
				{
					CDTXMania.Instance.Skin.soundゲーム終了音.t再生する();
					this.ct時間稼ぎ.t開始(0, 1, 0x3e8, CDTXMania.Instance.Timer);
					base.b初めての進行描画 = false;
				}
				this.ct時間稼ぎ.t進行();
				if (this.ct時間稼ぎ.b終了値に達した && !CDTXMania.Instance.Skin.soundゲーム終了音.b再生中)
				{
					return 1;
				}
				if (this.tx背景 != null)
				{
					this.tx背景.t2D描画(CDTXMania.Instance.Device, 0, 0);
				}
			}
			return 0;
		}

	}
}
