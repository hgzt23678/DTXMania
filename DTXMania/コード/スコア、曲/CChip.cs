﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DTXMania
{
	public class CChip : IComparable<CChip>, ICloneable
	{
		public bool bHit { get; set; }
		public bool b可視 { get; set; }
		public int n透明度 { get; set; }
		public int nLag { get; set; }       // 2011.2.1 yyagi
		public int n楽器パートでの出現順 { get; set; }               // #35411 2015.08.20 chnmr0
		public bool bTargetGhost判定済み { get; set; }               // #35411 2015.08.22 chnmr0
		public bool extendInfoForGhost { get; set; } // 2015.9.29 chnmr0

		public bool bIsAutoPlayed { get; private set; }             // 2011.6.10 yyagi		
		public double dbチップサイズ倍率 { get; private set; }
		public double db実数値 { get; private set; }
		public EAVIType eAVI種別 { get; private set; }
		public EBGAType eBGA種別 { get; private set; }
		public EPart e楽器パート { get; private set; }
		public EChannel eチャンネル番号 { get; set; }

		/// <summary>
		/// バーからの距離（スクロールスピードを反映したチップの発声時刻）
		/// プロパティに対してインデクサsetできないためまずフィールドとして持ち、プロパティでアクセスできるようにします。
		/// </summary>
		private STDGBSValue<int> nバーからの距離dotInternal;
		public int n整数値 { get; private set; }
		public int n整数値_内部番号 { get; private set; }
		public int n総移動時間 { get; private set; }
		public int n発声位置 { get; private set; }
		public int n発声時刻ms { get; private set; }
		public CDTX.CAVI rAVI { get; private set; }
		public CDTX.CAVIPAN rAVIPan { get; private set; }
		public CDTX.CBGA rBGA { get; private set; }
		public CDTX.CBGAPAN rBGAPan { get; private set; }
		public CDTX.CBMP rBMP { get; private set; }
		public CDTX.CBMPTEX rBMPTEX { get; private set; }
		public bool b演奏終了後も再生が続くチップである { get; private set; }  // #32248 2013.10.14 yyagi
		public bool b空打ちチップである { get; private set; }          // #34029 2014.7.15 yyagi

		public STDGBSValue<int> nバーからの距離dot
		{
			get
			{
				return nバーからの距離dotInternal;
			}
		}

		/// <summary>
		/// このチップがベースBPM、拡張BPM指示チップであることを示す。
		/// </summary>
		public bool bBPMチップである
		{
			get
			{
				return (
					this.eチャンネル番号 == EChannel.BPM ||
					this.eチャンネル番号 == EChannel.BPMEx
				);
			}
		}

		/// <summary>
		/// 可視 HHC~LBD
		/// </summary>
		public bool bDrums可視チップ
		{
			get
			{
				return EChannel.HiHatClose <= eチャンネル番号 && eチャンネル番号 <= EChannel.LeftBassDrum;
			}
		}


		/// <summary>
		/// 可視 HHC~LCY
		/// </summary>
		public bool bDrums可視チップ_LP_LBD含まない
		{
			get
			{
				return EChannel.HiHatClose <= eチャンネル番号 && eチャンネル番号 <= EChannel.LeftCymbal;
			}
		}

		/// <summary>
		/// 不可視 HHC~LBD
		/// </summary>
		public bool bDrums不可視チップ
		{
			get
			{
				return EChannel.HiHatClose_Hidden <= eチャンネル番号 && eチャンネル番号 <= EChannel.LeftBassDrum_Hidden;
			}
		}

		/// <summary>
		/// 不可視 HHC~LCY
		/// </summary>
		public bool bDrums不可視チップ_LP_LBD含まない
		{
			get
			{
				return EChannel.HiHatClose_Hidden <= eチャンネル番号 && eチャンネル番号 <= EChannel.LeftCymbal_Hidden;
			}
		}

		/// <summary>
		/// 空打ち HHC~LBD
		/// </summary>
		public bool bDrums空打ちチップ
		{
			get
			{
				return
					(EChannel.HiHatClose_NoChip <= eチャンネル番号 && eチャンネル番号 <= EChannel.RideCymbal_NoChip) ||
					(EChannel.LeftCymbal_NoChip <= eチャンネル番号 && eチャンネル番号 <= EChannel.LeftBassDrum_NoChip);
			}
		}

		/// <summary>
		/// このチップがギター/バスチップであることが前提で、
		/// そのRGBパターンを返す。
		/// </summary>
		public Eレーンビットパターン eRGBビットパターン
		{
			get
			{
				return (Eレーンビットパターン)((int)eチャンネル番号 & 7);
			}
		}

		/// <summary>
		/// Bass 可視チップであり、上位ビットのみ受け取る。
		/// Bass 可視チップ判定に使用してはならない。
		/// </summary>
		public int nGuitarBassUpper4Bit
		{
			get
			{
				return (int)eチャンネル番号 & 0xF0;
			}
		}

		/// <summary>
		/// Gutiar Open ~ Guitar RGB
		/// </summary>
		public bool bGuitar可視チップ
		{
			get
			{
				return EChannel.Guitar_Open <= eチャンネル番号 && eチャンネル番号 <= EChannel.Guitar_RGB;
			}
		}

		public bool bGuitar可視チップ_Wailing含む
		{
			get
			{
				return EChannel.Guitar_Open <= eチャンネル番号 && eチャンネル番号 <= EChannel.Guitar_Wailing;
			}
		}

		/// <summary>
		/// Bass Open ~ Bass RGB
		/// </summary>
		public bool bBass可視チップ
		{
			get
			{
				return EChannel.Bass_Open <= eチャンネル番号 && eチャンネル番号 <= EChannel.Bass_RGB;
			}
		}

		public bool bBass可視チップ_Wailing含む
		{
			get
			{
				return EChannel.Bass_Open <= eチャンネル番号 && eチャンネル番号 <= EChannel.Bass_Wailing;
			}
		}

		public static bool bBass可視チップ_Wailing含む_st(EChannel e)
		{
			return EChannel.Bass_Open <= e && e <= EChannel.Bass_Wailing;
		}

		/// <summary>
		/// Guitar or Bass Open ~ RGB
		/// </summary>
		public bool bGuitarBass可視チップ
		{
			get
			{
				return bGuitar可視チップ || bBass可視チップ;
			}
		}

		public bool bGuitarBass可視チップ_Wailing含む
		{
			get
			{
				return bGuitar可視チップ_Wailing含む || bBass可視チップ_Wailing含む;
			}
		}

		/// <summary>
		/// Guitar or Bass has R pattern
		/// </summary>
		public bool bGuitarBass_R
		{
			get
			{
				return bGuitarBass可視チップ && (((int)eチャンネル番号 & 4) > 0);
			}
		}

		public bool bGuitarBass_G
		{
			get
			{
				return bGuitarBass可視チップ && (((int)eチャンネル番号 & 2) > 0);
			}
		}

		public bool bGuitarBass_B
		{
			get
			{
				return bGuitarBass可視チップ && (((int)eチャンネル番号 & 1) > 0);
			}
		}

		public bool bGuitarBass_Wailing
		{
			get
			{
				return bGuitarBass可視チップ_Wailing含む && (((int)eチャンネル番号 & 0x0F) == 0x08);
			}
		}

		public bool bGuitarBass_Open
		{
			get
			{
				return bGuitarBass可視チップ && (((int)eチャンネル番号 & 0x0F) == 0x00);
			}
		}

		public bool this[EChannel x]
		{
			get
			{
				return eチャンネル番号 == x;
			}
		}

		public bool bBGALayer
		{
			get
			{
				return
					((eチャンネル番号 == EChannel.BGALayer1) ||
					(eチャンネル番号 == EChannel.BGALayer2) ||
					((EChannel.BGALayer3 <= eチャンネル番号) && (eチャンネル番号 <= EChannel.BGALayer7)) ||
					(eチャンネル番号 == EChannel.BGALayer8));
			}
		}

		public bool bBGALayerSwap
		{
			get
			{
				return
					((eチャンネル番号 == EChannel.BGALayer1_Swap) ||
					(eチャンネル番号 == EChannel.BGALayer2_Swap) ||
					((EChannel.BGALayer3_Swap <= eチャンネル番号) && (eチャンネル番号 <= EChannel.BGALayer7_Swap)) ||
					(eチャンネル番号 == EChannel.BGALayer8_Swap));
			}
		}

		public void DecideInstrumentPart()
		{
			if (bDrums可視チップ)
			{
				e楽器パート = EPart.Drums;
			}
			else if (bGuitar可視チップ)
			{
				e楽器パート = EPart.Guitar;
			}
			else if (bBass可視チップ)
			{
				e楽器パート = EPart.Bass;
			}
			else
			{
				e楽器パート = EPart.Unknown;
			}
		}

		/// <summary>
		/// #34029 2014.7.15 yyagi
		/// ドラムの空打ち音だったら、フラグを立てたうえで、通常チップのチャンネル番号に変更。ギターベースの空打ち音はとりあえずフラグを立てるだけ。
		/// </summary>
		public void ConvertNoChip()
		{
			if (EChannel.HiHatClose_NoChip <= eチャンネル番号 && eチャンネル番号 <= EChannel.RideCymbal_NoChip)
			{
				b空打ちチップである = true;
				eチャンネル番号 = eチャンネル番号 - 0xB0 + 0x10;
			}
			else if (this[EChannel.Guitar_NoChip] || this[EChannel.Bass_NoChip])
			{
				b空打ちチップである = true;
			}
			else if (this[EChannel.LeftCymbal_NoChip])
			{
				b空打ちチップである = true;
				eチャンネル番号 = EChannel.LeftCymbal;
			}
			else if (this[EChannel.LeftPedal_NoChip])
			{
				b空打ちチップである = true;
				eチャンネル番号 = EChannel.LeftPedal;
			}
			else if (this[EChannel.LeftBassDrum_NoChip])
			{
				b空打ちチップである = true;
				eチャンネル番号 = EChannel.LeftBassDrum;
			}
		}

		public void ConvertGBNoChip()
		{
			if (this[EChannel.Guitar_NoChip] || this[EChannel.Bass_NoChip])
			{
				eチャンネル番号 = this[EChannel.Guitar_NoChip] ? EChannel.Guitar_Open : EChannel.Bass_Open;
			}
		}


		/// <summary>
		/// このチップが音声と関連付けられたチップであることを示す。
		/// </summary>
		public bool bWAVを使うチャンネルである
		{
			get
			{
				bool ret = false;
				EChannel x = eチャンネル番号;

				if (x == EChannel.BGM ||
					(EChannel.HiHatClose <= x && x <= EChannel.LeftBassDrum) ||
					(EChannel.DrumsFillin == x) ||
					(EChannel.Guitar_Open <= x && x <= EChannel.Guitar_Wailing) || x == EChannel.Guitar_WailingSound ||
					(EChannel.HiHatClose_Hidden <= x && x <= EChannel.LeftBassDrum_Hidden) ||
					(EChannel.SE01 <= x && x <= EChannel.SE09) ||
					(EChannel.SE10 <= x && x <= EChannel.SE19) ||
					(EChannel.SE20 <= x && x <= EChannel.SE29) ||
					(EChannel.SE30 <= x && x <= EChannel.SE32) ||
					(EChannel.Bass_Open <= x && x <= EChannel.Bass_Wailing) || x == EChannel.Bass_WailingSound ||
					(EChannel.HiHatClose_NoChip <= x && x <= EChannel.LeftBassDrum_NoChip)
					)
				{
					ret = true;
				}
				return ret;
			}
		}

		public bool bMovie
		{
			get
			{
				return eチャンネル番号 == EChannel.MovieFull || eチャンネル番号 == EChannel.Movie;
			}
		}

		public bool bSE
		{
			get
			{
				if (((EChannel.SE01 <= eチャンネル番号) && (eチャンネル番号 <= EChannel.SE09)) ||
						((EChannel.SE10 <= eチャンネル番号) && (eチャンネル番号 <= EChannel.SE19)) ||
						((EChannel.SE20 <= eチャンネル番号) && (eチャンネル番号 <= EChannel.SE29)) ||
						((EChannel.SE30 <= eチャンネル番号) && (eチャンネル番号 <= EChannel.SE32)))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		public bool bOverrideSE
		{
			get
			{
				return EChannel.SE24 <= eチャンネル番号 && eチャンネル番号 <= EChannel.SE29;
			}
		}

		public int nBGALayerIndex
		{
			get
			{
				int ret = -1;
				switch (eチャンネル番号)
				{
					case EChannel.BGALayer1: ret = 0; break;
					case EChannel.BGALayer2: ret = 1; break;
					case EChannel.BGALayer3: ret = 2; break;
					case EChannel.BGALayer4: ret = 3; break;
					case EChannel.BGALayer5: ret = 4; break;
					case EChannel.BGALayer6: ret = 5; break;
					case EChannel.BGALayer7: ret = 6; break;
					case EChannel.BGALayer8: ret = 7; break;
				}
				return ret;
			}
		}

		public int nBGALayerSwapIndex
		{
			get
			{
				int ret = -1;
				switch (eチャンネル番号)
				{
					case EChannel.BGALayer1_Swap: ret = 0; break;
					case EChannel.BGALayer2_Swap: ret = 1; break;
					case EChannel.BGALayer3_Swap: ret = 2; break;
					case EChannel.BGALayer4_Swap: ret = 3; break;
					case EChannel.BGALayer5_Swap: ret = 4; break;
					case EChannel.BGALayer6_Swap: ret = 5; break;
					case EChannel.BGALayer7_Swap: ret = 6; break;
					case EChannel.BGALayer8_Swap: ret = 7; break;
				}
				return ret;
			}
		}

		public int nDrumsIndex
		{
			get
			{
				int ret = -1;
				switch (eチャンネル番号)
				{
					case EChannel.HiHatClose: ret = 0; break;
					case EChannel.Snare: ret = 1; break;
					case EChannel.BassDrum: ret = 2; break;
					case EChannel.HighTom: ret = 3; break;
					case EChannel.LowTom: ret = 4; break;
					case EChannel.Cymbal: ret = 5; break;
					case EChannel.FloorTom: ret = 6; break;
					case EChannel.HiHatOpen: ret = 7; break;
					case EChannel.RideCymbal: ret = 8; break;
					case EChannel.LeftCymbal: ret = 9; break;
					case EChannel.LeftPedal: ret = 10; break;
					case EChannel.LeftBassDrum: ret = 11; break;
				}
				return ret;
			}
		}

		public int nDrumsIndexHidden
		{
			get
			{
				int ret = -1;
				switch (eチャンネル番号)
				{
					case EChannel.HiHatClose_Hidden: ret = 0; break;
					case EChannel.Snare_Hidden: ret = 1; break;
					case EChannel.BassDrum_Hidden: ret = 2; break;
					case EChannel.HighTom_Hidden: ret = 3; break;
					case EChannel.LowTom_Hidden: ret = 4; break;
					case EChannel.Cymbal_Hidden: ret = 5; break;
					case EChannel.FloorTom_Hidden: ret = 6; break;
					case EChannel.HiHatOpen_Hidden: ret = 7; break;
					case EChannel.RideCymbal_Hidden: ret = 8; break;
					case EChannel.LeftCymbal_Hidden: ret = 9; break;
					case EChannel.LeftPedal_Hidden: ret = 10; break;
					case EChannel.LeftBassDrum_Hidden: ret = 11; break;
				}
				return ret;
			}
		}

		/// <summary>
		/// このチップが持つチャンネルの ESoundChipType を返す。
		///  チャンネル番号→Chipの種類、の変換。今後の拡張を容易にするために追加
		/// </summary>
		public ESoundChipType ESoundChipTypeを得る
		{
			get
			{
				switch (this.eチャンネル番号)
				{
					#region [ Drums ]
					case EChannel.HiHatClose:
					case EChannel.Snare:
					case EChannel.BassDrum:
					case EChannel.HighTom:
					case EChannel.LowTom:
					case EChannel.Cymbal:
					case EChannel.FloorTom:
					case EChannel.HiHatOpen:
					case EChannel.RideCymbal:
					case EChannel.LeftCymbal:
					case EChannel.LeftPedal:
					case EChannel.LeftBassDrum:

					case EChannel.HiHatClose_Hidden:
					case EChannel.Snare_Hidden:
					case EChannel.BassDrum_Hidden:
					case EChannel.HighTom_Hidden:
					case EChannel.LowTom_Hidden:
					case EChannel.Cymbal_Hidden:
					case EChannel.FloorTom_Hidden:
					case EChannel.HiHatOpen_Hidden:
					case EChannel.RideCymbal_Hidden:
					case EChannel.LeftCymbal_Hidden:
					case EChannel.LeftPedal_Hidden:
					case EChannel.LeftBassDrum_Hidden:

					case EChannel.HiHatClose_NoChip:
					case EChannel.Snare_NoChip:
					case EChannel.BassDrum_NoChip:
					case EChannel.HighTom_NoChip:
					case EChannel.LowTom_NoChip:
					case EChannel.Cymbal_NoChip:
					case EChannel.FloorTom_NoChip:
					case EChannel.HiHatOpen_NoChip:
					case EChannel.RideCymbal_NoChip:
					case EChannel.LeftCymbal_NoChip:
					case EChannel.LeftPedal_NoChip:
					case EChannel.LeftBassDrum_NoChip:

						return ESoundChipType.Drums;
					#endregion

					#region [ Guitar ]
					case EChannel.Guitar_Open:
					case EChannel.Guitar_xxB:
					case EChannel.Guitar_xGx:
					case EChannel.Guitar_xGB:
					case EChannel.Guitar_Rxx:
					case EChannel.Guitar_RxB:
					case EChannel.Guitar_RGx:
					case EChannel.Guitar_RGB:
					case EChannel.Guitar_Wailing:
					case EChannel.Guitar_WailingSound:
					case EChannel.Guitar_NoChip:
						return ESoundChipType.Guitar;
					#endregion

					#region [ Bass ]
					case EChannel.Bass_Open:
					case EChannel.Bass_xxB:
					case EChannel.Bass_xGx:
					case EChannel.Bass_xGB:
					case EChannel.Bass_Rxx:
					case EChannel.Bass_RxB:
					case EChannel.Bass_RGx:
					case EChannel.Bass_RGB:
					case EChannel.Bass_Wailing:
					case EChannel.Bass_WailingSound:
					case EChannel.Bass_NoChip:
						return ESoundChipType.Bass;
					#endregion

					#region [ SE ]
					case EChannel.SE01:
					case EChannel.SE02:
					case EChannel.SE03:
					case EChannel.SE04:
					case EChannel.SE05:
					case EChannel.SE06:
					case EChannel.SE07:
					case EChannel.SE08:
					case EChannel.SE09:
					case EChannel.SE10:
					case EChannel.SE11:
					case EChannel.SE12:
					case EChannel.SE13:
					case EChannel.SE14:
					case EChannel.SE15:
					case EChannel.SE16:
					case EChannel.SE17:
					case EChannel.SE18:
					case EChannel.SE19:
					case EChannel.SE20:
					case EChannel.SE21:
					case EChannel.SE22:
					case EChannel.SE23:
					case EChannel.SE24:
					case EChannel.SE25:
					case EChannel.SE26:
					case EChannel.SE27:
					case EChannel.SE28:
					case EChannel.SE29:
					case EChannel.SE30:
					case EChannel.SE31:
					case EChannel.SE32:
						return ESoundChipType.SE;
					#endregion

					#region [ BGM ]
					case EChannel.BGM:
						return ESoundChipType.BGM;
					#endregion

					#region [ その他 ]
					default:
						return ESoundChipType.Unknown;
						#endregion
				}
			}
		}
		public bool bIsVisibleChip
		{
			get
			{
				switch (this.eチャンネル番号)
				{
					case EChannel.HiHatClose:
					case EChannel.Snare:
					case EChannel.BassDrum:
					case EChannel.HighTom:
					case EChannel.LowTom:
					case EChannel.Cymbal:
					case EChannel.FloorTom:
					case EChannel.HiHatOpen:
					case EChannel.RideCymbal:
					case EChannel.LeftCymbal:
					case EChannel.LeftPedal:
					case EChannel.LeftBassDrum:

					case EChannel.Guitar_Open:
					case EChannel.Guitar_xxB:
					case EChannel.Guitar_xGx:
					case EChannel.Guitar_xGB:
					case EChannel.Guitar_Rxx:
					case EChannel.Guitar_RxB:
					case EChannel.Guitar_RGx:
					case EChannel.Guitar_RGB:

					case EChannel.Bass_Open:
					case EChannel.Bass_xxB:
					case EChannel.Bass_xGx:
					case EChannel.Bass_xGB:
					case EChannel.Bass_Rxx:
					case EChannel.Bass_RxB:
					case EChannel.Bass_RGx:
					case EChannel.Bass_RGB:
						return true;

					default:
						return false;
				}
			}
		}


		/// <summary>
		/// これが本当に一意なWAV番号となる。（無限定義の場合、chip.n整数値 は一意である保証がない。）
		/// </summary>
		/// <param name="index"></param>
		public void AssignInfiniteManageWAV(int index)
		{
			if (bWAVを使うチャンネルである)
			{
				n整数値_内部番号 = index;// 
			}
		}

		/// <summary>
		/// // これが本当に一意なBPM番号となる。(無限WAVも参照)
		/// </summary>
		/// <param name="index"></param>
		public void AssignInfiniteManageBPM(int index)
		{
			if (bBPMチップである)
			{
				n整数値_内部番号 = index;
			}
		}

		public void AdjustPlayPositionForFillin(int index)
		{
			if (this[EChannel.FillIn])
			{
				// ずらすのは、フィルインONチップと同じ位置にいるチップでも確実にフィルインが発動し、
				// 同様に、フィルインOFFチップと同じ位置にいるチップでも確実にフィルインが終了するようにするため。

				if ((index > 0) && (index != 2))
				{
					n発声位置 -= 32;  // 384÷32＝12 ということで、フィルインONチップは12分音符ほど前へ移動。
				}
				else if (index == 2)
				{
					n発声位置 += 32;  // 同じく、フィルインOFFチップは12分音符ほど後ろへ移動。
				}
			}
		}

		/// <summary>
		/// #BPMzz 行より前の行に出現した #BPMzz では、整数値・内部番号は -zz に初期化されている。
		/// </summary>
		/// <param name="zz"></param>
		/// <param name="newIndex"></param>
		public void AdjustInfiniteManageIntInternalIndex(bool chipCond, int zz, int newIndex)
		{
			if (chipCond && n整数値_内部番号 == -zz)
			{
				n整数値_内部番号 = newIndex;
			}
		}

		public void AddPlayPositionMsForSE(int increment)
		{
			if (this[EChannel.BGM] || bSE)
			{
				n発声時刻ms += increment;
			}
		}

		public void ApplyAVI(
			Dictionary<int, CDTX.CAVI> listAVI,
			Dictionary<int, CDTX.CAVIPAN> listAVIPAN
			)
		{
			if (bMovie)
			{
				eAVI種別 = EAVIType.Unknown;
				rAVI = null;
				rAVIPan = null;
				if (listAVIPAN.ContainsKey(n整数値))
				{
					CDTX.CAVIPAN cavipan = listAVIPAN[n整数値];
					if (listAVI.ContainsKey(cavipan.nAVI番号) && (listAVI[cavipan.nAVI番号].avi != null))
					{
						eAVI種別 = EAVIType.AVIPAN;
						rAVI = listAVI[cavipan.nAVI番号];
						rAVIPan = cavipan;
						return;
					}
				}
				if (listAVI.ContainsKey(n整数値) && (listAVI[n整数値].avi != null))
				{
					eAVI種別 = EAVIType.AVI;
					rAVI = listAVI[n整数値];
				}
			}
		}


		public void ApplyBMP_BMPTEX(
			Dictionary<int, CDTX.CBGA> listBGA,
			Dictionary<int, CDTX.CBGAPAN> listBGAPAN,
			Dictionary<int, CDTX.CBMP> listBMP,
			Dictionary<int, CDTX.CBMPTEX> listBMPTEX)
		{
			if (bBGALayer)
			{
				eBGA種別 = EBGAType.Unknown;
				rBMP = null;
				rBMPTEX = null;
				rBGA = null;
				rBGAPan = null;

				#region [ BGAPAN ]
				if (listBGAPAN.ContainsKey(n整数値))
				{
					CDTX.CBGAPAN cbgapan = listBGAPAN[n整数値];
					if (listBMPTEX.ContainsKey(cbgapan.nBMP番号) && listBMPTEX[cbgapan.nBMP番号].bUse)
					{
						eBGA種別 = EBGAType.BGAPAN;
						rBMPTEX = listBMPTEX[cbgapan.nBMP番号];
						rBGAPan = cbgapan;
						return;
					}
					if (listBMP.ContainsKey(cbgapan.nBMP番号) && listBMP[cbgapan.nBMP番号].bUse)
					{
						eBGA種別 = EBGAType.BGAPAN;
						rBMP = listBMP[cbgapan.nBMP番号];
						rBGAPan = cbgapan;
						return;
					}
				}
				#endregion

				#region [ BGA ]
				if (listBGA.ContainsKey(n整数値))
				{
					CDTX.CBGA cbga = listBGA[n整数値];
					if (listBMPTEX.ContainsKey(cbga.nBMP番号) && listBMPTEX[cbga.nBMP番号].bUse)
					{
						eBGA種別 = EBGAType.BGA;
						rBMPTEX = listBMPTEX[cbga.nBMP番号];
						rBGA = cbga;
						return;
					}
					if (listBMP.ContainsKey(cbga.nBMP番号) && listBMP[cbga.nBMP番号].bUse)
					{
						eBGA種別 = EBGAType.BGA;
						rBMP = listBMP[cbga.nBMP番号];
						rBGA = cbga;
						return;
					}
				}
				#endregion

				#region [ BMPTEX ]
				if (listBMPTEX.ContainsKey(n整数値) && listBMPTEX[n整数値].bUse)
				{
					eBGA種別 = EBGAType.BMPTEX;
					rBMPTEX = listBMPTEX[n整数値];
					return;
				}
				#endregion

				#region [ BMP ]
				if (listBMP.ContainsKey(n整数値) && listBMP[n整数値].bUse)
				{
					eBGA種別 = EBGAType.BMP;
					rBMP = listBMP[n整数値];
					return;
				}
				#endregion
			}
			#region [ BGA入れ替え ]
			if (bBGALayerSwap)
			{
				eBGA種別 = EBGAType.Unknown;
				rBMP = null;
				rBMPTEX = null;
				rBGA = null;
				rBGAPan = null;
				if (listBMPTEX.ContainsKey(n整数値) && listBMPTEX[n整数値].bUse)
				{
					eBGA種別 = EBGAType.BMPTEX;
					rBMPTEX = listBMPTEX[n整数値];
				}
				else if (listBMP.ContainsKey(n整数値) && listBMP[n整数値].bUse)
				{
					eBGA種別 = EBGAType.BMP;
					rBMP = listBMP[n整数値];
				}
			}
			#endregion
		}

		public void SetDBChipSizeFactor(double newFactor)
		{
			dbチップサイズ倍率 = newFactor;
		}


		public void SetSoundAfterPlayEnd(bool newBSound)
		{
			b演奏終了後も再生が続くチップである = newBSound;
		}

		public void CalculatePlayPositionMs(
			EDTX種別 type,
			double basebpm,
			Dictionary<int, CDTX.CBPM> listBPM,
			Dictionary<int, CDTX.CAVIPAN> listAVIPAN,
			Dictionary<int, CDTX.CBGAPAN> listBGAPAN,
			ref double bpm,
			ref double dbBarLength,
			ref int basePlayPosition,
			ref int ms,
			ref int nBar)
		{
			n発声時刻ms = ms + ((int)(((0x271 * (n発声位置 - basePlayPosition)) * dbBarLength) / bpm));
			if (((type == EDTX種別.BMS) || (type == EDTX種別.BME)) &&
				((dbBarLength != 1.0) && ((n発声位置 / 384) != nBar)))
			{
				basePlayPosition = n発声位置;
				ms = n発声時刻ms;
				dbBarLength = 1.0;
			}
			nBar = n発声位置 / 384;

			if (this[EChannel.BarLength])
			{
				basePlayPosition = n発声位置;
				ms = n発声時刻ms;
				dbBarLength = db実数値;
			}
			else if (this[EChannel.BPM])
			{
				basePlayPosition = n発声位置;
				ms = n発声時刻ms;
				bpm = basebpm + n整数値;
			}
			else if (this[EChannel.BPMEx])
			{
				basePlayPosition = n発声位置;
				ms = n発声時刻ms;
				if (listBPM.ContainsKey(n整数値_内部番号))
				{
					bpm = ((listBPM[n整数値_内部番号].n表記上の番号 == 0) ? 0.0 : basebpm) + listBPM[n整数値_内部番号].dbBPM値;
				}
			}
			else if (bMovie)
			{
				if (listAVIPAN.ContainsKey(n整数値))
				{
					int num21 = ms + ((int)(((0x271 * (n発声位置 - basePlayPosition)) * dbBarLength) / bpm));
					int num22 = ms + ((int)(((0x271 * ((n発声位置 + listAVIPAN[n整数値].n移動時間ct) - basePlayPosition)) * dbBarLength) / bpm));
					n総移動時間 = num22 - num21;
				}
			}
			else if (bBGALayer)
			{
				if (listBGAPAN.ContainsKey(n整数値))
				{
					int num19 = ms + ((int)(((0x271 * (n発声位置 - basePlayPosition)) * dbBarLength) / bpm));
					int num20 = ms + ((int)(((0x271 * ((n発声位置 + listBGAPAN[n整数値].n移動時間ct) - basePlayPosition)) * dbBarLength) / bpm));
					n総移動時間 = num20 - num19;
				}
			}
		}

		public void ConvertSE25_26_27toCY_RCY_LCY()
		{
			EChannel to = eチャンネル番号;
			switch (eチャンネル番号)
			{
				case EChannel.SE25: to = EChannel.Cymbal; break;
				case EChannel.SE26: to = EChannel.RideCymbal; break;
				case EChannel.SE27: to = EChannel.LeftCymbal; break;
			}
			eチャンネル番号 = to;
		}

		/// <summary>
		/// 演奏速度を適用。2回以上呼ぶとさらに短くなる。
		/// </summary>
		/// <param name="factor"></param>
		public void ApplyPlaySpeed(double factor)
		{
			if (factor > 0)
			{
				n発声時刻ms = (int)((double)n発声時刻ms / factor);
			}
		}

		public void CalcDistanceFromBar(long currentTime, STDGBSValue<double> ScrollSpeedInSetting)
		{
			const double speed = 526.5;// BPM150の時の1小節の長さ[dot]
			double ScrollSpeedDrums = (ScrollSpeedInSetting.Drums + 1.0) * 0.5 * 37.5 * speed / 60000.0;
			double ScrollSpeedGuitar = (ScrollSpeedInSetting.Guitar + 1.0) * 0.5 * 0.5 * 37.5 * speed / 60000.0;
			double ScrollSpeedBass = (ScrollSpeedInSetting.Bass + 1.0) * 0.5 * 0.5 * 37.5 * speed / 60000.0;

			//STDGBVALUE<int> newDot = new STDGBVALUE<int>();

			nバーからの距離dotInternal.Drums = (int)((n発声時刻ms - currentTime) * ScrollSpeedDrums);
			nバーからの距離dotInternal.Guitar = (int)((n発声時刻ms - currentTime) * ScrollSpeedGuitar);
			nバーからの距離dotInternal.Bass = (int)((n発声時刻ms - currentTime) * ScrollSpeedBass);

			//nバーからの距離dot = newDot;
		}

		public void RandomizeRGB(ERandom eRandom, int seed, bool existOpen)
		{
			int[,] nランダムレーン候補 = new int[,]
			{
					{ 0, 1, 2, 3, 4, 5, 6, 7 },
					{ 0, 2, 1, 3, 4, 6, 5, 7 },
					{ 0, 1, 4, 5, 2, 3, 6, 7 },
					{ 0, 2, 4, 6, 1, 3, 5, 7 },
					{ 0, 4, 1, 5, 2, 6, 3, 7 },
					{ 0, 4, 2, 6, 1, 5, 3, 7 }
				};
			Eレーンビットパターン nRGBレーンビットパターン;
			Eレーンビットパターン n新RGBレーンビットパターン = Eレーンビットパターン.OPEN;// 「未割り当てのローカル変数」ビルドエラー回避のために0を初期値に設定
			int nランダム化前BassFlag = nGuitarBassUpper4Bit;
			bool bAdjustRandomLane = true;

			if (bGuitarBass可視チップ)
			{
				switch (eRandom)
				{
					case ERandom.Random:    // 1小節単位でレーンのR/G/Bがランダムに入れ替わる
						eチャンネル番号 = (EChannel)((nGuitarBassUpper4Bit) | nランダムレーン候補[seed, (int)eRGBビットパターン]);
						return;

					case ERandom.Super: // チップごとにR/G/Bがランダムで入れ替わる(レーンの本数までは変わらない)。
						eチャンネル番号 = (EChannel)((nGuitarBassUpper4Bit) | nランダムレーン候補[CDTXMania.Instance.Random.Next(6), (int)eRGBビットパターン]);
						return;

					case ERandom.Hyper: // レーンの本数も変わる
						nRGBレーンビットパターン = eRGBビットパターン;
						// n新RGBレーンビットパターン = (int)Eレーンビットパターン.OPEN;	// この値は結局未使用なので削除
						if (
							(nRGBレーンビットパターン != Eレーンビットパターン.xxB) &&
							(nRGBレーンビットパターン != Eレーンビットパターン.xGx) &&
							(nRGBレーンビットパターン != Eレーンビットパターン.Rxx))    // xxB, xGx, Rxx レーン1本相当
						{
						}
						else
						{
							n新RGBレーンビットパターン = (Eレーンビットパターン)CDTXMania.Instance.Random.Next(6) + 1;   // レーン1本相当なら、レーン1本か2本(1～6)に変化して終了
							bAdjustRandomLane = false;
						}
						break;

					default:
						return;
				}
				if (bAdjustRandomLane)
				{
					switch (nRGBレーンビットパターン)
					{
						case Eレーンビットパターン.xGB: // xGB	レーン2本相当
						case Eレーンビットパターン.RxB: // RxB
						case Eレーンビットパターン.RGx: // RGx
							n新RGBレーンビットパターン = existOpen ? (Eレーンビットパターン)(CDTXMania.Instance.Random.Next(8)) : (Eレーンビットパターン)(CDTXMania.Instance.Random.Next(7) + 1);  // OPENあり譜面ならOPENを含むランダム, OPENなし譜面ならOPENを含まないランダム
							break;

						default:
							if (nRGBレーンビットパターン == Eレーンビットパターン.RGB)  // RGB レーン3本相当
							{
								if (existOpen)  // OPENあり譜面の場合
								{
									int n乱数パーセント = CDTXMania.Instance.Random.Next(100);
									if (n乱数パーセント < 30)
									{
										n新RGBレーンビットパターン = Eレーンビットパターン.OPEN;
									}
									else if (n乱数パーセント < 60)
									{
										n新RGBレーンビットパターン = Eレーンビットパターン.RGB;
									}
									else if (n乱数パーセント < 85)
									{
										switch (CDTXMania.Instance.Random.Next(3))
										{
											case 0:
												n新RGBレーンビットパターン = Eレーンビットパターン.xGB;
												break;

											case 1:
												n新RGBレーンビットパターン = Eレーンビットパターン.RxB;
												break;
										}
										n新RGBレーンビットパターン = Eレーンビットパターン.RGx;
									}
									else  // OPENでない場合
									{
										switch (CDTXMania.Instance.Random.Next(3))
										{
											case 0:
												n新RGBレーンビットパターン = Eレーンビットパターン.xxB;
												break;

											case 1:
												n新RGBレーンビットパターン = Eレーンビットパターン.xGx;
												break;
										}
										n新RGBレーンビットパターン = Eレーンビットパターン.Rxx;
									}
								}
								else  // OPENなし譜面の場合
								{
									int n乱数パーセント = CDTXMania.Instance.Random.Next(100);
									if (n乱数パーセント < 60)
									{
										n新RGBレーンビットパターン = Eレーンビットパターン.RGB;
									}
									else if (n乱数パーセント < 85)
									{
										switch (CDTXMania.Instance.Random.Next(3))
										{
											case 0:
												n新RGBレーンビットパターン = Eレーンビットパターン.xGB;
												break;

											case 1:
												n新RGBレーンビットパターン = Eレーンビットパターン.RxB;
												break;
										}
										n新RGBレーンビットパターン = Eレーンビットパターン.RGx;
									}
									else
									{
										switch (CDTXMania.Instance.Random.Next(3))
										{
											case 0:
												n新RGBレーンビットパターン = Eレーンビットパターン.xxB;
												break;

											case 1:
												n新RGBレーンビットパターン = Eレーンビットパターン.xGx;
												break;
										}
										n新RGBレーンビットパターン = Eレーンビットパターン.Rxx;
									}
								}
							}
							break;
					}
				}
				eチャンネル番号 = (EChannel)((nランダム化前BassFlag) | (int)n新RGBレーンビットパターン);
			}
		}

		public void SwapGB()
		{
			int offset = 0xA0 - 0x20;
			if (bBass可視チップ)
			{
				eチャンネル番号 -= offset;
			}
			else if (bGuitar可視チップ)
			{
				eチャンネル番号 += offset;
			}
			DecideInstrumentPart();
		}

		public CChip()
		{
			this.nバーからの距離dotInternal = new STDGBSValue<int>()
			{
				Drums = 0,
				Guitar = 0,
				Bass = 0,
			};
			b可視 = true;
			dbチップサイズ倍率 = 1.0;
			e楽器パート = EPart.Unknown;
			n透明度 = 0xff;
		}

		public CChip(int playPosition, int intData, int intDataInternalIndex, EChannel channel)
			: this(playPosition, intData, channel)
		{
			n整数値_内部番号 = intDataInternalIndex;
		}

		public CChip(int playPosition, int intData, EChannel channel)
			: this()
		{
			n発声位置 = playPosition;
			n整数値 = intData;
			eチャンネル番号 = channel;
		}

		public CChip(int playPosition, double phraseLengthFactor, EChannel channel)
			: this()
		{
			n発声位置 = playPosition;
			db実数値 = phraseLengthFactor;
			eチャンネル番号 = channel;
		}

		public CChip(EChannel channel)
			: this()
		{
			eチャンネル番号 = channel;
		}

		public CChip(int playPosition, int intData, int intDataInternalIndex, EChannel channel,
			int playPositionMs, bool bSoundAfterPlayEnd)
			: this(playPosition, intData, intDataInternalIndex, channel)
		{
			n発声時刻ms = playPositionMs;
			b演奏終了後も再生が続くチップである = bSoundAfterPlayEnd;
		}

		public override string ToString()
		{
			/*
			string[] chToStr = 
			{
				"??", "バックコーラス", "小節長変更", "BPM変更", "BMPレイヤ1", "??", "??", "BMPレイヤ2",
				"BPM変更(拡張)", "??", "??", "??", "??", "??", "??", "??",
				"??", "HHClose", "Snare", "Kick", "HiTom", "LowTom", "Cymbal", "FloorTom",
				"HHOpen", "RideCymbal", "LeftCymbal", "LeftPedal", "LeftBassDrum", "", "", "ドラム歓声切替",
				"ギターOPEN", "ギター - - B", "ギター - G -", "ギター - G B", "ギター R - -", "ギター R - B", "ギター R G -", "ギター R G B",
				"ギターWailing", "??", "??", "??", "??", "??", "??", "ギターWailing音切替",
				"??", "HHClose(不可視)", "Snare(不可視)", "Kick(不可視)", "HiTom(不可視)", "LowTom(不可視)", "Cymbal(不可視)", "FloorTom(不可視)",
				"HHOpen(不可視)", "RideCymbal(不可視)", "LeftCymbal(不可視)", "??", "??", "??", "??", "??",
				"??", "??", "??", "??", "??", "??", "??", "??", 
				"??", "??", "??", "??", "??", "??", "??", "??", 
				"小節線", "拍線", "MIDIコーラス", "フィルイン", "AVI", "BMPレイヤ3", "BMPレイヤ4", "BMPレイヤ5",
				"BMPレイヤ6", "BMPレイヤ7", "AVIFull", "??", "??", "??", "??", "??", 
				"BMPレイヤ8", "SE01", "SE02", "SE03", "SE04", "SE05", "SE06", "SE07",
				"SE08", "SE09", "??", "??", "??", "??", "??", "??", 
				"SE10", "SE11", "SE12", "SE13", "SE14", "SE15", "SE16", "SE17",
				"SE18", "SE19", "??", "??", "??", "??", "??", "??", 
				"SE20", "SE21", "SE22", "SE23", "SE24", "SE25", "SE26", "SE27",
				"SE28", "SE29", "??", "??", "??", "??", "??", "??", 
				"SE30", "SE31", "SE32", "??", "??", "??", "??", "??", 
				"??", "??", "??", "??", "??", "??", "??", "??", 
				"ベースOPEN", "ベース - - B", "ベース - G -", "ベース - G B", "ベース R - -", "ベース R - B", "ベース R G -", "ベース R G B",
				"ベースWailing", "??", "??", "??", "??", "??", "??", "ベースWailing音切替",
				"??", "HHClose(空うち)", "Snare(空うち)", "Kick(空うち)", "HiTom(空うち)", "LowTom(空うち)", "Cymbal(空うち)", "FloorTom(空うち)",
				"HHOpen(空うち)", "RideCymbal(空うち)", "ギター(空打ち)", "ベース(空打ち)", "LeftCymbal(空うち)", "LeftPedal(空打ち)", "LeftBassDrum(空打ち)", "??", 
				"??", "??", "??", "??", "BGAスコープ画像切替1", "??", "??", "BGAスコープ画像切替2",
				"??", "??", "??", "??", "??", "??", "??", "??", 
				"??", "??", "??", "??", "??", "BGAスコープ画像切替3", "BGAスコープ画像切替4", "BGAスコープ画像切替5",
				"BGAスコープ画像切替6", "BGAスコープ画像切替7", "ミキサー登録", "ミキサー削除", "??", "??", "??", "??", 
				"BGAスコープ画像切替8"
			};
			*/
			return string.Format("CChip: 位置:{0:D4}.{1:D3}, 時刻{2:D6}, Ch:{3:X2}({4}), Pn:{5}({11})(内部{6}), Pd:{7}, Sz:{8}, UseWav:{9}",
				this.n発声位置 / 384,
				this.n発声位置 % 384,
				this.n発声時刻ms,
				this.eチャンネル番号,
				this.eチャンネル番号,
				this.n整数値, this.n整数値_内部番号,
				this.db実数値,
				this.dbチップサイズ倍率,
				this.bWAVを使うチャンネルである,
				// this.b自動再生音チャンネルである,
				CDTX.tZZ(this.n整数値));
		}
		/// <summary>
		/// チップの再生長を取得する。現状、WAVチップとBGAチップでのみ使用可能。
		/// </summary>
		/// <returns>再生長(ms)</returns>
		public int GetDuration()
		{
			int nDuration = 0;

			if (this.bWAVを使うチャンネルである)   // WAV
			{
				CDTX.CWAV wc;
				CDTXMania.Instance.DTX.listWAV.TryGetValue(this.n整数値_内部番号, out wc);
				if (wc == null)
				{
					nDuration = 0;
				}
				else
				{
					nDuration = (wc.rSound[0] == null) ? 0 : wc.rSound[0].n総演奏時間ms;
				}
			}
			else if (this.eチャンネル番号 == EChannel.Movie || this.eチャンネル番号 == EChannel.MovieFull)  // AVI
			{
				if (this.rAVI != null && this.rAVI.avi != null)
				{
					nDuration = this.rAVI.avi.GetDuration();
				}
			}

			double _db再生速度 = (CDTXMania.Instance.DTXVmode.Enabled) ? CDTXMania.Instance.DTX.dbDTXVPlaySpeed : CDTXMania.Instance.DTX.db再生速度;
			return (int)(nDuration / _db再生速度);
		}

		public string GetSoundFilename()
		{
			if ( this.bWAVを使うチャンネルである )   // WAV
			{
				CDTX.CWAV wc;
				CDTXMania.Instance.DTX.listWAV.TryGetValue( this.n整数値_内部番号, out wc );
				if ( wc == null )
				{
					return null;
				}
				else if ( wc.rSound[ 0 ] == null )
				{
					return null;
				}
				else
				{
					return wc.rSound[ 0 ].strファイル名;
				}
			}
			//else if ( this.eチャンネル番号 == EChannel.Movie || this.eチャンネル番号 == EChannel.MovieFull )  // AVI
			//{
			//}

			return null;
		}

		#region [ IComparable 実装 ]
		//-----------------
		public int CompareTo(CChip other)
		{
			byte[] n優先度 = new byte[] {
					5, 5, 3, 3, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5,
					5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5,
					7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
				};


			// まずは位置で比較。

			if (this.n発声位置 < other.n発声位置)
				return -1;

			if (this.n発声位置 > other.n発声位置)
				return 1;


			// 位置が同じなら優先度で比較。
			int thisIndex = (int)eチャンネル番号;
			int tIndex = (int)other.eチャンネル番号;

			if (n優先度[thisIndex] < n優先度[tIndex])
				return -1;

			if (n優先度[thisIndex] > n優先度[tIndex])
				return 1;


			// 位置も優先度も同じなら同じと返す。

			return 0;
		}
		//-----------------
		#endregion



		public bool bAssignAutoPlayState()
		{
			bool ret = false;
			bool bGtBsR = bGuitarBass_R;
			bool bGtBsG = bGuitarBass_G;
			bool bGtBsB = bGuitarBass_B;
			bool bGtBsW = bGuitarBass_Wailing;
			bool bGtBsO = bGuitarBass_Open;
			if (bDrums可視チップ)
			{
				if (CDTXMania.Instance.ConfigIni.bAutoPlay[EnumConverter.PadFromChannel(eチャンネル番号)])
				{
					ret = true;
				}
			}
			else if (bGuitar可視チップ)
			{
				//Trace.TraceInformation( "chip:{0}{1}{2} ", bGtBsR, bGtBsG, bGtBsB );
				//Trace.TraceInformation( "auto:{0}{1}{2} ", bIsAutoPlay[ (int) Eレーン.GtR ], bIsAutoPlay[ (int) Eレーン.GtG ], bIsAutoPlay[ (int) Eレーン.GtB ]);
				ret = true;
				if (CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.GtPick] == false) ret = false;
				else
				{
					if (bGtBsR == true && CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.GtR] == false) ret = false;
					else if (bGtBsG == true && CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.GtG] == false) ret = false;
					else if (bGtBsB == true && CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.GtB] == false) ret = false;
					else if (bGtBsW == true && CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.GtWail] == false) ret = false;
					else if (bGtBsO == true &&
						(CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.GtR] == false ||
						CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.GtG] == false ||
						CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.GtB] == false))
						ret = false;
				}
				//Trace.TraceInformation( "{0:x2}: {1}", pChip.nチャンネル番号, bPChipIsAutoPlay.ToString() );
			}
			else if (bBass可視チップ)
			{
				ret = true;
				if (CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.BsPick] == false) ret = false;
				else
				{
					if (bGtBsR == true && CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.BsR] == false) ret = false;
					else if (bGtBsG == true && CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.BsG] == false) ret = false;
					else if (bGtBsB == true && CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.BsB] == false) ret = false;
					else if (bGtBsW == true && CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.BsWail] == false) ret = false;
					else if (bGtBsO == true &&
						(CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.BsR] == false ||
						CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.BsG] == false ||
						CDTXMania.Instance.ConfigIni.bAutoPlay[EPad.BsB] == false))
						ret = false;
				}
			}
			this.bIsAutoPlayed = ret;
			return ret;
		}


		/// <summary>
		/// shallow copyです。
		/// </summary>
		/// <returns></returns>
		public object Clone()
		{
			return MemberwiseClone();
		}

		internal void ConvertSE25_26_27ToCY_RCY_LCY()
		{
			if ((EChannel.SE25 <= eチャンネル番号) && (eチャンネル番号 <= EChannel.SE27))
			{
				EChannel[] ch = { EChannel.Cymbal, EChannel.RideCymbal, EChannel.LeftCymbal };
				eチャンネル番号 = ch[eチャンネル番号 - EChannel.SE25];
			}
		}

		internal void SetNoChipGuitarToOpen()
		{
			eチャンネル番号 = EChannel.Guitar_Open;
		}

		internal void SetNoChipBassToOpen()
		{
			eチャンネル番号 = EChannel.Bass_Open;
		}
	}

}
