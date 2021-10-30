#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <logging/log.h>
#include <sys/atomic.h>

#include "ble_gatt.hpp"
#include "ble_service.hpp"


LOG_MODULE_REGISTER(bluetooth);

namespace
{

constexpr static uint16_t connectionIntervalMin = 6; ///< Minimal connection interval in 1.25 milliseconds intervals
constexpr static uint16_t connectionIntervalMax = 8; ///< Maximum connection interval in 1.25 milliseconds intervals
constexpr static uint16_t connectionLatency = 0;     ///< Connection latancy
constexpr static uint16_t connectionTimeout = 400;   ///< Connection timeout in 10 msec intervals

//Bluetooth::Gatt::BleOutputWorker worker;   ///< Ble output characteristic worker
bt_conn *activeConnection = nullptr;       ///< Active connection

/**
 * @brief Callback called when new client is connected
 * 
 * @param connected connected bluetooth connection
 * @param err connection error
 */
void OnClientConnected(bt_conn *connected, uint8_t err)
{
    if (err)
    {
        LOG_ERR("Connection failed (err %u)", err);
    }
    else
    {
        LOG_INF("Connected");

        if ((!activeConnection) && err == 0)
        {
            activeConnection = bt_conn_ref(connected);
#if 0
            bt_le_conn_param param = BT_LE_CONN_PARAM_INIT(
                connectionIntervalMin,
                connectionIntervalMax, 
                connectionLatency, 
                connectionTimeout);
            int error = bt_conn_le_param_update(connected, &param);

            if (error)
            {
                LOG_ERR("Failed to update connection parameters (err = %d)\n", error);
                return;
            }
#endif
        }
    }
}

/**
 * @brief Callback called when client is disconnected. Used to stop BLE notification if no clients are connected.
 * 
 * @param disconn disconnected bluetooth connection
 * @param reason disconnection reason
 */
void OnClientDisconnected(struct bt_conn *disconn, uint8_t reason)
{
    if (activeConnection)
    {
        bt_conn_unref(activeConnection);
        activeConnection = nullptr;

    }
    atomic_set(&Bluetooth::Gatt::ads131m08NotificationsEnable, false);    
    LOG_INF("Disconnected (reason %u)", reason);
}

/**
 * @brief connection status callback
 */
bt_conn_cb connectionCallbacks =
{
    .connected = OnClientConnected,
    .disconnected = OnClientDisconnected,
};

} // namespace

namespace Bluetooth
{

/**
 * @brief Function used to setup BLE Service
 * 
 * @return BLE error code 
 */
int SetupBLE()
{
    //worker.Initialize();

    bt_conn_cb_register(&connectionCallbacks);

    int err = bt_enable(&Gatt::OnBluetoothStarted);
    if (err)
    {
        LOG_INF("enable Bluetooth with status %d", err);
    }
    return err;
}

/**
 * @brief Set the Accelerometer datasource.
 * 
 * @param datasource pointer to datasource with accelerometr data
 */
void SensorNotify(const uint8_t* data, const uint8_t len)
{
    if (atomic_get(&Gatt::ads131m08NotificationsEnable))
    {
        bt_gatt_notify(nullptr, &Gatt::bt832a_svc.attrs[Gatt::CharacteristicSensorData], data, len);
    }
}

} // namespace Bluetooth
