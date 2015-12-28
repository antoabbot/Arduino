import logging
import time
import uuid

from multiprocessing import Process, Lock

import Adafruit_BluefruitLE


# Enable debug output.
#logging.basicConfig(level=logging.DEBUG)

# Define service and characteristic UUIDs used by the UART service.
UART_SERVICE_UUID = uuid.UUID('6E400001-B5A3-F393-E0A9-E50E24DCCA9E')
TX_CHAR_UUID      = uuid.UUID('6E400002-B5A3-F393-E0A9-E50E24DCCA9E')
RX_CHAR_UUID      = uuid.UUID('6E400003-B5A3-F393-E0A9-E50E24DCCA9E')

# Get the BLE provider for the current platform.
ble = Adafruit_BluefruitLE.get_provider()

mutex = Lock()

def main():
    ble.clear_cached_data()

    # Get the first available BLE network adapter and make sure it's powered on.
    adapter = ble.get_default_adapter()
    adapter.power_on()
    print('Using adapter: {0}'.format(adapter.name))

    # Disconnect any currently connected UART devices.  Good for cleaning up and
    # starting from a fresh state.
    print('Disconnecting any connected UART devices...')
    ble.disconnect_devices([UART_SERVICE_UUID])

    # Scan for UART devices.
    print('Searching for UART device...')
    try:
        adapter.start_scan()
        # Search for the first UART device found (will time out after 60 seconds
        # but you can specify an optional timeout_sec parameter to change it).
        device = ble.find_device(service_uuids=[UART_SERVICE_UUID])
        if device is None:
            raise RuntimeError('Failed to find UART device!')
    finally:
        # Make sure scanning is stopped before exiting.
        adapter.stop_scan()

    print('Connecting to device...')
    device.connect()  # Will time out after 60 seconds, specify timeout_sec parameter
                      # to change the timeout.

    # Once connected do everything else in a try/finally to make sure the device
    # is disconnected when done.
    try:
        # Wait for service discovery to complete for at least the specified
        # service and characteristic UUID lists.  Will time out after 60 seconds
        # (specify timeout_sec parameter to override).
        print('Discovering services...')
        device.discover([UART_SERVICE_UUID], [TX_CHAR_UUID, RX_CHAR_UUID])

        # Find the UART service and its characteristics.
        uart = device.find_service(UART_SERVICE_UUID)
        rx = uart.find_characteristic(RX_CHAR_UUID)
        tx = uart.find_characteristic(TX_CHAR_UUID)

        # Local class to store received data
        class recvData:
            dist = 0
            angle = 90

        # Event handler for the returned BLE RX callback
        def received(data):
            with mutex:
                try:
                    # TODO: very crude parser - data might be wrapped, should concatenate and then split
                    if len(data) > 9 and data[0] == '!' and data[1] == 'S' and data[8] == ';':
                        recvData.dist = int(data[2:5])
                        recvData.angle = int(data[5:8])
                except:
                    print("Error in event handler: Buff=" + data)


        # Turn on notification of RX characteristics using the callback above.
        print('Subscribing to RX characteristic changes...')
        rx.start_notify(received)

        # Crude control of robot. Drive for a second in each direction
        while True:
            user_input = raw_input("Press 'q' to end: ")
            if user_input == 'q':
                break
            if user_input == 'a':
                tx.write_value("!B71;")
                time.sleep(1)
                tx.write_value("!B70;")
            elif user_input == 'z':
                tx.write_value("!B81;")
                time.sleep(1)
                tx.write_value("!B80;")
            elif user_input == 'x':
                tx.write_value("!B61;")
                time.sleep(1)
                tx.write_value("!B60;")
            elif user_input == 'c':
                tx.write_value("!B51;")
                time.sleep(1)
                tx.write_value("!B50;")
            elif user_input == 'v':
                tx.write_value("!B11;")
            elif user_input == 'b':
                tx.write_value("!B21;")
            elif user_input == 'n':
                tx.write_value("!B31;")
            elif user_input == 'm':
                tx.write_value("!B41;")
            elif user_input == 'p':
                print("Dist=" + str(recvData.dist) +", Angle=" + str(recvData.angle))

        print('Exiting...')
    finally:
        # Make sure device is disconnected on exit.
        device.disconnect()


# Initialize the BLE system.  MUST be called before other BLE calls!
ble.initialize()

# Start the mainloop to process BLE events, and run the provided function in
# a background thread.  When the provided main function stops running, returns
# an integer status code, or throws an error the program will exit.
ble.run_mainloop_with(main)
