// File: arduinoble.js

// Load library EasyBLE.
evothings.loadScript('libs/evothings/easyble/easyble.js');

/**
 * @namespace
 * @author Mikael Kindborg
 * @description <p>Functions for communicating with an Arduino BLE shield.</p>
 * <p>It is safe practise to call function {@link evothings.scriptsLoaded}
 * to ensure dependent libraries are loaded before calling functions
 * in this library.</p>
 *
 * @todo This is a very simple library that has only write capability,
 * read and notification functions should be added.
 *
 * @todo Add function to set the write characteristic UUID to make
 * the code more generic.
 */
evothings.arduinoble = {};

;(function()
{
	// Internal functions.
	var internal = {};

	/**
	 * Stop any ongoing scan and disconnect all devices.
	 * @public
	 */
	evothings.arduinoble.close = function()
	{
		evothings.easyble.stopScan();
		evothings.easyble.closeConnectedDevices();
	};

	/**
	 * Called when you've connected to an Arduino BLE shield.
	 * @callback evothings.arduinoble.connectsuccess
	 * @param {evothings.arduinoble.ArduinoBLEDevice} device -
	 * The connected BLE shield.
	 */

	/**
	 * Connect to a BLE-shield.
	 * @param deviceName BLE name if the shield.
	 * @param {evothings.arduinoble.connectsuccess} success -
	 * Success callback: success(device)
	 * @param {function} fail - Error callback: fail(errorCode)
	 * @example
	 * evothings.arduinoble.connect(
	 *   'arduinoble', // Name of BLE shield.
	 *   function(device)
	 *   {
	 *     console.log('connected!');
	 *     device.writeDataArray(new Uint8Array([1]));
	 *     evothings.arduinoble.close();
	 *   },
	 *   function(errorCode)
	 *   {
	 *     console.log('Error: ' + errorCode);
	 *   });
	 * @public
	 */
	evothings.arduinoble.connect = function(deviceName, success, fail)
	{
		evothings.easyble.startScan(
			function(device)
			{
				if (device.name == deviceName)
				{
					evothings.easyble.stopScan();
					internal.connectToDevice(device, success, fail);
				}
			},
			function(errorCode)
			{
				fail(errorCode);
			});
	};

	/**
	 * Connect to the BLE shield.
	 * @private
	 */
	internal.connectToDevice = function(device, success, fail)
	{
		device.connect(
			function(device)
			{
				// Get services info.
				internal.getServices(device, success, fail);
			},
			function(errorCode)
			{
				fail(errorCode);
			});
	};

	/**
	 * Read all services from the device.
	 * @private
	 */
	internal.getServices = function(device, success, fail)
	{
		device.readServices(
			null, // null means read info for all services
			function(device)
			{
				internal.addMethodsToDeviceObject(device);
				success(device);
			},
			function(errorCode)
			{
				fail(errorCode);
			});
	};
    
    internal.text2ua = function(s) {
        var ua = new Uint8Array(s.length);
        for (var i = 0; i < s.length; i++) {
            ua[i] = s.charCodeAt(i);
        }
    return ua;
    };
 
    internal.ua2text = function(ua) {
        var s = '';
        for (var i = 0; i < ua.length; i++) {
            s += String.fromCharCode(ua[i]);
        }
        return s;
    };

	/**
	 * Add instance methods to the device object.
	 * @private
	 */
	internal.addMethodsToDeviceObject = function(device)
	{
		/**
		 * Object that holds info about an Arduino BLE shield.
		 * @namespace evothings.arduinoble.ArduinoBLEDevice
		 */

		/**
		 * @function writeDataArray
		 * @description Write data to an Arduino BLE shield.
		 * @param {Uint8Array} uint8array - The data to be written.
		 * @memberof evothings.arduinoble.ArduinoBLEDevice
		 * @instance
		 * @public
		 */
		device.writeDataString = function(str)
		{
            var uint8array = internal.text2ua(str);
			device.writeCharacteristic(
				// TODO: Make this possible to set.
				'6e400002-b5a3-f393-e0a9-e50e24dcca9e',
				uint8array,
				function()
				{
					console.log('writeCharacteristic success ->' + str);
				},
				function(errorCode)
				{
					console.log('writeCharacteristic error: ' + errorCode);
				});
		};
	};
})();
