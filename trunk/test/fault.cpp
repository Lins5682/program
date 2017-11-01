#include "fault.h"

struct Fault::Faultstructure Fault::faultTable[] =
{
    {FAULT_INSULATION, LEVEL_INSULATION, "Insulation fault"},
    {FAULT_OUT_CONN_OVER_TEMP, LEVEL_OUT_CONN_OVER_TEMP, "Output connector over temperature"},
    {FAULT_CONNECTOR, LEVEL_CONNECTOR, "Connector fault"},
    {FAULT_BATTERY_TEMP_HIGH, LEVEL_BATTERY_TEMP_HIGH, "Battery temperature is too high"},
    {FAULT_HVOLTAGE_RELAY, LEVEL_HVOLTAGE_RELAY, "High voltage relay fault"},
    {FAULT_POINT_2_VOLTAGE, LEVEL_POINT_2_VOLTAGE, "Detection point 2 voltage detection fault"},
    {FAULT_CURRENT_OVER, LEVEL_CURRENT_OVER, "Over current"}
};

Fault::Fault(int number, QObject *parent) :
    QObject(parent)
{
    gunnumber = number;
    faultList.clear();
}

Fault::~Fault()
{
    faultList.clear();
}
