/*
 *  Copyright (C) 2005-2021 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "InstanceSettings.h"

#include "HTSPTypes.h"
#include "utilities/Logger.h"

#include "kodi/General.h"

using namespace tvheadend;
using namespace tvheadend::utilities;

namespace
{
const std::string DEFAULT_HOST = "127.0.0.1";
const bool DEFAULT_USE_HTTPS = false;
const int DEFAULT_HTTP_PORT = 9981;
const int DEFAULT_HTSP_PORT = 9982;
const std::string DEFAULT_USERNAME = "";
const std::string DEFAULT_PASSWORD = "";
const std::string DEFAULT_WOL_MAC = "";
const int DEFAULT_CONNECT_TIMEOUT = 10000; // millisecs
const int DEFAULT_RESPONSE_TIMEOUT = 5000; // millisecs
const bool DEFAULT_ASYNC_EPG = true;
const bool DEFAULT_PRETUNER_ENABLED = false;
const int DEFAULT_TOTAL_TUNERS = 1; // total tuners > 1 => predictive tuning active
const int DEFAULT_PRETUNER_CLOSEDELAY = 10; // secs
const bool DEFAULT_AUTOREC_USE_REGEX = false;
const std::string DEFAULT_STREAMING_PROFILE = "";
const bool DEFAULT_STREAMING_HTTP = false;
const int DEFAULT_DVR_PRIO = DVR_PRIO_NORMAL;
const int DEFAULT_DVR_LIFETIME = 15; // use backend setting
const int DEFAULT_DVR_DUPDETECT = DVR_AUTOREC_RECORD_ALL;
const bool DEFAULT_DVR_PLAYSTATUS = true;
const int DEFAULT_STREAM_CHUNKSIZE = 64; // KB
const bool DEFAULT_DVR_IGNORE_DUPLICATE_SCHEDULES = true;
const bool DEFAULT_STREAM_STALLED_THRESHOLD = 10; // seconds

} // namespace

InstanceSettings::InstanceSettings(kodi::addon::IAddonInstance& instance)
  : m_instance(instance),
    m_strHostname(DEFAULT_HOST),
    m_iPortHTSP(DEFAULT_HTTP_PORT),
    m_iPortHTTP(DEFAULT_HTSP_PORT),
    m_bUseHTTPS(DEFAULT_USE_HTTPS),
    m_strUsername(DEFAULT_USERNAME),
    m_strPassword(DEFAULT_PASSWORD),
    m_strWolMac(DEFAULT_WOL_MAC),
    m_iConnectTimeout(DEFAULT_CONNECT_TIMEOUT),
    m_iResponseTimeout(DEFAULT_RESPONSE_TIMEOUT),
    m_bAsyncEpg(DEFAULT_ASYNC_EPG),
    m_bPretunerEnabled(DEFAULT_PRETUNER_ENABLED),
    m_iTotalTuners(DEFAULT_TOTAL_TUNERS),
    m_iPreTunerCloseDelay(DEFAULT_PRETUNER_CLOSEDELAY),
    m_bAutorecUseRegEx(DEFAULT_AUTOREC_USE_REGEX),
    m_strStreamingProfile(DEFAULT_STREAMING_PROFILE),
    m_bUseHTTPStreaming(DEFAULT_STREAMING_HTTP),
    m_iDvrPriority(DEFAULT_DVR_PRIO),
    m_iDvrLifetime(DEFAULT_DVR_LIFETIME),
    m_iDvrDupdetect(DEFAULT_DVR_DUPDETECT),
    m_bDvrPlayStatus(DEFAULT_DVR_PLAYSTATUS),
    m_iStreamReadChunkSizeKB(DEFAULT_STREAM_CHUNKSIZE),
    m_bIgnoreDuplicateSchedules(DEFAULT_DVR_IGNORE_DUPLICATE_SCHEDULES),
    m_streamStalledThreshold(DEFAULT_STREAM_STALLED_THRESHOLD)
{
  ReadSettings();
}

void InstanceSettings::ReadSettings()
{
  /* Connection */
  SetHostname(ReadStringSetting("host", DEFAULT_HOST));
  SetPortHTSP(ReadIntSetting("htsp_port", DEFAULT_HTSP_PORT));
  SetPortHTTP(ReadIntSetting("http_port", DEFAULT_HTTP_PORT));
  SetUseHTTPS(ReadBoolSetting("https", DEFAULT_USE_HTTPS));
  SetUsername(ReadStringSetting("user", DEFAULT_USERNAME));
  SetPassword(ReadStringSetting("pass", DEFAULT_PASSWORD));
  SetWolMac(ReadStringSetting("wol_mac", DEFAULT_WOL_MAC));

  /* Note: Timeouts in settings UI are defined in seconds but we expect them to be in milliseconds. */
  SetConnectTimeout(ReadIntSetting("connect_timeout", DEFAULT_CONNECT_TIMEOUT / 1000) * 1000);
  SetResponseTimeout(ReadIntSetting("response_timeout", DEFAULT_RESPONSE_TIMEOUT / 1000) * 1000);

  /* Data Transfer */
  SetAsyncEpg(ReadBoolSetting("epg_async", DEFAULT_ASYNC_EPG));

  /* Predictive Tuning */
  m_bPretunerEnabled = ReadBoolSetting("pretuner_enabled", DEFAULT_PRETUNER_ENABLED);
  SetTotalTuners(m_bPretunerEnabled ? ReadIntSetting("total_tuners", DEFAULT_TOTAL_TUNERS) : 1);
  SetPreTunerCloseDelay(
      m_bPretunerEnabled ? ReadIntSetting("pretuner_closedelay", DEFAULT_PRETUNER_CLOSEDELAY) : 0);

  /* Auto recordings */
  SetAutorecUseRegEx(ReadBoolSetting("autorec_use_regex", DEFAULT_AUTOREC_USE_REGEX));

  /* Streaming */
  SetStreamingProfile(ReadStringSetting("streaming_profile", DEFAULT_STREAMING_PROFILE));
  SetStreamingHTTP(ReadBoolSetting("streaming_http", DEFAULT_STREAMING_HTTP));
  SetStreamStalledThreshold(
      ReadIntSetting("stream_stalled_threshold", DEFAULT_STREAM_STALLED_THRESHOLD));

  /* Default dvr settings */
  SetDvrPriority(ReadIntSetting("dvr_priority", DEFAULT_DVR_PRIO));
  SetDvrLifetime(ReadIntSetting("dvr_lifetime2", DEFAULT_DVR_LIFETIME));
  SetDvrDupdetect(ReadIntSetting("dvr_dubdetect", DEFAULT_DVR_DUPDETECT));

  /* Sever based play status */
  SetDvrPlayStatus(ReadBoolSetting("dvr_playstatus", DEFAULT_DVR_PLAYSTATUS));

  /* Stream read chunk size */
  SetStreamReadChunkSizeKB(ReadIntSetting("stream_readchunksize", DEFAULT_STREAM_CHUNKSIZE));

  /* Scheduled recordings */
  SetIgnoreDuplicateSchedules(
      ReadBoolSetting("dvr_ignore_duplicates", DEFAULT_DVR_IGNORE_DUPLICATE_SCHEDULES));
}

ADDON_STATUS InstanceSettings::SetSetting(const std::string& key,
                                          const kodi::addon::CSettingValue& value)
{
  /* Connection */
  if (key == "host")
    return SetStringSetting(GetHostname(), value);
  else if (key == "htsp_port")
    return SetIntSetting(GetPortHTSP(), value);
  else if (key == "http_port")
    return SetIntSetting(GetPortHTTP(), value);
  else if (key == "https")
    return SetBoolSetting(GetUseHTTPS(), value);
  else if (key == "user")
    return SetStringSetting(GetUsername(), value);
  else if (key == "pass")
    return SetStringSetting(GetPassword(), value);
  else if (key == "wol_mac")
    return SetStringSetting(GetWolMac(), value);
  else if (key == "connect_timeout")
  {
    SetConnectTimeout(value.GetInt() * 1000);
    return ADDON_STATUS_OK;
  }
  else if (key == "response_timeout")
  {
    SetResponseTimeout(value.GetInt() * 1000);
    return ADDON_STATUS_OK;
  }
  /* Data Transfer */
  else if (key == "epg_async")
    return SetBoolSetting(GetAsyncEpg(), value);
  /* Predictive Tuning */
  else if (key == "pretuner_enabled")
    return SetBoolSetting(m_bPretunerEnabled, value);
  else if (key == "total_tuners")
  {
    if (!m_bPretunerEnabled)
      return ADDON_STATUS_OK;
    else
      return SetIntSetting(GetTotalTuners(), value);
  }
  else if (key == "pretuner_closedelay")
  {
    SetPreTunerCloseDelay(value.GetInt());
    return ADDON_STATUS_OK;
  }
  /* Auto recordings */
  else if (key == "autorec_use_regex")
  {
    SetAutorecUseRegEx(value.GetBoolean());
    return ADDON_STATUS_OK;
  }
  /* Streaming */
  else if (key == "streaming_profile")
    return SetStringSetting(GetStreamingProfile(), value);
  else if (key == "streaming_http")
    return SetBoolSetting(GetStreamingHTTP(), value);
  else if (key == "stream_stalled_threshold")
    return SetIntSetting(GetStreamStalledThreshold(), value);
  /* Default dvr settings */
  else if (key == "dvr_priority")
    return SetIntSetting(GetDvrPriority(), value);
  else if (key == "dvr_lifetime2")
    return SetIntSetting(GetDvrLifetime(true), value);
  else if (key == "dvr_dubdetect")
    return SetIntSetting(GetDvrDupdetect(), value);
  /* Server based play status */
  else if (key == "dvr_playstatus")
    return SetBoolSetting(GetDvrPlayStatus(), value);
  else if (key == "stream_readchunksize")
    return SetIntSetting(GetStreamReadChunkSize(), value);
  else if (key == "dvr_ignore_duplicates")
  {
    SetIgnoreDuplicateSchedules(value.GetBoolean());
    return ADDON_STATUS_OK;
  }
  else
  {
    Logger::Log(LogLevel::LEVEL_ERROR, "InstanceSettings::SetSetting - unknown setting '%s'",
                key.c_str());
    return ADDON_STATUS_UNKNOWN;
  }
}

std::string InstanceSettings::ReadStringSetting(const std::string& key,
                                                const std::string& def) const
{
  std::string value;
  if (m_instance.CheckInstanceSettingString(key, value))
    return value;

  return def;
}

int InstanceSettings::ReadIntSetting(const std::string& key, int def) const
{
  int value;
  if (m_instance.CheckInstanceSettingInt(key, value))
    return value;

  return def;
}

bool InstanceSettings::ReadBoolSetting(const std::string& key, bool def) const
{
  bool value;
  if (m_instance.CheckInstanceSettingBoolean(key, value))
    return value;

  return def;
}

ADDON_STATUS InstanceSettings::SetStringSetting(const std::string& oldValue,
                                                const kodi::addon::CSettingValue& newValue)
{
  if (oldValue == newValue.GetString())
    return ADDON_STATUS_OK;

  return ADDON_STATUS_NEED_RESTART;
}

ADDON_STATUS InstanceSettings::SetIntSetting(int oldValue,
                                             const kodi::addon::CSettingValue& newValue)
{
  if (oldValue == newValue.GetInt())
    return ADDON_STATUS_OK;

  return ADDON_STATUS_NEED_RESTART;
}

ADDON_STATUS InstanceSettings::SetBoolSetting(bool oldValue,
                                              const kodi::addon::CSettingValue& newValue)
{
  if (oldValue == newValue.GetBoolean())
    return ADDON_STATUS_OK;

  return ADDON_STATUS_NEED_RESTART;
}

int InstanceSettings::GetDvrLifetime(bool asEnum) const
{
  if (asEnum)
    return m_iDvrLifetime;
  else
  {
    switch (m_iDvrLifetime)
    {
      case 0:
        return DVR_RET_1DAY;
      case 1:
        return DVR_RET_3DAY;
      case 2:
        return DVR_RET_5DAY;
      case 3:
        return DVR_RET_1WEEK;
      case 4:
        return DVR_RET_2WEEK;
      case 5:
        return DVR_RET_3WEEK;
      case 6:
        return DVR_RET_1MONTH;
      case 7:
        return DVR_RET_2MONTH;
      case 8:
        return DVR_RET_3MONTH;
      case 9:
        return DVR_RET_6MONTH;
      case 10:
        return DVR_RET_1YEAR;
      case 11:
        return DVR_RET_2YEARS;
      case 12:
        return DVR_RET_3YEARS;
      case 13:
        return DVR_RET_SPACE;
      case 14:
        return DVR_RET_FOREVER;
      case 15:
      default:
        return DVR_RET_DVRCONFIG;
    }
  }
}
