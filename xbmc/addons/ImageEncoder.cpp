/*
 *      Copyright (C) 2013 Arne Morten Kvarving
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#include "ImageEncoder.h"

namespace ADDON
{

CImageEncoder::CImageEncoder(const cp_extension_t* ext)
 : ImageEncoderDll(ext),
   m_mimetype(CAddonMgr::GetInstance().GetExtValue(ext->configuration, "@mimetype"))
{
}

CImageEncoder::CImageEncoder(const CImageEncoder& from)
  : ImageEncoderDll(from.Props()),
    m_mimetype(from.m_mimetype)
{
  m_strLibName = from.m_strLibName;
}


CImageEncoder::~CImageEncoder()
{
  if (m_image && Initialized())
    m_pStruct->Close(m_image);
}

AddonPtr CImageEncoder::Clone() const
{
  // Copy constructor is generated by compiler and calls parent copy constructor
  return AddonPtr(new CImageEncoder(*this));
}

bool CImageEncoder::LoadImageFromMemory(unsigned char* buffer, unsigned int bufSize,
                                        unsigned int width, unsigned int height)
{
  if (!Initialized())
    return false;

  m_width = width;
  m_height = height;
  m_image = m_pStruct->LoadImage(buffer, bufSize, &m_width, &m_height);

  return m_image != nullptr;
}

bool CImageEncoder::Decode(unsigned char* const pixels, unsigned int width,
                           unsigned int height, unsigned int pitch,
                           unsigned int format)
{
  if (!Initialized())
    return false;

  bool result = m_pStruct->Decode(m_image, pixels, width, height, pitch, format);
  m_width = width;
  m_height = height;

  return result;
}

bool CImageEncoder::CreateThumbnailFromSurface(unsigned char* bufferin, unsigned int width,
                                               unsigned int height, unsigned int format,
                                               unsigned int pitch, const std::string& destFile,
                                               unsigned char*& bufferout, unsigned int& bufferoutSize)
{
  if (!Initialized())
    return false;

  return m_pStruct->CreateThumbnail(bufferin, width, height, pitch, format, destFile.c_str());
}

bool CImageEncoder::Create(const std::string& mimetype)
{
  m_pInfo = new IMAGEENC_PROPS;
  m_pInfo->mimetype = mimetype.c_str();

  return ImageEncoderDll::Create();
}

} /*namespace ADDON*/
