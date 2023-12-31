﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Threading;
using System.IO;
using FDK;
using System.Runtime.Serialization.Formatters.Binary;


namespace DTXMania
{
	/// <summary>
	/// box.defによるスキン変更時に一時的に遷移する、スキン画像の一切無いステージ。
	/// </summary>
	internal class CStageChangeSkin : CStage
	{
		public CStageChangeSkin()
		{
			base.eステージID = CStage.Eステージ.ChangeSkin;
			base.b活性化してない = true;
		}

		public override void On活性化()
		{
			if (b活性化してない)
			{
				Trace.TraceInformation("スキン変更ステージを活性化します。");
				Trace.Indent();
				try
				{
					base.On活性化();
					Trace.TraceInformation("スキン変更ステージの活性化を完了しました。");
				}
				finally
				{
					Trace.Unindent();
				}
			}
		}

		public override void On非活性化()
		{
			if (b活性化してる)
			{
				Trace.TraceInformation("スキン変更ステージを非活性化します。");
				Trace.Indent();
				try
				{
					base.On非活性化();
					Trace.TraceInformation("スキン変更ステージの非活性化を完了しました。");
				}
				finally
				{
					Trace.Unindent();
				}
			}
		}

		public override int On進行描画()
		{
			if (base.b活性化してる)
			{
				if (base.b初めての進行描画)
				{
					base.b初めての進行描画 = false;
					return 0;
				}

				//スキン変更処理
				tChangeSkinMain();
				return 1;
			}
			return 0;
		}

		public void tChangeSkinMain()
		{
			Trace.TraceInformation("スキン変更:" + CDTXMania.Instance.Skin.GetCurrentSkinSubfolderFullName(false));

			CDTXMania.Instance.act文字コンソール.On非活性化();

			CDTXMania.Instance.Skin.PrepareReloadSkin();
			CDTXMania.Instance.Skin.ReloadSkin();

			#region [もし言語リソースがスキン内にあれば、それに切り替える]
			try
			{
				CDTXMania.Instance.Resources.csvCurrentPath = CDTXMania.Instance.ConfigIni.strSystemSkinSubfolderPath.Value;
				bool ret = CDTXMania.Instance.Resources.LoadResources(CDTXMania.Instance.ConfigIni.strLanguage);
				if (ret)
				{
					Trace.TraceInformation("スキンフォルダ内に言語リソースが見つかりました。この言語リソースを使用します。");
				}
				else
				{
					Trace.TraceInformation("スキンフォルダ内の言語リソースを使用できません。既定の言語リソースを使用します。");
					CDTXMania.Instance.Resources.ResetCsvPath();
					CDTXMania.Instance.Resources.LoadResources(CDTXMania.Instance.ConfigIni.strLanguage);   // 既定の言語リソースを使った起動ができているわけだから、
																											// 既定の言語リソースへの切り替えにあたってのエラー処理は省略。
				}
			}
			catch { };
			#endregion



			CDTXMania.Instance.act文字コンソール.On活性化();
		}
	}
}
