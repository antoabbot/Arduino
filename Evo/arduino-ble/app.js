// JavaScript code for the Adafruit BlueFruit Arduino BLE.

/**
 * The BLE plugin is loaded asynchronously so the ble
 * variable is set in the onDeviceReady handler.
 */
var ble = null;

/**
 * Called when HTML page has been loaded.
 */
$(document).ready( function()
{
	// Adjust canvas size when browser resizes
	$(window).resize( respondCanvas );

	// Adjust the canvas size when the document has loaded.
	respondCanvas();
});

/**
 * Adjust the canvas dimensions based on its container's dimensions.
 */
function respondCanvas()
{
	var canvas = $('#canvas');
	var container = $(canvas).parent();
	canvas.attr('width', $(container).width() ); // Max width
	//canvas.attr('height', $(container).height() ); // Max height
}

/**
 * Application object that holds data and functions used by the app.
 */
var app =
{
	// Discovered devices.
	knownDevices: {},

	// Reference to the device we are connecting to.
	connectee: null,

	// Handle to the connected device.
	deviceHandle: null,

	// Handles to characteristics and descriptor for reading and
	// writing data from/to the Arduino using the BLE shield.
	characteristicRead: null,
	characteristicWrite: null,
	descriptorNotification: null,

	// Data that is plotted on the canvas.
	dataPoints: [],

	initialize: function()
	{
		document.addEventListener(
			'deviceready',
			function() { evothings.scriptsLoaded(app.onDeviceReady) },
			false);
	},

	displayStatus: function(status)
	{
		if(document.getElementById('status').innerHTML == status)
			return;
		console.log('Status: '+status);
		document.getElementById('status').innerHTML = status
	},

	// Called when device plugin functions are ready for use.
	onDeviceReady: function()
	{
		ble = evothings.ble; // Evothings BLE plugin

		// app.startScan();
	},

	startScan: function()
	{
		app.displayStatus('Scanning...');
		evothings.ble.startScan(
			function(deviceInfo)
			{
				if (app.knownDevices[deviceInfo.address])
				{
					return;
				}
				console.log('found device: ' + deviceInfo.name);
				app.knownDevices[deviceInfo.address] = deviceInfo;
				if (deviceInfo.name == 'Dojo01' && !app.connectee)
				{
					console.log('Found Dojo01');
					connectee = deviceInfo;
					app.connect(deviceInfo.address);
				}
			},
			function(errorCode)
			{
				app.displayStatus('startScan error: ' + errorCode);
			});
	},

	connect: function(address)
	{
		evothings.ble.stopScan();
		app.displayStatus('Connecting...');
		evothings.ble.connect(
			address,
			function(connectInfo)
			{
				if (connectInfo.state == 2) // Connected
				{
					app.deviceHandle = connectInfo.deviceHandle;
					app.getServices(connectInfo.deviceHandle);
				}
				else
				{
					app.displayStatus('Disconnected');
				}
			},
			function(errorCode)
			{
				app.displayStatus('connect error: ' + errorCode);
			});
	},
    
    text2ua: function(s) 
    {
        var ua = new Uint8Array(s.length);
        for (var i = 0; i < s.length; i++) 
        {
            ua[i] = s.charCodeAt(i);
        }
    return ua;
    },
 
    ua2text: function(ua) 
    {
        var s = '';
        for (var i = 0; i < ua.byteLength; i++) 
        {
            var val = new DataView(ua).getUint8(i);
            s += String.fromCharCode(val);
        }
        return s;
    },

	scanleft: function()
	{
        var arr = app.text2ua('!B31;')
		app.write(
			'writeCharacteristic',
			app.deviceHandle,
			app.characteristicWrite,
			arr); 
	},

	scanmiddle: function()
	{
        var arr = app.text2ua('!B21;')
		app.write(
			'writeCharacteristic',
			app.deviceHandle,
			app.characteristicWrite,
			arr);
	},
    
	scanright: function()
	{
        var arr = app.text2ua('!B11;')
		app.write(
			'writeCharacteristic',
			app.deviceHandle,
			app.characteristicWrite,
			arr);
	},
    
    forward: function()
	{
        var arr = app.text2ua('!B71;')
		app.write(
			'writeCharacteristic',
			app.deviceHandle,
			app.characteristicWrite,
			arr);
	},
            
    left: function()
	{
        var arr = app.text2ua('!B61;')
		app.write(
			'writeCharacteristic',
			app.deviceHandle,
			app.characteristicWrite,
			arr);
	},    
    
    right: function()
	{
        var arr = app.text2ua('!B51;')
		app.write(
			'writeCharacteristic',
			app.deviceHandle,
			app.characteristicWrite,
			arr);
	},
    stop: function()
	{
        var arr = app.text2ua('!B70;')
		app.write(
			'writeCharacteristic',
			app.deviceHandle,
			app.characteristicWrite,
			arr);
	},

	write: function(writeFunc, deviceHandle, handle, value)
	{
		if (handle)
		{
			ble[writeFunc](
				deviceHandle,
				handle,
				value,
				function()
				{
					console.log(writeFunc + ': ' + handle + ' success.');
				},
				function(errorCode)
				{
					console.log(writeFunc + ': ' + handle + ' error: ' + errorCode);
				});
		}
	},

	startReading: function(deviceHandle)
	{
		app.displayStatus('Enabling notifications...');

		// Turn notifications on.
		app.write(
			'writeDescriptor',
			deviceHandle,
			app.descriptorNotification,
			new Uint8Array([1,0]));

		// Start reading notifications.
		evothings.ble.enableNotification(
			deviceHandle,
			app.characteristicRead,
			function(data)
			{
				//app.displayStatus('Active');
                app.displayStatus('Returned data:' + app.ua2text(data));
			},
			function(errorCode)
			{
				app.displayStatus('enableNotification error: ' + errorCode);
			});
	},


	getServices: function(deviceHandle)
	{
		app.displayStatus('Reading services...');

		evothings.ble.readAllServiceData(deviceHandle, function(services)
		{
			// Find handles for characteristics and descriptor needed.
			for (var si in services)
			{
				var service = services[si];
                
				for (var ci in service.characteristics)
				{ 
					var characteristic = service.characteristics[ci];

					if (characteristic.uuid == '6e400003-b5a3-f393-e0a9-e50e24dcca9e')
					{
						app.characteristicRead = characteristic.handle;
					}
					else if (characteristic.uuid == '6e400002-b5a3-f393-e0a9-e50e24dcca9e')
					{
						app.characteristicWrite = characteristic.handle;
					}

					for (var di in characteristic.descriptors)
					{
						var descriptor = characteristic.descriptors[di];
						if (characteristic.uuid == '6e400003-b5a3-f393-e0a9-e50e24dcca9e' &&
							descriptor.uuid == '00002902-0000-1000-8000-00805f9b34fb')
						{
							app.descriptorNotification = descriptor.handle;
						}
					}
				}
			}

			if (app.characteristicRead && app.characteristicWrite && app.descriptorNotification)
			{
				console.log('RX/TX services found.');
				app.startReading(deviceHandle);
			}
			else
			{
				app.displayStatus('ERROR: RX/TX services not found!');
			}
		},
		function(errorCode)
		{
			app.displayStatus('readAllServiceData error: ' + errorCode);
		});
	},

	openBrowser: function(url)
	{
		window.open(url, '_system', 'location=yes')
	}
};
// End of app object.

// Initialise app.
app.initialize();
