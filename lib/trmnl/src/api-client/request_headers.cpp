#include <api-client/request_headers.h>
#include <trmnl_log.h>

#if defined(BOARD_XIAO_EPAPER_DISPLAY) || defined(BOARD_XIAO_EPAPER_DISPLAY_3CLR)
// Single-cell Li-ion state-of-charge lookup table: resting voltage (V) -> charge (%).
// Points come from a typical LiPo discharge curve; values between points are
// linearly interpolated.
// 单节锂电池「静态电压 -> 电量百分比」对照表，取自典型锂电池放电曲线，
// 表内相邻点之间做线性插值。
struct BatterySocPoint
{
  float voltage;
  int percent;
};

static const BatterySocPoint BATTERY_SOC_TABLE[] = {
    {4.20f, 100}, {4.15f, 95}, {4.11f, 90}, {4.08f, 85}, {4.02f, 80},
    {3.98f, 75},  {3.95f, 70}, {3.91f, 65}, {3.87f, 60}, {3.85f, 55},
    {3.84f, 50},  {3.82f, 45}, {3.80f, 40}, {3.79f, 35}, {3.77f, 30},
    {3.75f, 25},  {3.73f, 20}, {3.71f, 15}, {3.69f, 10}, {3.61f, 5},
    {3.27f, 0},
};

// Convert a single-cell Li-ion voltage to a 0-100 charge percentage by
// interpolating the discharge-curve table above. Out-of-range inputs clamp
// to 100 (above the table top) or 0 (below the table bottom).
// 通过对上面的放电曲线表做线性插值，把单节锂电池电压换算成 0-100 的电量百分比；
// 超出表范围时分别钳位为 100（高于表顶）或 0（低于表底）。
static int batteryVoltageToPercent(float voltage)
{
  const size_t count = sizeof(BATTERY_SOC_TABLE) / sizeof(BATTERY_SOC_TABLE[0]);
  if (voltage >= BATTERY_SOC_TABLE[0].voltage)
    return 100;
  if (voltage <= BATTERY_SOC_TABLE[count - 1].voltage)
    return 0;
  for (size_t i = 0; i < count - 1; ++i)
  {
    const BatterySocPoint &high = BATTERY_SOC_TABLE[i];
    const BatterySocPoint &low = BATTERY_SOC_TABLE[i + 1];
    if (voltage <= high.voltage && voltage >= low.voltage)
    {
      float ratio = (voltage - low.voltage) / (high.voltage - low.voltage);
      return low.percent + (int)(ratio * (high.percent - low.percent) + 0.5f);
    }
  }
  return 0;
}
#endif // BOARD_XIAO_EPAPER_DISPLAY

HttpHeaderList buildDisplayHeaders(const ApiDisplayInputs &inputs)
{
  HttpHeaderList headers;
  headers.push_back({"ID", inputs.macAddress});
  headers.push_back({"Content-Type", "application/json"});
  headers.push_back({"Update-Source", inputs.updateSource});
  headers.push_back({"Access-Token", inputs.apiKey});
  headers.push_back({"Refresh-Rate", String(inputs.refreshRate)});
  headers.push_back({"Battery-Voltage", String(inputs.batteryVoltage)});
#ifdef BOARD_TRMNL_X
  headers.push_back({"Battery-Count", String(inputs.batteryCount)});
  headers.push_back({"Battery-Charging", String(inputs.batteryCharging)});
  headers.push_back({"USB-Connected", inputs.usbConnected ? "true" : "false"});
  headers.push_back({"Percent-Charged", String(inputs.stateOfCharge)});
  headers.push_back({"Battery-Health", String(inputs.stateOfHealth)});
  headers.push_back({"Battery-Current", String(inputs.batteryCurrent)});
  headers.push_back({"Battery-Temp", String(inputs.batteryTemperature)});
  headers.push_back({"Battery-Capacity", String(inputs.currentBatteryCapacity) + "/" + String(inputs.maxBatteryCapacity)});
#endif // BOARD_TRMNL_X
#if defined(BOARD_XIAO_EPAPER_DISPLAY) || defined(BOARD_XIAO_EPAPER_DISPLAY_3CLR)
  // Report charge percentage derived from battery voltage so the server uses it
  // instead of its own battery-voltage algorithm.
  // 上报由电池电压换算出的电量百分比，让服务器使用它而不是自身的电压算法。
  headers.push_back({"Percent-Charged", String(batteryVoltageToPercent(inputs.batteryVoltage))});
#endif // BOARD_XIAO_EPAPER_DISPLAY
  headers.push_back({"FW-Version", inputs.firmwareVersion});
  headers.push_back({"Model", String(inputs.model)});
  headers.push_back({"Image-Cached", inputs.imageCached ? "true" : "false"});
  headers.push_back({"Wake-Time", String(inputs.prevWakeTime)});
  headers.push_back({"RSSI", String(inputs.rssi)});
  if (inputs.wifiBand.length() > 0)
    headers.push_back({"WiFi-Band", inputs.wifiBand});
  headers.push_back({"Temperature-Profile", "true"});
  headers.push_back({"Width", String(inputs.displayWidth)});
  headers.push_back({"Height", String(inputs.displayHeight)});

  if (inputs.specialFunction != SF_NONE)
    headers.push_back({"special_function", "true"});

  return headers;
}

HttpHeaderList buildSetupHeaders(const ApiSetupInputs &inputs)
{
  HttpHeaderList headers;
  headers.push_back({"ID", inputs.macAddress});
  headers.push_back({"Content-Type", "application/json"});
  headers.push_back({"FW-Version", inputs.firmwareVersion});
  headers.push_back({"Model", inputs.model});
  return headers;
}

HttpHeaderList buildLogHeaders(const ApiLogInputs &inputs)
{
  HttpHeaderList headers;
  headers.push_back({"ID", inputs.macAddress});
  headers.push_back({"Accept", "application/json, */*"});
  headers.push_back({"Access-Token", inputs.apiKey});
  headers.push_back({"Content-Type", "application/json"});
  return headers;
}

String formatHeaders(const HttpHeaderList &headers)
{
  String out;
  for (size_t i = 0; i < headers.size(); ++i)
  {
    if (i > 0)
      out += "\n";
    out += headers[i].first + ": " + headers[i].second;
  }
  return out;
}

void logHeaders(const HttpHeaderList &headers)
{
  Log_info("Added headers:\n%s", formatHeaders(headers).c_str());
}
