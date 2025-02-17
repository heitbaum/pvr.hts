/*
 *  Copyright (C) 2005-2021 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "AutoRecording.h"

using namespace tvheadend;
using namespace tvheadend::entity;

AutoRecording::AutoRecording(const std::string& id /*= ""*/) : SeriesRecordingBase(id)
{
}

bool AutoRecording::operator==(const AutoRecording& right)
{
  return SeriesRecordingBase::operator==(right) && m_startWindowBegin == right.m_startWindowBegin &&
         m_startWindowEnd == right.m_startWindowEnd && m_startExtra == right.m_startExtra &&
         m_stopExtra == right.m_stopExtra && m_dupDetect == right.m_dupDetect &&
         m_fulltext == right.m_fulltext && m_broadcastType == right.m_broadcastType &&
         m_seriesLink == right.m_seriesLink;
}

bool AutoRecording::operator!=(const AutoRecording& right)
{
  return !(*this == right);
}

int AutoRecording::GetStartWindowBegin() const
{
  return m_startWindowBegin;
}

void AutoRecording::SetStartWindowBegin(int32_t start)
{
  m_startWindowBegin = start;
}

int AutoRecording::GetStartWindowEnd() const
{
  return m_startWindowEnd;
}

void AutoRecording::SetStartWindowEnd(int32_t end)
{
  m_startWindowEnd = end;
}

int64_t AutoRecording::GetMarginStart() const
{
  return m_startExtra;
}

void AutoRecording::SetMarginStart(int64_t startExtra)
{
  m_startExtra = startExtra;
}

int64_t AutoRecording::GetMarginEnd() const
{
  return m_stopExtra;
}

void AutoRecording::SetMarginEnd(int64_t stopExtra)
{
  m_stopExtra = stopExtra;
}

uint32_t AutoRecording::GetDupDetect() const
{
  return m_dupDetect;
}

void AutoRecording::SetDupDetect(uint32_t dupDetect)
{
  m_dupDetect = dupDetect;
}

bool AutoRecording::GetFulltext() const
{
  return m_fulltext > 0;
}

void AutoRecording::SetFulltext(uint32_t fulltext)
{
  m_fulltext = fulltext;
}

uint32_t AutoRecording::GetBroadcastType() const
{
  return m_broadcastType;
}
void AutoRecording::SetBroadcastType(uint32_t broadcastType)
{
  m_broadcastType = broadcastType;
}

const std::string& AutoRecording::GetSeriesLink() const
{
  return m_seriesLink;
}

void AutoRecording::SetSeriesLink(const std::string& seriesLink)
{
  m_seriesLink = seriesLink;
}
