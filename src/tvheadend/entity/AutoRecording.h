/*
 *  Copyright (C) 2005-2021 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "SeriesRecordingBase.h"

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace tvheadend
{
class InstanceSettings;

namespace entity
{

class AutoRecording : public SeriesRecordingBase
{
public:
  AutoRecording(const std::string& id = "");

  bool operator==(const AutoRecording& right);
  bool operator!=(const AutoRecording& right);

  void SetSettings(const std::shared_ptr<InstanceSettings>& settings) { m_settings = settings; }

  time_t GetStart() const;
  void SetStartWindowBegin(int32_t begin);

  time_t GetStop() const;
  void SetStartWindowEnd(int32_t end);

  int64_t GetMarginStart() const;
  void SetMarginStart(int64_t startExtra);

  int64_t GetMarginEnd() const;
  void SetMarginEnd(int64_t stopExtra);

  uint32_t GetDupDetect() const;
  void SetDupDetect(uint32_t dupDetect);

  bool GetFulltext() const;
  void SetFulltext(uint32_t fulltext);

  const std::string& GetSeriesLink() const;
  void SetSeriesLink(const std::string& seriesLink);

private:
  std::shared_ptr<InstanceSettings> m_settings;

  int32_t m_startWindowBegin{0}; // Begin of the starting window (minutes from midnight).
  int32_t m_startWindowEnd{0}; // End of the starting window (minutes from midnight).
  int64_t m_startExtra{0}; // Extra start minutes (pre-time).
  int64_t m_stopExtra{0}; // Extra stop minutes (post-time).
  uint32_t m_dupDetect{0}; // duplicate episode detect (numeric values: see dvr_autorec_dedup_t).
  uint32_t m_fulltext{0}; // Fulltext epg search.
  std::string m_seriesLink; // Series link.
};

typedef std::map<std::string, AutoRecording> AutoRecordingsMap;
typedef std::pair<std::string, AutoRecording> AutoRecordingMapEntry;

} // namespace entity
} // namespace tvheadend
