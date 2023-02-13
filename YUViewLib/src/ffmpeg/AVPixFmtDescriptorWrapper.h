/*  This file is part of YUView - The YUV player with advanced analytics toolset
 *   <https://github.com/IENT/YUView>
 *   Copyright (C) 2015  Institut für Nachrichtentechnik, RWTH Aachen University, GERMANY
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   In addition, as a special exception, the copyright holders give
 *   permission to link the code of portions of this program with the
 *   OpenSSL library under certain conditions as described in each
 *   individual source file, and distribute linked combinations including
 *   the two.
 *
 *   You must obey the GNU General Public License in all respects for all
 *   of the code used other than OpenSSL. If you modify file(s) with this
 *   exception, you may extend this exception to your version of the
 *   file(s), but you are not obligated to do so. If you do not wish to do
 *   so, delete this exception statement from your version. If you delete
 *   this exception statement from all source files in the program, then
 *   also delete it here.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "FFMpegLibrariesTypes.h"

#include <video/rgb/videoHandlerRGB.h>
#include <video/yuv/videoHandlerYUV.h>

namespace FFmpeg
{

class AVPixFmtDescriptorWrapper
{
public:
  AVPixFmtDescriptorWrapper() = default;
  AVPixFmtDescriptorWrapper(AVPixFmtDescriptor *sideData, LibraryVersion libVer);

  video::RawFormat           getRawFormat() const;
  video::yuv::PixelFormatYUV getPixelFormatYUV() const;
  video::rgb::PixelFormatRGB getRGBPixelFormat() const;

  bool setValuesFromPixelFormatYUV(video::yuv::PixelFormatYUV fmt);

  // AVPixFmtDescriptor
  QString name;
  int     nb_components{}; ///< The number of components each pixel has, (1-4)

  /**
   * Amount to shift the luma width/height right to find the chroma width/height.
   * For YV12 this is 1 for example.
   * chroma_width = -((-luma_width) >> log2_chroma_w)
   * The note above is needed to ensure rounding up.
   * This value only refers to the chroma components.
   */
  int log2_chroma_w{};
  int log2_chroma_h{};

  struct Flags
  {
    bool bigEndian{};
    bool pallette{};
    bool bitwisePacked{};
    bool hwAccelerated{};
    bool planar{};
    bool rgb{};
    bool pseudoPallette{};
    bool hasAlphaPlane{};
    bool bayerPattern{};
    bool floatValues{};

    bool operator==(const Flags &other) const;
  };
  Flags flags;

  /**
   * Parameters that describe how pixels are packed.
   * If the format has 2 or 4 components, then alpha is last.
   * If the format has 1 or 2 components, then luma is 0.
   * If the format has 3 or 4 components,
   * if the RGB flag is set then 0 is red, 1 is green and 2 is blue;
   * otherwise 0 is luma, 1 is chroma-U and 2 is chroma-V.
   */
  struct AVComponentDescriptor
  {
    int plane{};  ///< which of the 4 planes contains the component
    int step{};   ///< Number of elements between 2 horizontally consecutive pixels
    int offset{}; ///< Number of elements before the component of the first pixel
    int shift{};  ///< number of least significant bits that must be shifted away to get the value
    int depth{};  ///< number of bits in the component
  };

  QString               aliases{};
  AVComponentDescriptor comp[4];

  bool operator==(const AVPixFmtDescriptorWrapper &a);
};

} // namespace FFmpeg
